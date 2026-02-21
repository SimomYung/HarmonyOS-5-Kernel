/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: hpe
 */

#include "gtest/gtest.h"
#include "stub.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <execinfo.h>
#include <sys/time.h>


#ifdef __cplusplus
}
#endif

class ut_test_time : public testing::Test {
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

static void *TestCreateEntryForTime(void *arg)
{
    int *flag = (int *)arg;
    int index = *flag;
    time_t t1, t2;
    printf("I am TestCreateEntryForTime[%d]\n", index);

    *flag = 0XAB;
    usleep(10);

    t1 = time(0);
    time(&t2);
    EXPECT_EQ(t1, t2);

    sleep(1);
    t2 = time(0);
    EXPECT_EQ(t2 - t1, 1);

    return NULL;
}

TEST_F(ut_test_time, ut_clock_gettime_invalidparam)
{
    struct timespec tp1;
    int ret = clock_gettime(-1, &tp1);
    EXPECT_EQ(ret, -1);

    ret = clock_gettime(CLOCK_REALTIME, NULL);
    EXPECT_EQ(ret, -1);
}

TEST_F(ut_test_time, ut_clock_gettime_CLOCK_REALTIME)
{
    struct timespec tp1;
    struct timespec tp2;
    int ret = clock_gettime(CLOCK_REALTIME, &tp1);
    EXPECT_EQ(ret, 0);

    sleep(1);

    ret = clock_gettime(CLOCK_REALTIME, &tp2);

    uint64_t step = (tp2.tv_sec - tp1.tv_sec) * 1000000000 + (tp2.tv_nsec - tp1.tv_nsec);
    EXPECT_GE(step, 800000000);
    EXPECT_LE(step, 1200000000);
}

TEST_F(ut_test_time, ut_clock_gettime_CLOCK_MONOTONIC)
{
    struct timespec tp1;
    struct timespec tp2;
    int ret = clock_gettime(CLOCK_MONOTONIC, &tp1);
    EXPECT_EQ(ret, 0);

    sleep(1);

    ret = clock_gettime(CLOCK_MONOTONIC, &tp2);

    uint64_t step = (tp2.tv_sec - tp1.tv_sec) * 1000000000 + (tp2.tv_nsec - tp1.tv_nsec);
    EXPECT_GE(step, 800000000);
    EXPECT_LE(step, 1200000000);
}

/*
 * 不支持 CLOCK_PROCESS_CPUTIME_ID
 */
TEST_F(ut_test_time, ut_clock_gettime_CLOCK_PROCESS_CPUTIME_ID)
{
    struct timespec tp1;
    int ret = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tp1);
    EXPECT_EQ(ret, -1);
}

/*
 * 不支持 CLOCK_THREAD_CPUTIME_ID
 */
TEST_F(ut_test_time, ut_clock_gettime_CLOCK_THREAD_CPUTIME_ID)
{
    struct timespec tp1;
    int ret = clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tp1);
    EXPECT_EQ(ret, -1);
}

TEST_F(ut_test_time, ut_time)
{
    time_t t1, t2;
    t1 = time(0);
    time(&t2);
    EXPECT_EQ(t1, t2);

    sleep(1);
    t2 = time(0);
    EXPECT_EQ(t2 - t1, 1);
}

TEST_F(ut_test_time, ut_gettimeofday_invalidparam)
{
    struct timeval tv1;
    struct timeval tv2;
    struct timezone tz;
    int ret;
    ret = gettimeofday(NULL, &tz);
    EXPECT_EQ(ret, 0);

    ret = gettimeofday(&tv2, NULL);
    EXPECT_EQ(ret, 0);
}

TEST_F(ut_test_time, ut_gettimeofday)
{
    struct timeval tv1;
    struct timeval tv2;
    struct timezone tz;
    int ret;
    ret = gettimeofday(&tv1, &tz);
    EXPECT_EQ(ret, 0);

    sleep(1);
    ret = gettimeofday(&tv2, &tz);
    EXPECT_EQ(ret, 0);

    uint64_t step = (tv2.tv_sec - tv1.tv_sec) * 1000000 + (tv2.tv_usec - tv1.tv_usec);
    EXPECT_GE(step, 800000);
    EXPECT_LE(step, 1200000);
}

