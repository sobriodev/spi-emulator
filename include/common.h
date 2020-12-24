#ifndef SPI_EMULATOR_COMMON_H
#define SPI_EMULATOR_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------- */
/* --------------------------------- Macros -------------------------------- */
/* ------------------------------------------------------------------------- */

#ifdef __glibc_likely
#define LIKELY __glibc_likely
#else
#define LIKELY
#endif
#ifdef __glibc_unlikely
#define UNLIKELY __glibc_unlikely
#else
#define UNLIKELY
#endif

/** Return certain status code on unexpected NULL */
#define NOT_NULL(PTR, STATUS) if (UNLIKELY(NULL == (PTR))) return (STATUS)

#ifdef __cplusplus
}
#endif

#endif //SPI_EMULATOR_COMMON_H
