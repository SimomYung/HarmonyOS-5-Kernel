/**
 * @file allocator_ex.h
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * @brief 内存适配器 allocator_ex 对外头文件。
 * @details \n
 * 本头文件是在各种内存分配算法基础上，借鉴musl库对外提供的内存接口，加上DFX调试功能（DEBUG版本），\n
 * 从方便用户使用的角度出发，抽象提炼的内存适配器接口，相比allocator多提供的扩展接口。
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

/**
 * @defgroup allocator 内存适配器
 * @ingroup allocator_ex
 */

#ifndef ALLOCATOR_EX_H
#define ALLOCATOR_EX_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tag_topn_mem_use_info {
	uint16_t handle;
	uint16_t reserved[3]; // This is reserved variable. 3 is caculated number to reserved.
	size_t use_size;
} mem_handle_use_info;

/**
 * @ingroup allocator_ex
 * @brief 申请一块内存的扩展接口
 * @par 描述：根据用户输入的句柄信息，文件名，行号，以及分配所需的内存空间，并返回一个指向它的指针。
 * @attention \n
 * 1.该接口是release版本接口。
 * 2.该接口申请的内存，得用free_ex配套进行释放，不可用free接口进行释放。
 * 3.handle值请由接口mem_handle_get获取，是一个属于[0,36862]的整数值。
 * @param handle      [IN] 句柄
 * @param filename    [IN] 文件名（预留参数，当前不支持）
 * @param line        [IN] 行号（预留参数，当前不支持）
 * @param size        [IN] 申请内存空间的大小
 * @retval 指向用户内存起始地址的指针 申请成功
 * @retval NULL 申请失败
 * @par 依赖：无。
 * @li allocator_ex.h：该接口声明所在的文件。
 * @see free_ex
 */
void *malloc_ex(uint16_t handle, const char *filename, uint16_t line, size_t size);

/**
 * @ingroup allocator_ex
 * @brief 申请n个长度为size的连续内存
 * @par 描述：根据用户输入的句柄信息，文件名，行号，在内存的动态存储区中分配n个长度为size的连续空间，并将内存初始化为0。
 * @attention \n
 * 1.该接口是release版本接口。
 * 2.该接口申请的内存，得用free_ex配套进行释放，不可用free接口进行释放。
 * 3.handle值请由接口mem_handle_get获取，是一个属于[0,36862]的整数值。
 * @param handle      [IN] 句柄
 * @param filename    [IN] 文件名（预留参数，当前不支持）
 * @param line        [IN] 行号（预留参数，当前不支持）
 * @param n           [IN] 内存空间的个数
 * @param size        [IN] 单个内存空间的大小
 * @retval 指向用户内存起始地址的指针 申请成功
 * @retval NULL 申请失败
 * @par 依赖：无。
 * @li allocator_ex.h：该接口声明所在的文件。
 * @see free_ex
 */
void *calloc_ex(uint16_t handle, const char *filename, uint16_t line, size_t n, size_t size);

/**
 * @ingroup allocator_ex
 * @brief 动态内存调整函数
 * @par 描述：根据用户输入的句柄信息，文件名，行号，对用户指定的内存空间重新调整大小，返回一块拥有原内存内容信息的新内存地址。
 * @attention \n
 * 1.如果期望调整后的内存大小比原内存可用空间大小要小，将申请一块新的内存，拷贝原内存信息到新的内存，并\n
 * 将原内存地址free_ex，返回新内存地址，可能导致数据丢失。\n
 * 2.如果期望调整后的内存大小比原内存可用空间大小要大，则申请一块新的内存，拷贝原内存信息到新的内存，并\n
 * 将原内存地址free_ex，返回新内存地址。\n
 * 3.realloc_ex生成的内存，当内存不再使用时，应使用free_ex函数将内存块释放，不可用free接口进行释放。
 * 4.该接口是release版本接口。
 * 5.handle值请由接口mem_handle_get获取，是一个属于[0,36862]的整数值。
 * @param handle         [IN] 句柄
 * @param filename       [IN] 文件名（预留参数，当前不支持）
 * @param line           [IN] 行号（预留参数，当前不支持）
 * @param old_mem        [IN] 指向需要被调整的内存的指针
 * @param new_size       [IN] 期望调整后新内存的大小
 * @retval 指向用户内存起始地址的指针 调整成功
 * @retval NULL 调整失败
 * @par 依赖：无。
 * @li allocator_ex.h：该接口声明所在的文件。
 * @see malloc_ex calloc_ex free_ex
 */
