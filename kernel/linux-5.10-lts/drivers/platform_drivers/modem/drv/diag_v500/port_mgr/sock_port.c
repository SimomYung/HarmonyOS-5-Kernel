/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
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

#include "sock_port.h"
#include "bsp_print.h"
#include <securec.h>
#include <linux/socket.h>
#include <osl_thread.h>
#include "bsp_dfxcert.h"
#include <bsp_version.h>
#include <osl_sem.h>
#include <mdrv_socket.h>
#include <linux/net.h>
#include <linux/in.h>
#include <linux/netdevice.h>
#include <linux/inetdevice.h>
#include <osl_malloc.h>
#include "diag_debug.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define SOCKNAME_LEN 32
#define SOCK_NUM_MAX 1
#define SOCK_BUFSIZE 1000
#define SOCK_UDP_PACKAGE 65507

typedef struct {
    unsigned long long client_fd; // tcp accept client fd
    wait_queue_head_t wq;
    unsigned int ip_addr;
} sock_tcp_s;

typedef struct {
    int family;
    int type;
    int protocol;
    int port;
    struct sockaddr sock_addr;
    OSL_TASK_ID taskid;
    unsigned long long fd;
    sock_tcp_s tcp_ctrl;
    void *recv_buf;
    unsigned int recv_buf_len;
    sock_recv_cb recvcb;
    sock_event_cb eventcb;
    spinlock_t lock;
    enum sock_state state;
    char name[SOCKNAME_LEN];
    sock_chan_e chan;
    int init_ok;
} sock_port_chn_ctrl_s;

typedef struct {
    bool can_conn;
    struct notifier_block ipaddr_notifier;
    struct notifier_block dfxcert_notifer; /* diag socket查询socket端口是否可用 */
    sock_port_chn_ctrl_s port_chn[SOCK_PORT_CHAN_BUTT];
} diag_sock_ctrl_s;

diag_sock_ctrl_s g_sock_ctrl = {
    .port_chn[SOCK_CNF_CODER_CHAN] = {
        .family = AF_INET,
        .type = SOCK_STREAM,
        .protocol = 0,
        .port = 3000,
        .recv_buf_len = SOCK_BUFSIZE,
        .name = "sock_cnf",
        .chan = SOCK_CNF_CODER_CHAN,
    },
    .port_chn[SOCK_IND_CODER_CHAN] = {
        .family = AF_INET,
        .type = SOCK_DGRAM,
        .protocol = 0,
        .port = 20253,
        .recv_buf_len = SOCK_BUFSIZE,
        .chan = SOCK_IND_CODER_CHAN,
    },
    .port_chn[SOCK_AT_CHAN] = {
        .family = AF_INET,
        .type = SOCK_STREAM,
        .protocol = 0,
        .name = "sock_at",
        .chan = SOCK_AT_CHAN,
    },
    .port_chn[SOCK_EQUIP_CHAN] = {
        .family = AF_INET,
        .type = SOCK_STREAM,
        .protocol = 0,
        .name = "sock_equip",
        .chan = SOCK_EQUIP_CHAN,
    },
};

int sock_bind_listen(sock_port_chn_ctrl_s *chn)
{
    int ret;
    sock_tcp_s *tcp_ctrl = &chn->tcp_ctrl;
    struct sockaddr_in *skaddr = (struct sockaddr_in *)&chn->sock_addr;

    skaddr->sin_family = chn->family;
    skaddr->sin_port = htons(chn->port);
    skaddr->sin_addr.s_addr = tcp_ctrl->ip_addr;
    ret = mdrv_bind((int)chn->fd, (struct sockaddr *)skaddr, sizeof(struct sockaddr));
    if (ret) {
        diag_err("sock bind fail, ret:%d\n", ret);
        return ret;
    }
    ret = mdrv_listen((int)chn->fd, SOCK_NUM_MAX);
    if (ret) {
        diag_err("sock listen fail, ret:%d\n", ret);
        return ret;
    }
    diag_debug("bind port:%d, listen succ\n", skaddr->sin_port);
    return 0;
}

