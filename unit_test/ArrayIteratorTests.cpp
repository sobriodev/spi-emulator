#include "AllTests.h"
#include "array_iterator.h"

/* ------------------------------------------------------------------------- */
/* ---------------------------- Private macros ----------------------------- */
/* ------------------------------------------------------------------------- */

#define TEST_ARRAY_CONST_SIZE 5
#define TEST_ARRAY_SIZE 3

/* ------------------------------------------------------------------------- */
/* -------------------------- Private variables ---------------------------- */
/* ------------------------------------------------------------------------- */

static const u32 TEST_ARRAY_CONST[TEST_ARRAY_CONST_SIZE] = {0xCAFE, 0xBACA, 0xFEFE, 0xDEDE, 0xFFFF};
static u32 testArray[TEST_ARRAY_SIZE] = {0xAAAA, 0xBBBB, 0xCCCC};

/* ------------------------------------------------------------------------- */
/* ----------------------------- Test groups ------------------------------- */
/* ------------------------------------------------------------------------- */

TEST_GROUP(Ut_ArrayIterator)
{
    static const size ITER_CTX_SIZE = sizeof(array_iterator_ctx);
    iterator_instance cIter = {};
    iterator_instance ncIter = {};

    void setup() override
    {
        /* Init const iterator */
        auto status = iterator_construct(&cIter, ITER_CTX_SIZE);
        ENUMS_EQUAL_INT_TEXT(iterator_status_ok, status, "Cannot construct shared const iterator");
        status = iterator_init_as_const(&cIter, array_iterator_const_begin,
                                        array_iterator_const_next, array_iterator_const_end);
        ENUMS_EQUAL_INT_TEXT(iterator_status_ok, status, "Cannot initialize shared const iterator");

        /* Init non-const iterator */
        status = iterator_construct(&ncIter, ITER_CTX_SIZE);
        ENUMS_EQUAL_INT_TEXT(iterator_status_ok, status, "Cannot construct shared non-const iterator");
        status = iterator_init_as_non_const(&ncIter, array_iterator_begin, array_iterator_next, array_iterator_end);
        ENUMS_EQUAL_INT_TEXT(iterator_status_ok, status, "Cannot initialize shared non-const iterator");
    }

    void teardown() override
    {
        /* Destroy iterators */
        auto status = iterator_destruct(&cIter);
        ENUMS_EQUAL_INT_TEXT(iterator_status_ok, status, "Cannot destruct shared const iterator");

        status = iterator_destruct(&ncIter);
        ENUMS_EQUAL_INT_TEXT(iterator_status_ok, status, "Cannot destruct shared non-const iterator");
    }

    auto setConstContext()
    {
        return array_iterator_init_const_ctx(&cIter, &TEST_ARRAY_CONST[0], TEST_ARRAY_CONST_SIZE, sizeof(u32));
    }

    auto setNonConstContext()
    {
        return array_iterator_init_ctx(&ncIter, &testArray[0], TEST_ARRAY_SIZE, sizeof(u32));
    }
};

/* ------------------------------------------------------------------------- */
/* ------------------------------ Test cases ------------------------------- */
/* ------------------------------------------------------------------------- */

TEST(Ut_ArrayIterator, NullCases)
{
    /* array_iterator_init_const_ctx NULL cases */
    auto status = array_iterator_init_const_ctx(nullptr, &TEST_ARRAY_CONST[0],
                                                TEST_ARRAY_CONST_SIZE, sizeof(u32));
    ENUMS_EQUAL_INT(array_iterator_status_iptr, status);

    status = array_iterator_init_const_ctx(&cIter, nullptr,
                                           TEST_ARRAY_CONST_SIZE, sizeof(u32));
    ENUMS_EQUAL_INT(array_iterator_status_iptr, status);

    /* array_iterator_init_ctx NULL cases */
    status = array_iterator_init_ctx(nullptr, &testArray[0], TEST_ARRAY_SIZE, sizeof(u32));
    ENUMS_EQUAL_INT(array_iterator_status_iptr, status);

    status = array_iterator_init_ctx(&ncIter, nullptr, TEST_ARRAY_SIZE, sizeof(u32));
    ENUMS_EQUAL_INT(array_iterator_status_iptr, status);
}

