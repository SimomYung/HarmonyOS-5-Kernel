// SPDX-License-Identifier: GPL-2.0
/*
 * bk96285_qi.c
 *
 * bk96285 qi_protocol driver; ask: rx->tx; fsk: tx->rx
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
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

#include "bk96285.h"

#define HWLOG_TAG wireless_bk96285_qi
HWLOG_REGIST();

static u8 bk96285_get_ask_header(struct bk96285_dev_info *di, int data_len)
{
	if (!di || !di->g_val.qi_hdl || !di->g_val.qi_hdl->get_ask_hdr) {
		hwlog_err("get_ask_header: para null\n");
		return 0;
	}

	return di->g_val.qi_hdl->get_ask_hdr(data_len);
}

static u8 bk96285_get_fsk_header(struct bk96285_dev_info *di, int data_len)
{
	if (!di || !di->g_val.qi_hdl || !di->g_val.qi_hdl->get_fsk_hdr) {
		hwlog_err("get_fsk_header: para null\n");
		return 0;
	}

	return di->g_val.qi_hdl->get_fsk_hdr(data_len);
}

static int bk96285_qi_send_ask_msg(u8 cmd, u8 *data, int data_len, void *dev_data)
{
	int ret;
	u8 header;
	struct bk96285_dev_info *di = dev_data;

	if (!di) {
		hwlog_err("send_ask_msg: para null\n");
		return -ENODEV;
	}
	if ((data_len > BK96285_RX_SEND_MSG_DATA_LEN) || (data_len < 0)) {
		hwlog_err("send_ask_msg: data number out of range %d\n", data_len);
		return -EINVAL;
	}

	di->irq_val &= ~BK96285_RX_IRQ_RECV_ACK;
	di->irq_val &= ~BK96285_RX_IRQ_FSK_PKG_RECV;
	/* msg_len=cmd_len+data_len  cmd_len=1 */
	header = bk96285_get_ask_header(di, data_len + 1);
	if (header <= 0) {
		hwlog_err("send_ask_msg: header wrong\n");
		return -EINVAL;
	}
	ret = bk96285_write_byte(di, BK96285_TRX_ASK_PKG_HEADER_ADDR, header);
	if (ret) {
		hwlog_err("send_ask_msg: write header failed\n");
		return ret;
	}
	ret = bk96285_write_byte(di, BK96285_TRX_ASK_PKG_CMD_ADDR, cmd);
	if (ret) {
		hwlog_err("send_ask_msg: write cmd failed\n");
		return ret;
	}
	if (data && (data_len > 0)) {
		ret = bk96285_write_register(di, BK96285_TRX_ASK_PKG_DATA_ADDR, data, data_len);
		if (ret) {
			hwlog_err("send_ask_msg: write rx2tx-reg failed\n");
			return -EIO;
		}
	}

	ret = bk96285_write_dword_mask(di, BK96285_RX_CMD_ADDR,
		BK96285_RX_CMD_SEND_PPP_MASK, BK96285_RX_CMD_SEND_PPP_SHIFT,
		BK96285_RX_CMD_EN);
	if (ret) {
		hwlog_err("send_ask_msg: send rx msg to tx failed\n");
		return -EIO;
	}

	hwlog_info("[send_ask_msg] succ, cmd=0x%x len=%d\n", cmd, data_len);
	return 0;
}

static int bk96285_qi_wait_fsk_ack(struct bk96285_dev_info *di)
{
	int j;
	u8 val = 0;

	for (j = 0; j < BK96285_WAIT_FOR_ACK_RETRY_CNT; j++) {
		power_msleep(DT_MSLEEP_100MS, 0, NULL);
		if (di->irq_val & BK96285_RX_IRQ_RECV_ACK) {
			di->irq_val &= ~BK96285_RX_IRQ_RECV_ACK;
			hwlog_info("[send_ask_msg_ack] succ\n");
			return 0;
		}
		if (di->irq_val & BK96285_RX_IRQ_FSK_PKG_RECV) {
			di->irq_val &= ~BK96285_RX_IRQ_FSK_PKG_RECV;
			(void)bk96285_read_byte(di, BK96285_TRX_FSK_PKG_CMD_ADDR, &val);
			if (val == HWQI_CMD_ND)
				return 0;
		}
		if (di->g_val.rx_stop_chrg)
			return -EPERM;
	}

	return BK96285_QI_RETRY;
}

