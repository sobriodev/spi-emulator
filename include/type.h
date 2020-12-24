#ifndef SPI_EMULATOR_TYPE_H
#define SPI_EMULATOR_TYPE_H

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------- */
/* ------------------------------ Data types ------------------------------- */
/* ------------------------------------------------------------------------- */

/** @{ */
/** Fixed width unsigned type */
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef size_t size;
/** @} */

/** @{ */
/** Fixed width signed type */
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
/** @} */

#ifdef __cplusplus
}
#endif

#endif //SPI_EMULATOR_TYPE_H
