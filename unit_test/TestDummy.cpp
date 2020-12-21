#include "CppUTest/CommandLineTestRunner.h"
#include "dummy.h"

TEST_GROUP(DummyGroup)
{
    void setup() override
    {
        identify_lib();
    }
};

TEST(DummyGroup, DummyTest)
{
    int* somePtr = static_cast<int*>(malloc(sizeof(int)));
    *somePtr = 100;
    free(somePtr);
}
