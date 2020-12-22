#include "CppUTest/CommandLineTestRunner.h"
#include "dummy.h"
#include "type.h"

TEST_GROUP(DummyGroup)
{
    void setup() override
    {
        identify_lib();
    }
};

TEST(DummyGroup, DummyTest)
{
    i32* somePtr = static_cast<int*>(malloc(sizeof(i32)));
    *somePtr = 100;
    free(somePtr);
}
