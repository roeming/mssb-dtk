from os.path import join, exists

file_folder = join("orig", "GYQE01", "files")
input_file = join(file_folder, "aaaa.dat")
KNOWN_AAAA_FILES = [
    {
        "Input": input_file,
        "Output": join(file_folder, 'menus.rel'),
        "lookbackBitSize": 0xb,
        "repetitionBitSize": 0x4,
        "size": 0x1027E4,
        "offset": 0x800,
        "compressedSize": 0x5A818,
        "compressionFlag": 0x4,
    },
    {
        "Input": input_file,
        "Output": join(file_folder, 'game.rel'),
        "lookbackBitSize": 0xb,
        "repetitionBitSize": 0x4,
        "size": 0x2220F8,
        "offset": 0x5B800,
        "compressedSize": 0xF4450,
        "compressionFlag": 0x4,
    },
    {
        "Input": input_file,
        "Output": join(file_folder, 'challenge.rel'),
        "lookbackBitSize": 0xb,
        "repetitionBitSize": 0x4,
        "size": 0x5912C,
        "offset": 0x150000,
        "compressedSize": 0x271C0,
        "compressionFlag": 0x4,
    }
]

class FileCache:
    def __init__(self) -> None:
        self.__byte_cache__ = {}
    
    def __load_file(self, file_name:str):
        assert(file_name not in self.__byte_cache__)

        with open(file_name, "rb") as f:
            self.__byte_cache__[file_name] = f.read()

    def get_file_bytes(self, file_name:str)->bytes:
        if file_name not in self.__byte_cache__:
            self.__load_file(file_name)

        return self.__byte_cache__[file_name]
    
file_cache = FileCache()

class CompressionData:
    def __init__(self, flag:int=None, data:int=None, look_back:int=None, length:int=None) -> None:
        self.flag = flag
        self.data = data
        self.look_back = look_back
        self.length = length
    ORIGINAL_DATA = 1
    REPETITION_DATA = 0
    
    def is_original_data(self):
        return self.flag == self.ORIGINAL_DATA
    
    def is_repeated_data(self):
        return self.flag == self.REPETITION_DATA

    def __str__(self):
        if self.flag == self.ORIGINAL_DATA:
            return f"OriginalData({self.data:02x})"
        elif self.flag == self.REPETITION_DATA:
            return f"RepetitionData(look_back={self.look_back}, length={self.length})"
        else:
            return ""
        
    def __repr__(self) -> str:
        return self.__str__()

