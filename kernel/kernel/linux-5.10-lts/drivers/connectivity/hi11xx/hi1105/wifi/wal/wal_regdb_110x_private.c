/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : 管制域数据定制化文件读取接口
 * 作    者 :
 * 创建日期 : 2024年05月15日
 */

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#ifdef _PRE_WLAN_PRIVATE_REGDD
/* 通过客户定制化文件获取国家码信道对应表更新管制域信息 */
#include "wal_regdb_110x_private.h"

#define PRIV_REGDB_FILE_PATH_DEFAULT " "
static char g_priv_regdb_file_name[INI_FILE_PATH_LEN] = PRIV_REGDB_FILE_PATH_DEFAULT;

// 记录文件是否又被修改，避免每次都读文件
static unsigned long long g_priv_regdb_file_last_st_metime = 0;

#define PRIV_TAG_LEN    14
#define PRIV_TAG_IDX0   10
#define PRIV_TAG_IDX1   11
#define ALPHA2_LEN      3

#define STR_TO_DECIMAL  10

#define PRIV_REGDB_FILE_SUFFIX_NAME "_hi110x.conf"

// 配置默认值
static oal_ieee80211_regdomain_stru g_private_regdom = {
    .n_reg_rules = NUM_6_REG_RULE, // 6个规则
    .alpha2 = "99",
    .reg_rules = {
        REG_RULE(2402, 2482, 40, 6, 25, 0), // chan:1~13
        REG_RULE(5170, 5250, 160, 6, 25, 0), // chan:36~52
        REG_RULE(5250, 5330, 160, 6, 25, NL80211_RRF_DFS | 0), // chan:52~64 DFS Passive scan
        REG_RULE(5490, 5710, 160, 6, 25, NL80211_RRF_DFS | 0), // chan:100~140 DFS Passive scan
        REG_RULE(5735, 5835, 160, 6, 25, 0), // chan:149~165
        REG_RULE(4910, 4990, 80, 6, 25, 0), // chan:184~196
    }
};

typedef struct {
    int8_t *rule_flag_name;
    uint32_t rule_flag_var;
} wal_reg_rule_flag_stru;

const wal_reg_rule_flag_stru g_reg_rule_flag_table[] = {
    { "NL80211_RRF_NO_OFDM",        NL80211_RRF_NO_OFDM       },
    { "NL80211_RRF_NO_CCK",         NL80211_RRF_NO_CCK        },
    { "NL80211_RRF_NO_INDOOR",      NL80211_RRF_NO_INDOOR     },
    { "NL80211_RRF_NO_OUTDOOR",     NL80211_RRF_NO_OUTDOOR    },
    { "NL80211_RRF_DFS",            NL80211_RRF_DFS           },
    { "NL80211_RRF_PTP_ONLY",       NL80211_RRF_PTP_ONLY      },
    { "NL80211_RRF_PTMP_ONLY",      NL80211_RRF_PTMP_ONLY     },
    { "NL80211_RRF_NO_IR",          NL80211_RRF_NO_IR         },
    { "__NL80211_RRF_NO_IBSS",      __NL80211_RRF_NO_IBSS     },
    { "NL80211_RRF_AUTO_BW",        NL80211_RRF_AUTO_BW       },
    { "NL80211_RRF_IR_CONCURRENT",  NL80211_RRF_IR_CONCURRENT },
    { "NL80211_RRF_NO_HT40MINUS",   NL80211_RRF_NO_HT40MINUS  },
    { "NL80211_RRF_NO_HT40PLUS",    NL80211_RRF_NO_HT40PLUS   },
    { "NL80211_RRF_NO_80MHZ",       NL80211_RRF_NO_80MHZ      },
    { "NL80211_RRF_NO_160MHZ",      NL80211_RRF_NO_160MHZ     },
};

static int32_t is_tag_regdb_exist(oal_file_stru *fp, const char *tag_name)
{
    int32_t ret = INI_FAILED;
    char tmp[MAX_READ_LINE_NUM] = {0};

    if (oal_unlikely((fp == NULL) || (tag_name == NULL))) {
        ini_error("input is null");
        return INI_FAILED;
    }

    /* find the value of mode var, such as ini_wifi_mode
     * every mode except PLAT mode has only one mode var */
    for (;;) {
        ret = ko_read_line(fp, tmp, sizeof(tmp));
        if (ret == INI_FAILED) {
            ini_error("not find [g_regdom_xx]!!!");
            break;
        }

        if (oal_strncmp(tmp, tag_name, (uint32_t)OAL_STRLEN(tag_name)) == 0) {
            ini_info("have found [g_regdom_xx] succ");
            return INI_SUCC;
        }
    }
    return INI_FAILED;
}

