/*
* Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
* Description: Internal header for libdh vmap
* Author: Huawei OS Kernel Lab
* Create: Mon Nov 16 23:07:41 2020
*/
#ifndef ULIBS_LIBDEVHOST_VMAP_INTERNAL_H
#define ULIBS_LIBDEVHOST_VMAP_INTERNAL_H

struct libdh_vmap_node;
/*
 * remove node from vmap tree
 *
 * @param    vaddr  [I] base virtual address which is the key of vmap node
 *
 * @return   the node be removed
 * @note     caller should ensure addr has been page aligned
 */
struct libdh_vmap_node *__libdh_vmap_remove(const void *vaddr);

#endif /* ULIBS_DEVHOST_VMAP_INTERNAL_H */