class ArchiveDecompressor:
    def __init__(self, buffer:bytearray, lookback_bit_count:int, repetition_bit_count:int, original_size:int=None) -> None:
        self.bytes_to_decompress = bytearray(buffer)
        self.lookback_bit_count = lookback_bit_count
        self.repetition_bit_count = repetition_bit_count
        self.original_size = original_size
        self.__reset_buffer()
    
    @property
    def compressed_size(self):
        return self.__byte_index

    def __read_int(self)->int:
        if(self.__byte_index + 3 >= len(self.bytes_to_decompress)):
            raise ValueError("No more ints to read")

        value = int.from_bytes(self.bytes_to_decompress[self.__byte_index : self.__byte_index + 4], 'big')

        self.__byte_index += 4

        return value

    def __has_bits(self):
        if self.__byte_index >= len(self.bytes_to_decompress) - 1:
            return self.__bit_buffer != 0
        return True

    def __should_keep_decompressing(self, size:int):
        if self.original_size != None:
            return size < self.original_size
        return self.__has_bits()

    def __read_bits(self, bit_count:int):
        # if enough bits in buffer
        if bit_count <= self.__bits_in_buffer:
            # read the bits from the low end
            value = self.__bit_buffer & (2**bit_count - 1)
            # rotate out the bits
            self.__bit_buffer >>= bit_count
            # remove the count of those bits
            self.__bits_in_buffer -= bit_count
        else:
            # else not enough bits,
            # we need a new int
            new_buffer = self.__read_int()
            
            new_bits_needed = bit_count - self.__bits_in_buffer
            # the bits remaining in the buffer will be the high bits for the output data
            value = self.__bit_buffer << new_bits_needed

            self.__bits_in_buffer = 32 - new_bits_needed
            # the bits we're using from the new data will become the low bits of the output data
            value |= (new_buffer & (0xffffffff >> self.__bits_in_buffer))
            # rotate out the data
            self.__bit_buffer = new_buffer >> new_bits_needed

        return value
    
    def __reset_buffer(self):
        self.__byte_index = 0
        self.__bit_buffer = 0
        self.__bits_in_buffer = 0

    def is_valid_decompression(self) -> bool:
        if self.lookback_bit_count == 0 and self.repetition_bit_count == 0:
            return True
        
        self.__reset_buffer()

        written_bit_count = 0

        while self.__should_keep_decompressing(written_bit_count):
            # returns 0 or 1
            head_bit = self.__read_bits(1)
            if head_bit == 0:                
                far_back = self.__read_bits(self.lookback_bit_count)
                repetitions = self.__read_bits(self.repetition_bit_count) + 2
                if far_back >= written_bit_count:
                    # if there is a sequence requested to be read that is before the start of the array, then stop
                    return False
                written_bit_count += repetitions
            else:
                self.__read_bits(8)
                written_bit_count += 1
        return True
    
    def get_compression_instructions(self) -> list[CompressionData]:
        if self.lookback_bit_count == 0 and self.repetition_bit_count == 0:
            return []
        
        self.__reset_buffer()

        written_bit_count = 0
        instructions = []
        while self.__should_keep_decompressing(written_bit_count):
            # returns 0 or 1
            head_bit = self.__read_bits(1)
            if head_bit == 0:                
                far_back = self.__read_bits(self.lookback_bit_count)
                length = self.__read_bits(self.repetition_bit_count) + 2
                if far_back > written_bit_count:
                    # if there is a sequence requested to be read that is before the start of the array, then stop
                    raise ValueError("Invalid data, received too far lookback")

                instructions.append(CompressionData(flag=head_bit, look_back=far_back, length=length))
            else:
                instructions.append(CompressionData(flag=head_bit, data=self.__read_bits(8)))
                length = 1

            written_bit_count += length
        return instructions

    def decompress(self):
        if self.lookback_bit_count == 0 and self.repetition_bit_count == 0:
            if self.original_size != None:
                return self.bytes_to_decompress[:self.original_size]
            else:
                return bytearray()
        
        self.__reset_buffer()

        final_data = bytearray()

        while self.__should_keep_decompressing(len(final_data)):
            # returns 0 or 1
            head_bit = self.__read_bits(1)
            if head_bit == CompressionData.REPETITION_DATA:

                far_back = self.__read_bits(self.lookback_bit_count)
                
                repetitions = self.__read_bits(self.repetition_bit_count) + 2

                if far_back > len(final_data):
                    # if there is a sequence requested to be read that is before the start of the array, then stop
                    raise ValueError("Invalid data, received too far lookback")
                while repetitions != 0:
                    final_data.append(final_data[-1 - far_back])
                    repetitions -= 1
            else:
                final_data.append(self.__read_bits(8))

        return final_data

def decompress(d) -> bytearray:
    byte_data = file_cache.get_file_bytes(d["Input"])[d["offset"]: d["offset"] + d["compressedSize"]]
    return ArchiveDecompressor(byte_data, d["lookbackBitSize"], d["repetitionBitSize"]).decompress()

def main():
    if not exists(input_file):
        print(f"Could not find input file: {input_file}")
        exit()

    for rel in KNOWN_AAAA_FILES:
        if exists(rel["Output"]):
            continue
        out_bytes = decompress(rel)
        with open(rel["Output"], "wb") as f:
            f.write(out_bytes)

if __name__ == "__main__":
    main()