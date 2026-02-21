/**
 * @file mem_hm_memattr.c
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * @brief mem_hm_memattr src file
 * @details mem_hm_memattr src file
 * @author Anonymous
 * @date 2022-11-07
 * @version v0.0.1
 * *******************************************************************************************
 * @par Modify log:
 * <table>
 * <tr><th>Date		<th>Version  <th>Author	  <th>Description
 * <tr><td>2022-11-07  <td>0.0.1	<td>Anonymous   <td>Initial Version
 * </table>
 * *******************************************************************************************
 */
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <sched.h>
#include "syscall.h"
#include "mem_hm_memattr.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

#define SYSCALL_TRY_MAX_TIMES 10
#define BUF_MAX_SIZE 1024

typedef struct TagMemHMMemAttrChkInfo {
    int32_t memVaildPipe[2];
    int32_t pipeStartInit;
} MemHMMemAttrChkInfo;

static MemHMMemAttrChkInfo g_memHMMemAttrChkInfo;

void __attribute__((constructor)) MemHMDamageCheckInit(void)
{
    g_memHMMemAttrChkInfo.memVaildPipe[0] = -1;
    g_memHMMemAttrChkInfo.memVaildPipe[1] = -1;

    g_memHMMemAttrChkInfo.pipeStartInit = 0;
}

static int32_t MemHMPipeOpen(void)
{
    int32_t ret;

    /* 此处需要close，保证可重入 */
    (void)close(g_memHMMemAttrChkInfo.memVaildPipe[0]);
    (void)close(g_memHMMemAttrChkInfo.memVaildPipe[1]);

    ret = pipe(g_memHMMemAttrChkInfo.memVaildPipe);
    if (ret < 0) {
        return ret;
    }

    (void)fcntl(g_memHMMemAttrChkInfo.memVaildPipe[0], F_SETFL, O_NONBLOCK);
    (void)fcntl(g_memHMMemAttrChkInfo.memVaildPipe[1], F_SETFL, O_NONBLOCK);

    return 0;
}

/**
 * @ingroup mem_hm_damagecheck
 * @brief 判断内存地址范围内地址的读权限
 * @par 描述：判断内存地址范围内地址的读权限。
 * @attention
 * @param chkAddr   [IN] 检测起始地址
 * @param len       [IN] 检测的长度范围
 * @retval true 可读
 * @retval false 不可读
 * @par 依赖：无。
 */
bool MemHMMemAttrRdChk(uintptr_t chkAddr, int32_t len)
{
    int32_t ret;
    int32_t syscallTryCount = 0;
    long syscallRet;
    ssize_t bytes;
    uint8_t buf[BUF_MAX_SIZE];

    /* 首次调用要初始管道 */
    if (g_memHMMemAttrChkInfo.pipeStartInit == 0) {
        ret = MemHMPipeOpen();
        if (ret < 0) {
            return false;
        }
        g_memHMMemAttrChkInfo.pipeStartInit = 1;
    }

    /* 尝试将内存地址范围写管道，如若可写则证明此内存范围内具有可读权限 */
    do {
        syscallRet = syscall(SYS_write, g_memHMMemAttrChkInfo.memVaildPipe[1], chkAddr, len);
        syscallTryCount++;
    } while (errno == EINTR && syscallTryCount < SYSCALL_TRY_MAX_TIMES);

    if (syscallRet < 0) {
        return false;
    }

    /* 清理管道内信息 */
    bytes = read(g_memHMMemAttrChkInfo.memVaildPipe[0], buf, len);
    if (bytes < 0) {
        ret = MemHMPipeOpen();
        if (ret < 0) {
            g_memHMMemAttrChkInfo.pipeStartInit = 0;
            return false;
        }
    }

    return true;
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */