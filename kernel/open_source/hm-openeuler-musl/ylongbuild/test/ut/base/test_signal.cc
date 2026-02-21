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
#include "signal.h"
#include <pthread.h>
 
 
#ifdef __cplusplus
}
#endif
 
class ut_test_sig : public testing::Test {
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
 
static volatile sig_atomic_t g_sigCount = 0;
static volatile sig_atomic_t g_sigCount1 = 0;
static volatile const sig_atomic_t SIG_TEST_COUNT = 3;
static volatile const sig_atomic_t INVAILD_SIG = 1000;

static void SigPrint(int sig)
{
    g_sigCount = 1;
}
 
static void SigPrint1(int sig)
{
    g_sigCount++;
}
 
static void SigPrint2(int sig)
{
    g_sigCount += 100;
}
 
static void SigPrint3(int sig)
{
    g_sigCount1++;
}
 
TEST_F(ut_test_sig, signal_tc1)
{
    void (*ret)(int);
    int retValue;
    g_sigCount = 0;
    ret = signal(SIGALRM, SigPrint);
    EXPECT_NE(ret, (void *)-1);
    retValue = raise(SIGALRM);
    EXPECT_EQ(retValue, 0);
    sleep(1);
    retValue = raise(SIGALRM);
    EXPECT_EQ(retValue, 0);
    EXPECT_EQ(g_sigCount, 1);
}
 
TEST_F(ut_test_sig, signal_tc2)
{
    void (*ret)(int);
    int retValue;
    g_sigCount = 0;
    ret = signal(SIGUSR1, SigPrint1);
    EXPECT_NE(ret, (void *)-1);
    ret = signal(SIGUSR2, SigPrint2);
    EXPECT_NE(ret, (void *)-1);
    ret = signal(SIGUSR1, SIG_IGN);
    EXPECT_EQ(ret, SigPrint1);
}

TEST_F(ut_test_sig, sigSet_tc1)
{
    sigset_t set;
    void (*retSig)(int);
    int sig, fpid, retValue, status;
    int count = 0;
 
    signal(SIGCHLD, SIG_DFL);
    retSig = signal(SIGALRM, SigPrint1);
    EXPECT_NE(retSig, (void *)-1);
    retSig = signal(SIGUSR1, SigPrint1);
    EXPECT_NE(retSig, (void *)-1);
    retValue = sigemptyset(&set);
    EXPECT_EQ(retValue, 0);
    retValue = sigaddset(&set, SIGALRM);
    EXPECT_EQ(retValue, 0);
    retValue = sigaddset(&set, SIGUSR1);
    EXPECT_EQ(retValue, 0);
    fpid = fork();
    if (fpid == 0) {
        printf("sig wait begin\n");
        while (1) {
            retValue = sigwait(&set, &sig);
            if (retValue != 0) {
                exit(retValue);
            }
            printf("sig wait end\n");
            count++;
            if (count == 3) { // 3, Possible values for the current parameter
                break;
            }
        }
        exit(0);
    }
    sleep(1);
    retValue = kill(fpid, SIGUSR1);
    EXPECT_EQ(retValue, 0);
    sleep(1);
    retValue = kill(fpid, SIGALRM);
    EXPECT_EQ(retValue, 0);
    sleep(1);
    retValue = kill(fpid, SIGALRM);
    EXPECT_EQ(retValue, 0);
 
    retValue = waitpid(fpid, &status, 0);
    EXPECT_EQ(retValue, fpid);
 
    signal(SIGALRM, SIG_DFL);
    signal(SIGUSR1, SIG_DFL);
}
 
TEST_F(ut_test_sig, sigSet_tc2)
{
    void (*retSig)(int);
    int retValue;
    int status;
    int fpid;
 
    retSig = signal(SIGPWR, SigPrint);
    EXPECT_NE(retSig, (void *)-1);
 
    fpid = fork();
    if (fpid == 0) {
        printf("child pause 1\n");
        sigset_t newset;
        sigemptyset(&newset);
        sigprocmask(SIG_SETMASK, &newset, nullptr);
        sigaddset(&newset, SIGUSR1);
        sigprocmask(SIG_BLOCK, &newset, nullptr);
        int ret1 = pause();
        printf("child pause end 1 retSig = %d, errno=%d\n", ret1, errno);
        exit(errno);
    }
    sleep(1);
    retValue = kill(fpid, SIGUSR1);
    sleep(1);
    retValue = kill(fpid, SIGPWR);
    EXPECT_EQ(retValue, 0);
    retValue = waitpid(fpid, &status, 0);
    EXPECT_EQ(retValue, fpid);
 
    fpid = fork();
    if (fpid == 0) {
        printf("child pause 2\n");
        pause();
        if (g_sigCount != 1) {
            exit(g_sigCount);
        }
        printf("child pause end 2\n");
        exit(0);
    }
    sleep(1);
    retValue = kill(fpid, SIGKILL);
    EXPECT_EQ(retValue, 0);
    retValue = waitpid(fpid, &status, 0);
    EXPECT_EQ(retValue, fpid);
}
 
static void *ThreadSetFunc2(void *arg)
{
    int retValue;
    sigset_t set;
    int sig;
 
    retValue = sigemptyset(&set);
    EXPECT_EQ(retValue, 0);
    retValue = sigaddset(&set, SIGALRM);
    EXPECT_EQ(retValue, 0);
    retValue = sigaddset(&set, SIGUSR1);
    EXPECT_EQ(retValue, 0);
 
    retValue = sigwait(&set, &sig);
    EXPECT_EQ(retValue, 0);
}
 
static void *ThreadSetDfl(void *arg)
{
    int retValue;
    sigset_t set;
    int sig;
 
    retValue = sigemptyset(&set);
    EXPECT_EQ(retValue, 0);
    retValue = sigaddset(&set, SIGALRM);
    EXPECT_EQ(retValue, 0);
    retValue = sigaddset(&set, SIGUSR1);
    EXPECT_EQ(retValue, 0);
 
    retValue = sigwait(&set, &sig);
    EXPECT_EQ(retValue, 0);
}
 
static void *ThreadKill(void *arg)
{
    int retValue;
    sigset_t set;
    int sig;
 
    retValue = sigemptyset(&set);
    EXPECT_EQ(retValue, 0);
    retValue = sigaddset(&set, SIGALRM);
    EXPECT_EQ(retValue, 0);
    retValue = sigaddset(&set, SIGUSR1);
    EXPECT_EQ(retValue, 0);
 
    retValue = sigwait(&set, &sig);
    EXPECT_EQ(retValue, 0);
}
 
TEST_F(ut_test_sig, sigSet_tc3)
{
    int status = 0;
    int retValue;
    int fpid, fpids;
    pthread_t thread, thread1, thread2;
    int father;
 
    fpid = fork();
    if (fpid == 0) {
        fpids = fork();
        if (fpids == 0) {
            retValue = pthread_create(&thread, NULL, ThreadKill, 0);
            if (retValue != 0) {
                exit(retValue);
            }
            retValue = pthread_create(&thread2, NULL, ThreadSetFunc2, 0);
            if (retValue != 0) {
                exit(retValue);
            }
            retValue = pthread_create(&thread1, NULL, ThreadSetDfl, 0);
            if (retValue != 0) {
                exit(retValue);
            }
 
            father = getppid();
            sleep(1);
            retValue = kill(father, SIGKILL);
            if (retValue != 0) {
                exit(retValue);
            }
        }
        sleep(1);
        retValue = waitpid(fpids, &status, 0);
        EXPECT_EQ(retValue, -1);
        exit(1);
    }
    sleep(1);
    retValue = waitpid(fpid, &status, 0);
    EXPECT_EQ(retValue, fpid);
}
 
TEST_F(ut_test_sig, sigaction_tc1)
{
    int pid;
    int ret, status;
    sigset_t newset;
    struct sigaction act;
    act.sa_handler = SigPrint1;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    pid = fork();
 
    if (pid == 0) {
        ret = sigaction(SIGUSR1, &act, nullptr);
        EXPECT_EQ(ret, 0);
        sigemptyset(&newset);
        sigaddset(&newset, SIGCHLD);
        ret = sigsuspend(&newset);
        EXPECT_EQ(ret, -1);
 
        ret = sigsuspend(NULL);
        EXPECT_EQ(ret, -1);
 
        ret = sigsuspend((sigset_t *)2); // 2, suspend signal num.
        EXPECT_EQ(ret, -1);
 
        ret = sigpending(NULL);
        EXPECT_EQ(ret, -1);
 
        ret = sigpending((sigset_t *)2); // 2, pending signal num.
        EXPECT_EQ(ret, -1);
    }
    sleep(1);
    ret = kill(pid, SIGUSR1);
    EXPECT_EQ(ret, 0);
    ret = waitpid(pid, &status, 0);
    EXPECT_EQ(ret, pid);
}
 
TEST_F(ut_test_sig, sigaction_tc2)
{
    int pid = 0;
    int ret = 0;
    void (*retptr)(int);
    sigset_t newset, oldset;
 
    pid = fork();
    if (pid == 0) {
        sigemptyset(&newset);
        sigaddset(&newset, SIGUSR1);
        sigaddset(&newset, SIGUSR2);
        ret = sigprocmask(SIG_BLOCK, &newset, &oldset);
        EXPECT_EQ(ret, 0);
        retptr = signal(SIGUSR1, SigPrint1);
        EXPECT_NE(retptr, (void *)-1);
        retptr = signal(SIGUSR2, SigPrint3);
        EXPECT_NE(retptr, (void *)-1);
        raise(SIGUSR1);
        raise(SIGUSR2);
        sigdelset(&newset, SIGUSR2);
        ret = sigprocmask(SIG_UNBLOCK, &newset, &oldset);
        EXPECT_EQ(ret, 0);
        sleep(1);
        EXPECT_EQ(g_sigCount, 1);
        EXPECT_EQ(g_sigCount1, 0);
        sigdelset(&newset, SIGUSR1);
        sigaddset(&newset, SIGUSR2);
        ret = sigprocmask(SIG_UNBLOCK, &newset, &oldset);
        EXPECT_EQ(ret, 0);
        sleep(1);
        EXPECT_EQ(g_sigCount, 1);
        EXPECT_EQ(g_sigCount1, 1);
    }
    wait(&ret);
    EXPECT_EQ(ret, 0);
}
 
TEST_F(ut_test_sig, raise_tc1)
{
    int count = 0;
    int status = 0;
    int fpid;
    int fatherPid;
    int sonPid;
    void (*ret)(int) = NULL;
    int retValue;
 
    g_sigCount = 0;
    ret = signal(SIGTERM, SigPrint1);
    EXPECT_NE(ret, (void *)-1);
    retValue = raise(SIGTERM);
    EXPECT_EQ(retValue, 0);
    fatherPid = getpid();
    fpid = fork();
    if (fpid == 0) {
        while (1) {
            usleep(500000);
            count++;
            if (g_sigCount == SIG_TEST_COUNT) {
                break;
            }
            if ((count) < (0) || (count) > (SIG_TEST_COUNT * 2)) {
                exit(count);
            }
        }
        exit(0);
    } else {
        sonPid = fpid;
        usleep(10);
        while (1) {
            retValue = kill(sonPid, SIGTERM);
            sleep(1);
            count++;
            if (count == SIG_TEST_COUNT) {
                break;
            }
        }
        retValue = waitpid(sonPid, &status, 0);
        EXPECT_EQ(retValue, sonPid);
    }
    EXPECT_EQ(g_sigCount, 1);
}
 
static int TestRaiseWake()
{
    int count = 0;
    int fpid;
    int fatherPid;
    int status, retValue;
    void (*retSig)(int);
 
    g_sigCount = 0;
    retSig = signal(SIGTERM, SigPrint1);
    EXPECT_NE(retSig, (void *)-1);
    fatherPid = getpid();
    fpid = fork();
    if (fpid == 0) {
        while (1) {
            retValue = kill(fatherPid, SIGTERM);
            if (retValue != 0) {
                exit(retValue);
            }
            usleep(100000);
            count++;
            if (count == SIG_TEST_COUNT) {
                break;
            }
        }
        exit(0);
    } else {
        usleep(10);
        while (1) {
            count++;
            usleep(3000);
            if (g_sigCount == SIG_TEST_COUNT) {
                break;
            }
        }
        retValue = waitpid(fpid, &status, 0);
        EXPECT_EQ(fpid, retValue);
    }
    return g_sigCount;
}
 
TEST_F(ut_test_sig, raise_tc2)
{
    int retValue, status;
 
    int fpid = fork();
    if (fpid == 0) {
        retValue = TestRaiseWake();
        if (retValue != SIG_TEST_COUNT) {
            exit(retValue);
        }
        exit(0);
    }
    retValue = waitpid(fpid, &status, 0);
    EXPECT_EQ(retValue, fpid);
}

TEST_F(ut_test_sig, sigismember_tc1)
{
    sigset_t signalset;
    int ret;
    ret = sigemptyset(&signalset);
    EXPECT_NE(ret, -1);

    ret = sigaddset(&signalset, SIGALRM);
    EXPECT_NE(ret, -1);

    ret = sigismember(&signalset, SIGALRM);
    EXPECT_EQ(ret, 1);

    ret = sigdelset(&signalset, SIGALRM);
    EXPECT_NE(ret, -1);

    ret = sigismember(&signalset, SIGALRM);
    EXPECT_EQ(ret, 0);
}

TEST_F(ut_test_sig, sigismember_tc2)
{
    sigset_t signalset;
    int ret;

    ret = sigfillset(&signalset);
    EXPECT_NE(ret, -1);

    ret = sigaddset(&signalset, SIGALRM);
    EXPECT_NE(ret, -1);

    ret = sigismember(&signalset, SIGALRM);
    EXPECT_EQ(ret, 1);

    ret = sigdelset(&signalset, SIGALRM);
    EXPECT_NE(ret, -1);

    ret = sigismember(&signalset, SIGALRM);
    EXPECT_EQ(ret, 0);
}

static void *ThreadKillTest(void *arg)
{
    sleep(2);
    return NULL;
}

TEST_F(ut_test_sig, pthread_kill_tc)
{
    int ret, status;
    int fpid;

    fpid = fork();
    if (fpid == 0) {
        pthread_t thread1;
        ret = pthread_create(&thread1, NULL, ThreadKillTest, 0);
        EXPECT_EQ(ret, 0);
        if (ret != 0) {
            exit(ret);
        }
        sleep(1);
        ret = pthread_kill(thread1, INVAILD_SIG);
        EXPECT_NE(ret, 0);
        if (ret != EINVAL) {
            exit(ret);
        }
        sleep(2);
        ret = pthread_join(thread1, NULL);
        EXPECT_NE(ret, 0);
        exit(0);
    }
    ret = waitpid(fpid, &status, 0);
    EXPECT_EQ(ret, fpid);
}
