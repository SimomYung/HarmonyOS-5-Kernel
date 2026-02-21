#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "stub.h"
#include "gtest/gtest.h"


class st_test_other : public testing::Test {
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

int TestStrstrNinclude(const char *string, const char *substring)
{
    char *q = strstr((char *)string, (char *)substring);
    if (q) {
        printf("strstr(%s,%s) returned str+%d, wanted 0\n", string, substring, q - string);
        return 1;
    }

    return 0;
}
int TestStrstrYinclude(const char *string, const char *substring, int n)
{
    char *q = strstr((char *)string, (char *)substring);
    if (q == 0) {
        printf("strstr(%s,%s) returned str+%d, wanted 0\n", string, substring, q - string);
        return 1;
    }

    if (q - string != n) {
        printf("strstr(%s,%s) returned str+%d, wanted str+%d\n", string, substring, q - string, n);
        return 1;
    }

    return 0;
}

#ifdef __cplusplus
}
#endif

TEST_F(st_test_other, st_test_other_strstr)
{
    EXPECT_EQ(TestStrstrNinclude("", "a"), 0);
    EXPECT_EQ(TestStrstrNinclude("a", "aa"), 0);
    EXPECT_EQ(TestStrstrNinclude("a", "b"), 0);
    EXPECT_EQ(TestStrstrNinclude("aa", "ab"), 0);
    EXPECT_EQ(TestStrstrNinclude("aa", "aaa"), 0);
    EXPECT_EQ(TestStrstrNinclude("abba", "aba"), 0);
    EXPECT_EQ(TestStrstrNinclude("abc abc", "abcd"), 0);
    EXPECT_EQ(TestStrstrNinclude("0-1-2-3-4-5-6-7-8-9", "-3-4-56-7-8-"), 0);
    EXPECT_EQ(TestStrstrNinclude("0-1-2-3-4-5-6-7-8-9", "-3-4-5+6-7-8-"), 0);
    EXPECT_EQ(TestStrstrNinclude("_ _ _\xff_ _ _", "_\x7f_"), 0);
    EXPECT_EQ(TestStrstrNinclude("_ _ _\x7f_ _ _", "_\xff_"), 0);
    EXPECT_EQ(TestStrstrYinclude("", "", 0), 0);
    EXPECT_EQ(TestStrstrYinclude("abcd", "", 0), 0);
    EXPECT_EQ(TestStrstrYinclude("abcd", "a", 0), 0);
    EXPECT_EQ(TestStrstrYinclude("abcd", "b", 1), 0);
    EXPECT_EQ(TestStrstrYinclude("abcd", "c", 2), 0);
    EXPECT_EQ(TestStrstrYinclude("abcd", "d", 3), 0);
    EXPECT_EQ(TestStrstrYinclude("abcd", "ab", 0), 0);
    EXPECT_EQ(TestStrstrYinclude("abcd", "bc", 1), 0);
    EXPECT_EQ(TestStrstrYinclude("abcd", "cd", 2), 0);
    EXPECT_EQ(TestStrstrYinclude("ababa", "baba", 1), 0);
    EXPECT_EQ(TestStrstrYinclude("ababab", "babab", 1), 0);
    EXPECT_EQ(TestStrstrYinclude("abababa", "bababa", 1), 0);
    EXPECT_EQ(TestStrstrYinclude("abababab", "bababab", 1), 0);
    EXPECT_EQ(TestStrstrYinclude("ababababa", "babababa", 1), 0);
    EXPECT_EQ(TestStrstrYinclude("abbababab", "bababa", 2), 0);
    EXPECT_EQ(TestStrstrYinclude("abbababab", "ababab", 3), 0);
    EXPECT_EQ(TestStrstrYinclude("abacabcabcab", "abcabcab", 4), 0);
    EXPECT_EQ(TestStrstrYinclude("nanabanabanana", "aba", 3), 0);
    EXPECT_EQ(TestStrstrYinclude("nanabanabanana", "ban", 4), 0);
    EXPECT_EQ(TestStrstrYinclude("nanabanabanana", "anab", 1), 0);
    EXPECT_EQ(TestStrstrYinclude("nanabanabanana", "banana", 8), 0);
    EXPECT_EQ(TestStrstrYinclude("_ _\xff_ _", "_\xff_", 2), 0);
}

TEST_F(st_test_other, st_test_other_strtok)
{
    char str[80] = "This is - www.huawei.com - website";
    const char s[2] = "-";
    char *token = strtok(str, s);
    ASSERT_NE((uintptr_t)token, NULL);
    EXPECT_STREQ(str, "This is ");
    EXPECT_STREQ(token, "This is ");

    token = strtok(NULL, s);
    ASSERT_NE((uintptr_t)token, NULL);
    EXPECT_STREQ(token, " www.huawei.com ");

    token = strtok(NULL, s);
    EXPECT_STREQ(token, " website");

    token = strtok(NULL, s);
    ASSERT_EQ((uintptr_t)token, NULL);
}

