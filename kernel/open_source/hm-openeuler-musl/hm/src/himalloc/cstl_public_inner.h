/**
 * @file cstl_public_inner.h
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * @brief cstl_public_cstl公共模块定义
 * @details cstl公共定义实现源码
 * @author anonymous
 * @date 2021-05-14
 * @version v0.1.0
 * *******************************************************************************************
 * @par 修改日志：
 * <table>
 * <tr><th>Date        <th>Version  <th>Author    <th>Description
 * <tr><td>2021-05-14  <td>0.1.0    <td>anonymous <td>创建初始版本
 * </table>
 * *******************************************************************************************
 */

#ifndef CSTL_PUBLIC_INNER_H
#define CSTL_PUBLIC_INNER_H

#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CSTL_DEBUG
    #define CSTL_STATIC
    #ifndef CSTL_PRINTF
        #define CSTL_PRINTF printf
    #endif
#else
    #define CSTL_STATIC static
    #ifndef CSTL_PRINTF
        #define CSTL_PRINTF(format, ...)
    #endif
#endif

/* 0xC 内存不足。 */
#define CSTL_ERRNO_NOMEM (-12)

/* 0x16 入参无效。 */
#define CSTL_ERRNO_INVAL (-22)

typedef struct TagCstlUserData {
    uintptr_t inputData;   /* 用户实际输入的数据 */
    size_t dataSize;       /* 实际输入的大小 */
} CstlUserData;

/* 判断两数相乘在当前系统下会否溢出 */
bool IsMultiOverflow(size_t x, size_t y);

/* 判断两数相加在当前系统下会否溢出 */
bool IsAddOverflow(size_t x, size_t y);

/**
 * @details uintptr_t是cstl模块通用的存储数据类型，该入参主要是两个功能：
 * 功能一:存储用户数据的地址，那么对这种情况，用户需要自己编写比较函数，选择自己需要的方式
 * 功能二:用户直接存储整数，且范围在当前机器位数下intptr_t能够表示的范围中，那么此时用户可以
 * 使用此默认比较函数，它可以满足该范围内的数据的比较需求，且比较结果从小到大升序，如果用户需要
 * 降序或者其它排序方式，也需要自定义比较函数，cstl不提供默认降序比较函数
 * 功能三：如果用户需要存储的数据较大，必须用无符号长整型才能存储，那么该函数会将无符号整数
 * 转化为有符号整数进行比较，此时排序结果不是预期的，建议用户自定义比较函数来解决这种情况的数据比较，
 * 即对于大数A = uintptr_t(-1) 和 大数 B = 1ULL<<50，目前的函数会认为A < B，实际上A是大于B的。由于C语言
 * 无模板功能不能处理所有情况，建议用户在阅读该说明之后，根据自己的需求定义合理的比较函数。
 */
int32_t CstlIntCmpFunc(uintptr_t data1, uintptr_t data2);

#ifdef __cplusplus
}
#endif

#endif  /* CSTL_PUBLIC_INNER_H */

