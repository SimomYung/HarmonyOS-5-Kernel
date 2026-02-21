

#include "signal_hub_config.h"
#include "signal_hub_channel.h"
 
#include <securec.h>
#include <platform_include/basicplatform/linux/hw_cmdline_parse.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/pm_wakeup.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/printk.h>
 
#define MAX_MIPI_RES_NUM (2 * MAX_TUNER_INST_NUM)
#define MAX_CFG_RES ((MAX_INST_NUM_PER_TUNER_STATE + 1) * MAX_TUNER_STATE_NUM)
#define MAX_ANT_MAP_SIZE 12
#define MAX_MIPI_DRIVER_CFG_SIZE 2
#define OFFSET_MASKTABLE 16
#define MIPI_UNIT_BIT32_NUM 2
#define OFFSET_PORT_SEL 5
#define OFFSET_SLAVE_ID 8
#define OFFSET_REG_ADDR 12
#define OFFSET_MIPI_CTRL 28
#define OFFSET_DATA_BYTE_1 8
#define OFFSET_DATA_BYTE_2 16
#define OFFSET_DATA_BYTE_3 24
#define OFFSET_MIPI_PIN_ID 8
#define MAX_FIX_STATE_NUM 9
#define MAX_ANT_SYSTEM_IDX 3

static int read_scene_cfg(struct device_node *dn, int index, int sce_idx, signalhub_platform_data* signalhub_data)
{
    unsigned int t;
    unsigned int i;
    unsigned int len;
    struct property *prop = NULL;
    /* scene */
    if (of_property_read_u32(dn, "scene_idx", &t)) {
        hwlog_err("signalhub %s: read signalhub scene_idx fail\n", __func__);
        return FAIL;
    }
    signalhub_data->tuner_cfg[index].scene_tuner_cfg[sce_idx].scene = t;
    hwlog_info("signalhub %s: scene[%d]=%u\n", __func__, sce_idx,
        signalhub_data->tuner_cfg[index].scene_tuner_cfg[sce_idx].scene);

    uint32_t cfg_idx[MAX_FIX_STATE_NUM] = {0};
    prop = of_find_property(dn, "state_cfg_idx", NULL);
    if (!prop) {
        hwlog_err("signalhub %s:  prop is NULL\n", __func__);
        return FAIL;
    }
    len = (uint32_t)(prop->length) / sizeof(uint32_t);
    if (len > MAX_FIX_STATE_NUM) {
        hwlog_err("signalhub %s: length of state_cfg_idx is over, len=%d\n", __func__, len);
        return FAIL;
    }
    if (of_property_read_u32_array(dn, "state_cfg_idx", cfg_idx, len)) {
        hwlog_err("signalhub %s: read signalhub state_cfg_idx fail\n", __func__);
        return FAIL;
    }
    for (i = 0; i < len; i++) {
        signalhub_data->tuner_cfg[index].scene_tuner_cfg[sce_idx].tuner_state_cfg_index[i] = (uint16_t)cfg_idx[i];
        hwlog_info("signalhub %s: state_cfg_idx[%d] =%u\n", __func__, i,
            signalhub_data->tuner_cfg[index].scene_tuner_cfg[sce_idx].tuner_state_cfg_index[i]);
    }

    return SUCCESS;
}