TEST(Ut_ArrayIterator, array_iterator_begin__IdxEqualsToZero)
{
    auto status = setNonConstContext();
    ITERATOR_BEGIN(ncIter);
    ENUMS_EQUAL_INT(array_iterator_status_ok, status);
    auto ctx = static_cast<array_iterator_ctx*>(ncIter.context);
    UNSIGNED_LONGS_EQUAL(0, ctx->current_element_idx);
}

TEST(Ut_ArrayIterator, array_iterator_const_begin__IdxEqualsToZero)
{
    auto status = setConstContext();
    ITERATOR_BEGIN(cIter);
    ENUMS_EQUAL_INT(array_iterator_status_ok, status);
    auto ctx = static_cast<array_iterator_ctx*>(cIter.context);
    UNSIGNED_LONGS_EQUAL(0, ctx->current_element_idx);
}

TEST(Ut_ArrayIterator, array_iterator_init_ctx__ErorrOnWrongParams)
{
    /* Pass const iterator rather than non-const one */
    auto status = array_iterator_init_ctx(&cIter, &testArray[0], TEST_ARRAY_SIZE, sizeof(u32));
    ENUMS_EQUAL_INT(array_iterator_status_cerror, status);

    /* Element size must be positive */
    status = array_iterator_init_ctx(&ncIter, &testArray[0], TEST_ARRAY_SIZE, 0);
    ENUMS_EQUAL_INT(array_iterator_status_cerror, status);
}

TEST(Ut_ArrayIterator, array_iterator_init_const_ctx__ErrorOnWrongParams)
{
    /* Pass non-const iterator rather than const one */
    auto status = array_iterator_init_const_ctx(&ncIter, &TEST_ARRAY_CONST[0], TEST_ARRAY_CONST_SIZE, sizeof(u32));
    ENUMS_EQUAL_INT(array_iterator_status_cerror, status);

    /* Element size must be positive */
    status = array_iterator_init_const_ctx(&cIter, &TEST_ARRAY_CONST[0], TEST_ARRAY_CONST_SIZE, 0);
    ENUMS_EQUAL_INT(array_iterator_status_cerror, status);
}

TEST(Ut_ArrayIterator, ITERATOR_FOREACH__AllElementsShouldBeVisited)
{
    auto status = setNonConstContext();
    ENUMS_EQUAL_INT(array_iterator_status_ok, status);
    size i = 0;
    ITERATOR_FOREACH(element, ncIter) {
        auto actual = static_cast<u32*>(element);
        UNSIGNED_LONGS_EQUAL(testArray[i], *actual);
        ++i;
    }
    UNSIGNED_LONGS_EQUAL(TEST_ARRAY_SIZE, i);
}

TEST(Ut_ArrayIterator, ITERATOR_FOREACH_CONST__AllElementsShouldBeVisited)
{
    auto status = setConstContext();
    ENUMS_EQUAL_INT(array_iterator_status_ok, status);
    size i = 0;
    ITERATOR_FOREACH_CONST(elem, cIter) {
        auto actual = static_cast<const u32*>(elem);
        UNSIGNED_LONGS_EQUAL(TEST_ARRAY_CONST[i], *actual);
        ++i;
    }
    UNSIGNED_LONGS_EQUAL(TEST_ARRAY_CONST_SIZE, i);
}

