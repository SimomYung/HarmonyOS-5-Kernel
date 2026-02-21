#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <execinfo.h>
#include <pthread.h>
#include "stub.h"
#include "gtest/gtest.h"

class test_malloc : public testing::Test {
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

#define TEST_SMP_MEM_NUM 4

typedef struct {
    int32_t id;
    int32_t tid;
} test_smp_mem_info;

test_smp_mem_info g_test_tinfo[TEST_SMP_MEM_NUM];
void *test_smp_mem(void *arg)
{
    int32_t ret;
    test_smp_mem_info *info = (test_smp_mem_info *)arg;

    info->tid = gettid();
    cpu_set_t cpuset = {0};
    CPU_ZERO(&cpuset);

    if (info->id % 2 == 0) {
        CPU_SET(0, &cpuset);
        ret = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    } else {
        CPU_SET(1, &cpuset);
        ret = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    }

    int32_t times = 1;
    while (times-- > 0) {
        for (uint32_t loop = 0; loop < 100; loop++) {
            uint32_t size = 1024;
            void *addr[5];
            for (uint32_t i = 0; i < 5; i++) {
                addr[i] = malloc(size);
                memset(addr[i], 0x01, size);
                size = size * 2;
            }

            for (uint32_t i = 0; i < 5; i++) {
                uint8_t *bytes = (uint8_t *)addr[i];
                for (uint32_t j = 0; j < 1024; j++) {
                    if (bytes[j] != 0x01) {
                        *(int32_t *)0 = 1;
                    }
                }
            }

            for (uint32_t i = 0; i < 5; i++) {
                free(addr[i]);
            }
        }
    }
}

TEST_F(test_malloc, st_test_malloc)
{
    pthread_t thread[TEST_SMP_MEM_NUM] = {0};

    for (int32_t i = 0; i < TEST_SMP_MEM_NUM; i++) {
        g_test_tinfo[i].id = i;
        pthread_create(&(thread[i]), NULL, test_smp_mem, &(g_test_tinfo[i]));
    }

    for (int32_t i = 0; i < TEST_SMP_MEM_NUM; i++) {
        pthread_join(thread[i], NULL);
    }
}
