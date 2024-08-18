#!/usr/bin/env python3

###
# Generates build files for the project.
# This file also includes the project configuration,
# such as compiler flags and the object matching status.
#
# Usage:
#   python3 configure.py
#   ninja
#
# Append --help to see available options.
###

import argparse
import sys
from pathlib import Path
from typing import Any, Dict, List

from tools.project import (
    Object,
    ProjectConfig,
    calculate_progress,
    generate_build,
    is_windows,
)

# Game versions
DEFAULT_VERSION = 0
VERSIONS = [
    "GYQE01",  # 0
]

parser = argparse.ArgumentParser()
parser.add_argument(
    "mode",
    choices=["configure", "progress"],
    default="configure",
    help="script mode (default: configure)",
    nargs="?",
)
parser.add_argument(
    "-v",
    "--version",
    choices=VERSIONS,
    type=str.upper,
    default=VERSIONS[DEFAULT_VERSION],
    help="version to build",
)
parser.add_argument(
    "--build-dir",
    metavar="DIR",
    type=Path,
    default=Path("build"),
    help="base build directory (default: build)",
)
parser.add_argument(
    "--binutils",
    metavar="BINARY",
    type=Path,
    help="path to binutils (optional)",
)
parser.add_argument(
    "--compilers",
    metavar="DIR",
    type=Path,
    help="path to compilers (optional)",
)
parser.add_argument(
    "--map",
    action="store_true",
    help="generate map file(s)",
)
parser.add_argument(
    "--debug",
    action="store_true",
    help="build with debug info (non-matching)",
)
if not is_windows():
    parser.add_argument(
        "--wrapper",
        metavar="BINARY",
        type=Path,
        help="path to wibo or wine (optional)",
    )
parser.add_argument(
    "--dtk",
    metavar="BINARY | DIR",
    type=Path,
    help="path to decomp-toolkit binary or source (optional)",
)
parser.add_argument(
    "--objdiff",
    metavar="BINARY | DIR",
    type=Path,
    help="path to objdiff-cli binary or source (optional)",
)
parser.add_argument(
    "--sjiswrap",
    metavar="EXE",
    type=Path,
    help="path to sjiswrap.exe (optional)",
)
parser.add_argument(
    "--verbose",
    action="store_true",
    help="print verbose output",
)
parser.add_argument(
    "--non-matching",
    dest="non_matching",
    action="store_true",
    help="builds equivalent (but non-matching) or modded objects",
)
args = parser.parse_args()

config = ProjectConfig()
config.version = str(args.version)
version_num = VERSIONS.index(config.version)

# Apply arguments
config.build_dir = args.build_dir
config.dtk_path = args.dtk
config.objdiff_path = args.objdiff
config.binutils_path = args.binutils
config.compilers_path = args.compilers
config.debug = args.debug
config.generate_map = args.map
config.non_matching = args.non_matching
config.sjiswrap_path = args.sjiswrap
if not is_windows():
    config.wrapper = args.wrapper
# Don't build asm unless we're --non-matching
if not config.non_matching:
    config.asm_dir = None

# Tool versions
config.binutils_tag = "2.42-1"
config.compilers_tag = "20240706"
config.dtk_tag = "v0.9.4"
config.objdiff_tag = "v2.0.0-beta.3"
config.sjiswrap_tag = "v1.1.1"
config.wibo_tag = "0.6.11"

# Project
config.config_path = Path("config") / config.version / "config.yml"
config.check_sha_path = Path("config") / config.version / "build.sha1"
config.asflags = [
    "-mgekko",
    "--strip-local-absolute",
    "-I include",
    f"-I build/{config.version}/include",
    f"--defsym version={version_num}",
]
config.ldflags = [
    "-fp hardware",
    "-nodefaults",
    "-warn off",
    # "-listclosure", # Uncomment for Wii linkers
]
# Use for any additional files that should cause a re-configure when modified
config.reconfig_deps = []

# Base flags, common to most GC/Wii games.
# Generally leave untouched, with overrides added below.
cflags_base = [
    "-proc gekko",
    "-nodefaults",
    "-Cpp_exceptions off",
    "-RTTI off",
    "-fp hard",
    "-fp_contract on",
    "-O4,p",
    "-maxerrors 1",
    "-enum int",
    "-inline auto",
    "-str reuse,readonly",
    "-nosyspath",
    "-use_lmw_stmw on",
    "-sdata 8",
    "-sdata2 8",
    "-DVERNUM={version_num}",
    "-i include",
    "-i include/stl"
]