static int read_masktable(struct device_node *dn, int index, int sce_idx, signalhub_platform_data* signalhub_data)
{
    unsigned int t;
    unsigned int i;
    unsigned int len;
    struct property *prop = NULL;

    if (of_property_read_u32(dn, "masktable_num", &t)) {
        hwlog_err("signalhub %s: read signalhub masktable_num fail\n", __func__);
        return FAIL;
    }
    signalhub_data->tuner_cfg[index].scene_tuner_cfg[sce_idx].masktable_num = (uint16_t)t;

    uint32_t masktable[MAX_MASK_TABLE_NUM] = {0};
    prop = of_find_property(dn, "masktable", NULL);
    if (!prop) {
        hwlog_err("signalhub %s:  prop is NULL\n", __func__);
        return FAIL;
    }
    len = (uint32_t)(prop->length) / sizeof(uint32_t);
    if (len > MAX_MASK_TABLE_NUM) {
        hwlog_err("signalhub %s: length of masktable is over, len=%d\n", __func__, len);
        return FAIL;
    }
    if (of_property_read_u32_array(dn, "masktable", masktable, len)) {
        hwlog_err("signalhub %s: read signalhub masktable fail\n", __func__);
        return FAIL;
    }
    for (i = 0; i < len; i++) {
        signalhub_data->tuner_cfg[index].scene_tuner_cfg[sce_idx].masktable[i].mask_info.value =
            (uint16_t)(masktable[i] >> 0) & 0xFFFF;
        signalhub_data->tuner_cfg[index].scene_tuner_cfg[sce_idx].masktable[i].tuner_cfg_index =
            (uint16_t)(masktable[i] >> OFFSET_MASKTABLE) & 0xFFFF;
        hwlog_info("signalhub %s: mask_value[%d]=%u\n", __func__, i,
            signalhub_data->tuner_cfg[index].scene_tuner_cfg[sce_idx].masktable[i].mask_info.value);
        hwlog_info("signalhub %s: cfg_idx[%d]=%u\n", __func__, i,
            signalhub_data->tuner_cfg[index].scene_tuner_cfg[sce_idx].masktable[i].tuner_cfg_index);
    }

    return SUCCESS;
}

static int read_ant_array(struct device_node *dn, int index, signalhub_platform_data* signalhub_data)
{
    unsigned int i;
    unsigned int len;
    struct property *prop = NULL;

    uint32_t idx[MAX_ANT_SYSTEM_IDX] = {0};
    prop = of_find_property(dn, "hw_ant_idx", NULL);
    if (!prop) {
        hwlog_err("signalhub %s: prop is NULL\n", __func__);
        return FAIL;
    }
    len = (uint32_t)(prop->length) / sizeof(uint32_t);
    if (len > MAX_ANT_SYSTEM_IDX) {
        hwlog_err("signalhub %s: length of hw_ant_idx is over, len=%d\n", __func__, len);
        return FAIL;
    }
    if (of_property_read_u32_array(dn, "hw_ant_idx", idx, len)) {
        hwlog_err("signalhub %s: read signalhub hw_ant_idx fail\n", __func__);
        return FAIL;
    }
    for (i = 0; i < len; i++) {
        signalhub_data->tuner_cfg[index].hw_ant_index[i] = (uint8_t)idx[i];
        hwlog_info("signalhub %s: hw_ant_index[%d]=%u\n", __func__,
            i, signalhub_data->tuner_cfg[index].hw_ant_index[i]);
    }

    return SUCCESS;
}

static int read_system_array(struct device_node *dn, int index, signalhub_platform_data* signalhub_data)
{
    unsigned int i;
    unsigned int len;
    struct property *prop = NULL;

    uint32_t idx[MAX_ANT_SYSTEM_IDX] = {0};
    prop = of_find_property(dn, "subsystem_idx", NULL);
    if (!prop) {
        hwlog_err("signalhub %s:  prop is NULL\n", __func__);
        return FAIL;
    }
    len = (uint32_t)(prop->length) / sizeof(uint32_t);
    if (len > MAX_ANT_SYSTEM_IDX) {
        hwlog_err("signalhub %s: length of subsystem_idx is over, len=%d\n", __func__, len);
        return FAIL;
    }
    if (of_property_read_u32_array(dn, "subsystem_idx", idx, len)) {
        hwlog_err("signalhub %s: read signalhub subsystem_idx fail\n", __func__);
        return FAIL;
    }
    for (i = 0; i < len; i++) {
        signalhub_data->tuner_cfg[index].system[i] = idx[i];
        hwlog_info("signalhub %s: system_index[%d]=%u\n", __func__,
            i, signalhub_data->tuner_cfg[index].system[i]);
    }

    struct device_node *scene0_cfg = of_get_child_by_name(dn, "scene0_cfg");
    if (scene0_cfg == NULL) {
        hwlog_err("signalhub %s:Failed to find scene0_cfg node\n", __func__);
        return FAIL;
    }
    struct device_node *scene1_cfg = of_get_child_by_name(dn, "scene1_cfg");
    if (scene1_cfg == NULL) {
        hwlog_err("signalhub %s:Failed to find scene1_cfg node\n", __func__);
        return FAIL;
    }
    read_scene_cfg(scene0_cfg, index, 0, signalhub_data);
    read_masktable(scene0_cfg, index, 0, signalhub_data);

    read_scene_cfg(scene1_cfg, index, 1, signalhub_data);
    read_masktable(scene1_cfg, index, 1, signalhub_data);
    of_node_put(scene0_cfg);
    of_node_put(scene1_cfg);

    return SUCCESS;
}

