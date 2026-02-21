/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : WAL linux桥接文件
 * 作    者 : wifi
 * 创建日期 : 2012年11月19日
 */

#include "oam_stat_wifi.h"
#include "frw_ext_if.h"

#include "hmac_vap.h"
#include "mac_vap.h"
#include "mac_resource.h"
#include "mac_data.h"
#include "hmac_ext_if.h"

#include "wal_main.h"
#include "wal_linux_bridge.h"

#include "hmac_tx_data.h"
#include "hmac_device.h"
#include "hmac_resource.h"
#include "hmac_mintp.h"
#ifdef _PRE_WLAN_RR_PERFORMENCE_DEBUG
#include "hmac_rr_performance.h"
#endif

#ifdef CONFIG_ATP_SERVICE_CLASSIFY
#include "atp_skb_mark.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_LINUX_BRIDGE_C

#ifdef _PRE_WLAN_FEATURE_ESPE
/* E5硬转清除标志位和dma地址的接口 */
void bsp_wifi_hac_reset_hac_own(struct sk_buff *skb);
#endif

oal_net_dev_tx_enum wal_bridge_vap_xmit(oal_netbuf_stru *netbuf, oal_net_device_stru *net_device)
{
    if (oal_unlikely(netbuf == NULL)) {
        oam_error_log0(0, OAM_SF_TX, "{hmac_bridge_vap_xmit::netbuf NULL!}");
        return OAL_NETDEV_TX_OK;
    }

    if (oal_unlikely(net_device == NULL)) {
        oam_error_log0(0, OAM_SF_TX, "{hmac_bridge_vap_xmit::net_device NULL!}");
        oal_netbuf_free(netbuf);
        oam_stat_vap_incr(0, tx_abnormal_msdu_dropped, 1);
        return OAL_NETDEV_TX_OK;
    }
#ifdef _PRE_WLAN_FEATURE_ESPE
    bsp_wifi_hac_reset_hac_own(netbuf);
#endif
    host_start_record_performance(TX_XMIT_PROC);

    /*
     * NON-MINTP(main scene): hmac_bridge_vap_xmit
     * MINTP: ring tx --> hmac_bridge_vap_xmit_mintp; non-ring tx --> hmac_bridge_vap_xmit
     */
#ifdef CONFIG_ATP_SERVICE_CLASSIFY
    if (atp_common_skb_proc_hook(netbuf, 1) != OAL_SUCC) {
        return OAL_NETDEV_TX_OK;  /* 返回失败，产品内部进行内存释放 */
    }
#endif
    if (oal_likely(netbuf->protocol != htons(ETH_P_D2D)) || !hmac_ring_tx_enabled()) {
        return hmac_bridge_vap_xmit(netbuf, net_device);
    } else {
        return hmac_bridge_vap_xmit_mintp(netbuf, net_device);
    }
}
