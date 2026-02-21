#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/shm.h>
#include "securec.h"
#include "stub.h"
#include "gtest/gtest.h"

typedef struct {
    key_t key;
    size_t size;
    int shmid;
} TestShm;

#define TEST_SHM_SIZE 5
TestShm g_shminfo[TEST_SHM_SIZE] = {
    {32166, 0, -1},
    {32177, 2048, -1},
    {32188, 4096, -1},
    {32199, 40960, -1},
    {32211, 0x1400000, -1}  // 20M
};

void TestSuite_test_shm_Setup()
{
    pid_t pid;
    int shmid;
    void *addr;

    for (int i = 0; i < TEST_SHM_SIZE; i++) {
        shmid = shmget(g_shminfo[i].key, g_shminfo[i].size, IPC_CREAT | IPC_EXCL);
        if (shmid < 0) {
            shmid = shmget(g_shminfo[i].key, g_shminfo[i].size, 0);
            if (shmid < 0) {
                printf("test_shm setup Fail...%d\n", g_shminfo[i].key);
                continue;
            }
            shmctl(shmid, IPC_RMID, NULL);
            shmid = shmget(g_shminfo[i].key, g_shminfo[i].size, IPC_CREAT | IPC_EXCL);
            if (shmid < 0) {
                printf("test_shm setup Fail...%d\n", g_shminfo[i].key);
                continue;
            }
        }
        addr = shmat(shmid, 0, 0);
        if (g_shminfo[i].size > 0) {
            (void)memset(addr, i, 1024);
        }
    }
    
}

class st_test_shm : public testing::Test {
protected:
    static void SetUpTestSuite() {
        TestSuite_test_shm_Setup();
    }

    static void TearDownTestSuite() {
        int shmid;
        for (int i = 0; i < TEST_SHM_SIZE; i++) {
            shmid = shmget(g_shminfo[i].key, g_shminfo[i].size, 0);
            if (shmid < 0) {
                continue;
            }
            shmctl(shmid, IPC_RMID, NULL);
        }
    }

    virtual void SetUp() {}

    virtual void TearDown() {}
};

TEST_F(st_test_shm, st_test_shmget_invalid_size)
{
    int shmid;

    shmid = shmget(g_shminfo[0].key, g_shminfo[0].size, IPC_CREAT);
    EXPECT_EQ(shmid, -1);
}

TEST_F(st_test_shm, st_test_shmget_key_exist_execl)
{
    int shmid;

    shmid = shmget(g_shminfo[1].key, g_shminfo[1].size, IPC_CREAT | IPC_EXCL);
    EXPECT_EQ(shmid, -1);
}

TEST_F(st_test_shm, st_test_shmget_key_noexist_noexecl)
{
    int shmid;

    shmid = shmget(32222, 4096, 0);
    EXPECT_EQ(shmid, -1);
}

TEST_F(st_test_shm, st_test_shmget_key_exist)
{
    int shmid;

    for (int i = 1; i < TEST_SHM_SIZE; i++) {
        shmid = shmget(g_shminfo[i].key, g_shminfo[i].size, 0);
        EXPECT_GE(shmid, 0);
    }
}

TEST_F(st_test_shm, st_test_shmget_key_noexsit)
{
    int shmid;

    shmid = shmget(32222, 4096, IPC_CREAT);
    EXPECT_GE(shmid, 0);
}

int g_Task1Shmid = -1;
int g_Task2Shmid = -1;
void *test_shmget_thread1(void *arg)
{
    g_Task1Shmid = shmget(g_shminfo[1].key, g_shminfo[1].size, 0);
    return 0;
}

void *test_shmget_thread2(void *arg)
{
    g_Task2Shmid = shmget(g_shminfo[1].key, g_shminfo[1].size, 0);
    return 0;
}
TEST_F(st_test_shm, st_test_shmget_concurrency)
{
    int shmid;

    pthread_t lwt1, lwt2;
    pthread_attr_t attr1, attr2;
    pthread_attr_init(&attr1);
    pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_DETACHED);
    pthread_attr_init(&attr2);
    pthread_attr_setdetachstate(&attr2, PTHREAD_CREATE_DETACHED);
    int ret = pthread_create(&lwt1, &attr1, test_shmget_thread1, NULL);
    EXPECT_EQ(ret, 0);
    ret = pthread_create(&lwt2, &attr2, test_shmget_thread2, NULL);
    EXPECT_EQ(ret, 0);

    sleep(1);

    EXPECT_EQ(g_Task1Shmid, g_Task2Shmid);
}

TEST_F(st_test_shm, st_test_shmget_shmctl_pressure)
{
    int i;
    int shmid;
    int ret;

    for (i = 0; i < 10000; i++) {
        shmid = shmget(32233, 4096, IPC_CREAT);
        EXPECT_GE(shmid, 0);
        ret = shmctl(shmid, IPC_RMID, 0);
        EXPECT_EQ(ret, 0);
    }
}