TEST_F(ut_test_time, ut_localtime_r)
{
    time_t curTime = time(NULL);
    time_t aftTime = curTime + 3600 * 3;
    struct tm tm1;
    localtime_r(&curTime, &tm1);
    EXPECT_GE(tm1.tm_year + 1900, 1900);
    EXPECT_EQ((tm1.tm_mon + 1) >= 1 && (tm1.tm_mon + 1) <= 12, 1);
    EXPECT_EQ((tm1.tm_mday + 1) >= 1 && (tm1.tm_mon + 1) <= 29, 1);
    EXPECT_EQ((tm1.tm_hour + 1) >= 0 && (tm1.tm_mon + 1) <= 23, 1);
    EXPECT_EQ((tm1.tm_min + 1) >= 0 && (tm1.tm_min + 1) <= 59, 1);
    EXPECT_EQ((tm1.tm_sec + 1) >= 0 && (tm1.tm_sec + 1) <= 59, 1);
}

TEST_F(ut_test_time, ut_localtime)
{
    time_t curTime = time(NULL);
    time_t aftTime = curTime + 3600 * 3;
    struct tm tm1;
    localtime(&curTime);
    EXPECT_GE(tm1.tm_year + 1900, 1900);
    EXPECT_EQ((tm1.tm_mon + 1) >= 1 && (tm1.tm_mon + 1) <= 12, 1);
    EXPECT_EQ((tm1.tm_mday + 1) >= 1 && (tm1.tm_mon + 1) <= 29, 1);
    EXPECT_EQ((tm1.tm_hour + 1) >= 0 && (tm1.tm_mon + 1) <= 23, 1);
    EXPECT_EQ((tm1.tm_min + 1) >= 0 && (tm1.tm_min + 1) <= 59, 1);
    EXPECT_EQ((tm1.tm_sec + 1) >= 0 && (tm1.tm_sec + 1) <= 59, 1);
}

TEST_F(ut_test_time, ut_mktime)
{
    struct tm timep1;
    timep1.tm_year = 2000 - 1900;
    timep1.tm_mon = 4;
    timep1.tm_mday = 1;
    timep1.tm_hour = 0;
    timep1.tm_min = 0;
    timep1.tm_sec = 10;
    timep1.tm_isdst = -1;

    time_t tm = mktime(&timep1);
    EXPECT_EQ(tm, 957139210);
}

TEST_F(ut_test_time, ut_strftime)
{
    struct tm timep1;
    timep1.tm_year = 2000 - 1900;
    timep1.tm_mon = 4;
    timep1.tm_mday = 1;
    timep1.tm_hour = 0;
    timep1.tm_min = 0;
    timep1.tm_sec = 10;
    timep1.tm_isdst = -1;

    mktime(&timep1);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%c", &timep1);
    EXPECT_STREQ(buffer, "Mon May  1 00:00:10 2000");
}


TEST_F(ut_test_time, ut_strptime)
{
    const struct {
        const char *locale;
        const char *input;
        const char *format;
        int mon;
        int mday;
    } day_tests[] = {
        { "C", "19990502123412", "%Y%m%d%H%M%S", 4, 2 },
    };

    struct tm tm;
    size_t i;

    for (i = 0; i < sizeof(day_tests) / sizeof(day_tests[0]); ++i) {
        memset(&tm, '\0', sizeof(tm));

        if (setlocale(LC_ALL, day_tests[i].locale) == NULL) {
            printf("cannot set locale %s: %m\n", day_tests[i].locale);
        }

        strptime(day_tests[i].input, day_tests[i].format, &tm);

        EXPECT_EQ(tm.tm_mon, day_tests[i].mon);
        EXPECT_EQ(tm.tm_mday, day_tests[i].mday);
    }

}


TEST_F(ut_test_time, ut_difftime)
{
    time_t start_t, end_t;
    time(&start_t);
    sleep(2);
    time(&end_t);
    double diff_t = difftime(end_t, start_t);
    EXPECT_DOUBLE_EQ(diff_t, 2.00);
}

