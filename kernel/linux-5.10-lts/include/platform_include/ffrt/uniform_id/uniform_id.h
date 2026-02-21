#ifndef _UNIFORM_ID_H_
#define _UNIFORM_ID_H_

#include <linux/types.h>

#define FFRT_UNIFORM_ID_START 64
#define FFRT_UNIFORM_ID_NUM_MAX 960
#define FFRT_UNIFORM_ID_END (FFRT_UNIFORM_ID_START + FFRT_UNIFORM_ID_NUM_MAX - 1)
#define FFRT_INVALID_UNIFORM_ID 0xFFFF

uint16_t ffrt_get_uniform_id(pid_t pid);
int ffrt_put_uniform_id(uint16_t uid);
pid_t ffrt_get_pid_by_uid(uint16_t uid);

#endif