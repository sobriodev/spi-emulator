#ifndef SPI_EMULATOR_ALLTESTS_H
#define SPI_EMULATOR_ALLTESTS_H

#include "CppUTest/CommandLineTestRunner.h"

/* ------------------------------------------------------------------------- */
/* -------------------------------- Macros --------------------------------- */
/* ------------------------------------------------------------------------- */

/** Assert that pointer is nullptr */
#define POINTER_NULL(PTR) POINTERS_EQUAL(nullptr, (PTR))

/** Assert that pointer is not nullptr */
#define POINTER_NOT_NULL(PTR) CHECK_TRUE(nullptr != (PTR))

#endif //SPI_EMULATOR_ALLTESTS_H