static int read_cfg_table(struct device_node *dn, int index, signalhub_platform_data* signalhub_data)
{
    unsigned int t;
    struct device_node *cfg_table_node = of_get_child_by_name(dn, "cfg_table");
    if (cfg_table_node == NULL) {
        hwlog_err("signalhub %s: Failed to find cfg_table node in tuner[%d]\n", __func__, index);
        return FAIL;
    }

    if (of_property_read_u32(cfg_table_node, "enable", &t)) {
        hwlog_err("signalhub %s: read signalhub enable fail\n", __func__);
        return FAIL;
    }
    signalhub_data->tuner_cfg[index].enable = (uint8_t)t;

    if (of_property_read_u32(cfg_table_node, "tuner_index", &t)) {
        hwlog_err("signalhub %s: read signalhub tuner_index fail\n", __func__);
        return FAIL;
    }
    signalhub_data->tuner_cfg[index].tuner_index = (uint8_t)t;

    if (of_property_read_u32(cfg_table_node, "hw_ant_num", &t)) {
        hwlog_err("signalhub %s: read signalhub hw_ant_num fail\n", __func__);
        return FAIL;
    }
    signalhub_data->tuner_cfg[index].hw_ant_num = (uint8_t)t;

    if (of_property_read_u32(cfg_table_node, "subsystem_num", &t)) {
        hwlog_err("signalhub %s: read signalhub subsystem_num fail\n", __func__);
        return FAIL;
    }
    signalhub_data->tuner_cfg[index].system_num = (uint8_t)t;

    if (of_property_read_u32(cfg_table_node, "scene_num", &t)) {
        hwlog_err("signalhub %s: read signalhub scene_num fail\n", __func__);
        return FAIL;
    }
    signalhub_data->tuner_cfg[index].scene_num = (uint8_t)t;

    hwlog_info("signalhub %s: enable =%u\n", __func__, signalhub_data->tuner_cfg[index].enable);
    hwlog_info("signalhub %s: tuner_index =%u\n", __func__, signalhub_data->tuner_cfg[index].tuner_index);

    read_ant_array(cfg_table_node, index, signalhub_data);
    read_system_array(cfg_table_node, index, signalhub_data);
    of_node_put(cfg_table_node);
    return SUCCESS;
}

