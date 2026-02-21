#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include "stub.h"
#include "gtest/gtest.h"


class st_test_pthread_cond : public testing::Test {
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

typedef struct {
    pthread_mutex_t *m;
    pthread_cond_t *cond;
    int loop;
    int finish;
} TestCond;

static void *StartSignal(void *arg)
{
    TestCond *a = (TestCond *)arg;
    EXPECT_EQ(pthread_mutex_lock(a->m), 0);
    EXPECT_EQ(pthread_cond_signal(a->cond), 0);
    EXPECT_EQ(pthread_mutex_unlock(a->m), 0);

    a->finish = 1;
    return 0;
}

static void *StartWait(void *arg)
{
    TestCond *a = (TestCond *)arg;

    pthread_mutex_lock(a->m);
    while (a->loop) {
        pthread_cond_wait(a->cond, a->m);
    }
    pthread_mutex_unlock(a->m);

    a->finish++;
    return 0;
}

#ifdef __cplusplus
}
#endif

/*
 * 测试场景：主线程wait，子线程signal，
 * 期望：主线程可以被唤醒
 */
TEST_F(st_test_pthread_cond, st_test_pthread_cond_signal_1thread)
{
    pthread_mutex_t mtx;
    pthread_cond_t cond;
    TestCond testCond = {&mtx, &cond, 1, 0};

    EXPECT_EQ(pthread_mutex_init(&mtx, 0), 0);
    EXPECT_EQ(pthread_cond_init(&cond, 0), 0);
    EXPECT_EQ(pthread_mutex_lock(&mtx), 0);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_t t;
    int ret = pthread_create(&t, &attr, StartSignal, &testCond);
    if(ret != 0) {
        printf("pthread_create failed, ret = %d!errno=%d, %s\n", ret, errno, strerror(errno));
        return;
    }

    EXPECT_EQ(pthread_cond_wait(&cond, &mtx), 0);
    EXPECT_EQ(pthread_mutex_unlock(&mtx), 0);

    EXPECT_EQ(pthread_mutex_destroy(&mtx), 0);
    EXPECT_EQ(pthread_cond_destroy(&cond), 0);
}

/*
 * 测试场景：主线程带超时wait
 * 期望：主线程会超时
 */
TEST_F(st_test_pthread_cond, st_test_pthread_cond_timedwait_1thread)
{
    pthread_mutex_t mtx;
    pthread_cond_t cond;
    TestCond testCond = {&mtx, &cond, 1, 0};
    struct timespec timeOut = {0};
    timeOut.tv_sec = 1;

    EXPECT_EQ(pthread_mutex_init(&mtx, 0), 0);
    EXPECT_EQ(pthread_cond_init(&cond, 0), 0);
    EXPECT_EQ(pthread_mutex_lock(&mtx), 0);
    EXPECT_EQ(pthread_cond_timedwait(&cond, &mtx, &timeOut), ETIMEDOUT);
    EXPECT_EQ(pthread_mutex_unlock(&mtx), 0);

    EXPECT_EQ(pthread_mutex_destroy(&mtx), 0);
    EXPECT_EQ(pthread_cond_destroy(&cond), 0);
}

/*
 * 测试场景：主线程signal，子线程wait，
 * 期望：子线程可以被唤醒
 */
TEST_F(st_test_pthread_cond, st_test_pthread_cond_wait_1thread)
{
    pthread_mutex_t mtx;
    pthread_cond_t cond;
    TestCond testCond = {&mtx, &cond, 1, 0};

    EXPECT_EQ(pthread_mutex_init(&mtx, 0), 0);
    EXPECT_EQ(pthread_cond_init(&cond, 0), 0);
    EXPECT_EQ(pthread_mutex_lock(&mtx), 0);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_t t;
    int ret = pthread_create(&t, &attr, StartWait, &testCond);
    if(ret != 0) {
        printf("pthread_create failed, ret = %d!errno=%d, %s\n", ret, errno, strerror(errno));
        return;
    }
    EXPECT_EQ(pthread_mutex_unlock(&mtx), 0);

    sleep(1); //后面封装一个wait函数，将这种写死的等待统一替换掉，加速用例执行过程。
    testCond.loop = 0;
    EXPECT_EQ(pthread_mutex_lock(&mtx), 0);
    EXPECT_EQ(pthread_cond_signal(&cond), 0);
    EXPECT_EQ(pthread_mutex_unlock(&mtx), 0);

    sleep(1);
    EXPECT_EQ(testCond.finish, 1);

    EXPECT_EQ(pthread_mutex_destroy(&mtx), 0);
    EXPECT_EQ(pthread_cond_destroy(&cond), 0);
}

/*
 * 测试场景：创建三个子线程wait，主线程依次调用signal
 * 期望：每次只唤醒1个线程
 */
