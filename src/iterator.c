#include "iterator.h"

/* ------------------------------------------------------------------------- */
/* ----------------------------- Api functions ----------------------------- */
/* ------------------------------------------------------------------------- */

void iterator_construct_ext(iterator_instance* iterator, size user_data_len, mem_allocator allocator)
{
    (void)iterator;
    (void)user_data_len;
    (void)allocator;
}

void iterator_destruct(iterator_instance iterator)
{
    (void)iterator;
}
