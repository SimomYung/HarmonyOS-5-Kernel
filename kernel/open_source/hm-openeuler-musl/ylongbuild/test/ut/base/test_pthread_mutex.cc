#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include "stub.h"
#include "gtest/gtest.h"


class st_test_pthread_mutex : public testing::Test {
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
    sem_t *s;
    int *i;
} TestMutex;

static void *Relock(void *arg)
{
    printf("Relock tid %d\n", gettid());
    TestMutex *a = (TestMutex *)arg;
    int ret = pthread_mutex_lock(a->m);
    EXPECT_EQ(ret, 0);
    if (ret) {
        return NULL;
    }

    sem_post(a->s);
    ret = pthread_mutex_lock(a->m);
    printf("pthread_mutex_lock relock return %d, tid %d\n", ret, gettid());
    *(a->i) = ret;
    sem_post(a->s);

    pthread_mutex_unlock(a->m);
    if (*(a->i) == 0) {
        pthread_mutex_unlock(a->m);
    }

    return 0;
}

static int TestRelock(int mtype)
{
    struct timespec ts;
    pthread_t t;
    pthread_mutex_t m;
    pthread_mutexattr_t ma;
    sem_t s;
    int i;
    int ret;
    void *p;

    TestMutex a = { 0 };
    a.m = &m;
    a.s = &s;
    a.i = &i;

    printf("TestRelock\n");

    ret = pthread_mutexattr_init(&ma);
    EXPECT_EQ(ret, 0);
    if (ret != 0) {
        return 1;
    }

    ret = pthread_mutexattr_settype(&ma, mtype);
    EXPECT_EQ(ret, 0);
    if (ret != 0) {
        return 1;
    }

    ret = pthread_mutex_init(a.m, &ma);
    EXPECT_EQ(ret, 0);
    if (ret != 0) {
        return 1;
    }

    ret = pthread_mutexattr_destroy(&ma);
    EXPECT_EQ(ret, 0);
    if (ret != 0) {
        return 1;
    }

    sem_init(a.s, 0, 0);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    ret = pthread_create(&t, &attr, Relock, &a);
    EXPECT_EQ(ret, 0);
    if (ret != 0) {
        return 1;
    }

    sem_wait(a.s);

    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_nsec += 100 * 1000 * 1000;
    if (ts.tv_nsec >= 1000 * 1000 * 1000) {
        ts.tv_nsec -= 1000 * 1000 * 1000;
        ts.tv_sec += 1;
    }

    ret = sem_timedwait(a.s, &ts);
    if (ret == -1) {
        if (errno != ETIMEDOUT) {
            printf("sem_timedwait failed with unexpected error: %s\n", strerror(errno));
        }

        pthread_mutex_unlock(a.m);
        sched_yield();
        return -1;
    }

    pthread_mutex_destroy(a.m);
    sem_destroy(a.s);

    return i;
}

static void *Unlock(void *arg)
{
    TestMutex *a = (TestMutex *)arg;

    *(a->i) = pthread_mutex_unlock(a->m);

    sem_post(a->s);
    return 0;
}

static int TestUnlock(int mtype)
{
    pthread_t t;
    pthread_mutex_t m;
    pthread_mutexattr_t ma;
    int i;
    int ret;
    void *p;
    sem_t s;
    struct timespec ts;
    TestMutex a = { 0 };
    a.m = &m;
    a.s = &s;
    a.i = &i;

    ret = pthread_mutexattr_init(&ma);
    EXPECT_EQ(ret, 0);
    if (ret != 0) {
        return 1;
    }

    ret = pthread_mutexattr_settype(&ma, mtype);
    EXPECT_EQ(ret, 0);
    if (ret != 0) {
        return 1;
    }

    ret = pthread_mutex_init(a.m, &ma);
    EXPECT_EQ(ret, 0);
    if (ret != 0) {
        return 1;
    }

    ret = pthread_mutexattr_destroy(&ma);
    EXPECT_EQ(ret, 0);
    if (ret != 0) {
        return 1;
    }

    sem_init(a.s, 0, 0);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    ret = pthread_create(&t, &attr, Unlock, &a);
    if(ret != 0) {
        printf("pthread_create failed, ret = %d!errno=%d, %s\n", ret, errno, strerror(errno));
        return 1;
    }

    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_nsec += 100 * 1000 * 1000;
    if (ts.tv_nsec >= 1000 * 1000 * 1000) {
        ts.tv_nsec -= 1000 * 1000 * 1000;
        ts.tv_sec += 1;
    }

    ret = sem_timedwait(a.s, &ts);
    if (ret == -1) {
        printf("sem_timedwait failed with unexpected error: %s\n", strerror(errno));
    }

    pthread_mutex_destroy(a.m);
    sem_destroy(a.s);

    return i;
}

