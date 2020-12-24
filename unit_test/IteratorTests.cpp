#include "AllTests.h"
#include "type.h"
#include "iterator.h"
#include "Fakes.h"

/* ------------------------------------------------------------------------- */
/* ----------------------------- Test groups ------------------------------- */
/* ------------------------------------------------------------------------- */

TEST_GROUP(Ut_Iterator)
{
    void setup() override {}
    void teardown() override {}
};

/* ------------------------------------------------------------------------- */
/* ------------------------------ Test cases ------------------------------- */
/* ------------------------------------------------------------------------- */

TEST(Ut_Iterator, NullCases)
{
    iterator_instance iter;
    ENUMS_EQUAL_INT(iterator_status_iptr, iterator_construct_ext(nullptr, 10, malloc));
    ENUMS_EQUAL_INT(iterator_status_iptr, iterator_construct_ext(&iter, 10, nullptr));

    ENUMS_EQUAL_INT(iterator_status_iptr, iterator_destruct_ext(&iter, nullptr));
    ENUMS_EQUAL_INT(iterator_status_iptr, iterator_destruct_ext(nullptr, free));

    auto ci = [](void* ctx) -> const void * { static_cast<void>(ctx); return nullptr; };
    ENUMS_EQUAL_INT(iterator_status_iptr, iterator_init_as_const(nullptr, ci, ci, ci));
    ENUMS_EQUAL_INT(iterator_status_iptr, iterator_init_as_const(&iter, nullptr, ci, ci));
    ENUMS_EQUAL_INT(iterator_status_iptr, iterator_init_as_const(&iter, ci, nullptr, ci));
    ENUMS_EQUAL_INT(iterator_status_iptr, iterator_init_as_const(&iter, ci, ci, nullptr));

    auto i = [](void* ctx) -> void * { static_cast<void>(ctx); return nullptr; };
    ENUMS_EQUAL_INT(iterator_status_iptr, iterator_init_as_non_const(nullptr, i, i, i));
    ENUMS_EQUAL_INT(iterator_status_iptr, iterator_init_as_non_const(&iter, nullptr, i, i));
    ENUMS_EQUAL_INT(iterator_status_iptr, iterator_init_as_non_const(&iter, i, nullptr, i));
    ENUMS_EQUAL_INT(iterator_status_iptr, iterator_init_as_non_const(&iter, i, i, nullptr));
}

TEST(Ut_Iterator, iterator_construct_ext__ErrorStatusReturnedWhenMemoryAllocationFailed)
{
    iterator_instance iterator;
    auto status = iterator_construct_ext(&iterator, sizeof(i32), FakeMalloc);
    ENUMS_EQUAL_INT(iterator_status_merror, status);
}

TEST(Ut_Iterator, iterator_construct_ext__IteratorCreated)
{
    iterator_instance iterator;
    auto status = iterator_construct_ext(&iterator, sizeof(i32), malloc);
    ENUMS_EQUAL_INT(iterator_status_ok, status);
    POINTER_NOT_NULL(iterator.context);
    iterator_destruct_ext(&iterator, free);
}

TEST(Ut_Iterator, iterator_destruct__MemoryFreedAndFieldsCleared)
{
    iterator_instance iter;
    auto status = iterator_construct_ext(&iter, 50, malloc);
    ENUMS_EQUAL_INT(iterator_status_ok, status);

    status = iterator_destruct_ext(&iter, free);
    ENUMS_EQUAL_INT(iterator_status_ok, status);
    POINTER_NULL(iter.context);
}

TEST(Ut_Iterator, iterator_init_as_const__FieldsInitialized)
{
    iterator_instance iter;
    /* There is no need to implement these functions for test purposes */
    auto begin = [](void* ctx) -> const void* { static_cast<void>(ctx); return nullptr; };
    auto next = [](void* ctx) -> const void* { static_cast<void>(ctx); return nullptr; };
    auto end = [](void* ctx) -> const void* { static_cast<void>(ctx); return nullptr; };

    auto status = iterator_init_as_const(&iter, begin, next, end);
    ENUMS_EQUAL_INT(iterator_status_ok, status);
    const void* (*beginPtr)(void*) = begin;
    const void* (*nextPtr)(void*) = next;
    const void* (*endPtr)(void*) = end;
    ENUMS_EQUAL_INT(iterator_type_const, iter.type);
    FUNCTIONPOINTERS_EQUAL(beginPtr, iter.begin.begin_const);
    FUNCTIONPOINTERS_EQUAL(nextPtr, iter.next.next_const);
    FUNCTIONPOINTERS_EQUAL(endPtr, iter.end.end_const);
}

TEST(Ut_Iterator, iterator_init_as_non_const__FieldsInitialized)
{
    iterator_instance iter;
    /* There is no need to implement these functions for test purposes */
    auto begin = [](void* ctx) -> void* { static_cast<void>(ctx); return nullptr; };
    auto next = [](void* ctx) -> void* { static_cast<void>(ctx); return nullptr; };
    auto end = [](void* ctx) -> void* { static_cast<void>(ctx); return nullptr; };

    auto status = iterator_init_as_non_const(&iter, begin, next, end);
    ENUMS_EQUAL_INT(iterator_status_ok, status);
    void* (*beginPtr)(void*) = begin;
    void* (*nextPtr)(void*) = next;
    void* (*endPtr)(void*) = end;
    ENUMS_EQUAL_INT(iterator_type_non_const, iter.type);
    FUNCTIONPOINTERS_EQUAL(beginPtr, iter.begin.begin_non_const);
    FUNCTIONPOINTERS_EQUAL(nextPtr, iter.next.next_non_const);
    FUNCTIONPOINTERS_EQUAL(endPtr, iter.end.end_non_const);
}

TEST(Ut_Iterator, Misc)
{
    iterator_instance iter;
    auto status = iterator_construct(&iter, 3 * sizeof(u32));
    ENUMS_EQUAL_INT(iterator_status_ok, status);

    auto begin = [](void* ctx) -> void* {
        u32* arr = static_cast<u32*>(ctx);
        return &arr[0];
    };
    auto next = [](void* ctx) -> void* {
        u32* arr = static_cast<u32*>(ctx);
        return &arr[1];
    };
    auto end = [](void* ctx) -> void* {
        u32* arr = static_cast<u32*>(ctx);
        return &arr[2];
    };
    status = iterator_init_as_non_const(&iter, begin, next, end);
    ENUMS_EQUAL_INT(iterator_status_ok, status);

    u32* ctx = static_cast<u32*>(iter.context);
    ctx[0] = 100;
    ctx[1] = 200;
    ctx[2] = 300;

    u32 result = 0;
    result += *(static_cast<u32*>(iter.begin.begin_non_const(iter.context)));
    result += *(static_cast<u32*>(iter.next.next_non_const(iter.context)));
    result += *(static_cast<u32*>(iter.end.end_non_const(iter.context)));

    LONGS_EQUAL(600, result);
    ENUMS_EQUAL_INT(iterator_status_ok, iterator_destruct(&iter));
}