#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <execinfo.h>
#include "stub.h"
#include "gtest/gtest.h"


class test_backtrace : public testing::Test {
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

#ifdef __cplusplus
}
#endif
#define SIZE 100
void test_backtrace_symbols()
{
    int j, nptrs;
    void *buffer[100];
    char **strings=NULL;

    nptrs = backtrace(buffer, SIZE);
    EXPECT_GE(nptrs, 0);

    strings = backtrace_symbols(buffer, nptrs);
    EXPECT_EQ(strings!=NULL, true);

    for (j = 0; j < nptrs; j++) {
        printf("%s\n", strings[j]);
        EXPECT_EQ(strings[j]!=NULL, true);
    }
    EXPECT_NE(strstr(strings[0], "test_backtrace_symbols"), (const char *)NULL);
    EXPECT_EQ(strstr(strings[1], "test_none_sysmbols_release"), (const char *)NULL);
    free(strings);
}

static void test_none_sysmbols_release()
{
    test_backtrace_symbols();
}

TEST_F(test_backtrace, st_test_backtrace_symbols)
{
    test_none_sysmbols_release();
}
