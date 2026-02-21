#include "jpu.h"
#include "jpu_def.h"

#define JPEGD_LOCK_ID		70
#define JPEGD_LOCK_TIMEOUT  1000

static struct hwspinlock *g_jpegd_hwlock;

int32_t os_request_irq(uint32_t isrno, irq_handler_t handler, struct jpu_data_type *jpu_device)
{
	return request_irq(isrno, handler, 0, "jpgdec_irq", (void *)jpu_device);
}

void os_enable_irq(uint32_t isrno)
{
	enable_irq(isrno);
}

void os_disable_irq(uint32_t isrno)
{
	disable_irq(isrno);
}

void os_free_irq(uint32_t isrno)
{
	free_irq(isrno, 0);
}

void os_sem_up(os_sem sem)
{
	up(sem);
}

void os_sem_down(os_sem sem)
{
	down(sem);
}

void os_wait_ms(uint32_t millisec)
{
	msleep(millisec);
}

int jpu_hwspinlock_init(void)
{
	return 0;
}

int jpu_hardlock_get(void)
{
	return hwspin_lock_timeout(g_jpegd_hwlock, JPEGD_LOCK_TIMEOUT);
}

void jpu_hardlock_put(void)
{
	hwspin_unlock(g_jpegd_hwlock);
}

long os_timestamp_diff(const os_time_t *lasttime, const os_time_t *curtime)
{
	bool cond = ((lasttime == NULL) || (curtime == NULL));
	if (cond)
		return 0;

	return (curtime->tv_usec >= lasttime->tv_usec) ?
		(curtime->tv_usec - lasttime->tv_usec) :
		(1000000 - (lasttime->tv_usec - curtime->tv_usec)); /* clk cycle 1000000 us */
}

void os_get_timestamp(os_time_t *tv)
{
	struct timespec64 ts;

	ktime_get_ts64(&ts);
	tv->tv_sec = ts.tv_sec;
	tv->tv_usec = ts.tv_nsec / NSEC_PER_USEC;
}

int32_t jpu_wait_interrupt(struct jpu_interrupt_t *jpu_interrupt)
{
	int ret;
	unsigned long timeout_jiffies;
	int times = 0;
	uint32_t timeout_interval;
	if (jpu_interrupt->fpga_flag != 0)
		timeout_interval = JPGDEC_DONE_TIMEOUT_THRESHOLD_FPGA;
	else
		timeout_interval = JPGDEC_DONE_TIMEOUT_THRESHOLD_ASIC;

	do {
		timeout_jiffies = (unsigned long)msecs_to_jiffies(timeout_interval);
		/*lint -e578*/
		ret = (int)wait_event_interruptible_timeout(jpu_interrupt->jpu_dec_done_wq,
			jpu_interrupt->jpu_dec_done_flag, timeout_jiffies);
		if (ret != -ERESTARTSYS)
			break;
		times++;
		mdelay(10);
	} while(times < 50);

	return ret;
}

void jpu_wake_up_interrupt(struct jpu_interrupt_t *jpu_interrupt)
{
	wake_up_interruptible_all(&jpu_interrupt->jpu_dec_done_wq);
}

uint32_t os_copy_from_user(void *dst, unsigned long dst_size,
							const void __user *src, unsigned long src_size)
{
	if (src_size > dst_size) {
		return ERROR;
	}
	return copy_from_user(dst, src, src_size);
}