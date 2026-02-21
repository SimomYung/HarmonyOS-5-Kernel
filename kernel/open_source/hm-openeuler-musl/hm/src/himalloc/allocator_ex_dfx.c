/**
 * @file allocator_ex.c
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * @brief 内存适配器 allocator 源代码文件。
 * @details
 * @author anonymous
 * @date 2021-03-10
 * @version v1.0.0
 * *******************************************************************************************
 * @par 修改日志：
 * <table>
 * <tr><th>Date        <th>Version  <th>Author    <th>Description
 * <tr><td>2021-03-10  <td>1.0.0    <td>anonymous <td>创建初始版本
 * </table>
 *
 * *******************************************************************************************
 */
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "allocator_inner.h"
#include "himalloc.h"
#include "allocator.h"
#include "allocator_ex.h"
#include "allocator_ex_inner.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

#define HANDLE_ARRAY_SIZE                       7
#define HANDLE_NAME_LEN_MAX                     32

#define MEMINFO_HELP                            0    /* 0: 显示使用帮助信息 */
#define MEMINFO_TOTAL_GET                       1    /* 1: 获取进程级别的内存使用个数和使用总大小 */
#define MEMINFO_POSIX_PART_GET                  2    /* 2: 获取通过malloc等posix接口申请的内存总个数和总大小 */
#define MEMINFO_HANDLE_PART_GET                 3    /* 3: 获取通过malloc_ex等handle接口申请的内存总个数和总大小 */
#define MEMINFO_HANDLE_PART_SHOW                4    /* 4: 显示通过malloc_ex等handle接口申请的内存的详细信息 */
#define MEMINFO_HANDLE_PART_DETAIL_GET          5    /* 5: 根据指定handle值获取对应handle的内存申请个数和大小 */

/**
 * @ingroup allocator_ex_dfx
 * @enum log_level_e
 * @brief 日志等级枚举
 */
typedef enum {
    LEVEL_INFO = 0,
    LEVEL_WARNING = 1,
    LEVEL_ERROR = 2,
    LEVEL_EXCEPT = 3,
    LEVEL_BUTT
} log_level_e;

typedef struct {
    size_t count;
    size_t size;
} stats_info_s;

typedef struct {
    stats_info_s stats;
    uint16_t handle;
} handle_detail_info_s;

typedef struct {
    size_t total_count;
    size_t total_size;
    stats_info_s details[0];
} handle_stats_info_s;

typedef struct handle_box {
    struct handle_box *next;
    char name_array[HANDLE_ARRAY_SIZE][HANDLE_NAME_LEN_MAX];
} handle_box_s;

typedef struct {
	handle_box_s *first;
	handle_box_s *last;
	uint16_t count;
} handle_box_ctl_s;

static void meminfo_echo(int32_t level, const char *pscFmt, ...)
{
    ssize_t write_ret;
    (void)level;

    char buf[256]; /* 256 代表字符缓存池长度 */
    va_list ap;
    int ret;

    va_start(ap, pscFmt);
    ret = vsnprintf(buf, 256, pscFmt, ap); /* 256 代表字符缓存池长度 */
    va_end(ap);
    if (ret > 0) {
        do {
            write_ret = write(STDOUT_FILENO, buf, (size_t)ret);
        } while ((write_ret == -1) && (errno == EINTR));
    } else {
        do {
            write_ret = write(STDOUT_FILENO, "vsnprintf failed\n", 17); /* 17 代表失败信息字符长度 */
        } while ((write_ret == -1) && (errno == EINTR));
    }
}

static handle_box_ctl_s g_handle_box = {NULL, NULL, 0};
static malloc_ex_log_func g_malloc_ex_log_func = meminfo_echo;
static pthread_mutex_t g_handle_lock = PTHREAD_MUTEX_INITIALIZER;
static mem_handle_use_info *g_handle_size_array = NULL;

/**
 * @ingroup allocator_ex_dfx
 * @brief 日志输出函数宏
 * @see g_malloc_ex_log_func
 */