void sock_bind_udpchan(sock_port_chn_ctrl_s *ctrl, int sock_fd)
{
    int ret;
    int len = (int)sizeof(struct sockaddr_in);
    struct sockaddr_in *sock_addr = NULL;
    unsigned long flags;

    ret = mdrv_getpeername(sock_fd, (struct sockaddr *)&g_sock_ctrl.port_chn[SOCK_IND_CODER_CHAN].sock_addr, &len);
    if (ret != 0) {
        diag_err("bind pair socket get peer name fail, ret:%d\n", ret);
        return;
    }
    sock_addr = (struct sockaddr_in *)&g_sock_ctrl.port_chn[SOCK_IND_CODER_CHAN].sock_addr;
    sock_addr->sin_port = htons(g_sock_ctrl.port_chn[SOCK_IND_CODER_CHAN].port);
    spin_lock_irqsave(&ctrl->lock, flags);
    g_sock_ctrl.port_chn[SOCK_IND_CODER_CHAN].state = SOCK_CONNECT;
    spin_unlock_irqrestore(&ctrl->lock, flags);
    diag_err("modify udp port:%d, addr:0x%x\n", sock_addr->sin_port, sock_addr->sin_addr.s_addr);
}

// 默认cnf通道tcp连接accpet，更新整个sock通道状态
int sock_tcp_connect(sock_port_chn_ctrl_s *ctrl, int *acceptfd)
{
    struct sockaddr_in from = {0};
    int len = (int)sizeof(struct sockaddr_in);
    int tmpfd;
    int oldfd = *acceptfd;
    unsigned long flags;

    tmpfd = mdrv_accept((int)ctrl->fd, (struct sockaddr *)&from, &len);
    if (tmpfd < 0) {
        diag_err("accept fail, ret:%d\n", tmpfd);
        return -1;
    }
    diag_err("accept from port:0x%x, addr:0x%x\n", from.sin_port, from.sin_addr.s_addr);
    if (oldfd != -1) {
        mdrv_shutdown(oldfd, SHUT_RDWR);
        mdrv_close(oldfd);
        diag_err("new accept fd:%d connected, closed old accept fd:%d\n", tmpfd, oldfd);
    }
    *acceptfd = tmpfd;
    ctrl->tcp_ctrl.client_fd = tmpfd;

    spin_lock_irqsave(&ctrl->lock, flags);
    ctrl->state = SOCK_CONNECT;
    spin_unlock_irqrestore(&ctrl->lock, flags);
    if (ctrl->eventcb != NULL) {
        ctrl->eventcb(ctrl->state);
    }

    // IND通道udp协议只有发给工具方向，其sendto对端地址来自这里tcp连接获取到的ip
    if (ctrl->chan == SOCK_CNF_CODER_CHAN) {
        sock_bind_udpchan(ctrl, *acceptfd);
    }
    return 0;
}

void sock_notify_ind_chn_disconn(void)
{
    unsigned long flags;
    sock_port_chn_ctrl_s *ctrl = &g_sock_ctrl.port_chn[SOCK_IND_CODER_CHAN];

    spin_lock_irqsave(&ctrl->lock, flags);
    ctrl->state = SOCK_DISCONNECT;
    spin_unlock_irqrestore(&ctrl->lock, flags);
}

// 负责将cnf、ind对应的链接全部断开,实现更新状态
void sock_tcp_disconnect(sock_port_chn_ctrl_s *ctrl)
{
    unsigned long flags;

    mdrv_shutdown(ctrl->tcp_ctrl.client_fd, SHUT_RDWR);
    mdrv_close(ctrl->tcp_ctrl.client_fd);
    ctrl->tcp_ctrl.client_fd = -1;
    diag_err("acceptfd recv done, already closed\n");

    if (ctrl->chan == SOCK_CNF_CODER_CHAN) {
        sock_notify_ind_chn_disconn();
    }

    spin_lock_irqsave(&ctrl->lock, flags);
    ctrl->state = SOCK_DISCONNECT;
    spin_unlock_irqrestore(&ctrl->lock, flags);
    if (ctrl->eventcb != NULL) {
        ctrl->eventcb(ctrl->state);
    }
}

