#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <limits.h>
#include <stdint.h>
#include <semaphore.h>
#include "stub.h"
#include "gtest/gtest.h"


class st_test_pthread_attr : public testing::Test {
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


long __sysconf(int name){
    return sysconf(name);
} 


TEST_F(st_test_pthread_attr, st_test_pthread_attr_init_destroy)
{
    pthread_attr_t attr;
    EXPECT_EQ(pthread_attr_init(&attr), 0);
    EXPECT_EQ(pthread_attr_destroy(&attr), 0);
}

TEST_F(st_test_pthread_attr, st_test_pthread_attr_set_get_schedpolicy_rr)
{
    pthread_attr_t attr;
    int ret;
    ret = pthread_attr_init(&attr);
    EXPECT_EQ(ret, 0);

    ret = pthread_attr_setschedpolicy(&attr, SCHED_RR);
    EXPECT_EQ(ret, 0);

    int policy = -1;
    ret = pthread_attr_getschedpolicy(&attr, &policy);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(policy, SCHED_RR);

    EXPECT_EQ(pthread_attr_destroy(&attr), 0);
}

TEST_F(st_test_pthread_attr, st_test_pthread_attr_set_get_schedpolicy_fifo)
{
    pthread_attr_t attr;
    int ret;
    ret = pthread_attr_init(&attr);
    EXPECT_EQ(ret, 0);

    ret = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    EXPECT_EQ(ret, 0);

    int policy = -1;
    ret = pthread_attr_getschedpolicy(&attr, &policy);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(policy, SCHED_FIFO);

    EXPECT_EQ(pthread_attr_destroy(&attr), 0);
}

TEST_F(st_test_pthread_attr, st_test_pthread_attr_set_get_schedpolicy_other)
{
    pthread_attr_t attr;
    int ret;
    ret = pthread_attr_init(&attr);
    EXPECT_EQ(ret, 0);

    ret = pthread_attr_setschedpolicy(&attr, SCHED_OTHER);
    EXPECT_EQ(ret, EINVAL);

    int policy = -1;
    ret = pthread_attr_getschedpolicy(&attr, &policy);
    EXPECT_EQ(ret, 0);
    EXPECT_NE(policy, SCHED_OTHER);

    EXPECT_EQ(pthread_attr_destroy(&attr), 0);
}

TEST_F(st_test_pthread_attr, st_test_pthread_attr_set_get_schedpolicy_batch)
{
    pthread_attr_t attr;
    int ret;
    ret = pthread_attr_init(&attr);
    EXPECT_EQ(ret, 0);

    ret = pthread_attr_setschedpolicy(&attr, SCHED_BATCH);
    EXPECT_EQ(ret, EINVAL);

    int policy = -1;
    ret = pthread_attr_getschedpolicy(&attr, &policy);
    EXPECT_EQ(ret, 0);
    EXPECT_NE(policy, SCHED_BATCH);

    EXPECT_EQ(pthread_attr_destroy(&attr), 0);
}

TEST_F(st_test_pthread_attr, st_test_pthread_attr_set_get_schedpolicy_idle)
{
    pthread_attr_t attr;
    int ret;
    ret = pthread_attr_init(&attr);
    EXPECT_EQ(ret, 0);

    ret = pthread_attr_setschedpolicy(&attr, SCHED_IDLE);
    EXPECT_EQ(ret, EINVAL);

    int policy = -1;
    ret = pthread_attr_getschedpolicy(&attr, &policy);
    EXPECT_EQ(ret, 0);
    EXPECT_NE(policy, SCHED_IDLE);

    EXPECT_EQ(pthread_attr_destroy(&attr), 0);
}

TEST_F(st_test_pthread_attr, st_test_pthread_attr_set_get_schedpolicy_deadline)
{
    pthread_attr_t attr;
    int ret;
    ret = pthread_attr_init(&attr);
    EXPECT_EQ(ret, 0);

    ret = pthread_attr_setschedpolicy(&attr, SCHED_DEADLINE);
    EXPECT_EQ(ret, EINVAL);

    int policy = -1;
    ret = pthread_attr_getschedpolicy(&attr, &policy);
    EXPECT_EQ(ret, 0);
    EXPECT_NE(policy, SCHED_DEADLINE);

    EXPECT_EQ(pthread_attr_destroy(&attr), 0);
}

TEST_F(st_test_pthread_attr, st_test_pthread_attr_set_get_schedpolicy_reset_on_fork)
{
    pthread_attr_t attr;
    int ret;
    ret = pthread_attr_init(&attr);
    EXPECT_EQ(ret, 0);

    ret = pthread_attr_setschedpolicy(&attr, SCHED_RESET_ON_FORK);
    EXPECT_EQ(ret, EINVAL);

    int policy = -1;
    ret = pthread_attr_getschedpolicy(&attr, &policy);
    EXPECT_EQ(ret, 0);
    EXPECT_NE(policy, SCHED_RESET_ON_FORK);

    EXPECT_EQ(pthread_attr_destroy(&attr), 0);
}

TEST_F(st_test_pthread_attr, st_test_pthread_attr_setschedparam_normal)
{
    pthread_attr_t attr;
    int ret;
    ret = pthread_attr_init(&attr);
    EXPECT_EQ(ret, 0);

    struct sched_param setpara = {.sched_priority = 28};
    ret = pthread_attr_setschedparam(&attr, &setpara);
    EXPECT_EQ(ret, 0);

    struct sched_param getpara;
    ret = pthread_attr_getschedparam(&attr, &getpara);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(getpara.sched_priority, setpara.sched_priority);

    EXPECT_EQ(pthread_attr_destroy(&attr), 0);
}

TEST_F(st_test_pthread_attr, st_test_pthread_attr_setschedparam_unnormal_min)
{
    pthread_attr_t attr;
    int ret;
    ret = pthread_attr_init(&attr);
    EXPECT_EQ(ret, 0);

    struct sched_param setpara = {.sched_priority = -1};
    ret = pthread_attr_setschedparam(&attr, &setpara);
    EXPECT_EQ(ret, EINVAL);

    struct sched_param getpara;
    ret = pthread_attr_getschedparam(&attr, &getpara);
    EXPECT_EQ(ret, 0);
    EXPECT_NE(getpara.sched_priority, setpara.sched_priority);

    EXPECT_EQ(pthread_attr_destroy(&attr), 0);
}

TEST_F(st_test_pthread_attr, st_test_pthread_attr_setschedparam_unnormal_max)
{
    pthread_attr_t attr;
    int ret;
    ret = pthread_attr_init(&attr);
    EXPECT_EQ(ret, 0);

    struct sched_param setpara = {.sched_priority = 32};
    ret = pthread_attr_setschedparam(&attr, &setpara);
    EXPECT_EQ(ret, EINVAL);

    struct sched_param getpara;
    ret = pthread_attr_getschedparam(&attr, &getpara);
    EXPECT_EQ(ret, 0);
    EXPECT_NE(getpara.sched_priority, setpara.sched_priority);

    EXPECT_EQ(pthread_attr_destroy(&attr), 0);
}

TEST_F(st_test_pthread_attr, st_test_pthread_attr_setstacksize_normal)
{
    pthread_attr_t attr;
    int ret;
    ret = pthread_attr_init(&attr);
    EXPECT_EQ(ret, 0);

    size_t setStackSize = 8 * PTHREAD_STACK_MIN;
    ret = pthread_attr_setstacksize(&attr, setStackSize);
    EXPECT_EQ(ret, 0);

    size_t getStackSize = 0;
    ret = pthread_attr_getstacksize(&attr, &getStackSize);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(getStackSize, setStackSize);

    EXPECT_EQ(pthread_attr_destroy(&attr), 0);
}

TEST_F(st_test_pthread_attr, st_test_pthread_attr_setstacksize_unormal)
{
    pthread_attr_t attr;
    int ret;
    ret = pthread_attr_init(&attr);
    EXPECT_EQ(ret, 0);

    size_t setStackSize = SIZE_MAX;
    ret = pthread_attr_setstacksize(&attr, setStackSize);
    EXPECT_EQ(ret, EINVAL);

    size_t getStackSize = 0;
    ret = pthread_attr_getstacksize(&attr, &getStackSize);
    EXPECT_EQ(ret, 0);
    EXPECT_NE(getStackSize, setStackSize);

    EXPECT_EQ(pthread_attr_destroy(&attr), 0);
}

TEST_F(st_test_pthread_attr, st_test_pthread_attr_setdetachstate_detached)
{
    pthread_attr_t attr;
    int ret;
    ret = pthread_attr_init(&attr);
    EXPECT_EQ(ret, 0);

    int setState = PTHREAD_CREATE_DETACHED;
    ret = pthread_attr_setdetachstate(&attr, setState);
    EXPECT_EQ(ret, 0);

    int getState = 0;
    ret = pthread_attr_getdetachstate(&attr, &getState);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(setState, getState);

    EXPECT_EQ(pthread_attr_destroy(&attr), 0);
}

TEST_F(st_test_pthread_attr, st_test_pthread_attr_setdetachstate_joinable)
{
    pthread_attr_t attr;
    int ret;
    ret = pthread_attr_init(&attr);
    EXPECT_EQ(ret, 0);

    int setState = PTHREAD_CREATE_JOINABLE;
    ret = pthread_attr_setdetachstate(&attr, setState);
    EXPECT_EQ(ret, 0);

    int getState = 0;
    ret = pthread_attr_getdetachstate(&attr, &getState);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(setState, getState);

    EXPECT_EQ(pthread_attr_destroy(&attr), 0);
}

TEST_F(st_test_pthread_attr, st_test_pthread_attr_setdetachstate_unormal)
{
    pthread_attr_t attr;
    int ret;
    ret = pthread_attr_init(&attr);
    EXPECT_EQ(ret, 0);

    int setState = 3;
    ret = pthread_attr_setdetachstate(&attr, setState);
    EXPECT_EQ(ret, EINVAL);

    int getState = 0;
    ret = pthread_attr_getdetachstate(&attr, &getState);
    EXPECT_EQ(ret, 0);
    EXPECT_NE(setState, getState);

    EXPECT_EQ(pthread_attr_destroy(&attr), 0);
}
#if 0
TEST_F(st_test_pthread_attr, st_test_pthread_setaffinity_np_unormal)
{
    pthread_t tid = pthread_self(); 
    size_t size = sizeof(cpu_set_t) - 1;
    cpu_set_t set;

    CPU_ZERO(&set);

    EXPECT_EQ(pthread_setaffinity_np(tid, size, &set), EINVAL);
}

TEST_F(st_test_pthread_attr, st_test_pthread_setaffinity_np_normal)
{
    pthread_t tid = pthread_self(); 
    size_t size = sizeof(cpu_set_t);
    cpu_set_t set;
    cpu_set_t get;

    CPU_ZERO(&set);
    CPU_SET (0, &set); 

    int ret = pthread_setaffinity_np(tid, size, &set);
    EXPECT_EQ(ret, 0);
    if(ret != 0) {
        printf("pthread_setaffinity_np ret %d, %s\n", ret, strerror(errno));
    }

    EXPECT_EQ(pthread_getaffinity_np(tid, size, &get), 0);
    if(ret != 0) {
        printf("pthread_setaffinity_np ret %d, %s\n", ret, strerror(errno));
    }

    EXPECT_EQ(memcmp(&set, &get, size), 0);
}
#endif