static int read_mipi_res(struct device_node *dn, int index, signalhub_platform_data* signalhub_data)
{
    unsigned int i;
    unsigned int len;
	uint32_t mipi_res[MAX_MIPI_RES_NUM] = {0};
    struct property *prop = NULL;

    prop = of_find_property(dn, "mipi_res", NULL);
    if (!prop) {
        hwlog_err("signalhub %s:  prop is NULL!\n", __func__);
        return FAIL;
    }
    len = (uint32_t)(prop->length) / sizeof(uint32_t);
    if (len > MAX_MIPI_RES_NUM) {
        hwlog_err("signalhub %s: length of mipi_res is over, len=%d\n", __func__, len);
        return FAIL;
    }
    if (of_property_read_u32_array(dn, "mipi_res", mipi_res, len)) {
        hwlog_err("signalhub %s: read mipi_res from dts fail\n", __func__);
        return FAIL;
    }

    /*
     * 0x1F表示保留5位有效信息
     * 0x07表示保留3位有效信息
     * 0x0F表示保留4位有效信息
     * 0xFFFF表示保留16位有效信息
     * 0xFF表示保留8位有效信息
     */
    for (i = 0; i < len / MIPI_UNIT_BIT32_NUM; i++) {
        uint32_t cmd_value = mipi_res[2 * i];
        uint32_t data_value = mipi_res[2 * i + 1];
        signalhub_data->mipi_res[index][i].cmd.byte_cnt = (cmd_value >> 0) & 0x1F;
        signalhub_data->mipi_res[index][i].cmd.mipi_port_sel = (cmd_value >> OFFSET_PORT_SEL) & 0x07;
        signalhub_data->mipi_res[index][i].cmd.slave_id = (cmd_value >> OFFSET_SLAVE_ID) & 0x0F;
        signalhub_data->mipi_res[index][i].cmd.reg_addr = (cmd_value >> OFFSET_REG_ADDR) & 0xFFFF;
        signalhub_data->mipi_res[index][i].cmd.mipi_ctrl = (cmd_value >> OFFSET_MIPI_CTRL) & 0x0F;

        signalhub_data->mipi_res[index][i].data.byte0 = (data_value >> 0) & 0xFF;
        signalhub_data->mipi_res[index][i].data.byte1 = (data_value >> OFFSET_DATA_BYTE_1) & 0xFF;
        signalhub_data->mipi_res[index][i].data.byte2 = (data_value >> OFFSET_DATA_BYTE_2) & 0xFF;
        signalhub_data->mipi_res[index][i].data.byte3 = (data_value >> OFFSET_DATA_BYTE_3) & 0xFF;
    }
    hwlog_info("signalhub %s: mipi_port_sel =%u, slave_id =%u, reg_addr =%u, mipi_ctrl =%u, data.byte0 =%u\n",
        __func__, signalhub_data->mipi_res[index][0].cmd.mipi_port_sel,
        signalhub_data->mipi_res[index][0].cmd.slave_id, signalhub_data->mipi_res[index][0].cmd.reg_addr,
        signalhub_data->mipi_res[index][0].cmd.mipi_ctrl, signalhub_data->mipi_res[index][0].data.byte0);
    return SUCCESS;
}

static int read_cfg_res(struct device_node *dn, int index, signalhub_platform_data* signalhub_data)
{
    unsigned int i;
    unsigned int len;
    uint32_t cfg_res[MAX_CFG_RES] = {0};
    struct property *prop = NULL;

    prop = of_find_property(dn, "cfg_res", NULL);
    if (!prop) {
        hwlog_err("signalhub %s:  prop is NULL\n", __func__);
        return FAIL;
    }
    len = (uint32_t)(prop->length) / sizeof(uint32_t);
    if (len > MAX_CFG_RES) {
        hwlog_err("signalhub %s: length of cfg_res is over, len=%d\n", __func__, len);
        return FAIL;
    }
    if (of_property_read_u32_array(dn, "cfg_res", cfg_res, len)) {
        hwlog_err("signalhub %s: read cfg_res from dts fail\n", __func__);
        return FAIL;
    }
    for (i = 0; i < len / (MAX_INST_NUM_PER_TUNER_STATE + 1); i++) {
        signalhub_data->tuner_cfg_res[index][i].mipi_cnt = (uint8_t)cfg_res[i];
        hwlog_info("signalhub %s: cfg_res.mipi_cnt =%u\n", __func__, signalhub_data->tuner_cfg_res[index][i].mipi_cnt);
        for (int j = 0; j < MAX_INST_NUM_PER_TUNER_STATE; j++) {
            int offset = sizeof(signalhub_tuner_state_cfg) * i + 1;
            signalhub_data->tuner_cfg_res[index][i].mipi_index_list[j] = (uint8_t)cfg_res[offset + j];
        }
        hwlog_info("signalhub %s: cfg_res.mipi_index_list[0] =%u\n", __func__,
            signalhub_data->tuner_cfg_res[index][i].mipi_index_list[0]);
    }

    read_cfg_table(dn, index, signalhub_data);
    return SUCCESS;
}

