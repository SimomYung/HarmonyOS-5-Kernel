/*
* Copyright (c) 2017-2025 Huawei Device Co., Ltd.
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
 */

#ifndef __WIFI_FILTER_H__
#define __WIFI_FILTER_H__


#include <net/ip.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_ipv4/ip_tables.h>
typedef struct {
    unsigned short protocol;    //Protocol type
    unsigned short port;  //Destination Port Number
    unsigned int filter_cnt;    //Number of filtered messages
//    unsigned int uid;
} hw_wifi_filter_item;

struct hw_wlan_filter_ops{
    int (*set_filter_enable)(int);
    int (*set_filter_enable_ex)(int, int);
    int (*add_filter_items)(hw_wifi_filter_item*, int);
    int (*clear_filters)(void);
    int (*get_filter_pkg_stat)(hw_wifi_filter_item*, int, int*);
};


/******************************************************************************
   2 Function declaration
******************************************************************************/
/*---------driver----------*/
int hw_register_wlan_filter(struct hw_wlan_filter_ops *ops);

int hw_unregister_wlan_filter(void);

/*-------re-enable wifi filter after wifi restart-------*/
int hw_set_net_filter_re_enable(int state);

int hw_set_net_filter_user_sleep(int sleep);

/*---------wl----------*/

int hw_set_net_filter_enable(int enable);
/*-----kernel----*/
void get_filter_info(
    struct sk_buff *skb,
    const struct nf_hook_state *state,
    unsigned int hook,
    const struct xt_table_info *private,
    const struct ipt_entry *e);

void get_filter_infoEx(struct sk_buff *skb);
void get_pg_app_info(int port, int protocol);
#endif

