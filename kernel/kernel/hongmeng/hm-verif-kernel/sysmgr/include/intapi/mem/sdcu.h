/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: sdcu for swap
 */

#ifndef __SDCU_H__
#define __SDCU_H__

struct sdcu_ops {
    int (*sync_compress)(uintptr_t src, size_t slen, uintptr_t dst, size_t dlen);
    int (*sync_decompress)(uintptr_t src, size_t slen, uintptr_t dst, size_t dlen);
    /* move ops and async ops not needed currently */
};

struct sdcu_ops *get_sdcu_ops(void);
void register_sdcu_ops(struct sdcu_ops *ops);
#endif