static int signalhub_send_dts_cfg(signalhub_platform_data* signalhub_data)
{
    int ret = 0;
    int dataLength = sizeof(signalhub_dts_cmd_data_header) + 4; /* 4表示uint32大小 */
    sighub_send_msg_data* send_msg = (sighub_send_msg_data*)kmalloc(dataLength, GFP_KERNEL);
    if (send_msg == NULL) {
        hwlog_err("signalhub %s: kmalloc send_msg fail\n", __func__);
        return FAIL;
    }
    ret = memset_s(send_msg, dataLength, 0, dataLength);
    if (ret) {
        hwlog_err("signalhub %s: memset send_msg fail\n", __func__);
        kfree(send_msg);
        return FAIL;
    }
    send_msg->cmd = SIGNAL_HUB_CONFIG_CHANGE_CMD;
    send_msg->size = sizeof(signalhub_dts_cmd_data_header);

    signalhub_dts_cmd_data_header* cmd_hd = (signalhub_dts_cmd_data_header*)send_msg->data;
    cmd_hd->subcmd = SIGNAL_HUB_SET_DTS_CONFIG;
    cmd_hd->size = sizeof(signalhub_platform_data);
    ret = memcpy_s(&cmd_hd->data, sizeof(signalhub_platform_data), signalhub_data, sizeof(signalhub_platform_data));
    if (ret) {
        hwlog_err("signalhub %s: memcpy signalhub_data fail\n", __func__);
        kfree(send_msg);
        return FAIL;
    }

    hwlog_info("signalhub %s first data size:%u, cmd:%u\n", __func__, send_msg->size, send_msg->cmd);
    hwlog_info("signalhub %s second data size:%u, subcmd:%u\n", __func__, cmd_hd->size, cmd_hd->subcmd);
    ret = signal_hub_send_to_sensorhub(CMD_CMN_CONFIG_REQ, send_msg, dataLength);
    kfree(send_msg);
    return ret;
}

static int read_tuner_cfg(struct device_node * signalhub_node, signalhub_platform_data* signalhub_data)
{
    /* tuner_cfg */
    struct device_node *tuner_cfg0_node = of_get_child_by_name(signalhub_node, "tuner_cfg0");
    if (tuner_cfg0_node == NULL) {
        hwlog_err("signalhub %s: Failed to find tuner_cfg0 node\n", __func__);
        return FAIL;
    }
    read_mipi_res(tuner_cfg0_node, 0, signalhub_data);
    read_cfg_res(tuner_cfg0_node, 0, signalhub_data);

    struct device_node *tuner_cfg1_node = of_get_child_by_name(signalhub_node, "tuner_cfg1");
    if (tuner_cfg1_node == NULL) {
        hwlog_err("signalhub %s: Failed to find tuner_cfg1 node\n", __func__);
        return FAIL;
    }
    read_mipi_res(tuner_cfg1_node, 1, signalhub_data);
    read_cfg_res(tuner_cfg1_node, 1, signalhub_data);
    
    of_node_put(tuner_cfg0_node);
    of_node_put(tuner_cfg1_node);

    return SUCCESS;
}