TEST_F(ut_test_time, ut_asctime)
{
    struct tm t;

    t.tm_sec = 10;
    t.tm_min = 10;
    t.tm_hour = 6;
    t.tm_mday = 25;
    t.tm_mon = 2;
    t.tm_year = 89;
    t.tm_wday = 6;

    EXPECT_STREQ(asctime(&t), "Sat Mar 25 06:10:10 1989\n");
}

TEST_F(ut_test_time, ut_gmtime)
{
    struct tm timep1;
    timep1.tm_year = 2000 - 1900;
    timep1.tm_mon = 4;
    timep1.tm_mday = 1;
    timep1.tm_hour = 0;
    timep1.tm_min = 0;
    timep1.tm_sec = 10;
    timep1.tm_isdst = -1;

    time_t rawtime = mktime(&timep1);
    struct tm *info = gmtime(&rawtime);

    EXPECT_EQ(info->tm_year, timep1.tm_year);
    EXPECT_EQ(info->tm_mon, timep1.tm_mon);
    EXPECT_EQ(info->tm_mday, timep1.tm_mday);
    EXPECT_EQ(info->tm_hour, timep1.tm_hour);
    EXPECT_EQ(info->tm_min, timep1.tm_min);
    EXPECT_EQ(info->tm_sec, timep1.tm_sec);
}

TEST_F(ut_test_time, ut_asctime_r)
{
    struct tm t;

    t.tm_sec = 10;
    t.tm_min = 10;
    t.tm_hour = 6;
    t.tm_mday = 25;
    t.tm_mon = 2;
    t.tm_year = 89;
    t.tm_wday = 6;

    char buf[64] = {0};
    asctime_r(&t, buf);
    EXPECT_STREQ(buf, "Sat Mar 25 06:10:10 1989\n");
}

TEST_F(ut_test_time, ut_gmtime_r)
{
    struct tm timep1;
    timep1.tm_year = 2000 - 1900;
    timep1.tm_mon = 4;
    timep1.tm_mday = 1;
    timep1.tm_hour = 0;
    timep1.tm_min = 0;
    timep1.tm_sec = 10;
    timep1.tm_isdst = -1;

    time_t rawtime = mktime(&timep1);
    struct tm info;
    EXPECT_NE((uintptr_t)gmtime_r(&rawtime, &info), NULL);

    EXPECT_EQ(info.tm_year, timep1.tm_year);
    EXPECT_EQ(info.tm_mon, timep1.tm_mon);
    EXPECT_EQ(info.tm_mday, timep1.tm_mday);
    EXPECT_EQ(info.tm_hour, timep1.tm_hour);
    EXPECT_EQ(info.tm_min, timep1.tm_min);
    EXPECT_EQ(info.tm_sec, timep1.tm_sec);
}

TEST_F(ut_test_time, ut_ctime_r)
{
    struct tm timep1;
    timep1.tm_year = 2000 - 1900;
    timep1.tm_mon = 4;
    timep1.tm_mday = 1;
    timep1.tm_hour = 0;
    timep1.tm_min = 0;
    timep1.tm_sec = 10;
    timep1.tm_isdst = -1;

    time_t rawtime = mktime(&timep1);
    char buf[64] = {0};
    EXPECT_NE((uintptr_t)ctime_r(&rawtime, buf), NULL);
    EXPECT_STREQ(buf, "Mon May  1 00:00:10 2000\n");
}

TEST_F(ut_test_time, ut_multithread_time)
{
    int flag[64] = {0};
    int cnt = 0;
    int ret;
    int success = 0;
    time_t t1, t2;

    t1 = time(0);
    time(&t2);
    EXPECT_EQ(t1, t2);

    pthread_t lwt[64];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (int i = 0; i < 64; i++) {
        flag[i] = i;
        int ret = pthread_create(&lwt[i], &attr, TestCreateEntryForTime, &flag[i]);
        EXPECT_EQ(ret, 0);
 
    }

    for (int i = 0; i < 64; i++) {
        ret = pthread_join(lwt[i], NULL);
        EXPECT_EQ(ret, 0);

        EXPECT_EQ(flag[i], 0XAB);
        if (flag[i] == 0XAB) {
            success++;
        }
    }

    EXPECT_EQ(success, 64);
    sleep(1);
    t2 = time(0);
    EXPECT_LE(t2 - t1, 2);

}
