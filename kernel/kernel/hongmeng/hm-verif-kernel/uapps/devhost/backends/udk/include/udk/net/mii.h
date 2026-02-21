/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Medium Independent Interface(MII) standard register definition
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 05 20:15:52 2022
 */
#ifndef __UDK_NET_MII_H__
#define __UDK_NET_MII_H__

/* MII registers */
#define MII_BMCR		0x00
#define MII_BMSR		0x01
#define MII_PHYSID1		0x02
#define MII_PHYSID2		0x03
#define MII_ADVERTISE		0x04
#define MII_LPA			0x05
#define MII_CTRL1000		0x09
#define MII_STAT1000		0x0a
#define MII_ESTATUS		0x0f

/* Basic mode control register */
#define BMCR_FULLDPLX           0x0100  /* Full duplex			*/
#define BMCR_MSB		0x0040  /* Low bit for speed selection	*/
#define BMCR_ANRESTART		0x0200  /* Auto negotiation restart	*/
#define BMCR_ISOLATE		0x0400  /* Isolate data paths from MII	*/
#define BMCR_ANENABLE		0x1000  /* Enable auto negotiation	*/
#define BMCR_LSB		0x2000  /* High bit for speed selection */
#define BMCP_POWER_DOWN		0x0800  /* Power down			*/

#define BMCR_SPEED_MASK		0x2040  /* bit 6 and bit 13 for speed selection	*/
#define BMCR_SPEED1000          0x0040  /* Select speed 1000Mbps		*/
#define BMCR_SPEED100           0x2000  /* Select speed 100Mbps			*/
#define BMCR_SPEED10            0x0000  /* Select speed 10Mbps			*/

/* Advertisement control register */
#define ADVERTISE_10HALF	0x0020 /* 10mbps half-duplex */
#define ADVERTISE_10FULL	0x0040 /* 10mbps full-duplex */
#define ADVERTISE_100HALF	0x0080 /* 100mbps half-duplex */
#define ADVERTISE_100FULL	0x0100 /* 100mbps full-duplex */
#define ADVERTISE_100BASE4	0x0200 /* 100mbps 4k packets */
#define ADVERTISE_PAUSE_CAP	0x0400 /* pause */
#define ADVERTISE_PAUSE_ASYM	0x0800 /* asymetric pause */

/* 1000BASE-T advertisement control register */
#define ADVERTISE_1000FULL	0x0200 /* Advertise 1000BASE-T full duplex */
#define ADVERTISE_1000HALF	0x0100 /* Advertise 1000BASE-T half duplex */

/* 1000BASE-T status register */
#define LPA_1000MSFAIL		0x8000  /* Master/Slave resolution failure */
#define LPA_1000FULL		0x0800  /* Link partner 1000BASE-T full duplex */
#define LPA_1000HALF		0x0400  /* Link partner 1000BASE-T half duplex */

/* Basic mode status register */
#define BMSR_LSTATUS		0x0004 /* Link status */
#define BMSR_ANEGCAPABLE	0x0008 /* Able to do auto-negotiation */
#define BMSR_ANEGCOMPLETE	0x0020 /* Auto-negotiation complete */
#define BMSR_ESTATEN		0x0100 /* Extended Status in R15 */
#define BMSR_10HALF		0x0800 /* Can do 10mbps, half-duplex */
#define BMSR_10FULL		0x1000 /* Can do 10mbps, full-duplex */
#define BMSR_100HALF		0x2000 /* Can do 100mbps, half-duplex */
#define BMSR_100FULL		0x4000 /* Can do 100mbps, full-duplex */

/* 1000BASE-T mode status register */
#define ESTATUS_1000_THALF	0x1000 /* Can do 1000BT Half */
#define ESTATUS_1000_TFULL	0x2000 /* Can do 1000BT Full */

#endif /* __UDK_NET_MII_H__ */