static int TestUnlockOther(int mtype)
{
    pthread_t t;
    pthread_mutex_t m;
    pthread_mutexattr_t ma;
    int i;
    int ret;
    void *p;
    sem_t s;
    struct timespec ts;
    TestMutex a = { 0 };
    a.m = &m;
    a.s = &s;
    a.i = &i;

    ret = pthread_mutexattr_init(&ma);
    EXPECT_EQ(ret, 0);
    if (ret != 0) {
        return 1;
    }

    ret = pthread_mutexattr_settype(&ma, mtype);
    EXPECT_EQ(ret, 0);
    if (ret != 0) {
        return 1;
    }

    ret = pthread_mutex_init(a.m, &ma);
    EXPECT_EQ(ret, 0);
    if (ret != 0) {
        return 1;
    }

    ret = pthread_mutexattr_destroy(&ma);
    EXPECT_EQ(ret, 0);
    if (ret != 0) {
        return 1;
    }

    ret = pthread_mutex_lock(a.m);
    EXPECT_EQ(ret, 0);
    if (ret != 0) {
        return 1;
    }

    sem_init(a.s, 0, 0);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    ret = pthread_create(&t, &attr, Unlock, &a);
    if(ret != 0) {
        printf("pthread_create failed, ret = %d!errno=%d, %s\n", ret, errno, strerror(errno));
        return 1;
    }

    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_nsec += 100 * 1000 * 1000;
    if (ts.tv_nsec >= 1000 * 1000 * 1000) {
        ts.tv_nsec -= 1000 * 1000 * 1000;
        ts.tv_sec += 1;
    }

    ret = sem_timedwait(a.s, &ts);
    if (ret != 0) {
         printf("sem_timedwait failed with unexpected error: %s\n", strerror(errno));
    }

    ret = pthread_mutex_unlock(a.m);
    EXPECT_EQ(ret, 0);
    if (ret != 0) {
        return 1;
    }

    pthread_mutex_destroy(a.m);
    sem_destroy(a.s);

    return i;
}

#ifdef __cplusplus
}
#endif


TEST_F(st_test_pthread_mutex, st_test_pthread_mutex_attr)
{
    pthread_mutex_t m;
    pthread_mutexattr_t a;
    int r;
    int i;

    EXPECT_EQ(pthread_mutexattr_init(&a), 0);

    EXPECT_EQ(pthread_mutexattr_gettype(&a, &i), 0);
    EXPECT_EQ(i, PTHREAD_MUTEX_DEFAULT);
    if (i != PTHREAD_MUTEX_DEFAULT) {
        printf("default mutex type is %d, wanted PTHREAD_MUTEX_DEFAULT (%d)\n", i, PTHREAD_MUTEX_DEFAULT);
    }

    EXPECT_EQ(pthread_mutexattr_destroy(&a), 0);
}

TEST_F(st_test_pthread_mutex, st_test_pthread_mutex_attr2)
{
    pthread_mutex_t m;
    pthread_mutexattr_t a;
    int r;
    int i;

    EXPECT_EQ(pthread_mutexattr_init(&a), 0);

    EXPECT_EQ(pthread_mutexattr_settype(&a, PTHREAD_MUTEX_ERRORCHECK), 0);

    EXPECT_EQ(pthread_mutexattr_gettype(&a, &i), 0);
    EXPECT_EQ(i, PTHREAD_MUTEX_ERRORCHECK);
    if (i != PTHREAD_MUTEX_ERRORCHECK) {
        printf("setting error check mutex type failed failed: got %d, wanted %d\n", i, PTHREAD_MUTEX_ERRORCHECK);
    }

    EXPECT_EQ(pthread_mutexattr_destroy(&a), 0);
}

TEST_F(st_test_pthread_mutex, st_test_pthread_mutex_test_relock_normal)
{
    printf("test_relock_normal\n");
    int ret = TestRelock(PTHREAD_MUTEX_NORMAL);
    EXPECT_EQ(ret, -1);
    if (ret != -1) {
        printf("PTHREAD_MUTEX_NORMAL relock did not deadlock, got %s\n", strerror(ret));
    }
}


TEST_F(st_test_pthread_mutex, st_test_pthread_mutex_test_relock_errorcheck)
{
    printf("test_relock_errorcheck\n");
    int ret = TestRelock(PTHREAD_MUTEX_ERRORCHECK);
    EXPECT_EQ(ret, EDEADLK);
    if (ret != EDEADLK) {
        printf("PTHREAD_MUTEX_ERRORCHECK relock did not return EDEADLK, got %s\n",
            ret == -1 ? "deadlock" : strerror(ret));
    }
}