TEST_F(st_test_shm, st_test_shmat_invalid_shmid)
{
    void *vaddr = NULL;

    vaddr = shmat(777, 0, 0);
    EXPECT_EQ((uint64_t)vaddr, -1);
}

TEST_F(st_test_shm, st_test_shmat_invalid_vaddr)
{
    void *vaddr = NULL;
    int shmid;

    shmid = shmget(g_shminfo[1].key, g_shminfo[1].size, 0);
    vaddr = shmat(shmid, (void *)-1, 0);
    EXPECT_EQ((uint64_t)vaddr, -1);
}

TEST_F(st_test_shm, st_test_shmat_rw)
{
    char *vaddr1 = NULL;

    int shmid = shmget(g_shminfo[1].key, g_shminfo[1].size, 0);
    vaddr1 = (char *)shmat(shmid, 0, 0);
    EXPECT_EQ(vaddr1[0], 1);

    vaddr1[1] = 2;
    EXPECT_EQ(vaddr1[1], 2);
    vaddr1[1] = 1;
    // uninit
    shmdt(vaddr1);
}

TEST_F(st_test_shm, st_test_shmat)
{
    char *vaddr[TEST_SHM_SIZE] = {0};
    int shmid;

    for (int i = 1; i < TEST_SHM_SIZE; i++) {
        shmid = shmget(g_shminfo[i].key, g_shminfo[i].size, 0);
        vaddr[i] = (char *)shmat(shmid, 0, 0);
        EXPECT_NE((uint64_t)vaddr[i], 0);

        for (int j = 0; j < 1024; j++) {
            EXPECT_EQ(vaddr[i][j], i);
        }
        vaddr[i][g_shminfo[i].size - 1] = i;
        EXPECT_EQ(vaddr[i][g_shminfo[i].size - 1], i);
    }

    // uninit
    for (int i = 1; i < TEST_SHM_SIZE; i++) {
        shmdt(vaddr[i]);
    }
}

TEST_F(st_test_shm, st_test_shmat_fix_addr)
{
    char *vaddr = NULL;
    int shmid;

    shmid = shmget(g_shminfo[1].key, g_shminfo[1].size, 0);
    vaddr = (char *)shmat(shmid, (void *)0x9000000000, 0);
    EXPECT_EQ((uint64_t)vaddr, 0x9000000000);

    for (int i = 0; i < 1024; i++) {
        EXPECT_EQ(vaddr[i], 1);
    }

    // uninit
    shmdt((void *)0x9000000000);
}

void *test_shmat_thread(void *arg)
{
    char *vaddr = NULL;
    int shmid;

    shmid = shmget(g_shminfo[1].key, g_shminfo[1].size, 0);
    vaddr = (char *)shmat(shmid, 0, 0);
    for (int i = 0; i < 1024; i++) {
        EXPECT_EQ(vaddr[i], 1);
    }

    // uninit
    shmdt(vaddr);
    return 0;
}

TEST_F(st_test_shm, st_test_shmat_concurrency)
{
    int shmid;

    pthread_t lwt1, lwt2;
    pthread_attr_t attr1, attr2;
    pthread_attr_init(&attr1);
    pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_DETACHED);
    pthread_attr_init(&attr2);
    pthread_attr_setdetachstate(&attr2, PTHREAD_CREATE_DETACHED);
    int ret = pthread_create(&lwt1, &attr1, test_shmat_thread, NULL);
    EXPECT_EQ(ret, 0);
    ret = pthread_create(&lwt2, &attr2, test_shmat_thread, NULL);
    EXPECT_EQ(ret, 0);
}

#define SHMAT_FORK_TEXT 7
TEST_F(st_test_shm, st_test_shmat_fork)
{
    int shmid;
    char *vaddr = NULL;
    pid_t pid;

    shmid = shmget(32244, 4096, IPC_CREAT);
    vaddr = (char *)shmat(shmid, 0, 0);
    (void)memset(vaddr,  SHMAT_FORK_TEXT, 4096);

    pid = fork();
    if (pid == 0) {
        int i;

        shmid = shmget(32244, 4096, IPC_CREAT);
        vaddr = (char *)shmat(shmid, 0, 0);
        for (i = 0; i < 4096; i++) {
            EXPECT_EQ(vaddr[i], 7);
        }
        exit(0);
    }
}

TEST_F(st_test_shm, st_test_shmat_oversize)
{
    char *vaddr = NULL;
    int shmid;

    shmid = shmget(32222, 0x15e00000, 0);
    if (shmid > 0) {
        vaddr = (char *)shmat(shmid, 0, 0);
        EXPECT_EQ((uint64_t)vaddr, -1);

        shmctl(shmid, IPC_RMID, 0);
    } else {
        EXPECT_EQ(shmid, -1);
    }
}