#define log_write(level, args...)                   \
    do {                                            \
        if (g_malloc_ex_log_func != NULL) {         \
            g_malloc_ex_log_func(level, ##args);    \
        }                                           \
    } while (0)


static uint16_t find_handle_by_name(const char *component_name, handle_box_ctl_s *hbox)
{
    if (hbox->count == 0) {
        return (uint16_t)ALLOCATOR_ERR;
    }

    handle_box_s *cur_box = hbox->first;
    uint16_t box_index = (uint16_t)(hbox->count / HANDLE_ARRAY_SIZE);
    uint16_t array_index = (uint16_t)(hbox->count % HANDLE_ARRAY_SIZE);

    for (uint16_t i = 0; i < box_index; i++) {
        for (uint16_t index = 0; index < HANDLE_ARRAY_SIZE; index++) {
            if (strcmp(component_name, cur_box->name_array[index]) == 0) {
                return ((i * HANDLE_ARRAY_SIZE) + index);
            }
        }
        cur_box = cur_box->next;
    }

    for (uint16_t index = 0; index < array_index; index++) {
        if (strcmp(component_name, cur_box->name_array[index]) == 0) {
            return ((box_index * HANDLE_ARRAY_SIZE) + index);
        }
    }

    return (uint16_t)ALLOCATOR_ERR;
}

uint16_t mem_handle_get(const char *component_name)
{
    if ((component_name == NULL) || (strlen(component_name) >= HANDLE_NAME_LEN_MAX)) {
        return (uint16_t)ALLOCATOR_ERR;
    }

    (void)pthread_mutex_lock(&g_handle_lock);
    if (g_handle_box.count >= HANDLE_MAX) {
        (void)pthread_mutex_unlock(&g_handle_lock);
        return (uint16_t)ALLOCATOR_ERR;
    }

    uint16_t handle = find_handle_by_name(component_name, &g_handle_box);
    if (handle != (uint16_t)ALLOCATOR_ERR) {
        (void)pthread_mutex_unlock(&g_handle_lock);
        return handle;
    }

    uint32_t array_index = g_handle_box.count % HANDLE_ARRAY_SIZE;
    if (g_handle_box.count == 0) {
        g_handle_box.first = (handle_box_s *)ALLOCATOR_ALLOC_INNER(sizeof(handle_box_s));
        if (g_handle_box.first == NULL) {
            (void)pthread_mutex_unlock(&g_handle_lock);
            return (uint16_t)ALLOCATOR_ERR;
        }
        g_handle_box.last = g_handle_box.first;
        g_handle_box.last->next = NULL;
    } else {
        if (array_index == 0) {
            handle_box_s *new_box = (handle_box_s *)ALLOCATOR_ALLOC_INNER(sizeof(handle_box_s));
            if (new_box == NULL) {
                (void)pthread_mutex_unlock(&g_handle_lock);
                return (uint16_t)ALLOCATOR_ERR;
            }
            g_handle_box.last->next = new_box;
            g_handle_box.last = new_box;
            g_handle_box.last->next = NULL;
        }
    }

    (void)strcpy(g_handle_box.last->name_array[array_index], component_name);
    handle = g_handle_box.count++;
	(void)pthread_mutex_unlock(&g_handle_lock);

    return handle;
}

const char *handle_name_get(uint16_t handle)
{
    (void)pthread_mutex_lock(&g_handle_lock);
    if ((handle >= g_handle_box.count) || (g_handle_box.first == NULL)) {
        (void)pthread_mutex_unlock(&g_handle_lock);
        return NULL;
    }

    uint32_t box_index = handle / HANDLE_ARRAY_SIZE;
    uint32_t array_index = handle % HANDLE_ARRAY_SIZE;
    handle_box_s *cur_box = g_handle_box.first;
    while ((box_index > 0) && (cur_box != NULL)) {
        cur_box = cur_box->next;
        box_index--;
    }

    if (cur_box == NULL) {
        (void)pthread_mutex_unlock(&g_handle_lock);
        return NULL;
    }
    const char *name = cur_box->name_array[array_index];
    (void)pthread_mutex_unlock(&g_handle_lock);

    return name;
}

void malloc_ex_log_func_reg(malloc_ex_log_func func)
{
    g_malloc_ex_log_func = func;
}

static void meminfo_help(void)
{
    log_write(LEVEL_INFO, "\n");
    log_write(LEVEL_INFO, "meminfo use help:\n");
    log_write(LEVEL_INFO, "  (0,      *,     *,    *) show the help info.\n");
    log_write(LEVEL_INFO,
        "  (1,      *, count, size) obtains the number and total size of memory in current process.\n");
    log_write(LEVEL_INFO,
        "  (2,      *, count, size) obtains the number and total size of memory by malloc posix funcs.\n");
    log_write(LEVEL_INFO,
        "  (3,      *, count, size) obtains the number and total size of memory by malloc_ex handle funcs.\n");
    log_write(LEVEL_INFO, "  (4,      *,     *,    *) show the detail info of memory by malloc_ex handle funcs.\n");
    log_write(LEVEL_INFO, "  (5, handle, count, size) obtains the number and size of memory in specified handle.\n");
}

static int32_t all_mem_walk_func(void *addr, size_t size, void *para)
{
    stats_info_s *stats = para;

    if (size == 0) {
        log_write(LEVEL_ERROR, "The malloc_usable_size interface returns 0.\n");
        return ALLOCATOR_ERR;
    }

    stats->size += size;
    stats->count++;

    return ALLOCATOR_OK;
}

static int32_t all_handle_mem_walk_func(void *addr, size_t size, void *para)
{
    stats_info_s *stats = para;

    if ((*(uint32_t *)addr & ALLOCATOR_DFX_HEAD_MASK) == ALLOCATOR_RELEASE_DFX_HEAD_MAGIC) {
        if (size == 0) {
            log_write(LEVEL_ERROR, "The malloc_usable_size interface returns 0.\n");
            return ALLOCATOR_ERR;
        }

        stats->size += size;
        stats->count++;
    }

    return ALLOCATOR_OK;
}

static int32_t all_posix_mem_walk_func(void *addr, size_t size, void *para)
{
    stats_info_s *stats = para;

    if ((*(uint32_t *)addr & ALLOCATOR_DFX_HEAD_MASK) != ALLOCATOR_RELEASE_DFX_HEAD_MAGIC) {
        if (size == 0) {
            log_write(LEVEL_ERROR, "The malloc_usable_size interface returns 0.\n");
            return ALLOCATOR_ERR;
        }

        stats->size += size;
        stats->count++;
    }

    return ALLOCATOR_OK;
}

static uint32_t meminfo_total_get(size_t *count, size_t *size)
{
    if ((count == NULL) || (size == NULL)) {
        log_write(LEVEL_ERROR, "count or size is NULL.\n");
        return (uint32_t)ALLOCATOR_ERR;
    }

    int32_t ret;
    stats_info_s stats;

    stats.count = 0;
    stats.size = 0;

    ret = ALLOCATOR_ALL_MEM_TRAVERSE((void *)&stats, all_mem_walk_func, false);
    if (ret != ALLOCATOR_OK) {
        log_write(LEVEL_ERROR, "Failed to run the memory traversal query interface.\n");
        return (uint32_t)ALLOCATOR_ERR;
    }

    *count = stats.count;
    *size = stats.size;

    return ALLOCATOR_OK;
}

static uint32_t meminfo_handle_part_get(size_t *count, size_t *size)
{
    if ((count == NULL) || (size == NULL)) {
        log_write(LEVEL_ERROR, "count or size is NULL.\n");
        return (uint32_t)ALLOCATOR_ERR;
    }

    int32_t ret;
    stats_info_s stats;

    stats.count = 0;
    stats.size = 0;

    ret = ALLOCATOR_ALL_MEM_TRAVERSE((void *)&stats, all_handle_mem_walk_func, false);
    if (ret != ALLOCATOR_OK) {
        log_write(LEVEL_ERROR, "Failed to run the memory traversal query interface.\n");
        return (uint32_t)ALLOCATOR_ERR;
    }

    *count = stats.count;
    *size = stats.size;

    return ALLOCATOR_OK;
}

static uint32_t meminfo_posix_part_get(size_t *count, size_t *size)
{
    if ((count == NULL) || (size == NULL)) {
        log_write(LEVEL_ERROR, "count or size is NULL.\n");
        return (uint32_t)ALLOCATOR_ERR;
    }

    int32_t ret;
    stats_info_s stats;

    stats.count = 0;
    stats.size = 0;

    ret = ALLOCATOR_ALL_MEM_TRAVERSE((void *)&stats, all_posix_mem_walk_func, false);
    if (ret != ALLOCATOR_OK) {
        log_write(LEVEL_ERROR, "Failed to run the memory traversal query interface.\n");
        return (uint32_t)ALLOCATOR_ERR;
    }

    *count = stats.count;
    *size = stats.size;

    return ALLOCATOR_OK;
}

static int32_t all_handle_mem_detail_walk_func(void *addr, size_t size, void *para)
{
    handle_stats_info_s *handle_stats_info = para;

    if ((*(uint32_t *)addr & ALLOCATOR_DFX_HEAD_MASK) == ALLOCATOR_RELEASE_DFX_HEAD_MAGIC) {
        if (size == 0) {
            log_write(LEVEL_ERROR, "The malloc_usable_size interface returns 0.\n");
            return ALLOCATOR_ERR;
        }

        handle_stats_info->total_count++;
        handle_stats_info->total_size += size;
        uint16_t handle = (uint16_t)((*(uint32_t *)addr & ALLOCATOR_RELEASE_DFX_GET_HANDLE_MASK) >> SHIFT_BIT);
        (void)pthread_mutex_lock(&g_handle_lock);
        uint16_t count = g_handle_box.count;
        (void)pthread_mutex_unlock(&g_handle_lock);
        if (handle >= count && handle < HANDLE_MAX) { /* means invalid handle. */
            handle_stats_info->details[count].count++;
            handle_stats_info->details[count].size += size;
        } else {
            handle_stats_info->details[handle].count++;
            handle_stats_info->details[handle].size += size;
        }
    }

    return 0;
}

static int32_t one_handle_mem_detail_walk_func(void *addr, size_t size, void *para)
{
    handle_detail_info_s *handle_detail_info = para;
    if (((*(uint32_t *)addr & ALLOCATOR_DFX_HEAD_MASK) == ALLOCATOR_RELEASE_DFX_HEAD_MAGIC) &&
        (((*(uint32_t *)addr & ALLOCATOR_RELEASE_DFX_GET_HANDLE_MASK) >> SHIFT_BIT) == handle_detail_info->handle)) {
        if (size == 0) {
            log_write(LEVEL_ERROR, "The malloc_usable_size interface returns 0.\n");
            return ALLOCATOR_ERR;
        }
        handle_detail_info->stats.count++;
        handle_detail_info->stats.size += size;
    }

    return 0;
}

#define HIM_STATS_SHOW_HEAD	 "\n                  Table: memory alloced by handle stats info\n"
#define HIM_STATS_SHOW_SPLIT "+--------------------------------+-----------+-------------+-------------------+\n"
#define HIM_STATS_SHOW_MENU  "|          handle name           | handle id | alloc times | total size (Byte) |\n"

#define him_stats_show_prefix(handle_stats_info) do { \
    log_write(LEVEL_INFO, HIM_STATS_SHOW_HEAD); \
    log_write(LEVEL_INFO, HIM_STATS_SHOW_SPLIT); \
    log_write(LEVEL_INFO, "|%-32s|%-11u|%-13lu|%-19lu|\n", \
        "total count", (handle_stats_info)->total_count, "total size", (handle_stats_info)->total_size); \
    log_write(LEVEL_INFO, HIM_STATS_SHOW_SPLIT); \
    log_write(LEVEL_INFO, HIM_STATS_SHOW_MENU); \
    log_write(LEVEL_INFO, HIM_STATS_SHOW_SPLIT); \
} while (0)

