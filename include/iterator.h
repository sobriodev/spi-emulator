#ifndef SPI_EMULATOR_ITERATOR_H
#define SPI_EMULATOR_ITERATOR_H

#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------- */
/* ------------------------------- Data types ------------------------------ */
/* ------------------------------------------------------------------------- */

/* Const iterator function types */
/** Function type which returns const iterator pointing to the first element in a sequence */
typedef const void* (*const_iterator_begin)(void* context);
/** Function type which returns const iterator to the next element in a sequence */
typedef const void* (*const_iterator_next)(void* context);
/** Function type which returns const iterator pointing to the past-the-end element in a sequence */
typedef const void* (*const_iterator_end)(void* context);

/* Non-const iterator function types */
/** Function type which returns non-const iterator pointing to the first element in a sequence */
typedef void* (*iterator_begin)(void* context);
/** Function type which returns non-const iterator to the next element in a sequence */
typedef void* (*iterator_next)(void* context);
/** Function type which returns non-const iterator pointing to the past-the-end element in a sequence */
typedef void* (*iterator_end)(void* context);

/**
 * Supported iterator types
 */
typedef enum iterator_type_
{
    iterator_type_non_const, /**< Non-const iterator */
    iterator_type_const, /**< Const iterator */
} iterator_type;

/**
 * Iterator instance struct
 */
typedef struct iterator_instance_
{
    void* context; /**< User data managed by a specific iterator implementation. Do not use directly */
    iterator_type type; /**< Iterator type */
    union
    {
        const_iterator_begin begin_const; /**< Const begin implementation */
        iterator_begin begin_non_const; /**< Non-const begin implementation */
    } begin;
    union
    {
        const_iterator_next next_const; /**< Const next implementation */
        iterator_next next_non_const; /**< Non-const next implementation */
    } next;
    union
    {
        const_iterator_end end_const; /**< Const end implementation */
        iterator_end end_non_const; /**< Non-const end implementation */
    } end;
} iterator_instance;

/* ------------------------------------------------------------------------- */
/* ----------------------------- Api functions ----------------------------- */
/* ------------------------------------------------------------------------- */

//TODO add docs
void iterator_construct_ext(iterator_instance* iterator, size user_data_len, mem_allocator allocator);
//TODO add docs
void iterator_destruct(iterator_instance iterator);

#ifdef __cplusplus
}
#endif

#endif //SPI_EMULATOR_ITERATOR_H
