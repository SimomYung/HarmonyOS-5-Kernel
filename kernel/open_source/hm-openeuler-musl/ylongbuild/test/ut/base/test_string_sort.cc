#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "stub.h"
#include "gtest/gtest.h"

class st_test_sort : public testing::Test {
protected:
    static void SetUpTestSuite()
    {
        init(); /* STUB INIT */
    }

    static void TearDownTestSuite()
    {
        clearAllStub();
    }

    virtual void SetUp() {}

    virtual void TearDown() {}
};

#ifdef __cplusplus
extern "C" {
#endif
int cmpIntFunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}
#ifdef __cplusplus
}
#endif


TEST_F(st_test_sort, st_test_sort_qsort)
{

    int values[] = {0, 4, 5, 3, 2, 6, 2};
    qsort(values, 7, sizeof(int), cmpIntFunc);
    for (int i = 1; i < 7; i++) {
        EXPECT_GE(values[i], values[i-1]);
    }
}

TEST_F(st_test_sort, st_test_sort_bsearch)
{
    int values[] = {0, 1, 2, 3, 4};
    int *ret = NULL;
    int key = 2;
    ret = (int*)bsearch(&key, values, 5, sizeof(int), cmpIntFunc);
    EXPECT_EQ(*ret, 2);

    key = 5;
    ret = (int *)bsearch(&key, values, 5, sizeof(int), cmpIntFunc);
    EXPECT_EQ((uintptr_t)ret, NULL);
}