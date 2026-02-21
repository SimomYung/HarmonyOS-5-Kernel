#include <stdio.h>
#include <stdint.h>
#include <sched.h>
#include "securec.h"
#include "stub.h"
#include "gtest/gtest.h"

class st_test_tlstsd : public testing::Test {
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

__thread int64_t tls_value;
__thread int64_t tls_array[1024];
void *test_tlstsd_tls1(void *arg)
{
    int i;
    tls_value = 1;
    for (i = 0; i < 1024; i++) {
        tls_array[i] = 1;
    }
    sched_yield();
    EXPECT_EQ(tls_value, 1);
    for (i = 0; i < 1024; i++) {
        EXPECT_EQ(tls_array[i], 1);
    }
    return 0;
}

void *test_tlstsd_tls2(void *arg)
{
    int i;
    tls_value = 2;
    for (i = 0; i < 1024; i++) {
        tls_array[i] = 2;
    }
    sched_yield();
    EXPECT_EQ(tls_value, 2);
    for (i = 0; i < 1024; i++) {
        EXPECT_EQ(tls_array[i], 2);
    }
    return 0;
}

TEST_F(st_test_tlstsd, st_test_tlstsd_tls)
{
    pthread_t lwt1, lwt2;
    pthread_attr_t attr1, attr2;
    pthread_attr_init(&attr1);
    pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_DETACHED);
    pthread_attr_init(&attr2);
    pthread_attr_setdetachstate(&attr2, PTHREAD_CREATE_DETACHED);
    int ret = pthread_create(&lwt1, &attr1, test_tlstsd_tls1, NULL);
    EXPECT_EQ(ret, 0);
    ret = pthread_create(&lwt2, &attr2, test_tlstsd_tls2, NULL);
    EXPECT_EQ(ret, 0);

    sleep(2);
}

TEST_F(st_test_tlstsd, st_test_tlstsd_tsd_invalid_key)
{
    int key = 256;
    int ret;
    uint64_t value = 22;
    void *getvalue;

    ret = pthread_setspecific(key, &value);
    EXPECT_NE(ret, 0);

    getvalue = pthread_getspecific(key);
    EXPECT_EQ(getvalue, (void *)NULL);
}

pthread_key_t g_tsdkey;

void *test_tlstsd_tsd1(void *arg)
{
    int ret;
    void *value = NULL;
    int input = 1;

    ret = pthread_setspecific(g_tsdkey, (void *)(&input));
    EXPECT_EQ(ret, 0);

    sched_yield();
    value = pthread_getspecific(g_tsdkey);
    EXPECT_EQ(*(int *)value, 1);
    return 0;
}

void *test_tlstsd_tsd2(void *arg)
{
    int ret;
    void *value = NULL;
    int input = 2;

    ret = pthread_setspecific(g_tsdkey, (void *)(&input));
    EXPECT_EQ(ret, 0);

    sched_yield();
    value = pthread_getspecific(g_tsdkey);
    EXPECT_EQ(*(int *)value, 2);
    return 0;
}

TEST_F(st_test_tlstsd, st_test_tlstsd_tsd)
{
    int ret;

    ret = pthread_key_create(&g_tsdkey, NULL);
    EXPECT_EQ(ret, 0);

    pthread_t lwt1, lwt2;
    pthread_attr_t attr1, attr2;
    pthread_attr_init(&attr1);
    pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_DETACHED);
    pthread_attr_init(&attr2);
    pthread_attr_setdetachstate(&attr2, PTHREAD_CREATE_DETACHED);

    ret = pthread_create(&lwt1, &attr1, test_tlstsd_tsd1, NULL);
    EXPECT_EQ(ret, 0);
    ret = pthread_create(&lwt2, &attr2, test_tlstsd_tsd2, NULL);
    EXPECT_EQ(ret, 0);

    sleep(2);
    pthread_key_delete(g_tsdkey);
}

int g_testTsdDtor = 0;
static void tlstsd_dtor(void *dummy)
{
    g_testTsdDtor = 1;
}

void *test_tlstsd_tsd_keycreate(void *arg)
{
    int ret;
    int input = 2;
    ret = pthread_key_create(&g_tsdkey, tlstsd_dtor);
    EXPECT_EQ(ret, 0);
    ret = pthread_setspecific(g_tsdkey, (void *)(&input));
	EXPECT_EQ(ret, 0);
    return 0;
}

TEST_F(st_test_tlstsd, st_test_tlstsd_keycreate)
{
    int ret;
    pthread_t lwt1;
    pthread_attr_t attr1;
    pthread_attr_init(&attr1);
    pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_DETACHED);

    ret = pthread_create(&lwt1, &attr1, test_tlstsd_tsd_keycreate, NULL);
    EXPECT_EQ(ret, 0);
    pthread_join(lwt1, NULL);
    sleep(2);
    EXPECT_EQ(g_testTsdDtor, 1);
    pthread_key_delete(g_tsdkey);
}

TEST_F(st_test_tlstsd, st_test_tlstsd_tsd_invalidkeydel)
{
    int key = 256;
    int ret;

    ret = pthread_key_delete(key);
    EXPECT_NE(ret, 0);
}

TEST_F(st_test_tlstsd, st_test_tlstsd_tsd_keydel)
{
    int ret;
    pthread_key_t key;

    ret = pthread_key_create(&key, tlstsd_dtor);
    EXPECT_EQ(ret, 0);

    ret = pthread_key_delete(key);
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_tlstsd, st_test_tlstsd_tsd_nulldtor)
{
    int ret;
    pthread_key_t key;

    ret = pthread_key_create(&key, NULL);
    EXPECT_EQ(ret, 0);

    ret = pthread_key_delete(key);
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_tlstsd, st_test_tlstsd_tsd_maxkey)
{
    int ret;
    pthread_key_t key[512];
    int num = 0;

    while (1) {
        ret = pthread_key_create(&key[num], NULL);
        if (ret == 0) {
            num++;
        } else {
            break;
        }
    }
    EXPECT_EQ(num, 126);    /* gtest alloc two key */
    for (int i = 0; i < num; i++) {
        ret = pthread_key_delete(key[i]);
        EXPECT_EQ(ret, 0);
    }
}

pthread_key_t kkey[256];
int keyi;
static void tlstsd_dtorkey(void *dummy)
{
    void *buf = (void *)7;
    int ret = pthread_key_create(&kkey[keyi], tlstsd_dtorkey);
    if (ret == 0) {
        pthread_setspecific(kkey[keyi], buf);
        keyi++;
    }
}

void *test_tlstsd_tsd_dtorkey(void *arg)
{
    int ret;
    int input = 2;
    ret = pthread_key_create(&g_tsdkey, tlstsd_dtorkey);
    EXPECT_EQ(ret, 0);
    ret = pthread_setspecific(g_tsdkey, (void *)(&input));
	EXPECT_EQ(ret, 0);
    return 0;
}

TEST_F(st_test_tlstsd, st_test_tlstsd_dtorkey)
{
    int ret;
    pthread_t lwt1;
    pthread_attr_t attr1;
    pthread_attr_init(&attr1);
    pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_DETACHED);

    ret = pthread_create(&lwt1, &attr1, test_tlstsd_tsd_dtorkey, NULL);
    EXPECT_EQ(ret, 0);
    sleep(5);
    ret = pthread_key_delete(g_tsdkey);
    EXPECT_EQ(ret, 0);
    for (int i = 0; i < keyi; i++) {
        ret = pthread_key_delete(kkey[i]);
        EXPECT_EQ(ret, 0);
    }
}