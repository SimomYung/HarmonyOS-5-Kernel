#ifndef __SENSOR_DSM_SINGLE__H_
#define __SENSOR_DSM_SINGLE__H_

#ifdef CONFIG_HUAWEI_OHOS_DSM

#include <linux/list.h>

#define SENSOR_HISYS_DSM_MAX_EVENT_NUM  512
#define SENSOR_HISYSENVENT_BUF_MAX      2176
#define SENSOR_HISYSENVENT_DECIMAL      10
#define SENSOR_HISYSENVENT_PARAM_NUM    5
#define SENSOR_HISYSENVENT_DOMAIN_NUM   3
#define SENSOR_DSM_BUF_SIZE_0016		16
#define SENSOR_DSM_BUF_SIZE_0032		32
#define SENSOR_DSM_BUF_SIZE_0048		48

#define SENSOR_DSM_ERR             (-1)
#define SENSOR_DSM_OK              0

struct hisysevent_error_no_map {
	int error_no;
	int level;
	char domain[SENSOR_DSM_BUF_SIZE_0016];
	char name[SENSOR_DSM_BUF_SIZE_0032];
	char key[SENSOR_DSM_BUF_SIZE_0048];
};

enum sensor_dsm_level {
	SENSOR_DSM_LEVEL_FAULT = 1,
	SENSOR_DSM_LEVEL_STATISTIC,
	SENSOR_DSM_LEVEL_SECURITY,
	SENSOR_DSM_LEVEL_BEHAVIOR,
	SENSOR_DSM_LEVEL_MAX
};

struct sensor_dsm_err_map {
	int event_num;
	struct list_head map_node;
	struct hisysevent_error_no_map map[SENSOR_HISYS_DSM_MAX_EVENT_NUM];
};
int sensor_dsm_single_register(char *domain);
int sensor_dsm_single_convert_dsm_to_hisysevent(int err_no, int *dsm_error_level, char *dsm_domain, char *name, char *key);
int sensor_dsm_single_report_hiview(const char *domain, const char *error_name, const char *key, const char *value, unsigned int err_level);
int sensor_dsm_report_hiview(unsigned int err_no, const char *key, const char *value);
#endif // #ifdef CONFIG_HUAWEI_OHOS_DSM
#endif