static int32_t is_use_priv_regdb(void)
{
    int32_t ret;
    int32_t use_flag = 0;
    ret = get_cust_conf_int32(INI_MODU_WIFI, "use_priv_regdb", &use_flag);
    if (ret != INI_SUCC) {
        return INI_FAILED;
    }
    ini_info("use_priv_regdb::[%d]\n", use_flag);
    return use_flag;
}

static char* get_priv_regdb_file_path(void)
{
    int32_t ret;
    uint32_t path_len;
    char priv_regdb_file_t[INI_FILE_PATH_LEN] = {0};

    ret = get_cust_conf_string(INI_MODU_WIFI, "priv_regdb_file", priv_regdb_file_t, sizeof(priv_regdb_file_t) - 1);
    if (ret != INI_SUCC) {
        ini_error("can not find priv_regdb_file in ini");
        return NULL;
    }

    path_len = (uint32_t)OAL_STRLEN(priv_regdb_file_t) + (uint32_t)OAL_STRLEN(PRIV_REGDB_FILE_SUFFIX_NAME) + 1;
    if (path_len > INI_FILE_PATH_LEN) {
        ini_error("priv_regdb_file path is too long");
        return NULL;
    }

    if (strcat_s(priv_regdb_file_t, path_len, PRIV_REGDB_FILE_SUFFIX_NAME) != EOK) {
        ini_error("strcat_s fail, len is [%d]", path_len);
        return NULL;
    }

    if (snprintf_s(g_priv_regdb_file_name, sizeof(g_priv_regdb_file_name),
                sizeof(g_priv_regdb_file_name) - 1, "%s", priv_regdb_file_t) < 0) {
        ini_error("snprintf_s fail");
        return NULL;
    }

    if (!ini_file_exist(g_priv_regdb_file_name)) {
        ini_error("Cannot find priv_regdb_file !!! Please Check!!!\n");
        return NULL;
    }

    return g_priv_regdb_file_name;
}

static int32_t get_priv_regdb_alpha2(oal_file_stru *fp, oal_ieee80211_regdomain_stru* regdom)
{
    char tmp[MAX_READ_LINE_NUM + 1] = {0};
    const int8_t *var = "alpha2";
    uint32_t search_var_len = (uint32_t)OAL_STRLEN(var);

    if (oal_unlikely((fp == NULL) || (regdom == NULL))) {
        ini_error("input is null");
        return INI_FAILED;
    }

    for (;;) {
        if (ko_read_line(fp, tmp, sizeof(tmp)) == INI_FAILED) {
            return INI_FAILED;
        }

        if (tmp[0] == '[') {
            ini_error("not find alpha2!!!, check if var in correct mode");
            return INI_FAILED;
        }

        if ((oal_strncmp(tmp, var, search_var_len) == 0) &&
            (tmp[search_var_len] == '=') && (tmp[search_var_len + 1] != ' ')) {  /* 等号左右两边不能有空格 */
            search_var_len++;

            if (((search_var_len + 1 + ALPHA2_LEN) > MAX_READ_LINE_NUM) ||
                ((search_var_len + 1 + ALPHA2_LEN) > (uint32_t)OAL_STRLEN(tmp))) {
                ini_error("search_var_len[%d] is longer than maxLen[%d] or tmp_strlen[%d]",
                          (uint32_t)search_var_len, MAX_READ_LINE_NUM, (uint32_t)OAL_STRLEN(tmp));
                return INI_FAILED;
            }

            if (strncpy_s(regdom->alpha2, ALPHA2_LEN, &tmp[search_var_len + 1], ALPHA2_LEN - 1) != EOK) {
                ini_error("strcpy_s regdom->alpha2 is fail");
                return INI_FAILED;
            }
            break;
        }
    }

    return INI_SUCC;
}

