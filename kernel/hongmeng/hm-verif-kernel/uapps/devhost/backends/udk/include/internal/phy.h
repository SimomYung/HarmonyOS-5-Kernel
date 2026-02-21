/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Declare internal interface of phy
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 05 15:24:49 2022
 */

#ifndef __UDK_INTERNAL_PHY_H__
#define __UDK_INTERNAL_PHY_H__

#include <udk/phy.h>
#include <libstrict/strict.h>

#ifdef CONFIG_UDK_PHY

struct udk_phy_desc *udk_phy_desc_of(struct udk_phy *phy);
void phy_lock(struct udk_phy *phy);
void phy_unlock(struct udk_phy *phy);

#else
struct udk_phy_desc *udk_phy_desc_of(struct udk_phy *phy)
{
	UNUSED(phy);
	return NULL;
}

void phy_lock(struct udk_phy *phy)
{
	UNUSED(phy);
}

void phy_unlock(struct udk_phy *phy)
{
	UNUSED(phy);
}

#endif

#endif /* __UDK_INTERNAL_PHY_H__ */