TEST_F(st_test_other, st_test_other_strspn)
{
    char str1[8] = "ABCDEF";
    char str2[8] = "ABCACF";
    char str3[8] = "ABCABC";
    char string[8] = "ABC";

    int pos = strspn(str1, string);
    EXPECT_EQ(pos, 3);

    pos = strspn(str2, string);
    EXPECT_EQ(pos, 5);

    pos = strspn(str3, string);
    EXPECT_EQ(pos, strlen(str3));
}

TEST_F(st_test_other, st_test_other_strcasestr)
{
    const char *string = "abcabcabc";
    const char *substr1 = "cab";
    const char *subStr2 = "123";

    char *ret = strcasestr((char *)string, (char *)substr1);
    EXPECT_NE((uintptr_t)ret, NULL);
    EXPECT_STREQ(ret, string + 2);

    ret = strcasestr((char *)string, (char *)subStr2);
    EXPECT_EQ((uintptr_t)ret, NULL);
}

TEST_F(st_test_other, st_test_other_strerror_r)
{
    char strErr[128] = {0};

    strerror_r(EPERM, strErr, 128);
    EXPECT_STREQ(strErr, "Operation not permitted");

    strerror_r(EACCES, strErr, 128);
    EXPECT_STREQ(strErr, "Permission denied");

    strerror_r(ENOMEM, strErr, 128);
    EXPECT_STREQ(strErr, "Out of memory");

    strerror_r(EAGAIN, strErr, 128);
    EXPECT_STREQ(strErr, "Resource temporarily unavailable");

    strerror_r(ETIMEDOUT, strErr, 128);
    EXPECT_STREQ(strErr, "Operation timed out");

    strerror_r(EPIPE, strErr, 128);
    EXPECT_STREQ(strErr, "Broken pipe");
}

TEST_F(st_test_other, st_test_other_strsep)
{
    char string[32] = "abc=def==123=456";
    char *q = string;
    char *token = strsep(&q, "=");
    EXPECT_NE((uintptr_t)token, NULL);
    EXPECT_STREQ(token, "abc");

    token = strsep(&q, "=");
    EXPECT_NE((uintptr_t)token, NULL);
    EXPECT_STREQ(token, "def");

    token = strsep(&q, "=");
    EXPECT_NE((uintptr_t)token, NULL);
    EXPECT_STREQ(token, "");

    token = strsep(&q, "=");
    EXPECT_NE((uintptr_t)token, NULL);
    EXPECT_STREQ(token, "123");

    token = strsep(&q, "=");
    EXPECT_NE((uintptr_t)token, NULL);
    EXPECT_STREQ(token, "456");
}

TEST_F(st_test_other, st_test_other_isspace)
{
    /*
     * ' '     (0x20)    space (SPC) 空格符
     * '\t'    (0x09)    horizontal tab (TAB) 水平制表符
     * '\n'    (0x0a)    newline (LF) 换行符
     * '\v'    (0x0b)    vertical tab (VT) 垂直制表符
     * '\f'    (0x0c)    feed (FF) 换页符
     * '\r'    (0x0d)    carriage return (CR) 回车符
     */

    EXPECT_NE(isspace(' '), 0);
    EXPECT_NE(isspace('\t'), 0);
    EXPECT_NE(isspace('\n'), 0);
    EXPECT_NE(isspace('\v'), 0);
    EXPECT_NE(isspace(' '), 0);
    EXPECT_NE(isspace(' '), 0);

    EXPECT_EQ(isspace('1'), 0);
    EXPECT_EQ(isspace('a'), 0);
}

TEST_F(st_test_other, st_test_other_stpcpy)
{
    const char *src = "huawei";
    char dest[32] = {0};

    stpcpy(dest, src);
    EXPECT_STREQ(dest, src);
}


TEST_F(st_test_other, st_test_other_strcspn)
{
    char str1[8] = "ABCDEF";
    char str2[8] = "FDBAC";
    char str3[8] = "123CBA";
    char string[8] = "ABC";

    int pos = strcspn(str1, string);
    EXPECT_EQ(pos, 0);

    pos = strcspn(str2, string);
    EXPECT_EQ(pos, 2);

    pos = strcspn(str3, string);
    EXPECT_EQ(pos, 3);
}

TEST_F(st_test_other, st_test_other_strdup)
{
    char str1[8] = "abcd";
    char *str2 = strdup(str1);
    EXPECT_NE((uintptr_t)str2, NULL);
    EXPECT_STREQ(str1, str2);
    free(str2);
}


TEST_F(st_test_other, st_test_other_strndup)
{
    char str1[8] = "abcd";
    char *str2 = strndup(str1, 5);
    EXPECT_NE((uintptr_t)str2, NULL);
    EXPECT_STREQ(str2, str1);
    free(str2);
    str2 = NULL;

    str2 = strndup(str1, 2);
    EXPECT_NE((uintptr_t)str2, NULL);
    EXPECT_STREQ(str2, "ab");
    free(str2);
    str2 = NULL;

    str2 = strndup(str1, 4);
    EXPECT_NE((uintptr_t)str2, NULL);
    EXPECT_STREQ(str2, str1);
    free(str2);
    str2 = NULL;
}

