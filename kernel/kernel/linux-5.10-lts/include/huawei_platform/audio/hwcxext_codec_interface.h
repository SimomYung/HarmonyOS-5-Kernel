/*
 * hwcxext_codec_interface.h
 *
 * hwcxext_codec_interface header file
 *
 * Copyright (c) 2022 Huawei Technologies Co., Ltd.
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

#ifndef __HWCXEXT_CODEC_INTERFACE_H__
#define __HWCXEXT_CODEC_INTERFACE_H__

#include <sound/soc.h>
#define HWCXEXTDAILINKSIZE 1

#if defined(CONFIG_HWCXEXT_CODEC_INTERFACE_PC) || defined(CONFIG_HWCXEXT_CODEC_INTERFACE_HMPC)
void hwcxext_codec_register_in_machine_driver(struct platform_device *pdev,
	struct snd_soc_card **card,
struct snd_soc_dai_link *dai_link, unsigned int links_num);
int hwcxext_codec_set_dailink_in_machine_driver(struct platform_device *pdev,
	struct snd_soc_dai_link *dai_link);
int set_hwcxext_dai_link_at_pc_machine_dai_link_tail(struct platform_device *pdev,
    struct snd_soc_dai_link *pc_machine_dai_link, size_t pc_machine_dai_link_size,
    struct snd_soc_dai_link *new_pc_machine_dai_link, size_t new_pc_machine_dai_link_size);
void hwcxext_codec_v1_suspend_interface(struct device *dev);
void hwcxext_codec_v1_resume_interface(struct device *dev);
void hwcxext_codec_v1_restore_interface(struct device *dev);
void hwcxext_codec_v1_freeze_interface(struct device *dev);
void hwcxext_codec_v1_thaw_interface(struct device *dev);
void hwcxext_codec_v1_power_down_interface(void);
void hwcxext_codec_v1_power_up_interface(void);
#else
static inline void hwcxext_codec_register_in_machine_driver(
	struct platform_device *pdev,
	struct snd_soc_card **card,
	struct snd_soc_dai_link *dai_link, unsigned int links_num)
{
	(void)pdev;
	(void)card;
	(void)dai_link;
	(void)links_num;
}
static int hwcxext_codec_set_dailink_in_machine_driver(struct platform_device *pdev,
	struct snd_soc_dai_link *dai_link)
{
	(void)pdev;
	(void)dai_link;
	return 0;
}
static int set_hwcxext_dai_link_at_pc_machine_dai_link_tail(struct platform_device *pdev,
    struct snd_soc_dai_link *pc_machine_dai_link, size_t pc_machine_dai_link_size,
    struct snd_soc_dai_link *new_pc_machine_dai_link, size_t new_pc_machine_dai_link_size)
{
	(void)pdev;
	(void)pc_machine_dai_link;
	(void)pc_machine_dai_link_size;
	(void)new_pc_machine_dai_link;
	(void)new_pc_machine_dai_link_size;
	return 0;
}
static inline void hwcxext_codec_v1_suspend_interface(struct device *dev)
{
	(void)dev;
}
static inline void hwcxext_codec_v1_resume_interface(struct device *dev)
{
	(void)dev;
}
static inline void hwcxext_codec_v1_restore_interface(struct device *dev)
{
	(void)dev;
}
static inline void hwcxext_codec_v1_freeze_interface(struct device *dev)
{
	(void)dev;
}
static inline void hwcxext_codec_v1_thaw_interface(struct device *dev)
{
	(void)dev;
}
static inline void hwcxext_codec_v1_power_down_interface(void)
{
}
static inline void hwcxext_codec_v1_power_up_interface(void)
{
}
#endif

#endif /* hwevext_codec_interface.h */