TEST_F(st_test_shm, st_test_shmat_10m_20m)
{
    int shmid;
    char *vaddr = NULL;

    shmid = shmget(32255, 0xa00000, IPC_CREAT);
    EXPECT_GE(shmid, 0);
    vaddr = (char *)shmat(shmid, 0, 0);
    vaddr[0xa00000 - 1] = 7;
    EXPECT_EQ(vaddr[0xa00000 - 1], 7);

    //uninit
    shmdt(vaddr);
    shmctl(shmid, IPC_RMID, 0);

    shmid = shmget(32266, 0x1400000, IPC_CREAT);
    EXPECT_GE(shmid, 0);
    vaddr = (char *)shmat(shmid, 0, 0);
    vaddr[0x1400000 - 1] = 7;
    EXPECT_EQ(vaddr[0x1400000 - 1], 7);

    //uninit
    shmdt(vaddr);
    shmctl(shmid, IPC_RMID, 0);
}

TEST_F(st_test_shm, st_test_shmdt_invalid_vaddr)
{
    int ret;

    ret = shmdt((void *)0xFFFF000000010000);
    EXPECT_EQ(ret, -1);

    ret = shmdt((void *)0x1000000);
    EXPECT_EQ(ret, -1);
}

TEST_F(st_test_shm, st_test_shmdt)
{
    void *vaddr[TEST_SHM_SIZE] = {0};
    int i;
    int ret;
    int shmid;

    for (i = 1; i < TEST_SHM_SIZE; i++) {
        shmid = shmget(g_shminfo[i].key, g_shminfo[i].size, 0);
        vaddr[i] = (char *)shmat(shmid, 0, 0);
    }

    for (i = 1; i < TEST_SHM_SIZE; i++) {
        ret = shmdt(vaddr[i]);
        EXPECT_EQ(ret, 0);
    }
}

void *test_shmdt_thread(void *arg)
{
    int shmid;
    char *vaddr = NULL;
    int ret;

    shmid = shmget(g_shminfo[1].key, g_shminfo[1].size, 0);
    vaddr = (char *)shmat(shmid, 0, 0);
    ret = shmdt(vaddr);
    EXPECT_EQ(ret, 0);
    return 0;
}

TEST_F(st_test_shm, st_test_shmdt_concurrency)
{
    pthread_t lwt1, lwt2;
    pthread_attr_t attr1, attr2;
    pthread_attr_init(&attr1);
    pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_DETACHED);
    pthread_attr_init(&attr2);
    pthread_attr_setdetachstate(&attr2, PTHREAD_CREATE_DETACHED);
    int ret = pthread_create(&lwt1, &attr1, test_shmdt_thread, NULL);
    EXPECT_EQ(ret, 0);
    ret = pthread_create(&lwt2, &attr2, test_shmdt_thread, NULL);
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_shm, st_test_shmctl_invalid_shmid)
{
    int ret;

    ret = shmctl(999999, IPC_RMID, 0);
    EXPECT_EQ(ret, -1);
}

TEST_F(st_test_shm, st_test_shmctl_invalid_buf)
{
    int ret;
    int shmid;

    shmid = shmget(g_shminfo[1].key, g_shminfo[1].size, 0);
    ret = shmctl(shmid, IPC_STAT, 0);
    EXPECT_EQ(ret, -1);
}

TEST_F(st_test_shm, st_test_shmctl_info)
{
    int ret;
    int shmid;
    struct shmid_ds buf = {0};

    shmid = shmget(g_shminfo[2].key, g_shminfo[2].size, 0);
    ret = shmctl(shmid, IPC_STAT, &buf);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(g_shminfo[2].size, buf.shm_segsz);
}

TEST_F(st_test_shm, st_test_shmctl_rm)
{
    int ret;
    int shmid;

    shmid = shmget(g_shminfo[1].key, g_shminfo[1].size, 0);
    EXPECT_GE(shmid, 0);

    ret = shmctl(shmid, IPC_RMID, 0);
    EXPECT_EQ(ret, 0);

    shmid = shmget(g_shminfo[1].key, g_shminfo[1].size, 0);
    EXPECT_EQ(shmid, -1);
}

TEST_F(st_test_shm, st_test_shmctl_before_dettach)
{
    int ret;
    int shmid, shmidtest;
    char *ptr = NULL;
    char *ptr1 = NULL;

    shmid = shmget(32288, 4096, IPC_CREAT);
    EXPECT_GE(shmid, 0);

    shmidtest = shmget(32288, 4096, 0);
    EXPECT_EQ(shmid, shmidtest);

    ptr = (char *)shmat(shmid, 0, 0);
    ptr[0] = 'a';

    ret = shmctl(shmid, IPC_RMID, 0);
    EXPECT_EQ(ret, 0);

    shmidtest = shmget(g_shminfo[1].key, g_shminfo[1].size, 0);
    EXPECT_EQ(shmidtest, -1);

    EXPECT_EQ(ptr[0], 'a');

    ptr1 = (char *)shmat(shmid, 0, 0);
    EXPECT_EQ(ptr1[0], 'a');
    shmdt(ptr);
    shmdt(ptr1);
    ptr = (char *)shmat(shmid, 0, 0);
    EXPECT_EQ((uint64_t)ptr, -1);
}