int sock_tcp_accept_recv(sock_port_chn_ctrl_s *chn_ctrl, sock_tcp_s *tcp_ctrl)
{
    sk_fd_set fdmap = {0};
    int listenfd = (int)chn_ctrl->fd;
    int acceptfd = -1;
    int ret, maxfd;

    for (;;) {
        fdmap.fds_bits = 0;
        SK_FD_SET(listenfd, &fdmap);
        if (acceptfd != -1) {
            SK_FD_SET(acceptfd, &fdmap);
        }
        maxfd = (acceptfd > listenfd) ? acceptfd : listenfd;

        if (g_sock_ctrl.can_conn == false) {
            diag_err("sock disconn, listenfd:%d, acceptfd:%d, port:%d, addr:%u\n",
                listenfd, acceptfd, chn_ctrl->port, tcp_ctrl->ip_addr);
            goto socket_disconn;
        }

        ret = mdrv_select(maxfd + 1, &fdmap, NULL, NULL, NULL);
        if (ret < 0) {
            diag_err("sock select acceptfd:%d listenfd:%d err, ret:%d\n", acceptfd, listenfd, ret);
            continue;
        }

        if (SK_FD_ISSET(listenfd, &fdmap)) {
            diag_err("%s tcp connect\n", chn_ctrl->name);
            ret = sock_tcp_connect(chn_ctrl, &acceptfd);
            if (ret) {
                diag_err("sock connect fail, ret:%d\n", ret);
                continue;
            }
        }
        if (acceptfd == -1) {
            continue;
        }

        ret = mdrv_recv(acceptfd, chn_ctrl->recv_buf, (size_t)chn_ctrl->recv_buf_len, 0);
        if (ret <= 0) {
            sock_tcp_disconnect(chn_ctrl);
            chn_ctrl->tcp_ctrl.client_fd = -1;
            acceptfd = -1;
            continue;
        }
        if (chn_ctrl->recvcb != NULL) {
            if (chn_ctrl->chan == SOCK_CNF_CODER_CHAN) {
                g_diag_dbg.diag_msp_proc_dbg.sock_dl_cnf_coder_cnt++; // 暂时不修改，需要和hids解析同步修改
            }
            (void)chn_ctrl->recvcb(chn_ctrl->recv_buf, ret);
        }
    }
    return 0;
socket_disconn:
    sock_tcp_disconnect(chn_ctrl);
    return -1;
}

int sock_tcp_server(void *arg)
{
    sock_port_chn_ctrl_s *chn_ctrl = (sock_port_chn_ctrl_s *)arg;
    sock_tcp_s *tcp_ctrl = &chn_ctrl->tcp_ctrl;
    int ret;

    diag_err("%s task begin\n", chn_ctrl->name);
    wait_event(tcp_ctrl->wq, ((g_sock_ctrl.can_conn == true) && (tcp_ctrl->ip_addr != 0)));
    diag_err("%s task start work\n", chn_ctrl->name);
    ret = sock_bind_listen(chn_ctrl);
    if (ret) {
        return ret;
    }
    return sock_tcp_accept_recv(chn_ctrl, tcp_ctrl);
}

#define IP_ADDR_127_0_0_1 16777343
int socket_ipaddr_notifier(struct notifier_block *nb, unsigned long action, void *data)
{
    diag_sock_ctrl_s *ctrl = &g_sock_ctrl;
    struct in_ifaddr *ina = (struct in_ifaddr *)data;
    unsigned int i;

    diag_err("action=%lu\n", action);
    if ((ina == NULL) || action != NETDEV_UP) {
        return 0;
    }
    diag_err("ifa_label=%s, ifa_address = %u\n", ina->ifa_label, ina->ifa_address);

    if (ina->ifa_address == IP_ADDR_127_0_0_1) {
       diag_err("ifa_address = 127.0.0.1\n");
       return 0; 
    }

    for (i = SOCK_CNF_CODER_CHAN; i < SOCK_PORT_CHAN_BUTT; i++) {
        if (ctrl->port_chn[i].type == SOCK_DGRAM || ctrl->port_chn[i].init_ok == 0) {
            continue;
        }
        if (ctrl->port_chn[i].tcp_ctrl.ip_addr == 0) {
            if (ina->ifa_address != 0) {
                ctrl->port_chn[i].tcp_ctrl.ip_addr = ina->ifa_address;
                diag_err("chn %d ip_addr=0x%x\n", i, ina->ifa_address);
                wake_up(&ctrl->port_chn[i].tcp_ctrl.wq);
            }
        } else {
            diag_err("chn %d old ip_addr = 0x%x, new addr = 0x%x\n", i, \
                ctrl->port_chn[i].tcp_ctrl.ip_addr, ina->ifa_address);
        }
    }
    return 0;
}

int sock_dfxcert_verify_fn(struct notifier_block *nb, unsigned long action, void *data)
{
    int verify = bsp_dfxcert_status_get(DIAG_SOCKET);

    if (verify == DFXCERT_PORT_SUPPORT) {
        g_sock_ctrl.can_conn = true;
    } else {
        g_sock_ctrl.can_conn = false;
    }
    diag_err("sock_dfxcert_verify_fn, can_conn=%d\n", g_sock_ctrl.can_conn);
    return 0;
}