static void meminfo_handle_part_show(void)
{
    uint16_t count = g_handle_box.count;
    /* + 1 表示为非法内存预留内存 */
    size_t info_size = (count + 1) * sizeof(stats_info_s) + sizeof(handle_stats_info_s);
    handle_stats_info_s *handle_stats_info = (handle_stats_info_s *)ALLOCATOR_ALLOC_INNER(info_size);
    if (handle_stats_info == NULL) {
        log_write(LEVEL_ERROR, "Failed to apply for internal memory.\n");
        return;
    }

    memset(handle_stats_info, 0, info_size);
    int32_t ret = ALLOCATOR_ALL_MEM_TRAVERSE((void *)handle_stats_info, all_handle_mem_detail_walk_func, false);
    if (ret != ALLOCATOR_OK) {
        ALLOCATOR_FREE_INNER(handle_stats_info);
        log_write(LEVEL_ERROR, "Failed to run the memory traversal query interface.\n");
        return;
    }

    stats_info_s *sinfo = handle_stats_info->details;
    him_stats_show_prefix(handle_stats_info);
    for (uint16_t index = 0; index < count; index++) {
        log_write(LEVEL_INFO, "|%-32s|%-11u|%-13lu|%-19lu|\n",
            handle_name_get(index), (index), sinfo[index].count, sinfo[index].size);
        log_write(LEVEL_INFO, HIM_STATS_SHOW_SPLIT);
    }
    if (sinfo[count].count != 0) {
        /* memory alloced by invalid handle is existed. */
        log_write(LEVEL_INFO, "|%-32s|%-11x|%-13lu|%-19lu|\n",
            "invalid handle", HANDLE_MAX, sinfo[count].count, sinfo[count].size);
        log_write(LEVEL_INFO, HIM_STATS_SHOW_SPLIT);
    }

    ALLOCATOR_FREE_INNER(handle_stats_info);
}

