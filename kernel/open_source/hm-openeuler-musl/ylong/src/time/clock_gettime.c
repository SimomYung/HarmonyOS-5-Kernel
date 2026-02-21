#include <time.h>
#include <errno.h>
#include <stdint.h>
#include "stdbool.h"
#include "syscall.h"
#include "atomic.h"
#include "lock.h"

#define NS_PER_SECOND 1000000000
#define likely(x)	__builtin_expect(!!(x), 1)
#define unlikely(x)	__builtin_expect(!!(x), 0)

struct timeinfo {
    uintptr_t physAddr;
    uint32_t sysClock;
    uint32_t seq;
    struct timespec accDeltaFromSet; /* accumulative time delta from discontinuous modify, such as settimeofday */
};
static struct timeinfo *g_timeInfo = NULL;
static volatile int g_lock = 0;

void *SysGetTimeInfo()
{
    return (void *)__syscall(__NR_gettimeinfo);
}

static inline uint64_t ClockGetCycles()
{
    uint64_t res = 0;

    /*
     * This isb is required to prevent that the value is speculated.
     */
    __asm__ __volatile__("isb" : : : "memory");
    __asm__ __volatile__("mrs %0, cntpct_el0" : "=r"(res)::"memory");

    return res;
}

static inline void GetHwTime(struct timespec *hwTime, uint32_t sysClock)
{
    uint64_t cycles;
    cycles = ClockGetCycles();

    hwTime->tv_sec = cycles / sysClock;
    hwTime->tv_nsec = (cycles % sysClock) * NS_PER_SECOND / sysClock;
}

static inline __attribute__((always_inline)) struct timespec TimeSpecAdd(const struct timespec t1,
    const struct timespec t2)
{
    struct timespec ret;

    ret.tv_sec = t1.tv_sec + t2.tv_sec;
    ret.tv_nsec = t1.tv_nsec + t2.tv_nsec;
    if (ret.tv_nsec >= NS_PER_SECOND) {
        ret.tv_sec += 1;
        ret.tv_nsec -= NS_PER_SECOND;
    } else if (ret.tv_nsec < 0L) {
        ret.tv_sec -= 1;
        ret.tv_nsec += NS_PER_SECOND;
    }

    return ret;
}

#define WRITE_ONCE(var, val) (*((volatile __typeof__(val) *)(&(var))) = (val))
#define READ_ONCE(var) (*((volatile __typeof__(var) *)(&(var))))

static inline uint32_t TimeinfoReadBegin(struct timeinfo *timeinfo)
{
    uint32_t seq;
    while (unlikely((seq = READ_ONCE(timeinfo->seq)) & 1)) {
        __asm__ __volatile__("yield" ::: "memory");
    }
    return seq;
}

static inline uint32_t TimeinfoReadRetry(struct timeinfo *timeinfo, uint32_t start)
{
    uint32_t seq = READ_ONCE(timeinfo->seq);
    return seq != start;
}

static inline struct timespec GetAccDeltaTime(struct timeinfo *timeinfo)
{
    struct timespec deltaTime;
    uint32_t seq;
    do {
        seq = READ_ONCE(timeinfo->seq);
        deltaTime = timeinfo->accDeltaFromSet;
    } while (unlikely(TimeinfoReadRetry(timeinfo, seq)));

    return deltaTime;
}

static void __attribute__((constructor)) TimeInfoInit(void)
{
    g_timeInfo = SysGetTimeInfo();
}

int HpeClockGettime(clockid_t clockID, struct timespec *tp)
{
    struct timespec tmp;
    struct timespec hwTime;

    if (tp == NULL) {
        goto ERROUT;
    }

    if (unlikely(g_timeInfo == NULL)) {
        LOCK(&g_lock);
        if (g_timeInfo == NULL) {
            g_timeInfo = SysGetTimeInfo();
            if (g_timeInfo == NULL) {
                UNLOCK(&g_lock);
                goto ERROUT;
            }
        }
        UNLOCK(&g_lock);
    }

    GetHwTime(&hwTime, g_timeInfo->sysClock);

    switch (clockID) {
        case CLOCK_MONOTONIC_RAW:
        case CLOCK_MONOTONIC:
        case CLOCK_MONOTONIC_COARSE:
            tp->tv_sec = hwTime.tv_sec;
            tp->tv_nsec = hwTime.tv_nsec;
            break;
        case CLOCK_REALTIME:
        case CLOCK_REALTIME_COARSE:
            tmp = TimeSpecAdd(hwTime, GetAccDeltaTime(g_timeInfo));
            tp->tv_sec = tmp.tv_sec;
            tp->tv_nsec = tmp.tv_nsec;
            break;

        case CLOCK_THREAD_CPUTIME_ID:
        case CLOCK_PROCESS_CPUTIME_ID:
        case CLOCK_BOOTTIME:
        case CLOCK_REALTIME_ALARM:
        case CLOCK_BOOTTIME_ALARM:
        case CLOCK_TAI:
            return -ENOTSUP;
        default:
            return -EINVAL;
    }

    return 0;
ERROUT:
    return -EINVAL;
}

int __clock_gettime(clockid_t clk, struct timespec *ts)
{
    int r;
    r = HpeClockGettime(clk, ts);
    return __syscall_ret(r);
}

weak_alias(__clock_gettime, clock_gettime);