int sock_register_inetaddr_notifier(void)
{
    static int already_registe = 0;

    if (already_registe == 0) {
        g_sock_ctrl.ipaddr_notifier.notifier_call = socket_ipaddr_notifier;
        register_inetaddr_notifier(&g_sock_ctrl.ipaddr_notifier);
        already_registe = 1;
    }
    return 0;
}

int sock_register_dfxcert_notifier(diag_sock_ctrl_s *ctrl)
{
    if (ctrl->dfxcert_notifer.notifier_call == NULL) {
        ctrl->dfxcert_notifer.notifier_call = sock_dfxcert_verify_fn;
        return bsp_dfxcert_verify_notifier_register(&ctrl->dfxcert_notifer);
    }
    return 0;
}

int sock_udp_init(sock_port_chn_ctrl_s *ctrl)
{
    spin_lock_init(&ctrl->lock);
    ctrl->recv_buf = (void*)osl_malloc(ctrl->recv_buf_len);
    if (ctrl->recv_buf == NULL) {
        diag_err("sock udp malloc recv_buf fail, len:%d\n", ctrl->recv_buf_len);
        return -1;
    }
    ctrl->init_ok = 1;
    ctrl->fd = mdrv_socket(ctrl->family, ctrl->type, ctrl->protocol);
    return ctrl->fd;
}

int sock_tcp_init(sock_port_chn_ctrl_s *ctrl)
{
    int ret;

    spin_lock_init(&ctrl->lock);
    ctrl->recv_buf = (void*)osl_malloc(ctrl->recv_buf_len);
    if (ctrl->recv_buf == NULL) {
        diag_err("sock malloc recv_buf fail, len:%d\n", ctrl->recv_buf_len);
        return -1;
    }
    ret = mdrv_socket(ctrl->family, ctrl->type, ctrl->protocol);
    if (ret < 0) {
        osl_free(ctrl->recv_buf);
        diag_err("sock tcp create fail, ret: %d\n", ret);
        return -1;
    }
    ctrl->fd = ret;
    ctrl->tcp_ctrl.client_fd = -1;

    init_waitqueue_head(&ctrl->tcp_ctrl.wq);
    ret = sock_register_inetaddr_notifier();
    if (ret) {
        osl_free(ctrl->recv_buf);
        diag_err("sock register inetaddr notifier fail, ret: %d\n", ret);
        return -1;
    }

    ret = osl_task_init(ctrl->name, 0x4C, 0x2000,
        (OSL_TASK_FUNC)sock_tcp_server, (void*)ctrl, &ctrl->taskid);
    if (ret) {
        diag_err("sock create sock_server task fail, ret: %d\n", ret);
        osl_free(ctrl->recv_buf);
        return -1;
    }
    ctrl->init_ok = 1;
    return 0;
}

void sock_port_chn_exit(sock_port_chn_ctrl_s *ctrl)
{
    if (ctrl->recv_buf != NULL) {
        osl_free(ctrl->recv_buf);
    }
    if (ctrl->fd <= 0) {
        mdrv_shutdown(ctrl->fd, SHUT_RDWR);
        mdrv_close(ctrl->fd);
        ctrl->fd = 0;
    }
}

int sock_tcp_send(sock_port_chn_ctrl_s *chn, unsigned char *buf, unsigned int len)
{
    int ret, fd;

    fd = (int)chn->tcp_ctrl.client_fd;
    if (fd == -1) {
        diag_err("sock tcp send fail, client_fd:%d\n", fd);
        return -1;
    }
    ret = mdrv_send(fd, buf, (size_t)len, 0);
    if (ret != (int)len) {
        diag_err("sock tcp send fail, ret:%d, expeted len:%d\n", ret, len);
        return -1;
    }
    return ret;
}

int sock_udp_send(sock_port_chn_ctrl_s *chn, unsigned char *buf, unsigned int len)
{
    int ret;
    unsigned int sendlen;
    unsigned int already_send = 0;
    unsigned char *tmpdata = buf;
    struct hisk_info hiskinfo = {0};

    hiskinfo.fd = chn->fd;
    while (len) {
        sendlen = len > SOCK_UDP_PACKAGE ? SOCK_UDP_PACKAGE : len;
        hiskinfo.buff = tmpdata;
        hiskinfo.len = (size_t)sendlen;
        ret = mdrv_sendto(&hiskinfo, 0, &chn->sock_addr, sizeof(struct sockaddr));
        if (ret != sendlen) {
            diag_err("sock udp send fail:%d, expected send_len: %d)\n", ret, sendlen);
            return already_send + ret;
        }
        tmpdata += sendlen;
        len -= sendlen;
        already_send += sendlen;
        diag_debug("sock udp send %d data success, %d data left\n", sendlen, len);
    }
    return already_send;
}

