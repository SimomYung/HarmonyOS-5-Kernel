/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Entropy collections for RNG functions
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 25 11:49:29 2019
 */
/* libc */
#include <stdint.h>
#include <sys/param.h>

/* hongmeng ulibs */
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hongmeng/syscall.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_timer.h>

/* direct header */
#include <libentropy/entropy.h>
#include <libentropy/health_test.h>

#ifdef __HOST_LLT__
#define INNER
#else
#define INNER static
#endif

#define RANDOM_EVENTS_QUEUE_RING_MAX	512
#define RANDOM_EVENT_DATA_MAX		12

typedef struct random_event {
	uint8_t data[RANDOM_EVENT_DATA_MAX];
	uint32_t time;
} random_event_t;

/*
 * if in == out, the queue is empty;
 * if in != out, the queue is not empty;
 * if (in + 1) % (RANDOM_EVENTS_QUEUE_RING_MAX) == out, the queue is full;
 */
typedef struct random_events {
	struct events_queue_ring {
		struct random_event events[RANDOM_EVENTS_QUEUE_RING_MAX];
		unsigned int in;
		unsigned int out;
#ifdef CONFIG_NOISE_SOURCE_DATA_COLLECT
		unsigned int byte_in;
#endif
	} queue_ring;
} random_events_t;
INNER random_events_t random_events;
 /* lock for queue_ring.in */
static struct raw_mutex s_in_lock = RAW_MUTEX_INITIALIZER;
/* lock for queue_ring.out */
static struct raw_mutex s_out_lock = RAW_MUTEX_INITIALIZER;

/* Repetition Count Test */
static struct health_test_data rc_test_data = {0};
/* Adaptive Proportion Test */
static struct health_test_data ap_test_data = {0};
static struct raw_mutex health_test_lock = RAW_MUTEX_INITIALIZER;

unsigned int entropy_test_fail_count(void)
{
	return rc_test_data.fail_count + ap_test_data.fail_count;
}

void entropy_init_health_data(int rc_test_max_count, int ap_test_max_count)
{
	rc_test_data.max_count = rc_test_max_count;
	ap_test_data.max_count = ap_test_max_count;
}

/*
 * read event data, and store in @out buffer
 * @out buffer size must be N * (event.data + event.extra_randomness)
 */
static int read_random_event(unsigned char *out, unsigned int out_buf_len)
{
	unsigned int ring_in;
	unsigned int ring_out;
	unsigned int need_event_count;
	struct random_event *out_event = NULL;
	unsigned char *tmp_out = out;
	int out_len = 0;

	if (tmp_out == NULL || out_buf_len == 0) {
		return E_HM_INVAL;
	}

	if (out_buf_len % sizeof(random_event_t) != 0) {
		hm_error("read_random_event size is invalid\n");
		return E_HM_INVAL;
	}
	need_event_count = out_buf_len / sizeof(random_event_t);

	/*
	 * Here we don't need to lock the queue_ring.in, as we won't changed
	 * it and we don't need to know its latest value immediately. The
	 * worst-case scenario is the queue_ring.in has update but we use a
	 * old value and then the valid event nodes we get is less than it
	 * actually has.
	 */
	raw_mutex_lock(&s_out_lock);

	ring_in = random_events.queue_ring.in;
	ring_out = random_events.queue_ring.out;
	/*
	 * Loop termination condition:
	 * Following loop terminate when ring_out is equal to ring_in. Since ring_out
	 * is monotonic, ring_out keeps increasing in the loop body and modulo according to
	 * RANDOM_EVENTS_QUEUE_RING_MAX, ring_in is less than RANDOM_EVENTS_QUEUE_RING_MAX,
	 * it could be an upper bound.
	 * ring_in is constant and reasonable: ring_in is init in entropy_add_randomness,
	 * it is less than RANDOM_EVENTS_QUEUE_RING_MAX. So ring_in as the loop condition
	 * is reasonable.
	 */
	while (ring_out != ring_in && need_event_count > 0) {
		out_event = &random_events.queue_ring.events[ring_out];
		/* The operation is performed in the unit of random_event_t. */
		NOFAIL(memcpy_s(tmp_out, sizeof(random_event_t), out_event, sizeof(random_event_t)));
		NOFAIL(memset_s(out_event, sizeof(random_event_t), 0, sizeof(random_event_t)));
		tmp_out += sizeof(random_event_t);
		out_len += (int)sizeof(random_event_t);

		ring_out = (ring_out + 1) % RANDOM_EVENTS_QUEUE_RING_MAX;
		need_event_count -= 1;
	}
	random_events.queue_ring.out = ring_out;
	raw_mutex_unlock(&s_out_lock);

	return out_len;
}

