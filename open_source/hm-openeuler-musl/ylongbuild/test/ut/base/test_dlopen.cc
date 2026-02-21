#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <dlfcn.h>
#include "stub.h"
#include "gtest/gtest.h"


class st_test_dlopen : public testing::Test {
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

void TestDlopen(void) __attribute__((optimize("O0")));

void TestDlopen(void)
{
    printf("just for testing dlopen\n");
}

#ifdef __cplusplus
}
#endif

TEST_F(st_test_dlopen, st_test_dlopen)
{
    const char *hlibcSo = "libehlibc.so";

    void *h = dlopen(hlibcSo, RTLD_LAZY | RTLD_LOCAL);
    EXPECT_NE((uintptr_t)h, NULL);

    void *h1 = dlopen(hlibcSo, RTLD_LAZY | RTLD_LOCAL);
    EXPECT_EQ((uintptr_t)h, (uintptr_t)h1);

    EXPECT_EQ(dlclose(h1), 0);

    void *h2 = dlopen(hlibcSo, RTLD_LOCAL);
    EXPECT_EQ((uintptr_t)h2, 0);

    void *h3 = dlopen("no_exist.so", RTLD_LAZY | RTLD_LOCAL);
    EXPECT_EQ((uintptr_t)h3, NULL);

    void *h4 = dlopen(0, RTLD_GLOBAL | RTLD_NOW);
    EXPECT_NE((uintptr_t)h4, 0);
    EXPECT_EQ(dlclose(h4), 0);

    const char *hlibcCommitID = (const char *)dlsym(h, "g_HlibcCommitID");
    EXPECT_NE((uintptr_t)hlibcCommitID, NULL);
    printf("hlibcCommitID=%s\n", hlibcCommitID);

    EXPECT_EQ(dlclose(h), 0);
    EXPECT_NE(dlclose(0), 0);
}

TEST_F(st_test_dlopen, st_test_dlerror)
{
    void *g = dlopen(0, RTLD_LAZY | RTLD_LOCAL);
    EXPECT_NE((uintptr_t)g, NULL);

    const char *defaultHandle = (const char *)dlsym(RTLD_DEFAULT, "TestDlopen");
    EXPECT_NE((uintptr_t)defaultHandle, NULL);

    const char *nextHandle = (const char *)dlsym(RTLD_NEXT, "TestDlopen");
    EXPECT_EQ((uintptr_t)nextHandle, NULL);

    const char *hlibcCommitID = (const char *)dlsym(g, "NotExist");
    EXPECT_EQ((uintptr_t)hlibcCommitID, NULL);
    char *s = dlerror();
    EXPECT_NE((uintptr_t)s, 0);
    EXPECT_STREQ(s, "Symbol not found: NotExist");

    EXPECT_EQ((uintptr_t)dlsym(g, "TestDlopen"), (uintptr_t)TestDlopen);
    EXPECT_EQ(dlclose(g), 0);
}

TEST_F(st_test_dlopen, st_test_dladdr)
{
    Dl_info info;

    EXPECT_NE(dladdr((void *)TestDlopen, &info), 0);

    EXPECT_NE((uintptr_t)info.dli_fbase, NULL);
    EXPECT_NE((uintptr_t)info.dli_fname, NULL);
    EXPECT_NE((uintptr_t)info.dli_saddr, NULL);
    EXPECT_NE((uintptr_t)info.dli_sname, NULL);

    EXPECT_STREQ(info.dli_sname, "TestDlopen");
    EXPECT_EQ((uintptr_t)info.dli_saddr, (uintptr_t)TestDlopen);

    printf("[%s+0x%lx][%s+0x%lx]\n", info.dli_sname, info.dli_saddr, info.dli_fname, info.dli_fbase);
}
