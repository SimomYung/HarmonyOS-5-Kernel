/*
 * QIC V300 BUS ID INFO.
 *
 * Copyright (c) 2023-2023 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#ifndef __SOC_QIC_BUS_H__
#define __SOC_QIC_BUS_H__
/* You need to modify dts at the same time if you want to modify this field. */
enum QIC_BUS_ID {
	QIC_AO = 0x0,                /* 0x0 */
	QIC_ASP,                     /* 0x1 */
	QIC_IOMCU,                   /* 0x2 */
	QIC_MSPC,                    /* 0x3 */
	QIC_MDMPERI,                 /* 0x4 */
	QIC_CFGDMA,                  /* 0x5 */
	QIC_CPU,                     /* 0x6 */
	QIC_CENTER_CROSSBAR,         /* 0x7 */
	QIC_HSDT1,                   /* 0x8 */
	QIC_GPU_CFG,                 /* 0x9 */
	QIC_GPU_DATA,                /* 0xA */
	QIC_NPUTS,                   /* 0xB */
	QIC_HSDT0,                   /* 0xC */
	QIC_NPU,                     /* 0xD */
	QIC_MEDIA_1,                 /* 0xE */
	QIC_MEDIA_2,                 /* 0xF */
	QIC_BUS_ID_MAX,
};

#define QIC_AO_BUS                (1 << QIC_AO)
#define QIC_ASP_BUS               (1 << QIC_ASP)
#define QIC_IOMCU_BUS             (1 << QIC_IOMCU)
#define QIC_MSPC_BUS              (1 << QIC_MSPC)
#define QIC_MDMPERI_BUS           (1 << QIC_MDMPERI)
#define QIC_CFGDMA_BUS            (1 << QIC_CFGDMA)
#define QIC_CPU_BUS               (1 << QIC_CPU)
#define QIC_CENTER_CROSSBAR_BUS   (1 << QIC_CENTER_CROSSBAR)
#define QIC_HSDT1_BUS             (1 << QIC_HSDT1)
#define QIC_GPU_CFG_BUS           (1 << QIC_GPU_CFG)
#define QIC_GPU_DATA_BUS          (1 << QIC_GPU_DATA)
#define QIC_NPUTS_BUS             (1 << QIC_NPUTS)
#define QIC_HSDT0_BUS             (1 << QIC_HSDT0)
#define QIC_NPU_BUS               (1 << QIC_NPU)
#define QIC_MEDIA1_BUS            (1 << QIC_MEDIA_1)
#define QIC_MEDIA2_BUS            (1 << QIC_MEDIA_2)
#define QIC_BUS_ID_MASK           ((1 << QIC_BUS_ID_MAX) - 1)
#endif