#ifndef CONFIG_NOISE_SOURCE_DATA_COLLECT

/* The src will XOR byte by byte with the dst */
static void xor_itera_mix(void *dst, unsigned int dst_len,
			  const void *src, unsigned int src_len)
{
	unsigned int i;
	unsigned char *dst_buf = (unsigned char *)dst;
	unsigned char *src_buf = (unsigned char *)src;

	if (dst_len == 0) {
		return;
	}

	/*
	 * Loop termination condition:
	 * Following loop will terminate when i reach src_len. Since i is monotonic, keep
	 * increasing in loop body, it has an upper bound. Loop condition is constant and
	 * reasonable: src_len is less than HM_RNG_SHA256_DIGEST_SIZE(32), so the loop
	 * condition is reasonable.
	 */
	for (i = 0; i < src_len; i++) {
		dst_buf[i % dst_len] ^= src_buf[i];
	}
}

/*
 * conditioning component
 * noise source data will store in event data with xor operation
 * noise source data may be compressed to fewer bytes, entropy will decrease
 *
 * return val:
 *  >= 0 the event index of data store
 *  -1   event array is full
 */
static int store_event_with_xor(const void *data, unsigned int data_len, uint32_t time_data)
{
	unsigned int in;
	unsigned int out;
	struct random_event *events = NULL;

	/*
	 * Here we don't need to lock the queue_ring.out, as we won't changed
	 * it and we don't need to know its latest value immediately. The
	 * worst-case scenario is we think the queue is full by the value of
	 * out we getted, but the latest status is the queue is not full with
	 * some other threads(reading randomness) changing the out's value,
	 * that is no problem.
	 */
	raw_mutex_lock(&s_in_lock);

	in = random_events.queue_ring.in;
	out = random_events.queue_ring.out;
	events = &random_events.queue_ring.events[0];
	if (((in + 1) % RANDOM_EVENTS_QUEUE_RING_MAX) == out) {
		/* event array is full */
		raw_mutex_unlock(&s_in_lock);
		return -1;
	}
	xor_itera_mix(events[in].data, sizeof(events[in].data), data, data_len);
	events[in].time = time_data;
	random_events.queue_ring.in = (in + 1) % RANDOM_EVENTS_QUEUE_RING_MAX;

	raw_mutex_unlock(&s_in_lock);

	return (int)in;
}
#endif

/*
 * noise source data just copy to event arrary and noise source data will
 * not be compressed, entropy will keep.
 *
 * return:
 *  >= 0 the event index of data store
 *  -1   event array is full
 */
#ifdef CONFIG_NOISE_SOURCE_DATA_COLLECT

#define RANDOM_EVENTS_BYTES_MAX	(RANDOM_EVENTS_QUEUE_RING_MAX * sizeof(random_event_t))

static int store_event_with_copy(const void *data, unsigned int data_len, uint64_t time_data)
{
	unsigned int i;
	unsigned int byte_in;
	char *events_byte = (char *)&random_events.queue_ring.events[0];
	char *time_byte = (char *)&time_data;

	raw_mutex_lock(&s_in_lock);

	byte_in = random_events.queue_ring.byte_in;

	for (i = 0; i < data_len; i++) {
		events_byte[byte_in] = *((char *)data + i);
		byte_in = (byte_in + 1) % RANDOM_EVENTS_BYTES_MAX;
	}

	for (i = 0; i < sizeof(time_data); i++) {
		events_byte[byte_in] = time_byte[i];
		byte_in = (byte_in + 1) % RANDOM_EVENTS_BYTES_MAX;
	}

	random_events.queue_ring.byte_in = byte_in;
	random_events.queue_ring.in = byte_in / sizeof(random_event_t);

	raw_mutex_unlock(&s_in_lock);

	return (int)byte_in;
}
#endif

