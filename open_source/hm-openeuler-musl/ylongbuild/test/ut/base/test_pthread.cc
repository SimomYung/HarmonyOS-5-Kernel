#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/prctl.h>
#include "securec.h"
#include "stub.h"
#include "gtest/gtest.h"


class st_test_pthread : public testing::Test {
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

static sem_t lwtsem[64];

static void *TestCreateEntry(void *arg)
{
    int *flag = (int*)arg;
    int index = *flag;
    
    printf("I am TestCreateEntry[%d]\n", index);

    *flag = 0XAB;

    usleep(10);

    return NULL;
}

static void *TestPthreadSelfEntry(void *arg)
{
    pthread_t *pthreadId = (pthread_t*)arg;
    
    printf("I am TestPthreadSelfEntry[%lu]\n", pthreadId);

    *pthreadId = pthread_self();
    return NULL;
}

static void *TestGetPid(void *arg)
{
    pid_t *pid = (pid_t*)arg;

    *pid = getpid();
}

static void *TestGetTid(void *arg)
{
    pid_t *pid = (pid_t*)arg;

    *pid = gettid();
}

static void *TestGetTidAndWait(void *arg)
{
    pid_t *pid = (pid_t*)arg;

    *pid = gettid();

    sem_wait(&lwtsem[0]);
}

#ifdef __cplusplus
}
#endif

TEST_F(st_test_pthread, st_test_pthread_create1_detached)
{
    int flag = 0;

    pthread_t lwt;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    int ret = pthread_create(&lwt, &attr, TestCreateEntry, &flag);
    EXPECT_EQ(ret, 0);
    if(ret != 0) {
        printf("pthread_create failed, ret = %d!errno=%d, %s\n", ret, errno, strerror(errno));
        return;
    }

    ret = pthread_join(lwt, NULL);
    EXPECT_NE(ret, 0);

    usleep(500);
    EXPECT_EQ(flag, 0XAB);
}

TEST_F(st_test_pthread, st_test_pthread_create64)
{
    int flag[64] = {0};
    int cnt = 0;
    int success = 0;

    pthread_t lwt[64];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    for(int i = 0; i < 64; i++) {
        flag[i] = i;
        int ret = pthread_create(&lwt[i], &attr, TestCreateEntry, &flag[i]);
        EXPECT_EQ(ret, 0);
        if(ret != 0) {
            printf("pthread_create failed, ret = %d!errno=%d, %s\n", ret, errno, strerror(errno));
            break;
        }
        ret = pthread_join(lwt[i], NULL);
        EXPECT_NE(ret, 0);
    }

    sleep(2);

    for(int i = 0; i < 64; i++) {
        EXPECT_EQ(flag[i], 0XAB);
        if(flag[i] == 0XAB) {
            success++;
        }
    }

    EXPECT_EQ(success, 64);
}

TEST_F(st_test_pthread, st_test_pthread1_join)
{
    int flag = 0;
    int cnt = 0;

    pthread_t lwt;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    int ret = pthread_create(&lwt, &attr, TestCreateEntry, &flag);
    EXPECT_EQ(ret, 0);
    if(ret != 0) {
        printf("pthread_create failed, ret = %d!errno=%d, %s\n", ret, errno, strerror(errno));
        return;
    }

    ret = pthread_join(lwt, NULL);
    EXPECT_EQ(ret, 0);
    if(ret != 0) {
        printf("pthread_join failed, ret = %d!errno=%d, %s\n", ret, errno, strerror(errno));
    }

    EXPECT_EQ(flag, 0XAB);
}

TEST_F(st_test_pthread, st_test_pthread_join64)
{
    int flag[64] = {0};
    int cnt = 0;
    int ret;
    int success = 0;

    pthread_t lwt[64];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for(int i = 0; i < 64; i++) {
        flag[i] = i;
        int ret = pthread_create(&lwt[i], &attr, TestCreateEntry, &flag[i]);
        EXPECT_EQ(ret, 0);
        if(ret != 0) {
            printf("pthread_create failed, ret = %d!errno=%d, %s\n", ret, errno, strerror(errno));
            return;
        }
    }

    for(int i = 0; i < 64; i++) {
        ret = pthread_join(lwt[i], NULL);
        EXPECT_EQ(ret, 0);
        if(ret != 0) {
            printf("pthread_join failed, ret = %d!errno=%d, %s\n", ret, errno, strerror(errno));
            break;
        }

        EXPECT_EQ(flag[i], 0XAB);
        if(flag[i] == 0XAB) {
            success++;
        }
    }

    EXPECT_EQ(success, 64);
}

TEST_F(st_test_pthread, st_test_pthread_self)
{
    pthread_t flag = 0;
    pthread_t lwt;
    int cnt;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int ret = pthread_create(&lwt, &attr, TestPthreadSelfEntry, &flag);
    EXPECT_EQ(ret, 0);
    if (ret != 0) {
        printf("pthread_create failed, ret = %d!errno=%d, %s\n", ret, errno, strerror(errno));
        return;
    }

    pthread_join(lwt, NULL);
    EXPECT_EQ(flag, lwt);
}

TEST_F(st_test_pthread, st_test_getpid)
{
    pid_t pid = getpid();
    EXPECT_NE(pid, 0);

    pid_t pid2 = 0;
    pthread_t lwt;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    int ret = pthread_create(&lwt, &attr, TestGetPid, &pid2);
    EXPECT_EQ(ret, 0);
    if (ret != 0) {
        printf("pthread_create failed, ret = %d!errno=%d, %s\n", ret, errno, strerror(errno));
        return;
    }

    usleep(500);
    EXPECT_EQ(pid2, pid);
}