static int32_t get_priv_regdb_n_reg_rules(oal_file_stru *fp, oal_ieee80211_regdomain_stru* regdom)
{
    char tmp[MAX_READ_LINE_NUM + 1] = {0};
    const int8_t *var = "n_reg_rules";
    uint32_t search_var_len = (uint32_t)OAL_STRLEN(var);

    if (oal_unlikely((fp == NULL) || (regdom == NULL))) {
        ini_error("input is null");
        return INI_FAILED;
    }

    for (;;) {
        if (ko_read_line(fp, tmp, sizeof(tmp)) == INI_FAILED) {
            return INI_FAILED;
        }

        if (tmp[0] == '[') {
            ini_error("not find n_reg_rules!!!, check if var in correct mode");
            return INI_FAILED;
        }

        if ((oal_strncmp(tmp, var, search_var_len) == 0) &&
            (tmp[search_var_len] == '=') && (tmp[search_var_len + 1] != ' ')) {  /* 等号左右两边不能有空格 */
            if (!isdigit(tmp[search_var_len + 1])) {
                ini_info("n_reg_rules:[%c] is not digit", tmp[search_var_len + 1]);
                return INI_FAILED;
            }

            regdom->n_reg_rules = (uint32_t)simple_strtol(&tmp[search_var_len + 1], NULL, STR_TO_DECIMAL);
            ini_info("regdom->n_reg_rules:%d", regdom->n_reg_rules);
            break;
        }
    }

    return INI_SUCC;
}

static int32_t get_rule_flag_var(char *var)
{
    uint32_t i = 0;

    if (oal_unlikely(var == NULL)) {
        ini_error("input is null");
        return 0;
    }

    for (i = 0; i < oal_array_size(g_reg_rule_flag_table); i++) {
        if (oal_strncmp(var, g_reg_rule_flag_table[i].rule_flag_name,
                        (uint32_t)OAL_STRLEN(g_reg_rule_flag_table[i].rule_flag_name)) == 0) {
            return g_reg_rule_flag_table[i].rule_flag_var;
        }
    }

    ini_error("not find var in g_reg_rule_flag_table\n");
    return 0;
}

static int32_t get_reg_rule_flag_value(char *var)
{
    int8_t *pc_token = NULL;
    int8_t *pc_ctx = NULL;
    int8_t *pc_sep = "|";
    uint32_t res = 0;
    char auc_cust_param[CUS_PARAMS_LEN_MAX] = {0};

    if (oal_unlikely(var == NULL)) {
        ini_error("input is null");
        return 0;
    }

    if (memcpy_s(auc_cust_param, CUS_PARAMS_LEN_MAX, var, OAL_STRLEN(var) - 1) != EOK) {
        ini_error("memcpy fail");
        return res;
    }

    pc_token = oal_strtok(auc_cust_param, pc_sep, &pc_ctx);
    /* 获取定制化系数 */
    while (pc_token != NULL) {
        pc_token = oal_strim(pc_token);
        if (isdigit(pc_token[0])) {
            res |= (uint32_t)simple_strtol(pc_token, NULL, STR_TO_DECIMAL);
        } else {
            res |= get_rule_flag_var(pc_token);
        }
        pc_token = oal_strtok(NULL, pc_sep, &pc_ctx);
    }
    return res;
}

static int32_t get_separate_str(char *var, int32_t *value, uint8_t val_max_num, int8_t *sep_str)
{
    int8_t *pc_token = NULL;
    int8_t *pc_ctx = NULL;
    uint32_t param_num = 0;
    char auc_cust_param[CUS_PARAMS_LEN_MAX] = {0};

    if (oal_unlikely((var == NULL) || (value == NULL) || (sep_str == NULL) || (val_max_num > NUM_6_BYTES))) {
        ini_error("input is null, val_max_num is [%d]", val_max_num);
        return 0;
    }

    if (memcpy_s(auc_cust_param, CUS_PARAMS_LEN_MAX, var, OAL_STRLEN(var) - 1) != EOK) {
        ini_error("memcpy_s fail");
        return INI_FAILED;
    }

    pc_token = oal_strtok(auc_cust_param, sep_str, &pc_ctx);
    /* 获取定制化系数 */
    while (pc_token != NULL) {
        pc_token = oal_strim(pc_token);
        if (pc_token[0] == '(') {
            pc_token++;
        }

        if (param_num >= val_max_num) {
            ini_error("value_parm is fail, param_num[%d] val_max_num[%d]", param_num, val_max_num);
            return INI_FAILED;
        }

        if (isdigit(pc_token[0])) {
            *(value + param_num) = (uint32_t)simple_strtol(pc_token, NULL, STR_TO_DECIMAL);
        } else {
            *(value + param_num) = get_reg_rule_flag_value(pc_token);
        }

        pc_token = oal_strtok(NULL, sep_str, &pc_ctx);
        param_num++;
    }

    return INI_SUCC;
}

