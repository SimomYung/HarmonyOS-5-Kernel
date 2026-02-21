#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "stub.h"
#include "gtest/gtest.h"

class st_test_convert : public testing::Test {
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

TEST_F(st_test_convert, st_test_convert_atoi)
{
    const char *str = "12345.67";
    int value = atoi(str);
    EXPECT_EQ(value, 12345);

    char a[] = "-100";
    value = atoi(a);
    EXPECT_EQ(value, -100);

    char b[] = "123";
    value = atoi(b);
    EXPECT_EQ(value, 123);

    char c[] = "213dczd";
    value = atoi(c);
    EXPECT_EQ(value, 213);
    

    char d[] = "dczdd2";
    EXPECT_EQ(atoi(d), 0);
    EXPECT_EQ(atoi(""), 0);
    
}

TEST_F(st_test_convert, st_test_convert_atof)
{
    const char *str = "12345.67";
    float value = atof(str);
    EXPECT_FLOAT_EQ(value, 12345.67);

    char a[] = "-100.12";
    value = atof(a);
    EXPECT_FLOAT_EQ(value, -100.12);

    EXPECT_FLOAT_EQ(atof("0"), 0);
    EXPECT_FLOAT_EQ(atof("SDW"), 0);
    EXPECT_FLOAT_EQ(atof("123.41ASED"), 123.41);
    EXPECT_FLOAT_EQ(atof(""), 0);
}

TEST_F(st_test_convert, st_test_convert_ffs)
{
    int nums[]={0,1,2,4,8,16,32,64};
    for (int i = 0; i < 8; i++) {
        EXPECT_EQ(ffs(nums[i]), i);
    }
}

TEST_F(st_test_convert, st_test_convert_strtol)
{
    char *c;
    EXPECT_EQ(strtol("2147483647", 0, 0), 2147483647L);

    if (sizeof(long) == 4) {
        EXPECT_EQ(strtol("2147483648", &c, 0), 2147483647L);
        EXPECT_EQ(strtol("-2147483649", &c, 0), -2147483647L - 1);
    } else if (sizeof(long) == 8) {
        EXPECT_EQ(strtol("9223372036854775808", &c, 0), 9223372036854775807L);
        EXPECT_EQ(strtol("-9223372036854775809", &c, 0), -9223372036854775807L - 1);
    }

    EXPECT_EQ(strtol("z", 0, 36), 35);
    EXPECT_EQ(strtol("00010010001101000101011001111000", 0, 2), 0x12345678);
    EXPECT_EQ(strtol("0F5F", &c, 16), 0x0f5f);

    EXPECT_EQ(strtol("0xz", &c, 16), 0);
    EXPECT_EQ(strtol("1063", &c, 8), 563);
    EXPECT_EQ(strtol("0x1234", &c, 16), 0x1234);
    EXPECT_EQ(strtol("123", &c, 37), 0);

    EXPECT_EQ(strtol("", &c, 0), 0);

}

TEST_F(st_test_convert, st_test_convert_strtok_r)
{
    char str[80] = "This is - www.huawei.com - website";
    const char *s = "-";
    char *retStr = NULL;
    char *token = strtok_r(str, s, &retStr);
    ASSERT_NE((uintptr_t)token, NULL);
    EXPECT_STREQ(str, "This is ");
    EXPECT_STREQ(token, "This is ");

    token = strtok_r(NULL, s, &retStr);
    ASSERT_NE((uintptr_t)token, NULL);
    EXPECT_STREQ(token, " www.huawei.com ");

    token = strtok_r(NULL, s, &retStr);
    EXPECT_STREQ(token, " website");

    token = strtok_r(NULL, s, &retStr);
    ASSERT_EQ((uintptr_t)token, NULL);

    strcpy(str, "This is - www.huawei.com - website");
    const char *s2 = "--";
    token = strtok_r(str, s2, &retStr);
    ASSERT_NE((uintptr_t)token, NULL);
    EXPECT_STREQ(str, "This is ");
    EXPECT_STREQ(token, "This is ");

    token = strtok_r(NULL, s2, &retStr);
    ASSERT_NE((uintptr_t)token, NULL);
    EXPECT_STREQ(token, " www.huawei.com ");

    token = strtok_r(NULL, s2, &retStr);
    EXPECT_STREQ(token, " website");

    token = strtok_r(NULL, s2, &retStr);
    ASSERT_EQ((uintptr_t)token, NULL);

    strcpy(str, "This is - www.huawei.com - website");
    const char *s3 = "|";
    token = strtok_r(str, s3, &retStr);
    ASSERT_NE((uintptr_t)token, NULL);
    EXPECT_STREQ(token, "This is - www.huawei.com - website");

    strcpy(str, "This is - www.huawei.com - website");
    const char *s4 = "";
    token = strtok_r(str, s4, &retStr);
    ASSERT_NE((uintptr_t)token, NULL);
    EXPECT_STREQ(token, "This is - www.huawei.com - website");

    strcpy(str, "");
    const char *s5 = "";
    token = strtok_r(str, s5, &retStr);
    ASSERT_EQ((uintptr_t)token, NULL);

    char * str2 = NULL;
    const char *s6 = "";
    token = strtok_r(str, s6, &retStr);
    ASSERT_EQ((uintptr_t)token, NULL);
}

TEST_F(st_test_convert, st_test_convert_strtod)
{
    const char *str = "12345.678867sdlkf";
    char *ptr = NULL;
    double value = strtod(str, &ptr);
    EXPECT_DOUBLE_EQ(value, 12345.678867);
    ASSERT_NE((uintptr_t)ptr, NULL);
    EXPECT_STREQ(ptr, str + 12);

    char a[] = "-100.1234535";
    value = strtod(a, NULL);
    EXPECT_DOUBLE_EQ(value, -100.1234535);

}

TEST_F(st_test_convert, st_test_convert_strtof)
{
    const char *str = "12345.678867dsfs";
    char *ptr = NULL;
    float value = strtof(str, &ptr);
    EXPECT_FLOAT_EQ(value, 12345.678867);
    ASSERT_NE((uintptr_t)ptr, NULL);
    EXPECT_STREQ(ptr, str + 12);

    char a[] = "-100.1234535";
    value = strtof(a, NULL);
    EXPECT_FLOAT_EQ(value, -100.1234535);

}

TEST_F(st_test_convert, st_test_convert_strtoll)
{
    char *c;
    if (sizeof(long long) == 8) {
        EXPECT_EQ(strtoll("9223372036854775808", &c, 0), 9223372036854775807LL);
        EXPECT_EQ(strtoll("-9223372036854775809", &c, 0), -9223372036854775807LL - 1);
    } else {
        printf("sizeof(long long) == %d, not implemented\n", (int)sizeof(long long));
    }
}

TEST_F(st_test_convert, st_test_convert_strtoull)
{
    char *c;
    if (sizeof(long long) == 8) {
        EXPECT_EQ(strtoul("18446744073709551616", &c, 0), 18446744073709551615ULL);
        EXPECT_EQ(strtoul("-1", &c, 0), -1ULL);
        EXPECT_EQ(strtoul("-2", &c, 0), -2ULL);
        EXPECT_EQ(strtoul("-9223372036854775808", &c, 0), -9223372036854775808ULL);
        EXPECT_EQ(strtoul("-9223372036854775809", &c, 0), -9223372036854775809ULL);
        EXPECT_EQ(strtoul("-18446744073709551616", &c, 0), 18446744073709551615ULL);
    } else {
        printf("sizeof(long long) == %d, not implemented\n", (int)sizeof(long long));
    }
}

