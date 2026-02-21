#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "stub.h"
#include "gtest/gtest.h"


class st_test_cmp : public testing::Test {
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

TEST_F(st_test_cmp, st_test_cmp_memcmp)
{
    char str1[8] = "ac";
    char str2[8] = "acd";
    char str3[8] = "abc";

    EXPECT_EQ(memcmp(str1, str2, 2), 0);

    EXPECT_LT(memcmp(str1, str2, 3), 0);
    EXPECT_GT(memcmp(str2, str1, 3), 0);

    EXPECT_LT(memcmp(str3, str1, 2), 0);
    EXPECT_GT(memcmp(str1, str3, 2), 0);

    EXPECT_LT(memcmp(str3, str2, 3), 0);
    EXPECT_GT(memcmp(str2, str3, 3), 0);

    EXPECT_EQ(memcmp("", str1, 0), 0);
    EXPECT_LT(memcmp("", str1, 1), 0);
    EXPECT_EQ(memcmp("ac", str1, 3), 0);
    EXPECT_EQ(memcmp("ac", str1, 4), 0);

}

TEST_F(st_test_cmp, st_test_cmp_strcmp)
{
    char str1[8] = "ac";
    char str2[8] = "acd";
    char str3[8] = "abc";

    EXPECT_LT(strcmp(str1, str2), 0);
    EXPECT_GT(strcmp(str2, str1), 0);

    EXPECT_LT(strcmp(str3, str1), 0);
    EXPECT_GT(strcmp(str1, str3), 0);

    EXPECT_LT(strcmp(str3, str2), 0);
    EXPECT_GT(strcmp(str2, str3), 0);

    EXPECT_LT(strcmp("", "ac"), 0);
    EXPECT_EQ(strcmp("", ""), 0);
}

TEST_F(st_test_cmp, st_test_cmp_strcasecmp)
{
    char str1[8] = "ac";
    char str2[8] = "AC";
    char str3[8] = "aCd";

    EXPECT_EQ(strcasecmp(str1, str2), 0);
    EXPECT_EQ(strcasecmp(str2, str1), 0);

    EXPECT_LT(strcasecmp(str1, str3), 0);
    EXPECT_LT(strcasecmp(str2, str3), 0);

    EXPECT_LT(strcasecmp("", "ac"), 0);
    EXPECT_EQ(strcasecmp("", ""), 0);
    
}

TEST_F(st_test_cmp, st_test_cmp_strncasecmp)
{
    char str1[8] = "ac";
    char str2[8] = "AC";
    char str3[8] = "aCd";

    EXPECT_EQ(strncasecmp(str1, str2, 2), 0);
    EXPECT_EQ(strncasecmp(str2, str1, 2), 0);

    EXPECT_EQ(strncasecmp(str1, str3, 2), 0);
    EXPECT_EQ(strncasecmp(str2, str3, 2), 0);

    EXPECT_LT(strncasecmp(str1, str3, 3), 0);
    EXPECT_GT(strncasecmp(str3, str1, 3), 0);

    EXPECT_EQ(strncasecmp("", str1, 0), 0);
    EXPECT_LT(strncasecmp("", str1, 1), 0);
    EXPECT_EQ(strncasecmp("ac", str1, 3), 0);
    EXPECT_EQ(strncasecmp("ac", str1, 4), 0);
    EXPECT_EQ(strncasecmp("ac", str1, 124), 0);

}

TEST_F(st_test_cmp, st_test_cmp_strncmp)
{
    char str1[8] = "ac";
    char str2[8] = "acd";
    char str3[8] = "abc";

    EXPECT_EQ(strncmp(str1, str2, 2), 0);

    EXPECT_LT(strncmp(str1, str2, 3), 0);
    EXPECT_GT(strncmp(str2, str1, 3), 0);

    EXPECT_LT(strncmp(str3, str1, 2), 0);
    EXPECT_GT(strncmp(str1, str3, 2), 0);

    EXPECT_LT(strncmp(str3, str2, 3), 0);
    EXPECT_GT(strncmp(str2, str3, 3), 0);

    EXPECT_EQ(strncmp("", str1, 0), 0);
    EXPECT_LT(strncmp("", str1, 1), 0);
    EXPECT_EQ(strncmp("ac", str1, 3), 0);
    EXPECT_EQ(strncmp("ac", str1, 4), 0);
    EXPECT_EQ(strncmp("ac", str1, 124), 0);

}