static int bk96285_qi_send_ask_msg_ack(u8 cmd, u8 *data, int data_len, void *dev_data)
{
	int ret;
	int i;
	u8 val = 0;
	struct bk96285_dev_info *di = dev_data;

	if (!di) {
		hwlog_err("send_ask_msg_ack: para null\n");
		return -ENODEV;
	}

	for (i = 0; i < BK96285_SNED_MSG_RETRY_CNT; i++) {
		ret = bk96285_qi_send_ask_msg(cmd, data, data_len, di);
		if (ret)
			continue;
		ret = bk96285_qi_wait_fsk_ack(di);
		if (ret != BK96285_QI_RETRY)
			return ret;

		hwlog_info("[send_ask_msg_ack] retry, cnt=%d\n", i);
	}

	ret = bk96285_read_byte(di, BK96285_TRX_FSK_PKG_CMD_ADDR, &val);
	if (ret) {
		hwlog_err("send_ask_msg_ack: get rcv cmd data failed\n");
		return -EIO;
	}

	if ((val != HWQI_CMD_ACK) && (val != HWQI_CMD_ND)) {
		hwlog_err("send_ask_msg_ack: failed, ack=0x%x, cnt=%d\n",
			val, i);
		return -EIO;
	}

	return 0;
}

static int bk96285_qi_receive_fsk_msg(u8 *data, int data_len, void *dev_data)
{
	int ret;
	int cnt = 0;
	struct bk96285_dev_info *di = dev_data;

	if (!di || !data) {
		hwlog_err("receive_fsk_msg: para null\n");
		return -EINVAL;
	}

	do {
		if (di->irq_val & BK96285_RX_IRQ_FSK_PKG_RECV) {
			di->irq_val &= ~BK96285_RX_IRQ_FSK_PKG_RECV;
			goto func_end;
		}

		if (di->g_val.rx_stop_chrg)
			return -EPERM;
		power_msleep(DT_MSLEEP_100MS, 0, NULL);
		cnt++;
	} while (cnt < BK96285_RCV_MSG_SLEEP_CNT);

func_end:
	ret = bk96285_read_register(di, BK96285_TRX_FSK_PKG_CMD_ADDR, data, data_len);
	if (ret) {
		hwlog_err("receive_fsk_msg: get tx2rx data failed\n");
		return -EIO;
	}

	if (!data[0]) { /* data[0]: cmd */
		hwlog_err("receive_fsk_msg: no msg received from tx\n");
		return -EIO;
	}

	hwlog_info("[receive_fsk_msg] get tx2rx data(cmd:0x%x) succ\n", data[0]);
	return 0;
}

