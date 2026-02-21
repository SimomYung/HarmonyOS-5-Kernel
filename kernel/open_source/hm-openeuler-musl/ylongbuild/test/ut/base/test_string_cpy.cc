
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "stub.h"
#include "gtest/gtest.h"


class st_test_cpy : public testing::Test {
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


TEST_F(st_test_cpy, st_test_cpy_memcpy)
{
    char str1[8] = {'e', 'f', 'g', 'h', 'i', 'j'};
    const char *str2 = "abcd";

    memcpy(str1, str2, 1);
    EXPECT_EQ(strcmp(str1, "afghij"), 0);

    memcpy(str1, str2, 4);
    EXPECT_EQ(strcmp(str1, "abcdij"), 0);

    memcpy(str1, str2, 5);
    EXPECT_EQ(strcmp(str1, "abcd"), 0);

    memcpy(str1, "", 0);
    EXPECT_EQ(strcmp(str1, "abcd"), 0);
}

TEST_F(st_test_cpy, st_test_cpy_memset)
{
    char str1[8] = {0};

    memset(str1, 'a', 1);
    EXPECT_EQ(strcmp(str1, "a"), 0);

    memset(str1 + 1, 'b', 2);
    EXPECT_EQ(strcmp(str1, "abb"), 0);

    memset(str1 + 3, 'c', 3);
    EXPECT_EQ(strcmp(str1, "abbccc"), 0);

    memset(str1 + 2, 0, 1);
    EXPECT_EQ(strcmp(str1, "ab"), 0);

}

TEST_F(st_test_cpy, st_test_cpy_memmove)
{
    char str[] = "memmove can be very useful......";
    memmove(str+20,str+15,11);
    EXPECT_EQ(strcmp(str, "memmove can be very very useful."), 0);
}

TEST_F(st_test_cpy, st_test_cpy_strncpy)
{
    const char *src = "huawei";
    char dest[32] = {0};

    strncpy(dest, src, 32);
    EXPECT_STREQ(dest, src);

    memset(dest, 0, 32);
    strncpy(dest, src, 6);
    EXPECT_STREQ(dest, src);

    memset(dest, 0, 32);
    strncpy(dest, src, 4);
    EXPECT_STREQ(dest, "huaw");

}