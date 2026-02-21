/*
 * hifi_report.h
 *
 * hifi_report driver
 *
 * Copyright (c) 2022-2024 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef AUDIOMISC_HIFIREPORT_H
#define AUDIOMISC_HIFIREPORT_H

#ifdef CONFIG_AUDIO_HIFI_REPORT
int hifi_report_probe(struct device *dev, struct device_node *np);
#else
static inline int hifi_report_probe(struct device *dev, struct device_node *np)
{
	return 0;
}
#endif /* CONFIG_AUDIO_HIFI_REPORT */

#endif /* AUDIOMISC_HIFIREPORT_H */