static uint32_t meminfo_handle_part_detail_get(uint16_t handle, size_t *count, size_t *size)
{
    if ((count == NULL) || (size == NULL)) {
        log_write(LEVEL_ERROR, "count or size is NULL.\n");
        return (uint32_t)ALLOCATOR_ERR;
    }

    handle_detail_info_s detail;

    detail.stats.count = 0;
    detail.stats.size = 0;
    detail.handle = handle;

    int32_t ret = ALLOCATOR_ALL_MEM_TRAVERSE((void *)&detail, one_handle_mem_detail_walk_func, false);
    if (ret != ALLOCATOR_OK) {
        log_write(LEVEL_ERROR, "Failed to run the memory traversal query interface.\n");
        return (uint32_t)ALLOCATOR_ERR;
    }

    *count = detail.stats.count;
    *size = detail.stats.size;

    return ALLOCATOR_OK;
}

uint32_t meminfo_get(uint16_t type, uint16_t handle, size_t *count, size_t *size)
{
    uint32_t ret = ALLOCATOR_OK;

    switch (type) {
        case MEMINFO_HELP:
            meminfo_help();
            break;
        case MEMINFO_TOTAL_GET:
            ret = meminfo_total_get(count, size);
            break;
        case MEMINFO_POSIX_PART_GET:
            ret = meminfo_posix_part_get(count, size);
            break;
        case MEMINFO_HANDLE_PART_GET:
            ret = meminfo_handle_part_get(count, size);
            break;
        case MEMINFO_HANDLE_PART_SHOW:
            meminfo_handle_part_show();
            break;
        case MEMINFO_HANDLE_PART_DETAIL_GET:
            ret = meminfo_handle_part_detail_get(handle, count, size);
            break;
        default:
            ret = (uint32_t)ALLOCATOR_ERR;
            break;
    }

    return ret;
}

