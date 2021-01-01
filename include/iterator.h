#ifndef SPI_EMULATOR_ITERATOR_H
#define SPI_EMULATOR_ITERATOR_H

#include "type.h"
#include "common.h"

#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------- */
/* --------------------------------- Macros -------------------------------- */
/* ------------------------------------------------------------------------- */

/* Macros for convenience */
/** Call implementation specific begin function */
#define ITERATOR_CBEGIN(ITER) (ITER).begin.begin_const((ITER).context)

/** Call implementation specific next function */
#define ITERATOR_CNEXT(ITER)  (ITER).next.next_const((ITER).context)

/** Call implementation specific end function */
#define ITERATOR_CEND(ITER)   (ITER).end.end_const((ITER).context)

/** Call implementation specific const begin function */
#define ITERATOR_BEGIN(ITER)  (ITER).begin.begin_non_const((ITER).context)

/** Call implementation specific const next function */
#define ITERATOR_NEXT(ITER)   (ITER).next.next_non_const((ITER).context)

/** Call implementation specific const end function */
#define ITERATOR_END(ITER)    (ITER).end.end_non_const((ITER).context)

/** Iterate over non-const iterator */
#define ITERATOR_FOREACH(VAR, ITER) \
    for (void* VAR = ITERATOR_BEGIN((ITER)); VAR != ITERATOR_END((ITER)); VAR = ITERATOR_NEXT((ITER)))

/** Iterate over const iterator */
#define ITERATOR_FOREACH_CONST(VAR, ITER) \
    for (const void* VAR = ITERATOR_CBEGIN((ITER)); VAR != ITERATOR_CEND((ITER)); VAR = ITERATOR_CNEXT((ITER)))

/* ------------------------------------------------------------------------- */
/* ------------------------------- Data types ------------------------------ */
/* ------------------------------------------------------------------------- */

/* Const iterator function types */
/** Function type which returns const iterator pointing to the first element in a sequence */
typedef const void* (*iterator_const_begin)(void* context);
/** Function type which returns const iterator to the next element in a sequence */
typedef const void* (*iterator_const_next)(void* context);
/** Function type which returns const iterator pointing to the past-the-end element in a sequence */
typedef const void* (*iterator_const_end)(void* context);

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
    union begin_
    {
        iterator_const_begin begin_const; /**< Const begin implementation */
        iterator_begin begin_non_const; /**< Non-const begin implementation */
    } begin;
    union next_
    {
        iterator_const_next next_const; /**< Const next implementation */
        iterator_next next_non_const; /**< Non-const next implementation */
    } next;
    union end_
    {
        iterator_const_end end_const; /**< Const end implementation */
        iterator_end end_non_const; /**< Non-const end implementation */
    } end;
} iterator_instance;

/**
 * Status codes returned by API functions
 */
typedef enum iterator_status_
{
    iterator_status_ok, /**< OK */
    iterator_status_merror, /**< Memory allocator failed (system out of memory) */
    iterator_status_iptr /**< Unexpected invalid pointer (typically NULL) occurred */
} iterator_status;

/* ------------------------------------------------------------------------- */
/* ----------------------------- Api functions ----------------------------- */
/* ------------------------------------------------------------------------- */

/**
 * Construct iterator instance.
 *
 * This function allocates resources for context space. Other fields remain untouched and must be explicitly
 * initialized by calling iterator_init_as_const() or iterator_init_as_non_const. After all the iterator has to be
 * deconstructed or memory leaks are guaranteed.
 *
 * @param iterator Pointer to an iterator instance.
 * @param user_data_len The number of bytes which need to be reserved.
 * @param allocator Pointer to a custom memory allocator.
 *
 * @return Operation status. Valid values are:
 *          - iterator_status_iptr when NULL was passed instead of a valid pointer
 *          - iterator_status_merror when memory allocator failed
 *          - iterator_status_ok on success
 */
iterator_status iterator_construct_ext(iterator_instance* iterator, size user_data_len, mem_allocator allocator);

/**
 * Construct iterator instance with a default memory allocator.
 *
 * This function is a shortcut for iterator_construct_ext() function and can be used when a custom allocator is not
 * needed (in this case malloc() is used).
 *
 * @param iterator Pointer to an iterator instance.
 * @param user_data_len The number of context bytes needed.
 *
 * @return Operation status. Return values are the same as for iterator_construct_ext().
 */
static inline iterator_status iterator_construct(iterator_instance* iterator, size user_data_len)
{
    return iterator_construct_ext(iterator, user_data_len, malloc);
}

/**
 * Destruct iterator instance.
 *
 * The function frees context memory using custom deallocator and invalidate the pointer.
 * After the operation iterator cannot be used anymore. Passing destructed iterator is totally valid - in this case
 * nothing is done.
 *
 * @param iterator Pointer to an iterator instance.
 * @param deallocator Custom memory deallocator
 *
 * @return iterator_status_iptr when NULL was passed instead of a valid pointer, iterator_status_ok otherwise.
 */
iterator_status iterator_destruct_ext(iterator_instance* iterator, mem_deallocator deallocator);

/**
 * Destruct iterator with a default deallocator.
 *
 * This function is a shortcut for iterator_destruct_ext() and uses free() as a de-allocation function.
 *
 * @param iterator Pointer to an iterator.
 *
 * @return Operation status. Valid return values are the same as for iterator_destruct_ext().
 */
static inline iterator_status iterator_destruct(iterator_instance* iterator)
{
    return iterator_destruct_ext(iterator, free);
}

/**
 * Initialize iterator as a const iterator (populated with immutable elements).
 *
 * The function assigns passed function pointers and set type field, therefore makes possible reusing the API with
 * many specific implementations (eg. traversing an array or a linked list).
 *
 * @param iterator Pointer to an iterator instance.
 * @param beginFn Begin function pointer.
 * @param nextFn Next function pointer.
 * @param end End function pointer.
 *
 * @return iterator_status_ok on success, iterator_status_iptr otherwise.
 */
iterator_status iterator_init_as_const(iterator_instance* iterator,
                                       iterator_const_begin beginFn,
                                       iterator_const_next nextFn,
                                       iterator_const_end end);

/**
 * Initialize iterator as a non-const iterator (populated with mutable elements).
 *
 * The function assigns passed function pointers and set type field, therefore makes possible reusing the API with
 * many specific implementations (eg. traversing an array or a linked list).
 *
 * @param iterator Pointer to an iterator instance.
 * @param beginFn Begin function pointer.
 * @param nextFn Next function pointer.
 * @param end End function pointer.
 *
 * @return iterator_status_ok on success, iterator_status_iptr otherwise.
 */
iterator_status iterator_init_as_non_const(iterator_instance* iterator,
                                           iterator_begin beginFn,
                                           iterator_next nextFn,
                                           iterator_end);

/**
 * Check if the iterator is constructed.
 *
 * The function returns true if pointer to the context memory is not NULL. It may help with cases when user code do not
 * destroys actual iterator before reusing it. Passing an uninitialized iterator (without calling construct function
 * first) has an undefined behaviour.
 *
 * @param iter Pointer to an iterator instance.
 *
 * @return True if a valid pointer was passed as well as the iterator is actually destructed, false otherwise.
 */
static inline bool iterator_is_constructed(const iterator_instance* iter)
{
    NOT_NULL(iter, false);
    return (NULL != iter->context);
}

#ifdef __cplusplus
}
#endif

#endif //SPI_EMULATOR_ITERATOR_H