TEST(Ut_ArrayIterator, array_iterator_init_const_ctx__EmptyArrayShouldNotThrowAnError)
{
    auto status = array_iterator_init_const_ctx(&cIter, &TEST_ARRAY_CONST[0], 0, sizeof(u32));
    ENUMS_EQUAL_INT(array_iterator_status_ok, status);
    POINTERS_EQUAL(ITERATOR_CBEGIN(cIter), ITERATOR_CEND(cIter));
}

TEST(Ut_ArrayIterator, ITERATOR_FOREACH__IteratorShouldBeReusable)
{
    auto status = setNonConstContext();
    ENUMS_EQUAL_INT(array_iterator_status_ok, status);
    size i = 0;
    ITERATOR_FOREACH(e, ncIter) {
        ++i;
    }
    UNSIGNED_LONGS_EQUAL(TEST_ARRAY_SIZE, i);

    /* Iterate second time */
    size j = 0;
    ITERATOR_FOREACH(e, ncIter) {
        ++j;
    }
    UNSIGNED_LONGS_EQUAL(TEST_ARRAY_SIZE, j);
}

TEST(Ut_ArrayIterator, ITERATOR_FOREACH_CONST__NonConstDataAccesedFromConstIterator)
{
    u8 value = 0xAA;
    ENUMS_EQUAL_INT(array_iterator_status_ok, array_iterator_init_const_ctx(&cIter, &value, 1, sizeof(u8)));
    size iteration = 0;
    ITERATOR_FOREACH_CONST(v, cIter) {
        UNSIGNED_LONGS_EQUAL(*static_cast<const u8*>(v), static_cast<u8*>(&value)[iteration]);
        ++iteration;
    }
    UNSIGNED_LONGS_EQUAL(1, iteration);
}

TEST(Ut_ArrayIterator, ITERATOR_FOREACH_CONST__TestWithConstCharPtr)
{
    const char* text = "Some text";
    ENUMS_EQUAL_INT(array_iterator_status_ok, array_iterator_init_const_ctx(&cIter, text, 1, sizeof(const char*)));
    auto begin = ITERATOR_CBEGIN(cIter);
    POINTERS_EQUAL(text, begin);
    STRCMP_EQUAL(text, static_cast<const char*>(begin));
    POINTERS_EQUAL(text + sizeof(const char*), ITERATOR_CEND(cIter));
}

TEST(Ut_ArrayIterator, array_iterator_create_const__ReturnsFalseWhenWrongParametersArePassed)
{
    CHECK_FALSE(array_iterator_create_const(nullptr, TEST_ARRAY_CONST, TEST_ARRAY_CONST_SIZE, sizeof(u32)));

    iterator_instance iter;
    CHECK_FALSE(array_iterator_create_const(&iter, nullptr, TEST_ARRAY_CONST_SIZE, sizeof(u32)));
}

TEST(Ut_ArrayIterator, array_iterator_create_const__CreatesValidIterator)
{
    iterator_instance iter;
    CHECK_TRUE(array_iterator_create_const(&iter, TEST_ARRAY_CONST, TEST_ARRAY_CONST_SIZE, sizeof(u32)));

    /* The caller must free memory afterwards */
    ENUMS_EQUAL_INT(iterator_status_ok, iterator_destruct(&iter));
}

TEST(Ut_ArrayIterator, array_iterator_create__ReturnsFalseWhenWrongParametersArePassed)
{
    CHECK_FALSE(array_iterator_create(nullptr, testArray, TEST_ARRAY_SIZE, sizeof(u32)));

    iterator_instance iter;
    CHECK_FALSE(array_iterator_create(&iter, nullptr, TEST_ARRAY_SIZE, sizeof(u32)));
}

TEST(Ut_ArrayIterator, array_iterator_create__CreatesValidIterator)
{
    iterator_instance iter;
    CHECK_TRUE(array_iterator_create(&iter, testArray, TEST_ARRAY_SIZE, sizeof(u32)));

    /* The caller must free memory afterwards */
    ENUMS_EQUAL_INT(iterator_status_ok, iterator_destruct(&iter));
}