static int bk96285_qi_send_fsk_msg(u8 cmd, u8 *data, int data_len, void *dev_data)
{
	int ret, i;
	u8 header;
	struct bk96285_dev_info *di = dev_data;

	if (!di) {
		hwlog_err("send_fsk_msg: para null\n");
		return -ENODEV;
	}

	if ((data_len > BK96285_RX_SEND_MSG_DATA_LEN) || (data_len < 0)) {
		hwlog_err("send_fsk_msg: data number out of range %d\n", data_len);
		return -EINVAL;
	}

	if (cmd == HWQI_CMD_ACK)
		header = HWQI_CMD_ACK_HEAD;
	else
		header = bk96285_get_fsk_header(di, data_len + 1);
	if (header <= 0) {
		hwlog_err("send_fsk_msg: header wrong\n");
		return -EINVAL;
	}

	ret = bk96285_write_byte(di, BK96285_TRX_FSK_PKG_HEADER_ADDR, header);
	if (ret) {
		hwlog_err("send_fsk_msg: write header failed\n");
		return ret;
	}
	ret = bk96285_write_byte(di, BK96285_TRX_FSK_PKG_CMD_ADDR, cmd);
	if (ret) {
		hwlog_err("send_fsk_msg: write cmd failed\n");
		return ret;
	}
	hwlog_info("send_fsk_msg: header=0x%x, cmd=0x%x\n", header, cmd);
	if (data && (data_len > 0)) {
		ret = bk96285_write_register(di, BK96285_TRX_FSK_PKG_DATA_ADDR, data, data_len);
		if (ret) {
			hwlog_err("send_fsk_msg: write rx2tx-reg failed\n");
			return -EIO;
		}

		for (i = 0; i < data_len; i++)
			hwlog_info("send_fsk_msg: data[%d]=0x%x\n", i, data[i]);
	}

	ret += bk96285_write_dword(di, BK96285_TX_CMD_ADDR, BK96285_TX_CMD_SEND_FSK);
	if (ret) {
		hwlog_err("send_ask_msg: send rx msg to tx failed\n");
		return -EIO;
	}

	hwlog_info("[send_ask_msg] succ, cmd=0x%x len=%d\n", cmd, data_len);

	return 0;
}

static int bk96285_qi_send_fsk_with_ack(u8 cmd, u8 *data, int data_len, void *dev_data)
{
	int ret;
	int i, j;
	u8 val = 0;
	struct bk96285_dev_info *di = dev_data;

	if (!di) {
		hwlog_err("send_fsk_msg_ack: para null\n");
		return -ENODEV;
	}

	di->irq_val &= ~BK96285_TX_IRQ_ASK_PKT_RCVD;
	for (i = 0; i < BK96285_SNED_MSG_RETRY_CNT; i++) {
		ret = bk96285_qi_send_fsk_msg(cmd, data, data_len, di);
		if (ret)
			continue;
		for (j = 0; j < BK96285_WAIT_FOR_ACK_RETRY_CNT; j++) {
			power_msleep(DT_MSLEEP_100MS, 0, NULL);
			if (di->irq_val & BK96285_TX_IRQ_ASK_PKT_RCVD) {
				di->irq_val &= ~BK96285_TX_IRQ_ASK_PKT_RCVD;
				hwlog_info("[send_fsk_msg_ack] succ\n");
				return 0;
			}
			if (di->g_val.tx_stop_chrg)
				return -EPERM;
		}
		hwlog_info("[send_fsk_msg_ack] retry, cnt=%d\n", i);
	}

	ret = bk96285_read_byte(di, BK96285_TRX_ASK_PKG_CMD_ADDR, &val);
	if (ret) {
		hwlog_err("send_fsk_msg_ack: get rcv cmd data failed\n");
		return -EIO;
	}

	if ((val != HWQI_CMD_ACK) && (val != HWQI_CMD_ND)) {
		hwlog_err("send_fsk_msg_ack: failed, ack=0x%x, cnt=%d\n", val, i);
		return -EIO;
	}

	return 0;
}

static int bk96285_qi_receive_ask_pkt(u8 *pkt, int pkt_len, void *dev_data)
{
	int ret;
	int i;
	char buff[BK96285_RCVD_PKT_BUFF_LEN] = { 0 };
	char pkt_str[BK96285_RCVD_PKT_STR_LEN] = { 0 };
	struct bk96285_dev_info *di = dev_data;

	if (!di || !pkt || (pkt_len <= 0) || (pkt_len > BK96285_RCVD_MSG_PKT_LEN)) {
		hwlog_err("get_ask_pkt: para err\n");
		return -EINVAL;
	}

	ret = bk96285_read_register(di, BK96285_TRX_ASK_PKG_HEADER_ADDR, pkt, pkt_len);
	if (ret) {
		hwlog_err("get_ask_pkt: read failed\n");
		return ret;
	}

	for (i = 0; i < pkt_len; i++) {
		ret = snprintf_s(buff, BK96285_RCVD_PKT_BUFF_LEN,
			BK96285_RCVD_PKT_BUFF_LEN - 1, "0x%02x ", pkt[i]);
		if (ret < 0)
			return ret;
		ret = strncat_s(pkt_str, BK96285_RCVD_PKT_STR_LEN - 1, buff, strlen(buff));
		if (ret)
			return ret;
	}


	hwlog_info("[get_ask_pkt] RX back packet: %s\n", pkt_str);
	return 0;
}