# Debug flags
if config.debug:
    cflags_base.extend(["-sym on", "-DDEBUG=1"])
else:
    cflags_base.append("-DNDEBUG=1")

# Metrowerks library flags
cflags_runtime = [
    *cflags_base,
    "-use_lmw_stmw on",
    "-str reuse,pool,readonly",
    "-gccinc",
    "-common off",
    "-inline auto",
]

# REL flags
cflags_rel = [
    *cflags_base,
    "-sdata 0",
    "-sdata2 0",
]

config.linker_version = "GC/1.3.2"


# Helper function for Dolphin libraries
def DolphinLib(lib_name: str, objects: List[Object], flags: str = cflags_base, extra_cflags: list[str] = []) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.2.5n",
        "cflags": flags + extra_cflags,
        "host": False,
        "objects": objects,
    }
def RuntimeLib(lib_name, objects, flags = cflags_base, extra_cflags=[]):
    return {
        "lib": lib_name,
        "mw_version": "GC/2.6",
        "cflags": flags + extra_cflags,
        "host": False,
        "objects": objects,
    }
def MusyxLib(lib_name, objects, flags = cflags_base, extra_cflags=[]):
    return {
        "lib": lib_name,
        "mw_version": "GC/1.3.2",
        "cflags": flags + extra_cflags,
        "host": False,
        "objects": objects,
    }
def DebuggerLib(lib_name, objects, flags = cflags_base, extra_cflags=[]):
    return {
        "lib": lib_name,
        "mw_version": "GC/1.2.5",
        "cflags": flags + extra_cflags,
        "host": False,
        "objects": objects,
    }
def TRKLib(lib_name, objects, flags = cflags_base, extra_cflags=[]):
    return {
        "lib": lib_name,
        "mw_version": "GC/2.6",
        "cflags": flags + extra_cflags,
        "host": False,
        "objects": objects,
    }

# Helper function for REL script objects
def Rel(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.3.2",
        "cflags": cflags_rel,
        "host": True,
        "objects": objects,
    }

Matching = True                   # Object matches and should be linked
NonMatching = False               # Object does not match and should not be linked
Equivalent = config.non_matching  # Object should be linked when configured with --non-matching