HIDDEN mem_handle_use_info *get_handle_size_ex(void)
{
	return g_handle_size_array;
}

static int32_t all_handle_size_mem_walk_func(void *addr, size_t size, void *para)
{
	mem_handle_use_info *handle_size_array = (mem_handle_use_info *)para;
	uint16_t handle;

	if (((*(uint32_t *)addr & ALLOCATOR_DFX_HEAD_MASK) == ALLOCATOR_RELEASE_DFX_HEAD_MAGIC) &&
		(*((uint32_t *)addr + 1) != 0)) {
		if (size == 0) {
			return ALLOCATOR_ERR;
		}
		handle = (uint16_t)((*(uint32_t *)addr & ALLOCATOR_RELEASE_DFX_GET_HANDLE_MASK) >> SHIFT_BIT);
		handle_size_array[handle].use_size += size;
	}

	return ALLOCATOR_OK;
}

HIDDEN uint32_t update_handle_size_ex(void)
{
	(void)pthread_mutex_lock(&g_handle_lock);

	mem_handle_use_info *handle_size_array = g_handle_size_array;
	uint32_t count = g_handle_box.count;
	if (handle_size_array != NULL) {
		free(handle_size_array);
	}

	handle_size_array = (mem_handle_use_info *)malloc(count * sizeof(mem_handle_use_info));
	if (handle_size_array == NULL) {
		(void)pthread_mutex_unlock(&g_handle_lock);
		return (uint32_t)ALLOCATOR_ERR;
	}

	(void)memset(handle_size_array, 0, count * sizeof(mem_handle_use_info));
	for (uint32_t i = 0; i < count; i++) {
		handle_size_array[i].handle = (uint16_t)i;
	}

	int32_t ret = ALLOCATOR_ALL_MEM_TRAVERSE((void *)handle_size_array, all_handle_size_mem_walk_func, true);
	if (ret != ALLOCATOR_OK) {
		(void)pthread_mutex_unlock(&g_handle_lock);
		return (uint32_t)ALLOCATOR_ERR;
	}

	g_handle_size_array = handle_size_array;
	(void)pthread_mutex_unlock(&g_handle_lock);
	return (uint32_t)ALLOCATOR_OK;
}

HIDDEN uint32_t max_handle_num_get_ex(void)
{
	return g_handle_box.count;
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */
