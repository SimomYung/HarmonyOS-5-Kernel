/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <asm/current.h>
#include <linux/atomic.h>
#include <linux/socket.h>
#include <linux/bitops.h>
#include <net/tcp.h>
#include <linux/net_monitor_hook.h>
#include <securec.h>
#include "hook.h"
#include "debug.h"

static struct option_count *g_option_table = NULL;

static int net_monitor_level_table_init(void)
{
    g_option_table = (struct option_count *)kzalloc(sizeof(struct option_count), GFP_KERNEL);
    if (g_option_table == NULL) {
        return -ENOMEM;
    }
    bsp_err("Total table size is %d bytes.\n", sizeof(struct option_count));
    return 0;
}

struct option_count *net_monitor_get_option_count(void)
{
    return g_option_table;
}

static void socket_opt_add(int opt) {
    struct option_count *table = g_option_table;
    if ((0 < opt) && (opt <= SO_OPT_LEVEL_MAX)) {
        table->so_count[opt - 1]++;
    }
}

static void tcp_opt_add(int opt) {
    struct option_count *table = g_option_table;
    if ((0 < opt) && (opt <= TCP_OPT_LEVEL_MAX)) {
        table->tcp_count[opt - 1]++;
    }
}

static void udp_opt_add(int opt) {
    struct option_count *table = g_option_table;
    if ((0 < opt) && (opt <= UDP_OPT_LEVEL_MAX)) {
        table->udp_count[opt - 1]++;
    }
}

static void ip_opt_add(int opt) {
    struct option_count *table = g_option_table;
    if ((0 < opt) && (opt <= IP_OPT_LEVEL_MAX)) {
        table->ip_count[opt - 1]++;
    }
    if ((MRT_BASE <= opt) && (opt <= MRT_MAX)) {
        table->ipmr_count[opt - MRT_BASE]++;
    }
}

static void ip6_opt_add(int opt) {
    struct option_count *table = g_option_table;
    if ((0 < opt) && (opt <= IPV6_OPT_LEVEL_MAX)) {
        table->ipv6_count[opt - 1]++;
    }
    if ((MRT6_BASE <= opt) && (opt <= MRT6_MAX)) {
        table->ipmr6_count[opt - MRT6_BASE]++;
    }
}

static void icmp6_opt_add(int opt) {
    struct option_count *table = g_option_table;
    if ((0 < opt) && (opt <= IPV6_OPT_LEVEL_MAX)) {
        table->icmpv6_count[opt - 1]++;
    }

}

static int net_monitor_level_table_add(int level, int opt) {
    switch (level) {
        case SOL_SOCKET:
            socket_opt_add(opt);
            break;
        case SOL_TCP:
            tcp_opt_add(opt);
            break;
        case SOL_UDP:
            udp_opt_add(opt);
            break;
        case SOL_IP:
            ip_opt_add(opt);
            break;
        case SOL_IPV6:
            ip6_opt_add(opt);
            break;
        case SOL_ICMPV6:
            icmp6_opt_add(opt);
            break;
        default:
            break;
    }

    return 0;
}

static int net_monitor_cmd_table_add(int cmd) {
    struct option_count *table = g_option_table;
    if ((cmd & 0xff00) >> 16 != SOCK_IOC_TYPE) {
        return -EINVAL;
    }
    table->ioctl_count[cmd & 0xff]++;
    return 0;
}

static int net_monitor_flag_table_add(unsigned int flag) {
    unsigned int bit;
    unsigned long bitmap = flag;
    struct option_count *table = g_option_table;
    for_each_set_bit(bit, &bitmap, FLAG_MAX) {
        table->flag_count[bit]++;
    }
    return 0;
}

static void sys_setsockopt_callback(void **para_stack, int size)
{
    int level, option, family;

    if (size < 3) {
        return;
    }

    family = *((unsigned short *)para_stack[2]);
    if ((family != AF_INET) && (family != AF_INET6)) {
        return;
    }

    level = *((int *)para_stack[0]);
    option = *((int *)para_stack[1]);

    net_monitor_level_table_add(level, option);
}

static void __sys_getsockopt_callback(void **para_stack, int size)
{
    int level, option, family;

    if (size < 3) {
        return;
    }

    family = *((unsigned short *)para_stack[2]);
    if ((family != AF_INET) && (family != AF_INET6)) {
        return;
    }

    level = *((int *)para_stack[0]);
    option = *((int *)para_stack[1]);

    if ((family != AF_INET) && (family != AF_INET6)) {
        return;
    }

    level = *((int *)para_stack[0]);
    option = *((int *)para_stack[1]);

    net_monitor_level_table_add(level, option);
}

static void compat_sock_ioctl_callback(void **para_stack, int size)
{
    int cmd, family;

    if (size < 3) {
        return;
    }

    family = *((unsigned short *)para_stack[2]);
    if ((family != AF_INET) && (family != AF_INET6)) {
        return;
    }

    cmd = *((int *)para_stack[0]);

    net_monitor_cmd_table_add(cmd);
}

static void sys_msg_callback(void **para_stack, int size)
{
    unsigned short family;
    unsigned int flag;
    if (size < 2) {
        return;
    }
    family = *((unsigned short *)para_stack[0]);
    if ((family != AF_INET) && (family != AF_INET6)) {
        return;
    }

    flag = *((unsigned int *)para_stack[1]);
    net_monitor_flag_table_add(flag);
}

int net_monitor_socket_opt_init(void)
{
    if (net_monitor_level_table_init()) {
        return -1;
    }

    net_monitor_hook_register(NM_ENUM(__sys_setsockopt), sys_setsockopt_callback);
    net_monitor_hook_register(NM_ENUM(__sys_getsockopt), __sys_getsockopt_callback);
    net_monitor_hook_register(NM_ENUM(compat_sock_ioctl), compat_sock_ioctl_callback);
    net_monitor_hook_register(NM_ENUM(___sys_sendmsg), sys_msg_callback);
    net_monitor_hook_register(NM_ENUM(___sys_recvmsg), sys_msg_callback);

    return 0;
}
