/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2025. All rights reserved.
 * Description: Log dictionaries are grouped by module headers
 * Author: Yujianfeng @huawei.com
 * Create: 2024-12-31
 */
 
#ifndef CONTEXTHUB_LOG_THROTTLING_H
#define CONTEXTHUB_LOG_THROTTLING_H

#define SENSORHUB_TAG_MAX           400
#define SENSORHUB_HEAD_MAX          40
#define FAIL                        (-1)
#define SUCCESS                     (0)

typedef struct {
	uint16_t lenth;
	uint32_t addr_group[SENSORHUB_TAG_MAX];
	uint16_t label_tag[SENSORHUB_TAG_MAX];
} log_label_tabel;

typedef struct {
	uint32_t addr_start;
	uint32_t addr_end;
}log_thro_data_t;

struct ssh_logthro_work {
	struct work_struct worker;
	int len;
	void *data;
};

int ssh_log_label_tabel_generate(void);
int collect_ssh_log_num(const uint8_t *logbuff, size_t len);
#endif