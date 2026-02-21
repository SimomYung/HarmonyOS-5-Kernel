#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include <time.h>
#include "stub.h"
#include "gtest/gtest.h"


class st_test_sleep : public testing::Test {
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

TEST_F(st_test_sleep, st_test_sleep)
{
    struct timeval tv1;
    struct timeval tv2;
    struct timezone tz;
    int ret;

    for(int i = 0; i < 3; i++) {
        gettimeofday(&tv1, &tz);
        sleep(1);
        gettimeofday(&tv2, &tz);

        uint64_t step = (tv2.tv_sec - tv1.tv_sec) * 1000000 + (tv2.tv_usec - tv1.tv_usec);
        EXPECT_GE(step, 990000);
        EXPECT_LE(step, 2000000);
        printf("The diff of sleep(1) is %dus\n", step);
    }
}

TEST_F(st_test_sleep, st_test_usleep)
{
    struct timeval tv1;
    struct timeval tv2;
    struct timezone tz;
    int ret;

    // 1ms
    gettimeofday(&tv1, &tz);
    usleep(1000);
    gettimeofday(&tv2, &tz);

    uint64_t step = (tv2.tv_sec - tv1.tv_sec) * 1000000 + (tv2.tv_usec - tv1.tv_usec);
    EXPECT_GE(step, 900);
    EXPECT_LE(step, 1000000);
    printf("the diff of usleep(1000) is %dus\n", step);

    // 0.1ms
    gettimeofday(&tv1, &tz);
    usleep(100);
    gettimeofday(&tv2, &tz);

    step = (tv2.tv_sec - tv1.tv_sec) * 1000000 + (tv2.tv_usec - tv1.tv_usec);
    EXPECT_GE(step, 900);
    EXPECT_LE(step, 1000000);
    printf("the diff of usleep(100) is %dus\n", step);
}

TEST_F(st_test_sleep, st_test_nanosleep)
{
    struct timespec tp1;
    struct timespec tp2;
    struct timespec intval = {0}; 

    // 1s
    intval.tv_sec = 1;

    clock_gettime(CLOCK_REALTIME, &tp1);
    nanosleep(&intval, NULL);
    clock_gettime(CLOCK_REALTIME, &tp2);

    uint64_t step = (tp2.tv_sec - tp1.tv_sec) * 1000000000 + (tp2.tv_nsec - tp1.tv_nsec);
    EXPECT_GE(step,  990000000);
    EXPECT_LE(step, 2000000000);
    printf("the diff of nanosleep(%ld) is %dns\n", intval.tv_nsec, step);

    // 1ms
    intval.tv_sec = 0;
    intval.tv_nsec = 1000000;

    clock_gettime(CLOCK_REALTIME, &tp1);
    nanosleep(&intval, NULL);
    clock_gettime(CLOCK_REALTIME, &tp2);

    step = (tp2.tv_sec - tp1.tv_sec) * 1000000000 + (tp2.tv_nsec - tp1.tv_nsec);
    EXPECT_GE(step,  900000);
    EXPECT_LE(step, 1000000000);
    printf("the diff of nanosleep(%ld) is %dns\n", intval.tv_nsec, step);

    // 1us
    intval.tv_nsec = 1000;

    clock_gettime(CLOCK_REALTIME, &tp1);
    nanosleep(&intval, NULL);
    clock_gettime(CLOCK_REALTIME, &tp2);

    step = (tp2.tv_sec - tp1.tv_sec) * 1000000000 + (tp2.tv_nsec - tp1.tv_nsec);
    EXPECT_GE(step,  900000);
    EXPECT_LE(step, 1000000000);
    printf("the diff of nanosleep(%ld) is %dns\n", intval.tv_nsec, step);
}
