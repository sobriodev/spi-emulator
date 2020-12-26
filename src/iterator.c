#include "iterator.h"
#include "common.h"

/* ------------------------------------------------------------------------- */
/* ----------------------------- Api functions ----------------------------- */
/* ------------------------------------------------------------------------- */

iterator_status iterator_construct_ext(iterator_instance* iterator, size user_data_len, mem_allocator allocator)
{
    NOT_NULL(iterator, iterator_status_iptr);
    NOT_NULL(allocator, iterator_status_iptr);

    /* Reserve context memory */
    iterator->context = allocator(user_data_len);
    NOT_NULL(iterator->context, iterator_status_merror);

    return iterator_status_ok;
}

iterator_status iterator_destruct_ext(iterator_instance* iterator, mem_deallocator deallocator)
{
    NOT_NULL(iterator, iterator_status_iptr);
    NOT_NULL(deallocator, iterator_status_iptr);

    deallocator(iterator->context);
    iterator->context = NULL;

    return iterator_status_ok;
}

iterator_status iterator_init_as_const(iterator_instance* iterator,
                                       iterator_const_begin beginFn,
                                       iterator_const_next nextFn,
                                       iterator_const_end endFn)
{
    NOT_NULL(iterator, iterator_status_iptr);
    NOT_NULL(beginFn, iterator_status_iptr);
    NOT_NULL(nextFn, iterator_status_iptr);
    NOT_NULL(endFn, iterator_status_iptr);

    iterator->type = iterator_type_const;
    iterator->begin.begin_const = beginFn;
    iterator->next.next_const = nextFn;
    iterator->end.end_const = endFn;
    return iterator_status_ok;
}

iterator_status iterator_init_as_non_const(iterator_instance* iterator,
                                           iterator_begin beginFn,
                                           iterator_next nextFn,
                                           iterator_end endFn)
{
    NOT_NULL(iterator, iterator_status_iptr);
    NOT_NULL(beginFn, iterator_status_iptr);
    NOT_NULL(nextFn, iterator_status_iptr);
    NOT_NULL(endFn, iterator_status_iptr);

    iterator->type = iterator_type_non_const;
    iterator->begin.begin_non_const = beginFn;
    iterator->next.next_non_const = nextFn;
    iterator->end.end_non_const = endFn;
    return iterator_status_ok;
}
