#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include "stub.h"
#include "gtest/gtest.h"


class st_test_pthread_rwlock : public testing::Test {
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
    pthread_rwlock_t *rw;
    int ret;
    int finish;
} TestRwlock;

static void *TryRdlock(void *arg)
{
    TestRwlock *rwlock = (TestRwlock*)arg;

    rwlock->ret = pthread_rwlock_tryrdlock(rwlock->rw);

    rwlock->finish = 1;
    return 0;
}

static void *TryWrlock(void *arg)
{
    TestRwlock *rwlock = (TestRwlock*)arg;

    rwlock->ret = pthread_rwlock_trywrlock(rwlock->rw);

    rwlock->finish = 1;
    return 0;
}

static void *Rdlock(void *arg)
{
    TestRwlock *rwlock = (TestRwlock*)arg;

    rwlock->ret = pthread_rwlock_rdlock(rwlock->rw);

    rwlock->finish = 1;
    return 0;
}

static void *Wrlock(void *arg)
{
    TestRwlock *rwlock = (TestRwlock*)arg;

    rwlock->ret = pthread_rwlock_wrlock(rwlock->rw);

    rwlock->finish = 1;
    return 0;
}

#ifdef __cplusplus
}
#endif

TEST_F(st_test_pthread_rwlock, st_test_pthread_rdlock_normal_1thread)
{
    pthread_rwlock_t rw = PTHREAD_RWLOCK_INITIALIZER;
    int ret;

    for(int i = 0; i < 100; i++) {
        ret = pthread_rwlock_rdlock(&rw);
        EXPECT_EQ(ret, 0);

        ret = pthread_rwlock_unlock(&rw);
        EXPECT_EQ(ret, 0);
    }

    ret = pthread_rwlock_destroy(&rw);
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_pthread_rwlock, st_test_pthread_tryrdlock_normal_1thread)
{
    pthread_rwlock_t rw = PTHREAD_RWLOCK_INITIALIZER;
    int ret;

    for(int i = 0; i < 100; i++) {
        ret = pthread_rwlock_tryrdlock(&rw);
        EXPECT_EQ(ret, 0);

        ret = pthread_rwlock_unlock(&rw);
        EXPECT_EQ(ret, 0);
    }

    ret = pthread_rwlock_destroy(&rw);
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_pthread_rwlock, st_test_pthread_wrlock_normal_1thread)
{
    pthread_rwlock_t rw = PTHREAD_RWLOCK_INITIALIZER;
    int ret;

    for(int i = 0; i < 100; i++) {
        ret = pthread_rwlock_wrlock(&rw);
        EXPECT_EQ(ret, 0);

        ret = pthread_rwlock_unlock(&rw);
        EXPECT_EQ(ret, 0);
    }

    ret = pthread_rwlock_destroy(&rw);
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_pthread_rwlock, st_test_pthread_trywrlock_normal_1thread)
{
    pthread_rwlock_t rw = PTHREAD_RWLOCK_INITIALIZER;
    int ret;

    for(int i = 0; i < 100; i++) {
        ret = pthread_rwlock_trywrlock(&rw);
        EXPECT_EQ(ret, 0);

        ret = pthread_rwlock_unlock(&rw);
        EXPECT_EQ(ret, 0);
    }

    ret = pthread_rwlock_destroy(&rw);
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_pthread_rwlock, st_test_pthread_rdlock_tryrdlock_unlock_1thread)
{
    pthread_rwlock_t rw = PTHREAD_RWLOCK_INITIALIZER;
    int ret;

    for(int i = 0; i < 100; i++) {
        ret = pthread_rwlock_rdlock(&rw);
        EXPECT_EQ(ret, 0);

        ret = pthread_rwlock_tryrdlock(&rw);
        EXPECT_EQ(ret, 0);

        ret = pthread_rwlock_unlock(&rw);
        EXPECT_EQ(ret, 0);
    }

    ret = pthread_rwlock_destroy(&rw);
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_pthread_rwlock, st_test_pthread_tryrdlock_tryrdlock_unlock_1thread)
{
    pthread_rwlock_t rw = PTHREAD_RWLOCK_INITIALIZER;
    int ret;

    for(int i = 0; i < 100; i++) {
        ret = pthread_rwlock_tryrdlock(&rw);
        EXPECT_EQ(ret, 0);

        ret = pthread_rwlock_tryrdlock(&rw);
        EXPECT_EQ(ret, 0);

        ret = pthread_rwlock_unlock(&rw);
        EXPECT_EQ(ret, 0);
    }

    ret = pthread_rwlock_destroy(&rw);
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_pthread_rwlock, st_test_pthread_rdlock_trywrlock_unlock_1thread)
{
    pthread_rwlock_t rw = PTHREAD_RWLOCK_INITIALIZER;
    int ret;

    for(int i = 0; i < 100; i++) {
        ret = pthread_rwlock_rdlock(&rw);
        EXPECT_EQ(ret, 0);

        ret = pthread_rwlock_trywrlock(&rw);
        EXPECT_EQ(ret, EBUSY);

        ret = pthread_rwlock_unlock(&rw);
        EXPECT_EQ(ret, 0);
    }

    ret = pthread_rwlock_destroy(&rw);
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_pthread_rwlock, st_test_pthread_tryrdlock_trywrlock_unlock_1thread)
{
    pthread_rwlock_t rw = PTHREAD_RWLOCK_INITIALIZER;
    int ret;

    for(int i = 0; i < 100; i++) {
        ret = pthread_rwlock_tryrdlock(&rw);
        EXPECT_EQ(ret, 0);

        ret = pthread_rwlock_trywrlock(&rw);
        EXPECT_EQ(ret, EBUSY);

        ret = pthread_rwlock_unlock(&rw);
        EXPECT_EQ(ret, 0);
    }

    ret = pthread_rwlock_destroy(&rw);
    EXPECT_EQ(ret, 0);
}