TEST_F(st_test_pthread, st_test_gettid)
{
    pid_t tid = gettid();
    EXPECT_NE(tid, 0);
    
    pid_t tid2 = 0;
    pthread_t lwt;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    int ret = pthread_create(&lwt, &attr, TestGetTid, &tid2);
    EXPECT_EQ(ret, 0);
    if (ret != 0) {
        printf("pthread_create failed, ret = %d!errno=%d, %s\n", ret, errno, strerror(errno));
        return;
    }

    usleep(500);
    EXPECT_NE(tid, tid2);
}

TEST_F(st_test_pthread, st_test_getuid)
{
    uid_t uid = getuid();

    // 认为设备上不会有那么多的user
    EXPECT_LT(uid, 100);
    printf("getuid() is %u\n", uid);
}

TEST_F(st_test_pthread, st_test_getschedparam1)
{
    pthread_t lwt;

    pid_t tid2 = 0;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    sem_init(&lwtsem[0], 0, 0);

    int ret = pthread_create(&lwt, &attr, TestGetTidAndWait, &tid2);
    if(ret != 0) {
        printf("pthread_create failed, ret = %d!errno=%d, %s\n", ret, errno, strerror(errno));
        return;
    }

    int setPolicy = SCHED_RR;
    struct sched_param setParam = {28};
    ret = pthread_setschedparam(lwt, setPolicy, &setParam);
    EXPECT_EQ(ret, 0);

    int getPolicy = 0;
    struct sched_param getParam = {0};
    pthread_getschedparam(lwt, &getPolicy, &getParam);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(setPolicy, getPolicy);
    EXPECT_EQ(setParam.sched_priority, getParam.sched_priority);

    sem_post(&lwtsem[0]);
}

TEST_F(st_test_pthread, st_test_getschedparam2)
{
    pthread_t lwt;

    pid_t tid2 = 0;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    sem_init(&lwtsem[0], 0, 0);

    int ret = pthread_create(&lwt, &attr, TestGetTidAndWait, &tid2);
    if(ret != 0) {
        printf("pthread_create failed, ret = %d!errno=%d, %s\n", ret, errno, strerror(errno));
        return;
    }

    int setPolicy = SCHED_FIFO;
    struct sched_param setParam = {28};
    ret = pthread_setschedparam(lwt, setPolicy, &setParam);
    EXPECT_EQ(ret, 0);

    int getPolicy = 0;
    struct sched_param getParam = {0};
    pthread_getschedparam(lwt, &getPolicy, &getParam);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(setPolicy, getPolicy);
    EXPECT_EQ(setParam.sched_priority, getParam.sched_priority);

    sem_post(&lwtsem[0]);
}

TEST_F(st_test_pthread, st_test_getschedparam3)
{
    pthread_t lwt;

    pid_t tid2 = 0;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    sem_init(&lwtsem[0], 0, 0);

    int ret = pthread_create(&lwt, &attr, TestGetTidAndWait, &tid2);
    if(ret != 0) {
        printf("pthread_create failed, ret = %d!errno=%d, %s\n", ret, errno, strerror(errno));
        return;
    }

    int setPolicy = SCHED_OTHER;
    struct sched_param setParam = {28};
    ret = pthread_setschedparam(lwt, setPolicy, &setParam);
    EXPECT_EQ(ret, EINVAL);

    sem_post(&lwtsem[0]);
}

TEST_F(st_test_pthread, st_test_getschedparam4)
{
    pthread_t lwt;

    pid_t tid2 = 0;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    sem_init(&lwtsem[0], 0, 0);

    int ret = pthread_create(&lwt, &attr, TestGetTidAndWait, &tid2);
    if(ret != 0) {
        printf("pthread_create failed, ret = %d!errno=%d, %s\n", ret, errno, strerror(errno));
        return;
    }

    int setPolicy = SCHED_FIFO;
    struct sched_param setParam = {-1};
    ret = pthread_setschedparam(lwt, setPolicy, &setParam);
    EXPECT_EQ(ret, EINVAL);

    sem_post(&lwtsem[0]);
}

TEST_F(st_test_pthread, st_test_getschedparam5)
{
    pthread_t lwt;

    pid_t tid2 = 0;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    sem_init(&lwtsem[0], 0, 0);

    int ret = pthread_create(&lwt, &attr, TestGetTidAndWait, &tid2);
    if(ret != 0) {
        printf("pthread_create failed, ret = %d!errno=%d, %s\n", ret, errno, strerror(errno));
        return;
    }

    int setPolicy = SCHED_FIFO;
    struct sched_param setParam = {32};
    ret = pthread_setschedparam(lwt, setPolicy, &setParam);
    EXPECT_EQ(ret, EINVAL);

    sem_post(&lwtsem[0]);
}

#define USR_TASK_NAME_LEN  16
TEST_F(st_test_pthread, st_test_setname)
{
    char name[USR_TASK_NAME_LEN] = {0};

    prctl(PR_SET_NAME, "test_prctl", 0, 0, 0);
    pthread_getname_np(pthread_self(), name, 0);
    EXPECT_EQ(strcmp(name, "test_prctl"), 0);

    memset(name, 0, USR_TASK_NAME_LEN);
    pthread_t lwt;
    pid_t tid;
    int ret = pthread_create(&lwt, NULL, TestGetTid, &tid);
    pthread_setname_np(lwt, "child_test");
    pthread_getname_np(lwt, name, 0);
    EXPECT_EQ(strcmp(name, "child_test"), 0);
}