static int read_signalhub_common_cfg(struct device_node *dn, signalhub_platform_data* signalhub_data)
{
    unsigned int i;
    unsigned int len;
    struct property *prop = NULL;

    /* ant map */
    uint32_t ant_map[MAX_ANT_MAP_SIZE] = {0};
    prop = of_find_property(dn, "ant_map", NULL);
    if (!prop) {
        hwlog_err("signalhub %s:  prop is NULL\n", __func__);
        return FAIL;
    }
    len = (uint32_t)(prop->length) / sizeof(uint32_t);
    if (len > MAX_ANT_MAP_SIZE) {
        hwlog_err("signalhub %s: length of ant_map is over, len=%d\n", __func__, len);
        return FAIL;
    }
    if (of_property_read_u32_array(dn, "ant_map", ant_map, len)) {
        hwlog_err("signalhub %s: read ant_map from dts fail\n", __func__);
        return FAIL;
    }
    for (i = 0; i < len; i++) {
        signalhub_data->ant_map[i] = (uint8_t)ant_map[i];
        hwlog_info("signalhub %s: ant_map[%d] =%u\n", __func__, i, signalhub_data->ant_map[i]);
    }

    /* mipi driver */
    uint32_t mipi_driver_cfg[MAX_MIPI_DRIVER_CFG_SIZE] = {0};
    prop = of_find_property(dn, "mipi_driver_cfg", NULL);
    if (!prop) {
        hwlog_err("signalhub %s:  prop is NULL\n", __func__);
        return FAIL;
    }
    len = (uint32_t)(prop->length) / sizeof(uint32_t);
    if (len > MAX_MIPI_DRIVER_CFG_SIZE) {
        hwlog_err("signalhub %s: length of mipi_driver_cfg is over, len=%d\n", __func__, len);
        return FAIL;
    }
    if (of_property_read_u32_array(dn, "mipi_driver_cfg", mipi_driver_cfg, len)) {
        hwlog_err("signalhub %s: read mipi_driver_cfg from dts fail\n", __func__);
        return FAIL;
    }
    
    signalhub_data->mipi_driver_cfg.mipi_pin_id = (uint16_t)mipi_driver_cfg[0];
    signalhub_data->mipi_driver_cfg.mipi_div_freq = (uint16_t)mipi_driver_cfg[1];
    hwlog_info("signalhub %s: mipi_div_freq =%u\n", __func__, signalhub_data->mipi_driver_cfg.mipi_pin_id);
    hwlog_info("signalhub %s: mipi_pin_id =%u\n", __func__, signalhub_data->mipi_driver_cfg.mipi_div_freq);
    
    return SUCCESS;
}

int init_config_from_dts(void)
{
    int ret;
    char *sensor_st = NULL;
    signalhub_platform_data* signalhub_data =
        (signalhub_platform_data*)kmalloc(sizeof(signalhub_platform_data), GFP_KERNEL);
    if (signalhub_data == NULL) {
        hwlog_err("signalhub %s: malloc signalhub_data fail\n", __func__);
        return FAIL;
    }
    ret = memset_s(signalhub_data, sizeof(signalhub_data), 0, sizeof(signalhub_data));
    if (ret) {
        hwlog_err("signalhub %s: memset signalhub_data fail\n", __func__);
        kfree(signalhub_data);
        return FAIL;
    }
    struct device_node *signalhub_node = of_find_compatible_node(NULL, NULL, "huawei,signalhub");
    if (signalhub_node == NULL) {
        hwlog_err("signalhub %s: Failed to find signalhub node\n", __func__);
        kfree(signalhub_data);
        return FAIL;
    }

    /* status: ok or disabled */
    if (of_property_read_string(signalhub_node, "status", (const char **)&sensor_st)) {
        hwlog_err("signalhub %s: get sensor status fail\n", __func__);
        kfree(signalhub_data);
        return FAIL;
    }
    hwlog_info("signalhub %s: read status config successful! status =%s\n", __func__, sensor_st);
    if (strncmp("ok", sensor_st, strlen(sensor_st))) {
        hwlog_info("%s : sensor %s status is %s\n", sensor_st);
        kfree(signalhub_data);
        return FAIL;
    }

    ret = read_signalhub_common_cfg(signalhub_node, signalhub_data);
    if (ret) {
        hwlog_err("signalhub %s: Failed to read signalhub data from dts!\n", __func__);
        kfree(signalhub_data);
        return FAIL;
    }

    ret = read_tuner_cfg(signalhub_node, signalhub_data);
    if (ret) {
        hwlog_err("signalhub %s: Failed to read tuner_cfg from dts!\n", __func__);
        kfree(signalhub_data);
        return FAIL;
    }

    of_node_put(signalhub_node);
    signalhub_send_dts_cfg(signalhub_data);
    kfree(signalhub_data);

    return SUCCESS;
}