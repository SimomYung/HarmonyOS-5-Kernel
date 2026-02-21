#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <stdarg.h>
#include <limits.h>
#include "stub.h"
#include "gtest/gtest.h"

class ut_test_realpath : public testing::Test {
protected:
    static void SetUpTestSuite() {}

    static void TearDownTestSuite() {}

    virtual void SetUp() {}

    virtual void TearDown() {}
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/* 正常使用 */
TEST_F(ut_test_realpath, ut_realpath_01)
{
    const char path[PATH_MAX] = "/var/../var/run/../run";
    char rpath[PATH_MAX] = { 0 };
    const char expect[PATH_MAX] = "/var/run";

    char *retPath = realpath(path, rpath);
    if(retPath == NULL) {
        return;
    }

    EXPECT_STREQ(retPath, expect);
    printf("retPath: %s\n", retPath);
}

/* rpath传NULL */
TEST_F(ut_test_realpath, ut_realpath_02)
{
    const char path[PATH_MAX] = "/var/../var/run/..";
    char *rpath = NULL;
    const char expect[PATH_MAX] = "/var";

    char *retPath = realpath(path, rpath);
    if(retPath == NULL) {
        return;
    }

    EXPECT_STREQ(retPath, expect);
    printf("retPath: %s\n", retPath);

    free(retPath);
}

/* path传NULL */
TEST_F(ut_test_realpath, ut_realpath_03)
{
    const char *path = NULL;
    char rpath[PATH_MAX] = { 0 };

    char *retPath = realpath(path, rpath);
    EXPECT_EQ((uintptr_t)retPath, 0);
}

/* path传空字符串 */
TEST_F(ut_test_realpath, ut_realpath_04)
{
    const char path[PATH_MAX] = "";
    char rpath[PATH_MAX] = { 0 };

    char *retPath = realpath(path, rpath);
    EXPECT_EQ((uintptr_t)retPath, 0);
}

/* 路径不存在 */
TEST_F(ut_test_realpath, ut_realpath_05)
{
    const char path[PATH_MAX] = "/run/../run";
    char rpath[PATH_MAX] = { 0 };

    char *retPath = realpath(path, rpath);
    EXPECT_EQ((uintptr_t)retPath, 0);
}