#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include "stub.h"
#include "gtest/gtest.h"


class st_test_pthread_spin : public testing::Test {
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
    pthread_spinlock_t *spin;
    int ret;
    int cnt[10];
    int finish;
} TestSpinlock;

static void *TrySpinlock(void *arg)
{
    TestSpinlock *spinlock = (TestSpinlock*)arg;

    spinlock->ret = pthread_spin_trylock(spinlock->spin);

    spinlock->finish = 1;
    return 0;
}

static void *Spinlock(void *arg)
{
    TestSpinlock *spinlock = (TestSpinlock*)arg;

    spinlock->ret = pthread_spin_lock(spinlock->spin);

    spinlock->finish = 1;
    return 0;
}

#ifdef __cplusplus
}
#endif

TEST_F(st_test_pthread_spin, st_test_pthread_spin_lock_unlock)
{
    pthread_spinlock_t spin;
    int ret;

    EXPECT_EQ(pthread_spin_init(&spin, 0), 0);

    for(int i = 0; i < 100; i++) {
        ret = pthread_spin_lock(&spin);
        EXPECT_EQ(ret, 0);

        ret = pthread_spin_unlock(&spin);
        EXPECT_EQ(ret, 0);
    }

    ret = pthread_spin_destroy(&spin);
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_pthread_spin, st_test_pthread_spin_unlock_lock)
{
    pthread_spinlock_t spin;
    int ret;

    EXPECT_EQ(pthread_spin_init(&spin, 0), 0);

    for(int i = 0; i < 100; i++) {
        ret = pthread_spin_unlock(&spin);
        EXPECT_EQ(ret, 0);

        ret = pthread_spin_lock(&spin);
        EXPECT_EQ(ret, 0);
    }

    ret = pthread_spin_destroy(&spin);
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_pthread_spin, st_test_pthread_spin_lock_trylock_unlock)
{
    pthread_spinlock_t spin;
    int ret;

    EXPECT_EQ(pthread_spin_init(&spin, 0), 0);

    for(int i = 0; i < 100; i++) {
        ret = pthread_spin_lock(&spin);
        EXPECT_EQ(ret, 0);

        ret = pthread_spin_trylock(&spin);
        EXPECT_EQ(ret, EBUSY);

        ret = pthread_spin_unlock(&spin);
        EXPECT_EQ(ret, 0);
    }

    ret = pthread_spin_destroy(&spin);
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_pthread_spin, st_test_pthread_spin_trylock_trylock_unlock)
{
    pthread_spinlock_t spin;
    int ret;

    EXPECT_EQ(pthread_spin_init(&spin, 0), 0);

    for(int i = 0; i < 100; i++) {
        ret = pthread_spin_trylock(&spin);
        EXPECT_EQ(ret, 0);

        ret = pthread_spin_trylock(&spin);
        EXPECT_EQ(ret, EBUSY);

        ret = pthread_spin_unlock(&spin);
        EXPECT_EQ(ret, 0);
    }

    ret = pthread_spin_destroy(&spin);
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_pthread_spin, st_test_pthread_spin_lock_trylock_2thread)
{
    pthread_spinlock_t spin;
    EXPECT_EQ(pthread_spin_init(&spin, 0), 0);

    TestSpinlock spinlock = {&spin, 0, {0}, 0};

    pthread_t lwt;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    int ret = pthread_spin_lock(&spin);
    EXPECT_EQ(ret, 0);

    ret = pthread_create(&lwt, &attr, TrySpinlock, &spinlock);
    EXPECT_EQ(ret, 0);

    sleep(1);

    EXPECT_EQ(spinlock.finish, 1);
    EXPECT_EQ(spinlock.ret, EBUSY);

    ret = pthread_spin_destroy(&spin);
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_pthread_spin, st_test_pthread_spin_lock_lock_2thread)
{
    pthread_spinlock_t spin;
    EXPECT_EQ(pthread_spin_init(&spin, 0), 0);

    TestSpinlock spinlock = {&spin, 0, {0}, 0};

    pthread_t lwt;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    int ret = pthread_spin_lock(&spin);
    EXPECT_EQ(ret, 0);

    ret = pthread_create(&lwt, &attr, Spinlock, &spinlock);
    EXPECT_EQ(ret, 0);

    sleep(1);

    EXPECT_EQ(spinlock.finish, 0);
    EXPECT_EQ(spinlock.ret, 0);

    ret = pthread_spin_unlock(&spin);
    EXPECT_EQ(ret, 0);

    sleep(1);

    EXPECT_EQ(spinlock.finish, 1);
    EXPECT_EQ(spinlock.ret, 0);

    ret = pthread_spin_destroy(&spin);
    EXPECT_EQ(ret, 0);
}