void *realloc_ex(uint16_t handle, const char *filename, uint16_t line, void *old_mem, size_t new_size);

/**
 * @ingroup allocator_ex
 * @brief 释放内存
 * @par 描述：扩展接口对应的释放内存接口。
 * @attention \n
 * 1.该接口是release版本接口。
 * 2.该接口仅能用于释放由malloc_ex、calloc_ex、realloc_ex申请而来的内存块。
 * 3.handle值请由接口mem_handle_get获取，是一个属于[0,36862]的整数值。
 * @param handle      [IN] 句柄
 * @param mem         [IN] 想要释放的内存指针
 * @retval 无
 * @par 依赖：无。
 * @li allocator_ex.h：该接口声明所在的文件。
 * @see malloc_ex calloc_ex realloc_ex
 */
void free_ex(uint16_t handle, void *mem);

/**
 * @ingroup allocator_ex
 * @brief 获取当前内存地址的内存可用大小
 * @par 描述：获取当前地址mem指向的内存空间的可用大小。
 * @attention \n
 * 1.该接口是release版本接口。
 * 2.该接口仅能用于获取由malloc_ex、calloc_ex、realloc_ex申请而来的内存块的最大可用空间。
 * 3.handle值请由接口mem_handle_get获取，是一个属于[0,36862]的整数值。
 * @param handle      [IN] 句柄
 * @param mem         [IN] 需要获取可用大小的内存块地址
 * @retval 可用大小
 * @retval 0 获取失败
 * @par 依赖：无。
 * @li allocator_ex.h：该接口声明所在的文件。
 * @see malloc_ex calloc_ex realloc_ex
 */
size_t malloc_usable_size_ex(uint16_t handle, void *mem);

/**
 * @ingroup allocator_ex
 * @brief 根据输入的组件名称，获取对应的handle值。
 * @par 描述：根据输入的组件名称，获取对应的handle值。
 * @attention \n
 * 1.该接口是release版本接口。
 * 2.handle值取值范围为[0,36862]。
 * @param component_name   [IN] 组件名称
 * @retval >= 0 && <= 36862 获取成功
 * @retval ((uint16_t)-1) 获取失败
 * @par 依赖：无。
 * @li allocator_ex.h：该接口声明所在的文件。
 */
uint16_t mem_handle_get(const char *component_name);

/**
 * @ingroup allocator_ex
 * @brief 根据输入的handle值，获取对应的组件名称。
 * @par 描述：根据输入的handle值，获取对应的组件名称。
 * @attention \n
 * 1.该接口是release版本接口。
 * 2.handle值取值范围为[0,36862]。
 * @param handle   [IN] handle值
 * @retval 字符串，获取成功
 * @retval NULL 获取失败
 * @par 依赖：无。
 * @li allocator_ex.h：该接口声明所在的文件。
 */
const char *handle_name_get(uint16_t handle);

/**
 * @ingroup allocator_ex
 * @brief 获取当前进程内的内存统计信息
 * @par 描述：获取当前进程内的内存统计信息
 * @attention \n
 * 1.该接口是release版本接口。
 * 2.在按照handle进行统计时，使用未分配handle申请的内存会被统计到非法handle中。
 * @param type      [IN] 获取的信息类型
 * @param handle    [IN] handle值
 * @param count     [IN] 符合查询信息类型的内存总块数
 * @param size      [IN] 符合查询信息类型的内存总大小
 * @retval 0 获取成功
 * @retval 其他 获取失败
 * @par 依赖：无。
 * @li allocator_ex.h：该接口声明所在的文件。
 */
uint32_t meminfo_get(uint16_t type, uint16_t handle, size_t *count, size_t *size);

/**
 * @ingroup allocator_ex
 * @brief 该函数提供日志记录功能。
 * @param level      [IN] 日志等级
 * @param fmt        [IN] 字符串信息
 * @retval 无
 */
typedef void (*malloc_ex_log_func)(int32_t level, const char *fmt, ...);