int sock_send(sock_chan_e chn, unsigned char *buf, unsigned int len)
{
    diag_sock_ctrl_s *ctrl = &g_sock_ctrl;
    sock_port_chn_ctrl_s *port_ctrl;
    int ret = 0;
    unsigned long flags;

    if (chn >= SOCK_PORT_CHAN_BUTT || buf == NULL) {
        diag_err("ppm sock_send fail, chn:%d\n", chn);
        return -1;
    }
    port_ctrl = &ctrl->port_chn[chn];

    spin_lock_irqsave(&port_ctrl->lock, flags);
    if (port_ctrl->state != SOCK_CONNECT) {
        spin_unlock_irqrestore(&port_ctrl->lock, flags);
        diag_err("ppm sock not connect, can't send data\n");
        return -1;
    }
    spin_unlock_irqrestore(&port_ctrl->lock, flags);

    if (port_ctrl->type == SOCK_STREAM) {
        ret = sock_tcp_send(port_ctrl, buf, len);
    } else if (port_ctrl->type == SOCK_DGRAM) {
        ret = sock_udp_send(port_ctrl, buf, len);
    } else {
        diag_err("ppm sock_send fail, chn: %d\n", chn);
        return -1;
    }
    g_diag_dbg.mntn_data.sock_dbg[chn].sock_send_len += ret;

    return ret;
}

int bsp_sock_send(sock_chan_e chn, unsigned char *buf, unsigned int len)
{
    return sock_send(chn, buf, len);
}

int sock_register_recv_cb(sock_chan_e chn, sock_recv_cb cb)
{
    diag_sock_ctrl_s *ctrl = &g_sock_ctrl;

    if (chn >= SOCK_PORT_CHAN_BUTT) {
        diag_err("sock register_recv_cb fail, chn: %d\n", chn);
        return -1;
    }
    ctrl->port_chn[chn].recvcb = cb;
    return 0;
}

int bsp_sock_register_recv_cb(sock_chan_e chn, sock_recv_cb cb)
{
    return sock_register_recv_cb(chn, cb);
}

int sock_register_event_cb(sock_chan_e chn, sock_event_cb cb)
{
    g_sock_ctrl.port_chn[chn].eventcb = cb;
    return 0;
}

int bsp_sock_register_event_cb(sock_chan_e chn, sock_event_cb cb)
{
    return sock_register_event_cb(chn, cb);
}

int sock_get_state(void)
{
    return g_sock_ctrl.port_chn[SOCK_CNF_CODER_CHAN].state;
}

int sock_init(void)
{
    int ret;
    diag_sock_ctrl_s *ctrl = &g_sock_ctrl;

    ret = sock_udp_init(&ctrl->port_chn[SOCK_IND_CODER_CHAN]);
    if (ret < 0) {
        diag_err("sock create ind_ch fail, ret: %d\n", ret);
        goto failure;
    }
    ret = sock_tcp_init(&ctrl->port_chn[SOCK_CNF_CODER_CHAN]);
    if (ret < 0) {
        diag_err("sock create cnf_chn fail, ret: %d\n", ret);
        goto failure;
    }
    diag_err("ppm sock init succ\n");
    return 0;

failure:
    sock_port_chn_exit(&ctrl->port_chn[SOCK_CNF_CODER_CHAN]);
    sock_port_chn_exit(&ctrl->port_chn[SOCK_IND_CODER_CHAN]);
    return -1;
}

int sock_sec_init(void)
{
    int ret;
    diag_sock_ctrl_s *ctrl = &g_sock_ctrl;

    if (bsp_get_version_info()->plat_type == PLAT_ASIC) {
        ret = sock_register_dfxcert_notifier(ctrl);
        if (ret) {
            diag_err("sock register dfxcert notifier fail, ret: %d\n", ret);
            return -1;
        }
    } else {
        ctrl->can_conn = true;
    }
    return 0;
}

int bsp_sock_cfg_chn(sock_chan_e chn, sock_cfg_s *cfg)
{
    diag_sock_ctrl_s *ctrl = &g_sock_ctrl;

    if (chn >= SOCK_PORT_CHAN_BUTT || cfg == NULL) {
        diag_err("para invalid\n");
        return -1;
    }

    ctrl->port_chn[chn].port = cfg->port;
    ctrl->port_chn[chn].recv_buf_len = cfg->recv_buf_len;
    sock_tcp_init(&ctrl->port_chn[chn]);
    return 0;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