static int32_t get_priv_regdb_reg_rules_each_value(int8_t *var, uint8_t cnt, oal_ieee80211_regdomain_stru* regdom)
{
    int32_t rules_value[NUM_6_BYTES] = {0};

    if (get_separate_str(var, rules_value, NUM_6_BYTES, ",") !=  INI_SUCC) {
        return INI_FAILED;
    }

    regdom->reg_rules[cnt].freq_range.start_freq_khz = MHZ_TO_KHZ(rules_value[0]);
    regdom->reg_rules[cnt].freq_range.end_freq_khz = MHZ_TO_KHZ(rules_value[NUM_1_BYTES]);
    regdom->reg_rules[cnt].freq_range.max_bandwidth_khz = MHZ_TO_KHZ(rules_value[NUM_2_BYTES]);
    regdom->reg_rules[cnt].power_rule.max_antenna_gain = DBI_TO_MBI(rules_value[NUM_3_BYTES]);
    regdom->reg_rules[cnt].power_rule.max_eirp = DBM_TO_MBM(rules_value[NUM_4_BYTES]);
    regdom->reg_rules[cnt].flags = rules_value[NUM_5_BYTES];
    regdom->reg_rules[cnt].dfs_cac_ms = 0;

    return INI_SUCC;
}

static int32_t get_priv_regdb_reg_rules(oal_file_stru *fp, oal_ieee80211_regdomain_stru* regdom, int8_t *rules_cnt)
{
    const int8_t *var = "reg_rules";
    uint32_t search_var_len = (uint32_t)OAL_STRLEN(var);
    char tmp[MAX_READ_LINE_NUM + 1] = {0};
    int8_t reg_rules_cnt = -1;

    if (oal_unlikely((fp == NULL) || (regdom == NULL) || (rules_cnt == NULL))) {
        ini_error("input is null");
        return INI_FAILED;
    }

    for (;;) {
        if (ko_read_line(fp, tmp, sizeof(tmp)) == INI_FAILED) {
            return INI_FAILED;
        }

        if (tmp[0] == '[') {
            ini_error("not find reg_rules!!!, check if var in correct mode");
            return INI_FAILED;
        }

        if (tmp[0] == '}') { /* find end */
            *rules_cnt = reg_rules_cnt;
            break;
        } else if (reg_rules_cnt >= 0) { /* find success and try get value */
            if (get_priv_regdb_reg_rules_each_value(tmp, reg_rules_cnt, regdom) != INI_SUCC) {
                return INI_FAILED;
            }
            reg_rules_cnt++;
        } else if ((oal_strncmp(tmp, var, search_var_len) == 0) &&
            (tmp[search_var_len] == '=') && (tmp[search_var_len + 1] == '{')) { /* find success */
            reg_rules_cnt = 0; /* start reg rules cnt */
        }
    }

    return INI_SUCC;
}

static int32_t get_priv_reg_regdb_para(oal_file_stru *fp, oal_ieee80211_regdomain_stru* regdom)
{
    loff_t offset = 0;
    int8_t rules_cnt = 0;

    if (oal_unlikely((fp == NULL) || (regdom == NULL))) {
        ini_error("input is null");
        return INI_FAILED;
    }
    offset = fp->f_pos;
    if (get_priv_regdb_alpha2(fp, regdom) != INI_SUCC) {
        ini_error("get alpha2 fail");
        return INI_FAILED;
    }

    fp->f_pos = offset;
    if (get_priv_regdb_reg_rules(fp, regdom, &rules_cnt) != INI_SUCC) {
        ini_error("get reg_rules fail");
        return INI_FAILED;
    }

    fp->f_pos = offset;
    if (get_priv_regdb_n_reg_rules(fp, regdom) != INI_SUCC) {
        ini_error("get n_reg_rules fail");
        return INI_FAILED;
    }

    if (rules_cnt != regdom->n_reg_rules) {
        ini_error("rules_cnt[%d] != n_reg_rules[%d], please check",
                  rules_cnt, regdom->n_reg_rules);
        return INI_FAILED;
    }

    return INI_SUCC;
}