TEST_F(st_test_pthread_rwlock, st_test_pthread_wrlock_tryrdlock_unlock_1thread)
{
    pthread_rwlock_t rw = PTHREAD_RWLOCK_INITIALIZER;
    int ret;

    for(int i = 0; i < 100; i++) {
        ret = pthread_rwlock_wrlock(&rw);
        EXPECT_EQ(ret, 0);

        ret = pthread_rwlock_tryrdlock(&rw);
        EXPECT_EQ(ret, EBUSY);

        ret = pthread_rwlock_unlock(&rw);
        EXPECT_EQ(ret, 0);
    }

    ret = pthread_rwlock_destroy(&rw);
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_pthread_rwlock, st_test_pthread_trywrlock_tryrdlock_unlock_1thread)
{
    pthread_rwlock_t rw = PTHREAD_RWLOCK_INITIALIZER;
    int ret;

    for(int i = 0; i < 100; i++) {
        ret = pthread_rwlock_trywrlock(&rw);
        EXPECT_EQ(ret, 0);

        ret = pthread_rwlock_tryrdlock(&rw);
        EXPECT_EQ(ret, EBUSY);

        ret = pthread_rwlock_unlock(&rw);
        EXPECT_EQ(ret, 0);
    }

    ret = pthread_rwlock_destroy(&rw);
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_pthread_rwlock, st_test_pthread_wrlock_wrlock_unlock_1thread)
{
    pthread_rwlock_t rw = PTHREAD_RWLOCK_INITIALIZER;
    int ret;

    for(int i = 0; i < 100; i++) {
        ret = pthread_rwlock_wrlock(&rw);
        EXPECT_EQ(ret, 0);

        ret = pthread_rwlock_trywrlock(&rw);
        EXPECT_EQ(ret, EBUSY);

        ret = pthread_rwlock_unlock(&rw);
        EXPECT_EQ(ret, 0);
    }

    ret = pthread_rwlock_destroy(&rw);
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_pthread_rwlock, st_test_pthread_trywrlock_trywrlock_unlock_1thread)
{
    pthread_rwlock_t rw = PTHREAD_RWLOCK_INITIALIZER;
    int ret;

    for(int i = 0; i < 100; i++) {
        ret = pthread_rwlock_trywrlock(&rw);
        EXPECT_EQ(ret, 0);

        ret = pthread_rwlock_trywrlock(&rw);
        EXPECT_EQ(ret, EBUSY);

        ret = pthread_rwlock_unlock(&rw);
        EXPECT_EQ(ret, 0);
    }

    ret = pthread_rwlock_destroy(&rw);
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_pthread_rwlock, st_test_pthread_rdlock_tryrdlock_2thread)
{
    pthread_rwlock_t rw = PTHREAD_RWLOCK_INITIALIZER;
    TestRwlock rwlock = {&rw, 0, 0};

    pthread_t lwt;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    int ret = pthread_rwlock_rdlock(&rw);
    EXPECT_EQ(ret, 0);

    ret = pthread_create(&lwt, &attr, TryRdlock, &rwlock);
    EXPECT_EQ(ret, 0);

    sleep(1);

    EXPECT_EQ(rwlock.finish, 1);
    EXPECT_EQ(rwlock.ret, 0);

    ret = pthread_rwlock_destroy(&rw);
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_pthread_rwlock, st_test_pthread_rdlock_trywrlock_2thread)
{
    pthread_rwlock_t rw = PTHREAD_RWLOCK_INITIALIZER;
    TestRwlock rwlock = {&rw, 0, 0};

    pthread_t lwt;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    int ret = pthread_rwlock_rdlock(&rw);
    EXPECT_EQ(ret, 0);

    ret = pthread_create(&lwt, &attr, TryWrlock, &rwlock);
    EXPECT_EQ(ret, 0);

    sleep(1);

    EXPECT_EQ(rwlock.finish, 1);
    EXPECT_EQ(rwlock.ret, EBUSY);

    ret = pthread_rwlock_destroy(&rw);
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_pthread_rwlock, st_test_pthread_rdlock_rdlock_2thread)
{
    pthread_rwlock_t rw = PTHREAD_RWLOCK_INITIALIZER;
    TestRwlock rwlock = {&rw, 0, 0};

    pthread_t lwt;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    int ret = pthread_rwlock_rdlock(&rw);
    EXPECT_EQ(ret, 0);

    ret = pthread_create(&lwt, &attr, Rdlock, &rwlock);
    EXPECT_EQ(ret, 0);

    sleep(1);

    EXPECT_EQ(rwlock.finish, 1);
    EXPECT_EQ(rwlock.ret, 0);

    ret = pthread_rwlock_destroy(&rw);
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_pthread_rwlock, st_test_pthread_rdlock_wrlock_2thread)
{
    pthread_rwlock_t rw = PTHREAD_RWLOCK_INITIALIZER;
    TestRwlock rwlock = {&rw, 0, 0};

    pthread_t lwt;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    int ret = pthread_rwlock_rdlock(&rw);
    EXPECT_EQ(ret, 0);

    ret = pthread_create(&lwt, &attr, Wrlock, &rwlock);
    EXPECT_EQ(ret, 0);

    sleep(1);

    EXPECT_EQ(rwlock.finish, 0);
    EXPECT_EQ(rwlock.ret, 0);

    ret = pthread_rwlock_unlock(&rw);
    EXPECT_EQ(ret, 0);

    sleep(1);

    EXPECT_EQ(rwlock.finish, 1);
    EXPECT_EQ(rwlock.ret, 0);

    ret = pthread_rwlock_destroy(&rw);
    EXPECT_EQ(ret, 0);
}

