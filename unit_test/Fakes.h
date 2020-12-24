#ifndef SPI_EMULATOR_FAKES_H
#define SPI_EMULATOR_FAKES_H

#include "type.h"

/* ------------------------------------------------------------------------- */
/* ----------------------------- Api functions ----------------------------- */
/* ------------------------------------------------------------------------- */

/**
 * Fake memory allocation implementation.
 *
 * The function always returns nullptr, thus can be used for testing "null branches" in production code.
 *
 * @param len Number of bytes to reserve (not used by implementation).
 * @return Always nullptr.
 */
void* FakeMalloc(size len);

#endif //SPI_EMULATOR_FAKES_H