/**
 * @ingroup allocator_ex
 * @brief malloc_ex日志钩子注册函数
 * @par malloc_ex日志钩子注册函数
 * @attention \n
 * 1.若产品不主动调用该接口，日志打印功能将采用默认方式打印到屏幕上。
 * @param  func       [IN]     日志钩子
 * @retval 无
 * @par 依赖：无。
 * @li allocator_ex.h：该接口声明所在的文件。
 * @see malloc_ex_log_func
 */
void malloc_ex_log_func_reg(malloc_ex_log_func func);

/**
 * @ingroup allocator_ex
 * @brief 根据内存地址获取当前内存的引用计数。
 * @par 描述：根据内存地址获取当前内存的引用计数。
 * @attention \n
 * 1.该接口是release版本接口。
 * 2.该接口只能获取ex函数申请的内存引用计数。
 * @param addr             [IN] 地址
 * @param ref_count        [OUT] 引用计数
 * @retval 0，获取成功
 * @retval -1，获取失败
 * @par 依赖：无。
 * @li allocator_ex.h：该接口声明所在的文件。
 */
uint32_t mem_ref_get_ex(void *addr, uint32_t *ref_count);

/**
 * @ingroup allocator_ex
 * @brief 根据内存地址增加当前内存的引用计数。
 * @par 描述：根据内存地址增加当前内存的引用计数。
 * @attention \n
 * 1.该接口是release版本接口。
 * 2.该接口只能增加ex函数申请的内存引用计数。
 * @param addr             [IN] 地址
 * @retval 0，获取成功
 * @retval -1，获取失败
 * @par 依赖：无。
 * @li allocator_ex.h：该接口声明所在的文件。
 */
uint32_t mem_ref_inc_ex(void *addr);

/**
 * @ingroup allocator_ex
 * @brief 根据handle获取当前handle内存大小。
 * @par 描述：根据handle获取当前handle内存大小。
 * @attention \n
 * 1.该接口是release版本接口。
 * 2.该接口只能获取ex函数申请的内存大小。
 * @param handle      [IN] handle
 * @param size        [OUT] 内存大小
 * @retval 0，获取成功
 * @retval -1，获取失败
 * @par 依赖：无。
 * @li allocator_ex.h：该接口声明所在的文件。
 */
uint32_t mem_user_size_get_ex(uint16_t handle, size_t *size);

/**
 * @ingroup allocator_ex
 * @brief 获取N个最大handle内存大小。
 * @par 描述：获取N个最大handle内存大小。
 * @attention \n
 * 1.该接口是release版本接口。
 * 2.如果handle_num大于实际handle个数,只会获取实际handle个数的信息。
 * @param handle_num         [IN/OUT] 获取的handle个数
 * @param handle_info        [OUT] 获取到的handle信息
 * @retval 0，获取成功
 * @retval -1，获取失败
 * @par 依赖：无。
 * @li allocator_ex.h：该接口声明所在的文件。
 */
uint32_t mem_topn_size_get_ex(uint32_t *handle_num, mem_handle_use_info handle_info[]);

/**
 * @ingroup allocator_ex
 * @brief 分配指定大小和对齐方式的内存空间。
 * @par 描述：分配指定大小和对齐方式的内存空间。
 * @attention \n
 * 1.该接口是release版本接口。
 * 2.该接口申请的内存，得用free_ex配套进行释放，不可用free接口进行释放。
 * 3.handle值请由接口mem_handle_get获取，是一个属于[0,36862]的整数值。
 * @param handle      [IN] handle
 * @param align       [IN] 对齐大小
 * @param size        [IN] 申请大小
 * @retval 地址，获取成功
 * @retval NULL，获取失败
 * @par 依赖：无。
 * @li allocator_ex.h：该接口声明所在的文件。
 */
void *memalign_ex(uint16_t handle, size_t align, size_t size);

/**
 * @ingroup allocator_ex
 * @brief 根据内存地址获取handle。
 * @par 描述：根据内存地址获取handle。
 * @attention \n
 * 1.该接口是release版本接口。
 * @param mem      [IN] handle
 * @retval 有效handle，获取成功
 * @retval 无效handle，获取失败
 * @par 依赖：无。
 * @li allocator_ex.h：该接口声明所在的文件。
 */
uint16_t mem_handle_get_by_addr(void *mem);

#ifdef __cplusplus
}
#endif

#endif