static int entropy_source_health_test(const void *data, unsigned int data_len, __u64 clock)
{
	int ret;

	raw_mutex_lock(&health_test_lock);
	/* health Repetition Count tset */
	ret = health_rc_test_batch(&rc_test_data, (uint8_t *)data, (int)data_len);
	if (ret == E_HM_OK) {
		ret = health_rc_test_batch(&rc_test_data, (uint8_t *)&clock, (int)sizeof(clock));
	}
	if (ret < E_HM_OK) {
		rc_test_data.fail_count++;
		if (rc_test_data.fail_count > HEALTH_FAILURE_COUNT_MAX) {
			rc_test_data.fail_count = HEALTH_FAILURE_COUNT_MAX;
		}
		raw_mutex_unlock(&health_test_lock);
		return -1;
	}
	rc_test_data.fail_count = 0;

	/* health Adaptive Proportion test */
	ret = health_ap_test_batch(&ap_test_data, (uint8_t *)data, (int)data_len);
	if (ret == E_HM_OK) {
		ret = health_ap_test_batch(&ap_test_data, (uint8_t *)&clock, (int)sizeof(clock));
	}
	if (ret < E_HM_OK) {
		ap_test_data.fail_count++;
		if (ap_test_data.fail_count > HEALTH_FAILURE_COUNT_MAX) {
			ap_test_data.fail_count = HEALTH_FAILURE_COUNT_MAX;
		}
		raw_mutex_unlock(&health_test_lock);
		return -1;
	}
	ap_test_data.fail_count = 0;

	/* health startup test */
	if (health_startup_test(data_len + sizeof(clock)) == E_HM_AGAIN) {
		/* discard startup data */
		raw_mutex_unlock(&health_test_lock);
		return -1;
	}

	raw_mutex_unlock(&health_test_lock);
	return 0;
}

#define NS_PER_S 1000000000ULL
static int get_monotonic_clock(__u64 *time)
{
	struct timespec ts;
	int ret;

	ret = hm_clock_gettime(CLOCK_MONOTONIC, &ts);
	if (ret == E_HM_OK) {
		*time = (__u64)ts.tv_sec * NS_PER_S + (__u64)ts.tv_nsec;
	}
	return ret;
}

/*
 * 1 health test for entropy source and failure process (NIST SP800-90B section 4)
 *
 *   Repetition Count Test and Adaptive Proportion Test
 *
 *   Failure Process:
 *     1) When the health tests fail, the entropy source shall notify the
 *        consuming application of the error condition. Our consuming
 *        application is crypto. We will count the number of health test
 *        failures here.
 *     2) When the health tests fail, the store_envent_with_xor() will
 *        not store any nosie data.
 *
 * 2. store noise or entropy data
 *
 *   control by CONFIG_NOISE_SOURCE_DATA_COLLECT
 */
static void store_random_event(const void *data, unsigned int data_len, unsigned int entropy_src)
{
	__u64 clock;

	if (data == NULL || data_len == 0) {
		return;
	}
	entropy_src %= ENTROPY_NOISE_SOURCE_MAX;

	if (((random_events.queue_ring.in + 1) % RANDOM_EVENTS_QUEUE_RING_MAX) == random_events.queue_ring.out) {
		/* event array is full */
		return ;
	}

	if (get_monotonic_clock(&clock) != E_HM_OK) {
		return;
	}

	if (entropy_src != ENTROPY_BOOT_NOISE_SOURCE) {
		if (entropy_source_health_test(data, data_len, clock) < 0) {
			return;
		}
	}

#ifndef CONFIG_NOISE_SOURCE_DATA_COLLECT
	/* Compress the time data to 32 bit, because low bits are the most variable */
	uint32_t *clock_u32 = ptr_to_type(&clock, uint32_t *);
	clock_u32[0] ^= clock_u32[1];
	/* store event data with time xor */
	(void)store_event_with_xor(data, data_len, clock_u32[0]);
#else
	/* just copy data in event array (no process) */
	(void)store_event_with_copy(data, data_len, clock);
#endif
}

void entropy_add_randomness(const void *data, unsigned int data_len,
			    unsigned int entropy_src)
{
	store_random_event(data, data_len, entropy_src);
}

int entropy_read_randomness(unsigned char *out, unsigned int out_buf_len,
			    unsigned int *fail_count)
{
	int ret = read_random_event(out, out_buf_len);
	if (fail_count != NULL) {
		*fail_count = entropy_test_fail_count();
	}
	return ret;
}