config.warn_missing_config = True
config.warn_missing_source = False
config.libs = [
    RuntimeLib(
        "Runtime",
        [
            Object(Matching, "Dolphin/Runtime/__va_arg.c"),
            Object(Matching, "Dolphin/Runtime/global_destructor_chain.c"),
            Object(Matching, "Dolphin/Runtime/runtime.c"),
            Object(Matching, "Dolphin/Runtime/__init_cpp_exceptions.cpp"),
            Object(Matching, "Dolphin/Runtime/Gecko_ExceptionPPC.cp",
                extra_cflags=["-inline deferred", "-Cpp_exceptions on"],
            ),
            Object(Matching, "Dolphin/Runtime/GCN_mem_alloc.c"),
            Object(Matching, "Dolphin/Runtime/__mem.c", 
                   extra_cflags=["-inline deferred"]),
        ],
    ),
    RuntimeLib(
        "MSL_C",
        [
            Object(Matching, "Dolphin/MSL_C/PPC_EABI/abort_exit.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/alloc.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/ansi_files.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/ansi_fp.c",
                   extra_cflags=["-inline deferred", "-str pool"],
            ),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/buffer_io.c"),
            Object(Matching, "Dolphin/MSL_C/PPC_EABI/critical_regions.gamecube.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/ctype.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/direct_io.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/errno.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/file_io.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/FILE_POS.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/mbstring.c",
                   extra_cflags=["-inline noauto,deferred"],
            ),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/mem.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/mem_funcs.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/misc_io.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/printf.c",
                   extra_cflags=["-inline deferred", "-str pool"],
            ),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/rand.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/string.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/wchar_io.c"),
            Object(Matching, "Dolphin/MSL_C/PPC_EABI/uart_console_io_gcn.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_acos.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_asin.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_atan2.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_fmod.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_pow.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_rem_pio2.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_cos.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_rem_pio2.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_sin.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_tan.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_atan.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_copysign.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_cos.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_floor.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_frexp.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_ldexp.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_modf.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_sin.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_tan.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_acos.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_asin.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_atan2.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_exp.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_fmod.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_pow.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_sqrt.c"),
            Object(Matching, "Dolphin/MSL_C/PPC_EABI/math_ppc.c"),
        ],
        extra_cflags=["-inline deferred"]
    ),
    DolphinLib(
        "base",
        [
            Object(Matching, "Dolphin/base/PPCArch.c"),
        ],
    ),
    DolphinLib(
        "OS",
        [
            Object(Matching, "Dolphin/os/OS.c"),
            Object(Matching, "Dolphin/os/OSAlarm.c"),
            Object(Matching, "Dolphin/os/OSAlloc.c"),
            Object(Matching, "Dolphin/os/OSArena.c"),
            Object(Matching, "Dolphin/os/OSAudioSystem.c"),
            Object(Matching, "Dolphin/os/OSCache.c"),
            Object(Matching, "Dolphin/os/OSContext.c"),
            Object(Matching, "Dolphin/os/OSError.c"),
            Object(NonMatching, "Dolphin/os/OSExec.c"),
            Object(Matching, "Dolphin/os/OSFont.c"),
            Object(Matching, "Dolphin/os/OSInterrupt.c"),
            Object(Matching, "Dolphin/os/OSLink.c"),
            Object(Matching, "Dolphin/os/OSMemory.c"),
            Object(Matching, "Dolphin/os/OSMessage.c"),
            Object(Matching, "Dolphin/os/OSMutex.c"),
            Object(Matching, "Dolphin/os/OSReboot.c"),
            Object(Matching, "Dolphin/os/OSReset.c"),
            Object(Matching, "Dolphin/os/OSResetSW.c"),
            Object(Matching, "Dolphin/os/OSRtc.c"),
            Object(Matching, "Dolphin/os/OSSemaphore.c"),
            Object(Matching, "Dolphin/os/OSSync.c"),
            Object(Matching, "Dolphin/os/OSThread.c"),
            Object(Matching, "Dolphin/os/OSTime.c"),
            Object(Matching, "Dolphin/os/__ppc_eabi_init.cpp"),
        ],
        extra_cflags=["-str noreadonly", "-sym on"]
    ),
    DolphinLib(
        "db",
        [
            Object(Matching, "Dolphin/db/db.c")
        ],
        extra_cflags=["-str noreadonly"]
    ),
    DolphinLib(
        "mtx",
        [
            Object(Matching, "Dolphin/mtx/mtx.c"),
            Object(Matching, "Dolphin/mtx/mtxvec.c"),
            Object(Matching, "Dolphin/mtx/mtx44.c"),
            Object(Matching, "Dolphin/mtx/mtx44vec.c"),
            Object(Matching, "Dolphin/mtx/vec.c"),
            Object(Matching, "Dolphin/mtx/quat.c"),
            Object(Matching, "Dolphin/mtx/psmtx.c"),
        ],
        extra_cflags=["-fp_contract off"]
    ),
    DolphinLib(
        "dvd",
        [
            Object(Matching, "Dolphin/dvd/dvdlow.c"),
            Object(Matching, "Dolphin/dvd/dvdfs.c"),
            Object(Matching, "Dolphin/dvd/dvd.c"),
            Object(Matching, "Dolphin/dvd/dvdqueue.c"),
            Object(Matching, "Dolphin/dvd/dvderror.c"),
            Object(Matching, "Dolphin/dvd/dvdidutils.c"),
            Object(Matching, "Dolphin/dvd/dvdfatal.c"),
            Object(Matching, "Dolphin/dvd/fstload.c"),
        ],
        extra_cflags=["-str noreadonly"]
    ),
    DolphinLib(
        "vi",
        [
            Object(Matching, "Dolphin/vi/vi.c")
        ],
        extra_cflags=["-str noreadonly"]
    ),
    DolphinLib(
        "pad",
        [
            Object(Matching, "Dolphin/pad/Padclamp.c"),
            Object(Matching, "Dolphin/pad/Pad.c"),
        ],
        extra_cflags=["-str noreadonly"]
    ),
    DolphinLib(
        "ai",
        [
            Object(Matching, "Dolphin/ai/ai.c"),
        ],
        extra_cflags=["-str noreadonly"]
    ),
    DolphinLib(
        "ar",
        [
            Object(Matching, "Dolphin/ar/ar.c"),
            Object(Matching, "Dolphin/ar/arq.c"),
        ],
        extra_cflags=["-str noreadonly"]
    ),
    DolphinLib(
        "dsp",
        [
            Object(Matching, "Dolphin/dsp/dsp.c"),
            Object(Matching, "Dolphin/dsp/dsp_debug.c"),
            Object(Matching, "Dolphin/dsp/dsp_task.c"),
        ],
        extra_cflags=["-str noreadonly"]
    ),
    DolphinLib(
        "card",
        [
            Object(Matching, "Dolphin/card/CARDBios.c"),
            Object(Matching, "Dolphin/card/CARDUnlock.c"),
            Object(Matching, "Dolphin/card/CARDRdwr.c"),
            Object(Matching, "Dolphin/card/CARDBlock.c"),
            Object(Matching, "Dolphin/card/CARDDir.c"),
            Object(Matching, "Dolphin/card/CARDCheck.c"),
            Object(Matching, "Dolphin/card/CARDMount.c"),
            Object(Matching, "Dolphin/card/CARDFormat.c"),
            Object(Matching, "Dolphin/card/CARDOpen.c"),
            Object(Matching, "Dolphin/card/CARDCreate.c"),
            Object(Matching, "Dolphin/card/CARDRead.c"),
            Object(Matching, "Dolphin/card/CARDWrite.c"),
            Object(Matching, "Dolphin/card/CARDDelete.c"),
            Object(Matching, "Dolphin/card/CARDStat.c"),
            Object(Matching, "Dolphin/card/CARDStatEx.c"),
            Object(Matching, "Dolphin/card/CARDNet.c"),
        ],
        extra_cflags=["-str noreadonly"]
    ),
    
    DolphinLib(
        "gx",
        [
            Object(Matching, "Dolphin/gx/GXInit.c"),
            Object(Matching, "Dolphin/gx/GXFifo.c"),
            Object(Matching, "Dolphin/gx/GXAttr.c"),
            Object(Matching, "Dolphin/gx/GXMisc.c"),
            Object(Matching, "Dolphin/gx/GXGeometry.c"),
            Object(Matching, "Dolphin/gx/GXFrameBuf.c"),
            Object(Matching, "Dolphin/gx/GXLight.c"),
            Object(Matching, "Dolphin/gx/GXTexture.c"),
            Object(Matching, "Dolphin/gx/GXBump.c"),
            Object(Matching, "Dolphin/gx/GXTev.c"),
            Object(Matching, "Dolphin/gx/GXPixel.c"),
            Object(Matching, "Dolphin/gx/GXDraw.c"),
            Object(Matching, "Dolphin/gx/GXDisplayList.c"),
            Object(Matching, "Dolphin/gx/GXTransform.c"),
            Object(Matching, "Dolphin/gx/GXPerf.c"),
        ],
        extra_cflags=["-common off", "-str noreadonly", "-fp_contract off"]
    ),
    TRKLib(
        "TRK",
        [
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/mainloop.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/nubevent.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/nubinit.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/msg.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/msgbuf.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/serpoll.c",
                   extra_cflags=["-inline deferred", "-sdata 8"]),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/usr_put.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/dispatch.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/msghndlr.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/support.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/mutex_TRK.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/notify.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/flush_cache.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/mem_TRK.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/targimpl.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/targsupp.c",
                   extra_cflags=["-inline deferred", "-func_align 16"]),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/mpc_7xx_603e.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/dolphin_trk.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/main_TRK.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/dolphin_trk_glue.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/targcont.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/target_options.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/mslsupp.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/UDP_stubs.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/ddh/main.c",
                   extra_cflags=["-inline deferred", "-sdata 8"]),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/CircleBuffer.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/gdev/main.c",
                   extra_cflags=["-inline deferred", "-sdata 8"]),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/MWTRACE.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/MWCriticalSection_gc.cpp"),
        ],
        extra_cflags=["-inline deferred", "-sdata 0", "-sdata2 0"]
    ),
    DolphinLib(
        "amcstubs",
        [
            Object(Matching, "Dolphin/amcstubs/AmcExi2Stubs.c"),
        ]
    ),
    DolphinLib(
        "odenotstub",
        [
            Object(Matching, "Dolphin/odenotstub/odenotstub.c"),
        ]
    ),
    DebuggerLib(
        "OdemuExi2",
        [
            Object(Matching, "Dolphin/OdemuExi2/DebuggerDriver.c"),
        ]
    ),
    DolphinLib(
        "exi",
        [
            Object(Matching, "Dolphin/exi/EXIBios.c"),
            Object(Matching, "Dolphin/exi/EXIUart.c"),
        ],
        extra_cflags=["-str noreadonly"]
    ),
    DolphinLib(
        "si",
        [
            Object(Matching, "Dolphin/si/SIBios.c"),
            Object(Matching, "Dolphin/si/SISamplingRate.c"),
        ],
        extra_cflags=["-str noreadonly"]
    ),
    {
        "lib": "unk",
        "mw_version": "GC/2.6",
        "cflags": cflags_base + ["-O4,p", "-sym on"],
        "host": False,
        "objects": [
            Object(NonMatching, "Unknown/File_0x800a6304.c", extra_cflags=["-cpp_exceptions on"]),
            Object(NonMatching, "Unknown/File_0x800a64e0.c", extra_cflags=["-cpp_exceptions on"]),
        ],
    },
    {
        "lib": "C3",
        "mw_version": "GC/1.3.2",
        "cflags": cflags_base + ["-O4,p", "-sym on", "-cpp_exceptions on"],
        "host": False,
        "objects": [
            Object(NonMatching, "C3/control/control.c", extra_cflags=["-inline deferred"]),
        ],
    },
    MusyxLib(
        "musyx",
        [
            Object(Matching, "Musyx/seq.c"),
            Object(NonMatching, "Musyx/synth.c"),
            Object(Matching, "Musyx/seq_api.c"),
            Object(Matching, "Musyx/snd_synthapi.c"),
            Object(NonMatching, "Musyx/stream.c"),
            Object(NonMatching, "Musyx/synthdata.c"),
            Object(NonMatching, "Musyx/synthmacros.c"),
            Object(NonMatching, "Musyx/synthvoice.c"),
            Object(Matching, "Musyx/synth_ac.c"),
            Object(NonMatching, "Musyx/synth_adsr.c"),
            Object(NonMatching, "Musyx/synth_vsamples.c"),
            Object(NonMatching, "Musyx/s_data.c"),
            Object(NonMatching, "Musyx/hw_dspctrl.c"),
            Object(NonMatching, "Musyx/hw_volconv.c"),
            Object(NonMatching, "Musyx/snd3d.c"),
            Object(Matching, "Musyx/snd_init.c"),
            Object(NonMatching, "Musyx/snd_math.c"),
            Object(NonMatching, "Musyx/snd_midictrl.c"),
            Object(NonMatching, "Musyx/snd_service.c"),
            Object(NonMatching, "Musyx/hardware.c"),
            Object(NonMatching, "Musyx/hw_aramdma.c"),
            Object(NonMatching, "Musyx/hw_dolphin.c"),
            Object(Matching, "Musyx/hw_memory.c"),
            Object(Matching, "Musyx/reverb_fx.c"),
            Object(Matching, "Musyx/reverb.c"),
            Object(Matching, "Musyx/chorus_fx.c"),
        ],
        flags=[
            "-proc gekko",
            "-nodefaults",
            "-Cpp_exceptions off",
            "-RTTI off",
            "-fp hard",
            "-fp_contract on",
            "-O4,p",
            "-maxerrors 1",
            "-enum int",
            "-inline auto",
            "-str reuse",
            "-nosyspath",
            "-i include",
            "-i include/stl",
            f"-DVERSION={version_num}",
            "-DNONMATCHING=0",],
        extra_cflags=["-str reuse,pool,readonly",
                      "-fp_contract off", 
                      "-use_lmw_stmw off"]
    ),
]

if args.mode == "configure":
    # Write build.ninja and objdiff.json
    generate_build(config)
elif args.mode == "progress":
    # Print progress and write progress.json
    config.progress_each_module = args.verbose
    calculate_progress(config)
else:
    sys.exit("Unknown mode: " + args.mode)
