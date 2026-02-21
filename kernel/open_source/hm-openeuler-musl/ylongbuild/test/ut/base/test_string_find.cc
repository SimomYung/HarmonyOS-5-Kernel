#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include "stub.h"
#include "gtest/gtest.h"

class st_test_find : public testing::Test {
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

TEST_F(st_test_find, st_test_find_memchr)
{
    char string[128] = "aaababccdd0001122223";

    char *ret = (char*)memchr(string, 'b', strlen(string));
    EXPECT_STREQ(ret, string + 3);

    ret = (char *)memchr(string, 'z', strlen(string));
    EXPECT_EQ((uintptr_t)ret, NULL);

    ret = (char *)memchr(string, 0, strlen(string));
    ASSERT_EQ((uintptr_t)ret, NULL);

    ret = (char *)memchr(string, 0, strlen(string) + 1);
    ASSERT_NE((uintptr_t)ret, NULL);

    ret = (char *)memchr(string, '1', 4);
    ASSERT_EQ((uintptr_t)ret, NULL);
    
}

TEST_F(st_test_find, st_test_find_strchr)
{
    char string[128] = "aaababccdd0001122223";

    char *ret = strchr(string, 'b');
    EXPECT_STREQ(ret, string + 3);

    ret = strchr(string, 'z');
    EXPECT_EQ((uintptr_t)ret, NULL);

    ret = strchr(string, 0);
    ASSERT_NE((uintptr_t)ret, NULL);

    
}

TEST_F(st_test_find, st_test_find_strrchr)
{
    char string[128] = "aaababccdd0001122223";

    char *ret = strrchr(string, 'b');
    EXPECT_STREQ(ret, string + 5);

    ret = strrchr(string, 'z');
    EXPECT_EQ((uintptr_t)ret, NULL);

    ret = strrchr(string, 0);
    ASSERT_NE((uintptr_t)ret, NULL);
}

TEST_F(st_test_find, st_test_find_strlen)
{
    char str1[8] = "a";
    char str2[8] = "ac";

    EXPECT_EQ(strlen(str1), 1);
    EXPECT_EQ(strlen(str2), 2);

    EXPECT_EQ(strlen(""), 0);

}

TEST_F(st_test_find, st_test_find_strnlen)
{
    char string[32] = "12345678";

    int len = strnlen(string, 32);
    EXPECT_EQ(len, 8);

    len = strnlen(string, 8);
    EXPECT_EQ(len, 8);

    len = strnlen(string, 4);
    EXPECT_EQ(len, 4);

    EXPECT_EQ(strnlen("", 2), 0);

}