TEST_F(st_test_pthread_cond, st_test_pthread_cond_wait_3thread)
{
    pthread_mutex_t mtx;
    pthread_cond_t cond;
    TestCond testCond = {&mtx, &cond, 1, 0};

    EXPECT_EQ(pthread_mutex_init(&mtx, 0), 0);
    EXPECT_EQ(pthread_cond_init(&cond, 0), 0);
    EXPECT_EQ(pthread_mutex_lock(&mtx), 0);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_t t[3];
    EXPECT_EQ(pthread_create(&t[0], &attr, StartWait, &testCond), 0);
    EXPECT_EQ(pthread_create(&t[1], &attr, StartWait, &testCond), 0);
    EXPECT_EQ(pthread_create(&t[2], &attr, StartWait, &testCond), 0);
    EXPECT_EQ(pthread_mutex_unlock(&mtx), 0);

    sleep(1);
    testCond.loop = 0;

    EXPECT_EQ(pthread_mutex_lock(&mtx), 0);
    EXPECT_EQ(pthread_cond_signal(&cond), 0);
    EXPECT_EQ(pthread_mutex_unlock(&mtx), 0);
    sleep(1);
    EXPECT_EQ(testCond.finish, 1);

    EXPECT_EQ(pthread_mutex_lock(&mtx), 0);
    EXPECT_EQ(pthread_cond_signal(&cond), 0);
    EXPECT_EQ(pthread_mutex_unlock(&mtx), 0);
    sleep(1);
    EXPECT_EQ(testCond.finish, 2);

    EXPECT_EQ(pthread_mutex_lock(&mtx), 0);
    EXPECT_EQ(pthread_cond_signal(&cond), 0);
    EXPECT_EQ(pthread_mutex_unlock(&mtx), 0);
    sleep(1);
    EXPECT_EQ(testCond.finish, 3);

    EXPECT_EQ(pthread_mutex_destroy(&mtx), 0);
    EXPECT_EQ(pthread_cond_destroy(&cond), 0);
}

/*
 * 测试场景：创建三个子线程wait，主线程broadcast
 * 期望：主线程调用一次broadcast可以唤醒所有子线程
 */
TEST_F(st_test_pthread_cond, st_test_pthread_cond_broadcast_3thread)
{
    pthread_mutex_t mtx;
    pthread_cond_t cond;
    TestCond testCond = {&mtx, &cond, 1, 0};

    EXPECT_EQ(pthread_mutex_init(&mtx, 0), 0);
    EXPECT_EQ(pthread_cond_init(&cond, 0), 0);
    EXPECT_EQ(pthread_mutex_lock(&mtx), 0);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_t t[3];
    EXPECT_EQ(pthread_create(&t[0], &attr, StartWait, &testCond), 0);
    EXPECT_EQ(pthread_create(&t[1], &attr, StartWait, &testCond), 0);
    EXPECT_EQ(pthread_create(&t[2], &attr, StartWait, &testCond), 0);
    EXPECT_EQ(pthread_mutex_unlock(&mtx), 0);

    sleep(1);
    testCond.loop = 0;

    EXPECT_EQ(pthread_mutex_lock(&mtx), 0);
    EXPECT_EQ(pthread_cond_broadcast(&cond), 0);
    EXPECT_EQ(pthread_mutex_unlock(&mtx), 0);
    sleep(1);
    EXPECT_EQ(testCond.finish, 3);

    EXPECT_EQ(pthread_mutex_destroy(&mtx), 0);
    EXPECT_EQ(pthread_cond_destroy(&cond), 0);
}

/*
 * 测试场景：创建三个子线程wait，主线程先调用一次signal，然后调用broadcast
 * 期望：主线程调用一次signal只唤醒一个线程，然后调用一次broadcast可以唤醒剩下的所有线程
 */
TEST_F(st_test_pthread_cond, st_test_pthread_cond_sigal_3thread)
{
    pthread_mutex_t mtx;
    pthread_cond_t cond;
    TestCond testCond = {&mtx, &cond, 1, 0};

    EXPECT_EQ(pthread_mutex_init(&mtx, 0), 0);
    EXPECT_EQ(pthread_cond_init(&cond, 0), 0);
    EXPECT_EQ(pthread_mutex_lock(&mtx), 0);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_t t[3];
    EXPECT_EQ(pthread_create(&t[0], &attr, StartWait, &testCond), 0);
    EXPECT_EQ(pthread_create(&t[1], &attr, StartWait, &testCond), 0);
    EXPECT_EQ(pthread_create(&t[2], &attr, StartWait, &testCond), 0);
    EXPECT_EQ(pthread_mutex_unlock(&mtx), 0);

    sleep(1);
    testCond.loop = 0;

    EXPECT_EQ(pthread_mutex_lock(&mtx), 0);
    EXPECT_EQ(pthread_cond_signal(&cond), 0);
    EXPECT_EQ(pthread_mutex_unlock(&mtx), 0);
    sleep(1);
    EXPECT_EQ(testCond.finish, 1);

    EXPECT_EQ(pthread_mutex_lock(&mtx), 0);
    EXPECT_EQ(pthread_cond_broadcast(&cond), 0);
    EXPECT_EQ(pthread_mutex_unlock(&mtx), 0);
    sleep(1);
    EXPECT_EQ(testCond.finish, 3);

    EXPECT_EQ(pthread_mutex_destroy(&mtx), 0);
    EXPECT_EQ(pthread_cond_destroy(&cond), 0);
}
