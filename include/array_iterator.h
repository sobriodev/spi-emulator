#ifndef SPI_EMULATOR_ARRAY_ITERATOR_H
#define SPI_EMULATOR_ARRAY_ITERATOR_H

#include "type.h"
#include "iterator.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------- */
/* ------------------------------- Data types ------------------------------ */
/* ------------------------------------------------------------------------- */

/**
 * Array iterator context
 */
typedef struct array_iterator_ctx_
{
    union array_addr_
    {
        const void* addr_const; /**< Address of the first array element (const version) */
        void* addr_non_const; /**< Address of the first array element (non-const version) */
    } array_addr;
    size num_of_elements; /**< The number of elements in the array */
    size element_size; /**< Size of an element */
    size current_element_idx; /**< Id of the current element. Do not use directly */
} array_iterator_ctx;

/**
 * Status codes returned by API functions
 */
typedef enum array_iterator_status_
{
    array_iterator_status_ok, /**< Success */
    array_iterator_status_iptr, /**< NULL pointer passed instead of a valid pointer */
    array_iterator_status_cerror /**< An error occurred while setting up the context */
} array_iterator_status;

/* ------------------------------------------------------------------------- */
/* ----------------------------- Api functions ----------------------------- */
/* ------------------------------------------------------------------------- */

/**
 * Initialize iterator context for array traversing (const version).
 *
 * The function sets up all needed fields. After the operation iterator is ready to use.
 *
 * @param iterator Pointer to an iterator instance. It has to be preconfigured as a const type as well as the context
 *                 memory must be allocated.
 * @param array Address of the array.
 * @param elements Total numbers of elements in the array.
 * @param element_size The size of a single element. Cannot be zero.
 *
 * @return Valid return codes are:
 *          - array_iterator_status_iptr when NULL was passed instead of a valid pointer
 *          - array_iterator_status_cerror when one or more parameters are invalid
 *          - array_iterator_status_ok on success
 */
array_iterator_status array_iterator_init_const_ctx(iterator_instance* iterator,
                                                    const void* array,
                                                    size elements,
                                                    size element_size);

/**
 * Initialize iterator context for array traversing (non-const version).
 *
 * The function sets up all needed fields. AAfter the operation iterator is ready to use.
 *
 * @param iterator Pointer to an iterator instance. It has to be preconfigured as a non-const type as well as the
 *                 context memory must be allocated.
 * @param array Address of the first array element.
 * @param elements Total numbers of elements in the array.
 * @param element_size The size of a single element. Cannot be zero.
 *
 * @return Valid return codes are:
 *          - array_iterator_status_iptr when NULL was passed instead of a valid pointer
 *          - array_iterator_status_cerror when one or more parameters are invalid
 *          - array_iterator_status_ok on success
 */
array_iterator_status array_iterator_init_ctx(iterator_instance* iterator,
                                              void* array,
                                              size elements,
                                              size element_size);

/**
 * Return const iterator pointing to the first element in the array.
 *
 * @param context Pointer to an iterator context.
 *
 * @return Address of the element that must be explicitly casted to a desired type afterwards.
 */
const void* array_iterator_const_begin(void* context);

/**
 * Return const iterator pointing to the next element in the array.
 *
 * @param context Pointer to an iterator context.
 *
 * @return Address of the element that must be explicitly casted to a desired type afterwards.
 */
const void* array_iterator_const_next(void* context);

/**
 * Return const iterator pointing to the past-the-end element in the array.
 *
 * @param context Pointer to an iterator context.
 *
 * @return Address of the past-the-end element.
 */
const void* array_iterator_const_end(void* context);

/**
 * Return non-const iterator pointing to the first element in the array.
 *
 * @param context Pointer to an iterator context.
 *
 * @return Address of the element that must be explicitly casted to a desired type afterwards.
 */
void* array_iterator_begin(void* context);

/**
 * Return non-const iterator pointing to the next element in the array.
 *
 * @param context Pointer to an iterator context.
 *
 * @return Address of the element that must be explicitly casted to a desired type afterwards.
 */
void* array_iterator_next(void* context);

/**
 * Return non-const iterator pointing to the past-the-end element in the array.
 *
 * @param context Pointer to an iterator context.
 *
 * @return Address of the past-the-end element.
 */
void* array_iterator_end(void* context);

#ifdef __cplusplus
}
#endif

#endif //SPI_EMULATOR_ARRAY_ITERATOR_H
