#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include "stub.h"
#include "gtest/gtest.h"


class st_test_print : public testing::Test {
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

TEST_F(st_test_print, st_test_print_printf)
{
    char str1[8] = "ac";
    char str2[8] = "acd";
    char str3[8] = "abc";

    int len = printf("%d\n", 123);
    EXPECT_EQ(len, 4);

    len = printf("%s\n", "123");
    EXPECT_EQ(len, 4);

    len = printf("%.2f\n", 0.123);
    EXPECT_EQ(len, 5);

    len = printf("%.2f\n", 0.1);
    EXPECT_EQ(len, 5);
}

static const struct {
    const char *fmt;
    int i;
    const char *expect;
} int_tests[] = {
    /* width, precision, alignment */
    { "%04d", 12, "0012" },
    { "%.3d", 12, "012" },
    { "%3d", 12, " 12" },
    { "%-3d", 12, "12 " },
    { "%+3d", 12, "+12" },
    { "%+-5d", 12, "+12  " },
    { "%+- 5d", 12, "+12  " },
    { "%- 5d", 12, " 12  " },
    { "% d", 12, " 12" },
    { "%0-5d", 12, "12   " },
    { "%-05d", 12, "12   " },

    /* ...explicit precision of 0 shall be no characters except for alt-octal. */
    { "%.0d", 0, "" },
    { "%.0o", 0, "" },
    { "%#.0d", 0, "" },
    { "%#.0o", 0, "0" },
    { "%#.0x", 0, "" },

    /* ...but it still has to honor width and flags. */
    { "%2.0u", 0, "  " },
    { "%02.0u", 0, "  " },
    { "%2.0d", 0, "  " },
    { "%02.0d", 0, "  " },
    { "% .0d", 0, " " },
    { "%+.0d", 0, "+" },

    /* hex: test alt form and case */
    { "%x", 63, "3f" },
    { "%#x", 63, "0x3f" },
    { "%X", 63, "3F" },

    /* octal: test alt form */
    { "%o", 15, "17" },
    { "%#o", 15, "017" },

    /* octal: corner cases */
    { "%#o", 0, "0" },
    { "%#.0o", 0, "0" },
    { "%#.1o", 0, "0" },
    { "%#o", 1, "01" },
    { "%#.0o", 1, "01" },
    { "%#.1o", 1, "01" },
    { "%#04o", 1, "0001" },
    { "%#04.0o", 1, "  01" },
    { "%#04.1o", 1, "  01" },
    { "%04o", 1, "0001" },
    { "%04.0o", 1, "   1" },
    { "%04.1o", 1, "   1" },

    { NULL, 0, NULL }
};

static const struct {
    const char *fmt;
    double f;
    const char *expect;
} fp_tests[] = {
    /* basic form, handling of exponent/precision for 0 */
    { "%a", 0.0, "0x0p+0" },
    { "%e", 0.0, "0.000000e+00" },
    { "%f", 0.0, "0.000000" },
    { "%g", 0.0, "0" },
    { "%#g", 0.0, "0.00000" },
    { "%la", 0.0, "0x0p+0" },
    { "%le", 0.0, "0.000000e+00" },
    { "%lf", 0.0, "0.000000" },
    { "%lg", 0.0, "0" },
    { "%#lg", 0.0, "0.00000" },

    /* rounding */
    { "%f", 1.1, "1.100000" },
    { "%f", 1.2, "1.200000" },
    { "%f", 1.3, "1.300000" },
    { "%f", 1.4, "1.400000" },
    { "%f", 1.5, "1.500000" },
    { "%.4f", 1.06125, "1.0613" }, /* input is not representible exactly as double */
    { "%.4f", 1.03125, "1.0312" }, /* 0x1.08p0 */
    { "%.2f", 1.375, "1.38" },
    { "%.1f", 1.375, "1.4" },
    { "%.1lf", 1.375, "1.4" },
    { "%.15f", 1.1, "1.100000000000000" },
    { "%.16f", 1.1, "1.1000000000000001" },
    { "%.17f", 1.1, "1.10000000000000009" },
    { "%.2e", 1500001.0, "1.50e+06" },
    { "%.2e", 1505000.0, "1.50e+06" },
    { "%.2e", 1505000.00000095367431640625, "1.51e+06" },
    { "%.2e", 1505001.0, "1.51e+06" },
    { "%.2e", 1506000.0, "1.51e+06" },

    /* correctness in DBL_DIG places */
    { "%.15g", 1.23456789012345, "1.23456789012345" },

    /* correct choice of notation for %g */
    { "%g", 0.0001, "0.0001" },
    { "%g", 0.00001, "1e-05" },
    { "%g", 123456, "123456" },
    { "%g", 1234567, "1.23457e+06" },
    { "%.7g", 1234567, "1234567" },
    { "%.7g", 12345678, "1.234568e+07" },
    { "%.8g", 0.1, "0.1" },
    { "%.9g", 0.1, "0.1" },
    { "%.10g", 0.1, "0.1" },
    { "%.11g", 0.1, "0.1" },

    /* pi in double precision, printed to a few extra places */
    { "%.15f", M_PI, "3.141592653589793" },
    { "%.18f", M_PI, "3.141592653589793116" },

    /* exact conversion of large integers */
    { "%.0f", 340282366920938463463374607431768211456.0,
      "340282366920938463463374607431768211456" },

    { NULL, 0.0, NULL }
};

TEST_F(st_test_print, st_test_print_sprintf)
{
    char b[2000];
    int len = sprintf(b, "%.0s", "goodbye");
    EXPECT_EQ(len, 0);

    strcpy(b, "xxxxxxxx");
    len = sprintf(b, "%d", 123456);
    EXPECT_EQ(len, 6);
    EXPECT_STREQ(b, "123456");
    EXPECT_EQ(b[5], '6');

    len = sprintf(b, "%.1022f", 0x1p-1021);
    EXPECT_EQ(len, 1024);

    int j;
    for (j = 0; int_tests[j].fmt; j++) {
        len = sprintf(b, int_tests[j].fmt, int_tests[j].i);
        EXPECT_EQ(len, strlen(int_tests[j].expect));
        EXPECT_STREQ(b, int_tests[j].expect);
    }

    for (j = 0; fp_tests[j].fmt; j++) {
        len = sprintf(b, fp_tests[j].fmt, fp_tests[j].f);
        EXPECT_EQ(len, strlen(fp_tests[j].expect));
        EXPECT_STREQ(b, fp_tests[j].expect);
    }

    char b2[8];
    memset(b2, '\0', sizeof(b2));
    len = sprintf(b2, "%s", "1234567");
    EXPECT_EQ(len, 7);
    EXPECT_STREQ(b2, "1234567");

}


TEST_F(st_test_print, st_test_print_snprintf)
{
    char b[2000];
    int len = snprintf(0, 0, "%d", 123456);
    EXPECT_EQ(len, 6);

    len = snprintf(0, 0, "%.4s", "hello");
    EXPECT_EQ(len, 4);

    len = snprintf(b, 0, "%.0s", "goodbye");
    EXPECT_EQ(len, 0);

    strcpy(b, "xxxxxxxx");
    len = snprintf(b, 4, "%d", 123456);
    EXPECT_EQ(len, 6);
    EXPECT_STREQ(b, "123");
    EXPECT_EQ(b[5], 'x');

    len = snprintf(b, sizeof(b), "%.1022f", 0x1p-1021);
    EXPECT_EQ(len, 1024);

    len = snprintf(NULL, 0, "%.*u", 2147483647);
    EXPECT_EQ(len, 2147483647);

    int j;
    for (j = 0; int_tests[j].fmt; j++) {
        len = snprintf(b, sizeof b, int_tests[j].fmt, int_tests[j].i);
        EXPECT_EQ(len, strlen(int_tests[j].expect));
        EXPECT_STREQ(b, int_tests[j].expect);
    }

    for (j = 0; fp_tests[j].fmt; j++) {
        len = snprintf(b, sizeof b, fp_tests[j].fmt, fp_tests[j].f);
        EXPECT_EQ(len, strlen(fp_tests[j].expect));
        EXPECT_STREQ(b, fp_tests[j].expect);
    }

    len = snprintf(0, 0, "%.4a", 1.0);
    EXPECT_EQ(len, 11);

    char b2[8];
    memset(b2, '\0', sizeof(b2));
    len = snprintf(b2, 4, "%s", "12345678");
    EXPECT_EQ(len, 8);
    EXPECT_STREQ(b2, "123");

    memset(b2, '\0', sizeof(b2));
    len = snprintf(b2, 8, "%s", "12345678");
    EXPECT_EQ(len, 8);
    EXPECT_STREQ(b2, "1234567");
}


#ifdef __cplusplus
extern "C" {
#endif

int testVsnprintf(char *s, size_t n, const char *fmt, ...)
{
    int ret;
    va_list ap;
    va_start(ap, fmt);
    ret = vsnprintf(s, n, fmt, ap);
    va_end(ap);
    return ret;
}

#ifdef __cplusplus
}
#endif

TEST_F(st_test_print, st_test_print_vsnprintf)
{
    char b[2000];
    int len = testVsnprintf(0, 0, "%d", 123456);
    EXPECT_EQ(len, 6);

    len = testVsnprintf(0, 0, "%.4s", "hello");
    EXPECT_EQ(len, 4);

    len = testVsnprintf(b, 0, "%.0s", "goodbye");
    EXPECT_EQ(len, 0);

    strcpy(b, "xxxxxxxx");
    len = testVsnprintf(b, 4, "%d", 123456);
    EXPECT_EQ(len, 6);
    EXPECT_STREQ(b, "123");
    EXPECT_EQ(b[5], 'x');

    len = testVsnprintf(b, sizeof(b), "%.1022f", 0x1p-1021);
    EXPECT_EQ(len, 1024);

    len = testVsnprintf(NULL, 0, "%.*u", 2147483647, 0);
    EXPECT_EQ(len, 2147483647);

    int j;
    for (j = 0; int_tests[j].fmt; j++) {
        len = testVsnprintf(b, sizeof b, int_tests[j].fmt, int_tests[j].i);
        EXPECT_EQ(len, strlen(int_tests[j].expect));
        EXPECT_STREQ(b, int_tests[j].expect);
    }

    for (j = 0; fp_tests[j].fmt; j++) {
        len = testVsnprintf(b, sizeof b, fp_tests[j].fmt, fp_tests[j].f);
        EXPECT_EQ(len, strlen(fp_tests[j].expect));
        EXPECT_STREQ(b, fp_tests[j].expect);
    }

    len = testVsnprintf(0, 0, "%.4a", 1.0);
    EXPECT_EQ(len, 11);
}

TEST_F(st_test_print, st_test_print_strerror)
{
    EXPECT_EQ(strcmp(strerror(EACCES), "Permission denied"), 0);
    EXPECT_EQ(strcmp(strerror(EPERM), "Operation not permitted"), 0);
    EXPECT_EQ(strcmp(strerror(EACCES), "Permission denied"), 0);
    EXPECT_EQ(strcmp(strerror(ENOMEM), "Out of memory"), 0);
    EXPECT_EQ(strcmp(strerror(EAGAIN), "Resource temporarily unavailable"), 0);
    EXPECT_EQ(strcmp(strerror(ETIMEDOUT), "Operation timed out"), 0);
    EXPECT_EQ(strcmp(strerror(EPIPE), "Broken pipe"), 0);

    EXPECT_EQ(strcmp(strerror(999), "No error information"), 0);
}