static int bk96285_get_chip_fw_version(u8 *data, int len, void *dev_data)
{
	struct bk96285_chip_info chip_info = { 0 };
	struct bk96285_dev_info *di = dev_data;

	/* fw version length must be 4 */
	if (!di || !data || (len != 4)) {
		hwlog_err("get_chip_fw_version: para err\n");
		return -EINVAL;
	}

	if (bk96285_get_chip_info(di, &chip_info)) {
		hwlog_err("get_chip_fw_version: get chip info failed\n");
		return -EIO;
	}

	/* byte[0:1]=chip_id, byte[2:3]=mtp_ver */
	data[0] = (u8)((chip_info.chip_id >> 0) & POWER_MASK_BYTE);
	data[1] = (u8)((chip_info.chip_id >> POWER_BITS_PER_BYTE) & POWER_MASK_BYTE);
	data[2] = (u8)((chip_info.mtp_ver >> 0) & POWER_MASK_BYTE);
	data[3] = (u8)((chip_info.mtp_ver >> POWER_BITS_PER_BYTE) & POWER_MASK_BYTE);

	return 0;
}

static int bk96285_qi_set_rx_rpp_format(struct bk96285_dev_info *di, u8 pmax)
{
	int ret;

	ret = bk96285_write_byte(di, BK96285_RX_RP_PMAX_ADDR, pmax);
	ret = bk96285_write_dword_mask(di, BK96285_RX_CMD_ADDR,
		BK96285_RX_CMD_24BIT_RPP_MASK, BK96285_RX_CMD_24BIT_RPP_SHIFT, BK96285_RX_CMD_EN);
	if (ret) {
		hwlog_err("set_rx_rpp_format: failed\n");
		return -EIO;
	}

	hwlog_info("[set_rx_rpp_format] succ\n");
	return 0;
}

static int bk96285_qi_set_rpp_format(u8 pmax, int mode, void *dev_data)
{
	struct bk96285_dev_info *di = dev_data;

	if (!di)
		return -ENODEV;

	if (mode == WIRELESS_RX)
		return bk96285_qi_set_rx_rpp_format(di, pmax);

	return 0;
}

static struct hwqi_ops g_bk96285_qi_ops = {
	.chip_name              = "bk96285",
	.send_msg               = bk96285_qi_send_ask_msg,
	.send_msg_with_ack      = bk96285_qi_send_ask_msg_ack,
	.receive_msg            = bk96285_qi_receive_fsk_msg,
	.send_fsk_msg           = bk96285_qi_send_fsk_msg,
	.auto_send_fsk_with_ack = bk96285_qi_send_fsk_with_ack,
	.get_ask_packet         = bk96285_qi_receive_ask_pkt,
	.get_chip_fw_version    = bk96285_get_chip_fw_version,
	.get_tx_id_pre          = NULL,
	.set_rpp_format_post    = bk96285_qi_set_rpp_format,
};

int bk96285_qi_ops_register(struct wltrx_ic_ops *ops, struct bk96285_dev_info *di)
{
	if (!ops || !di)
		return -ENODEV;

	ops->qi_ops = kzalloc(sizeof(*(ops->qi_ops)), GFP_KERNEL);
	if (!ops->qi_ops)
		return -ENODEV;

	(void)memcpy_s(ops->qi_ops, sizeof(g_bk96285_qi_ops),
		&g_bk96285_qi_ops, sizeof(g_bk96285_qi_ops));
	ops->qi_ops->dev_data = (void *)di;
	return hwqi_ops_register(ops->qi_ops, di->ic_type);
}