TEST_F(st_test_pthread_mutex, st_test_pthread_mutex_test_relock_recursive)
{
    printf("test_relock_recursive\n");
    int ret = TestRelock(PTHREAD_MUTEX_RECURSIVE);
    EXPECT_EQ(ret, 0);
    if (ret != 0) {
        printf("PTHREAD_MUTEX_RECURSIVE relock did not succeed, got ret=%d, %s\n", ret,
            ret == -1 ? "deadlock" : strerror(ret));
    }
}

TEST_F(st_test_pthread_mutex, st_test_pthread_mutex_test_unlock_errorcheck)
{
    int ret = TestUnlock(PTHREAD_MUTEX_ERRORCHECK);
    EXPECT_EQ(ret, EPERM);
    if (ret != EPERM) {
        printf("PTHREAD_MUTEX_ERRORCHECK unlock did not return EPERM, got ret =%d, %s\n", ret, strerror(ret));
    }
}

TEST_F(st_test_pthread_mutex, st_test_pthread_mutex_test_unlock_recursive)
{
    int ret = TestUnlock(PTHREAD_MUTEX_RECURSIVE);
    EXPECT_EQ(ret, EPERM);
    if (ret != EPERM) {
        printf("PTHREAD_MUTEX_RECURSIVE unlock did not return EPERM, got ret=%d, %s\n", ret, strerror(ret));
    }
}

TEST_F(st_test_pthread_mutex, st_test_pthread_mutex_test_unlock_other_errorcheck)
{
    int ret = TestUnlockOther(PTHREAD_MUTEX_ERRORCHECK);
    EXPECT_EQ(ret, EPERM);
    if (ret != EPERM) {
        printf("PTHREAD_MUTEX_ERRORCHECK unlock did not return EPERM, got ret=%d, %s\n", ret, strerror(ret));
    }
}

TEST_F(st_test_pthread_mutex, st_test_pthread_mutex_test_unlock_other_recursive)
{
    int ret = TestUnlockOther(PTHREAD_MUTEX_RECURSIVE);
    EXPECT_EQ(ret, EPERM);
    if (ret != EPERM) {
        printf("PTHREAD_MUTEX_RECURSIVE unlock did not return EPERM, got ret=%d, %s\n", ret, strerror(ret));
    }
}

TEST_F(st_test_pthread_mutex, st_test_pthread_mutex_trylock)
{
    pthread_mutex_t m;

    int ret = pthread_mutex_init(&m, NULL);
    EXPECT_EQ(ret, 0);
    if (ret != 0) {
        printf("pthread_mutex_init err, ret=%d, %s\n", ret, strerror(ret));
        return;
    }

    ret = pthread_mutex_trylock(&m);
    EXPECT_EQ(ret, 0);
    if (ret != 0) {
        printf("pthread_mutex_trylock err, ret=%d, %s\n", ret, strerror(ret));
        pthread_mutex_destroy(&m);
        return;
    }

    ret = pthread_mutex_unlock(&m);
    EXPECT_EQ(ret, 0);

    pthread_mutex_destroy(&m);
}

TEST_F(st_test_pthread_mutex, st_test_pthread_mutex_trylock_error)
{
    pthread_mutex_t m;

    int ret = pthread_mutex_init(&m, NULL);
    EXPECT_EQ(ret, 0);
    if (ret != 0) {
        printf("pthread_mutex_init err, ret=%d, %s\n", ret, strerror(ret));
        return;
    }

    ret = pthread_mutex_lock(&m);
    EXPECT_EQ(ret, 0);
    if (ret != 0) {
        printf("pthread_mutex_lock err, ret=%d, %s\n", ret, strerror(ret));
        pthread_mutex_destroy(&m);
        return;
    }

    ret = pthread_mutex_trylock(&m);
    EXPECT_EQ(ret, EBUSY);
    if (ret != EBUSY) {
        printf("pthread_mutex_trylock err, ret=%d, %s\n", ret, strerror(ret));
    }

    ret = pthread_mutex_unlock(&m);
    EXPECT_EQ(ret, 0);
    if (ret != 0) {
        return;
    }

    pthread_mutex_destroy(&m);
}

TEST_F(st_test_pthread_mutex, st_test_pthread_mutex_relock_test)
{
    pthread_mutex_t m;
    pthread_mutexattr_t ma;

    int ret = pthread_mutexattr_init(&ma);
    EXPECT_EQ(ret, 0);

    ret = pthread_mutexattr_settype(&ma, PTHREAD_MUTEX_ERRORCHECK);
    EXPECT_EQ(ret, 0);

    ret = pthread_mutex_init(&m, &ma);
    EXPECT_EQ(ret, 0);

    ret = pthread_mutex_lock(&m);
    EXPECT_EQ(ret, 0);

    ret = pthread_mutex_lock(&m);
    EXPECT_EQ(ret, EDEADLK);

    ret = pthread_mutex_unlock(&m);
    EXPECT_EQ(ret, 0);

    pthread_mutex_destroy(&m);
}