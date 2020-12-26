#include "array_iterator.h"
#include "common.h"

/* ------------------------------------------------------------------------- */
/* --------------------------- Private functions --------------------------- */
/* ------------------------------------------------------------------------- */

/* These two functions allow pointer arithmetic by casting void pointer to char pointer */
static inline const i8* array_begin_as_const_char_ptr(const array_iterator_ctx* ctx)
{
    return ctx->array_addr.addr_const;
}

static inline i8* array_begin_as_char_ptr(const array_iterator_ctx* ctx)
{
    return ctx->array_addr.addr_non_const;
}

/* ------------------------------------------------------------------------- */
/* ----------------------------- Api functions ----------------------------- */
/* ------------------------------------------------------------------------- */

array_iterator_status array_iterator_init_const_ctx(iterator_instance* iterator,
                                                    const void* array,
                                                    size elements,
                                                    size element_size)
{
    NOT_NULL(iterator, array_iterator_status_iptr);
    NOT_NULL(array, array_iterator_status_iptr);

    if (iterator_type_const != iterator->type || 0 == element_size) {
        return array_iterator_status_cerror;
    }

    array_iterator_ctx* ctx = iterator->context;
    ctx->array_addr.addr_const = array;
    ctx->num_of_elements = elements;
    ctx->element_size = element_size;

    return array_iterator_status_ok;
}

array_iterator_status array_iterator_init_ctx(iterator_instance* iterator,
                                              void* array,
                                              size elements,
                                              size element_size)
{
    NOT_NULL(iterator, array_iterator_status_iptr);
    NOT_NULL(array, array_iterator_status_iptr);

    if (iterator_type_non_const != iterator->type || 0 == element_size) {
        return array_iterator_status_cerror;
    }

    array_iterator_ctx* ctx = iterator->context;
    ctx->array_addr.addr_non_const = array;
    ctx->num_of_elements = elements;
    ctx->element_size = element_size;

    return array_iterator_status_ok;
}

const void* array_iterator_const_begin(void* context)
{
    array_iterator_ctx* ctx = context;
    ctx->current_element_idx = 0;
    return array_begin_as_const_char_ptr(ctx);
}

const void* array_iterator_const_next(void* context)
{
    array_iterator_ctx* ctx = context;
    ++ctx->current_element_idx;
    return array_begin_as_const_char_ptr(ctx) + ctx->current_element_idx * ctx->element_size;
}

const void* array_iterator_const_end(void* context)
{
    array_iterator_ctx* ctx = context;
    return array_begin_as_const_char_ptr(ctx) + ctx->num_of_elements * ctx->element_size;
}

void* array_iterator_begin(void* context)
{
    array_iterator_ctx* ctx = context;
    ctx->current_element_idx = 0;
    return array_begin_as_char_ptr(ctx);
}

void* array_iterator_next(void* context)
{
    array_iterator_ctx* ctx = context;
    ++ctx->current_element_idx;
    return array_begin_as_char_ptr(ctx) + ctx->current_element_idx * ctx->element_size;
}

void* array_iterator_end(void* context)
{
    array_iterator_ctx* ctx = context;
    return array_begin_as_char_ptr(ctx) + ctx->num_of_elements * ctx->element_size;
}