// 先加写锁
TEST_F(st_test_pthread_rwlock, st_test_pthread_wrlock_tryrdlock_2thread)
{
    pthread_rwlock_t rw = PTHREAD_RWLOCK_INITIALIZER;
    TestRwlock rwlock = {&rw, 0, 0};

    pthread_t lwt;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    int ret = pthread_rwlock_wrlock(&rw);
    EXPECT_EQ(ret, 0);

    ret = pthread_create(&lwt, &attr, TryRdlock, &rwlock);
    EXPECT_EQ(ret, 0);

    sleep(1);

    EXPECT_EQ(rwlock.finish, 1);
    EXPECT_EQ(rwlock.ret, EBUSY);

    ret = pthread_rwlock_destroy(&rw);
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_pthread_rwlock, st_test_pthread_wrlock_trywrlock_2thread)
{
    pthread_rwlock_t rw = PTHREAD_RWLOCK_INITIALIZER;
    TestRwlock rwlock = {&rw, 0, 0};

    pthread_t lwt;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    int ret = pthread_rwlock_wrlock(&rw);
    EXPECT_EQ(ret, 0);

    ret = pthread_create(&lwt, &attr, TryWrlock, &rwlock);
    EXPECT_EQ(ret, 0);

    sleep(1);

    EXPECT_EQ(rwlock.finish, 1);
    EXPECT_EQ(rwlock.ret, EBUSY);

    ret = pthread_rwlock_destroy(&rw);
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_pthread_rwlock, st_test_pthread_wrlock_rdlock_2thread)
{
    pthread_rwlock_t rw = PTHREAD_RWLOCK_INITIALIZER;
    TestRwlock rwlock = {&rw, 0, 0};

    pthread_t lwt;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    int ret = pthread_rwlock_wrlock(&rw);
    EXPECT_EQ(ret, 0);

    ret = pthread_create(&lwt, &attr, Rdlock, &rwlock);
    EXPECT_EQ(ret, 0);

    sleep(1);

    EXPECT_EQ(rwlock.finish, 0);
    EXPECT_EQ(rwlock.ret, 0);

    ret = pthread_rwlock_unlock(&rw);
    EXPECT_EQ(ret, 0);

    sleep(1);

    EXPECT_EQ(rwlock.finish, 1);
    EXPECT_EQ(rwlock.ret, 0);

    ret = pthread_rwlock_destroy(&rw);
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_pthread_rwlock, st_test_pthread_wrlock_wrlock_2thread)
{
    pthread_rwlock_t rw = PTHREAD_RWLOCK_INITIALIZER;
    TestRwlock rwlock = {&rw, 0, 0};

    pthread_t lwt;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    int ret = pthread_rwlock_wrlock(&rw);
    EXPECT_EQ(ret, 0);

    ret = pthread_create(&lwt, &attr, Wrlock, &rwlock);
    EXPECT_EQ(ret, 0);

    sleep(1);

    EXPECT_EQ(rwlock.finish, 0);
    EXPECT_EQ(rwlock.ret, 0);

    ret = pthread_rwlock_unlock(&rw);
    EXPECT_EQ(ret, 0);

    sleep(1);

    EXPECT_EQ(rwlock.finish, 1);
    EXPECT_EQ(rwlock.ret, 0);
    
    ret = pthread_rwlock_destroy(&rw);
    EXPECT_EQ(ret, 0);
}