static int32_t get_priv_reg_regdb_info(char *file, const char *tag, oal_ieee80211_regdomain_stru* regdom)
{
    oal_file_stru *fp = NULL;
    int32_t ret;

    if (oal_unlikely((file == NULL) || (tag == NULL) || (regdom == NULL))) {
        ini_error("input is null");
        return INI_FAILED;
    }

    fp = oal_file_open_readonly(file);
    if (fp == NULL) {
        ini_error("open priv_regdb_file failed!!!");
        return OAL_FAIL;
    }

    /* find the tag, such as [CN] of regdb list */
    ret = is_tag_regdb_exist(fp, tag);
    if (ret != INI_SUCC) {
        oal_file_close(fp);
        return OAL_FAIL;
    }

    /* find the regdb para of tag */
    ret = get_priv_reg_regdb_para(fp, regdom);
    if (ret != INI_SUCC) {
        oal_file_close(fp);
        return OAL_FAIL;
    }

    oal_file_close(fp);
    return OAL_SUCC;
}

STATIC int32_t check_priv_regdb_file_is_updated(char *file)
{
    oal_file_stru *fp = NULL;

    if (oal_unlikely(file == NULL)) {
        ini_error("input is null");
        return INI_FAILED;
    }

    fp = oal_file_open_readonly(file);
    if (fp == NULL) {
        ini_error("open priv_regdb_file failed!!!");
        return OAL_FAIL;
    }

    if (g_priv_regdb_file_last_st_metime != inf_file_get_ctime(fp->f_path.dentry)) {
        g_priv_regdb_file_last_st_metime = inf_file_get_ctime(fp->f_path.dentry);
        ini_info("priv_regdb_file is updated in %lld", g_priv_regdb_file_last_st_metime);
        oal_file_close(fp);
        return INI_SUCC;
    }

    oal_file_close(fp);
    return INI_FAILED;
}

int32_t wal_regdb_find_db_from_private_file(int8_t *str)
{
    int8_t priv_tag[PRIV_TAG_LEN] = "[g_regdom_xx]";
    char *regdb_file = NULL;
    uint32_t str_len;
    if (oal_unlikely((str == NULL) || (OAL_STRLEN(str) < NUM_2_BYTES))) {
        ini_error("input is fail, please check");
        return OAL_FAIL;
    }
    str_len = (uint32_t)OAL_STRLEN(str);

    if (is_use_priv_regdb() != OAL_TRUE) {
        return OAL_FAIL;
    }

    regdb_file = get_priv_regdb_file_path();
    if (regdb_file == NULL) {
        ini_error("regdb_file is null");
        return OAL_FAIL;
    }

    if (check_priv_regdb_file_is_updated(regdb_file) != INI_SUCC) {
        ini_info("private regdb file is not updated");
        return OAL_SUCC;
    }

    priv_tag[PRIV_TAG_IDX0] = str[0];
    priv_tag[PRIV_TAG_IDX1] = str[NUM_1_BYTES];
    ini_info("try find priv_tag[%c%c]", priv_tag[PRIV_TAG_IDX0], priv_tag[PRIV_TAG_IDX1]);

    return get_priv_reg_regdb_info(regdb_file, priv_tag, &g_private_regdom);
}

oal_ieee80211_regdomain_stru *wal_get_private_regdom(void)
{
    uint8_t i = 0;

    oal_io_print(".alpha2 = %c%c\n", g_private_regdom.alpha2[0], g_private_regdom.alpha2[1]);
    oal_io_print(".reg_rules = {\n");
    for (i = 0; i < g_private_regdom.n_reg_rules; i++) {
        oal_io_print("    REG_RULE(%d, %d, %d, %d, %d, %d)\n",
                     KHZ_TO_MHZ(g_private_regdom.reg_rules[i].freq_range.start_freq_khz),
                     KHZ_TO_MHZ(g_private_regdom.reg_rules[i].freq_range.end_freq_khz),
                     KHZ_TO_MHZ(g_private_regdom.reg_rules[i].freq_range.max_bandwidth_khz),
                     MBI_TO_DBI(g_private_regdom.reg_rules[i].power_rule.max_antenna_gain),
                     MBM_TO_DBM(g_private_regdom.reg_rules[i].power_rule.max_eirp),
                     (g_private_regdom.reg_rules[i].flags));
    }
    oal_io_print(".n_reg_rules = %d\n", g_private_regdom.n_reg_rules);

    return &g_private_regdom;
}
#endif
#endif
