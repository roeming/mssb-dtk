#include "types.h"

extern f64 __ieee754_acos(f64 __x);

/**
 * @note Address: 0x800CF96C
 * @note Size: 0x20
 */

f64 acos(f64 __x) { return (f64)__ieee754_acos(__x); }
