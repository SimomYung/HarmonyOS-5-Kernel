#include <stdint.h>

#include "gtest/gtest.h"
#include <time.h>
#include <sys/time.h>
#include <signal.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TIMER_FIRST     50000000
#define TIMER_INTERVAL  10000000

struct timeval tv;
time_t timeStart;

int flag = true;
int check = 0;
int count = 0;

void PthreadTimerHandle(int sig)
{
    gettimeofday(&tv, NULL);
    time_t interval = tv.tv_sec * 1000 + tv.tv_usec / 1000 - timeStart;
    timeStart = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    EXPECT_GE(interval, check / 1000000 - 3);
    EXPECT_LT(interval, check * 2 / 1000000);

    check = TIMER_INTERVAL;
    printf("[TEST_TIMER] Posix Timer! sigval: %d, line %d\n", sig, __LINE__);

    flag = false;
    count++;
}

void TimerTestOnce(void)
{
    struct sigevent sev;
    struct itimerspec ts;
    timer_t ptimer;
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, 50);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);

    signal(50, PthreadTimerHandle);

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = 50;
    sev.sigev_value.sival_ptr = &ptimer;
    timer_create(CLOCK_REALTIME, &sev, &ptimer);
    EXPECT_EQ(0, 0);

    flag = true;

    ts.it_value.tv_sec = 0;
    ts.it_value.tv_nsec = TIMER_FIRST;
    ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = 0;

    check = TIMER_FIRST;
    gettimeofday(&tv, NULL);
    timeStart = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    timer_settime(ptimer, 0, &ts, NULL);
    printf("sig test, set sig timer\n");
    EXPECT_EQ(0, 0);

    while (flag) {
        sched_yield();
    }

    timer_delete(ptimer);
    EXPECT_EQ(0, 0);
}

void TimerTestOpp(void)
{
    struct sigevent sev;
    struct itimerspec ts;
    timer_t ptimer;
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, 50);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);

    signal(50, PthreadTimerHandle);

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = 50;
    sev.sigev_value.sival_ptr = &ptimer;
    timer_create(CLOCK_REALTIME, &sev, &ptimer);
    EXPECT_EQ(0, 0);

    flag = true;
    count = 0;

    ts.it_value.tv_sec = 0;
    ts.it_value.tv_nsec = TIMER_FIRST;
    ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = TIMER_INTERVAL;

    check = TIMER_FIRST;
    gettimeofday(&tv, NULL);
    timeStart = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    timer_settime(ptimer, 0, &ts, NULL);
    printf("sig test, set sig timer\n");
    EXPECT_EQ(0, 0);

    while (flag || (count < 10)) {
        sched_yield();
    }

    timer_delete(ptimer);
    EXPECT_EQ(0, 0);
}

#ifdef __cplusplus
}
#endif

class ut_test_timer : public testing::Test {
protected:
    static void SetUpTestSuite()
    {
        return;
    }

    static void TearDownTestSuite()
    {
        return;
    }

    virtual void SetUp()
    {
        return;
    }

    virtual void TearDown()
    {
        return;
    }
};

TEST_F(ut_test_timer, TimerTestOnce)
{
    TimerTestOnce();
}

TEST_F(ut_test_timer, TimerTestOpp)
{
    TimerTestOpp();
}