/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
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

#include <securec.h>
#include <osl_types.h>
#include <mdrv_socket.h>
#include <product_config.h>
#include <bsp_version.h>
#include <bsp_msocket.h>
#include <bsp_esl_socket.h>

#if (defined(FEATURE_SVLSOCKET) || (defined(CONFIG_OM_SOCKET)))

#define SOCK_ESL_VERSION 1
int sock_esl_version_check(void)
{
    const bsp_version_info_s *verinfo = bsp_get_version_info();
    if ((verinfo != NULL) && ((verinfo->plat_type == PLAT_ESL) || (verinfo->plat_type == PLAT_HYBRID))) {
        return SOCK_ESL_VERSION;
    }

    return 0;
}

int mdrv_socket(int family, int type, int protocol)
{
    if (sock_esl_version_check()) {
        return bsp_esl_socket(family, type, protocol);
    }

    return bsp_socket(family, type, protocol);
}

int mdrv_bind(int fd, struct sockaddr *addr, int addrlen)
{
    if (sock_esl_version_check()) {
        return bsp_esl_bind(fd, addr, addrlen);
    }

    return bsp_bind(fd, addr, addrlen);
}

int mdrv_listen(int fd, int backlog)
{
    if (sock_esl_version_check()) {
        return bsp_esl_listen(fd, backlog);
    }

    return bsp_listen(fd, backlog);
}

int mdrv_accept(int fd, struct sockaddr *upeer_sockaddr, int *upeer_addrlen)
{
    if (sock_esl_version_check()) {
        return bsp_esl_accept(fd, upeer_sockaddr, upeer_addrlen);
    }

    return bsp_accept(fd, upeer_sockaddr, upeer_addrlen);
}

int mdrv_select(int n, sk_fd_set *inp, sk_fd_set *outp, sk_fd_set *exp_fdset, struct timespec64 *tvp)
{
    if (sock_esl_version_check()) {
        return bsp_esl_select(n, inp, outp, exp_fdset, tvp);
    }

    return bsp_select(n, inp, outp, exp_fdset, tvp);
}

int mdrv_close(int fd)
{
    if (sock_esl_version_check()) {
        return bsp_esl_close(fd);
    }

    return bsp_skclose(fd);
}

int mdrv_send(int fd, void *buff, size_t len, int flags)
{
    if (sock_esl_version_check()) {
        return bsp_esl_send(fd, buff, len, flags);
    }

    return bsp_send(fd, buff, len, flags);
}

int mdrv_recv(int fd, void *ubuf, size_t size, int flags)
{
    if (sock_esl_version_check()) {
        return bsp_esl_recv(fd, ubuf, size, flags);
    }

    return bsp_recv(fd, ubuf, size, flags);
}

int mdrv_sendto(struct hisk_info *hiskinfo, unsigned flags, struct sockaddr *addr, int addr_len)
{
    if (sock_esl_version_check()) {
        return bsp_esl_sendto(hiskinfo, flags, addr, addr_len);
    }

    return bsp_sendto(hiskinfo, flags, addr, addr_len);
}

int mdrv_getpeername(int fd, struct sockaddr *addr, int *addrlen)
{
    if (sock_esl_version_check()) {
        return bsp_esl_getpeername(fd, addr, addrlen);
    }

    return bsp_getpeername(fd, addr, addrlen);
}

int mdrv_getsockopt(int fd, int level, int optname, char *optval, int *optlen)
{
        if (sock_esl_version_check()) {
            return bsp_esl_getsockopt(fd, level, optname, optval, optlen);
        }

        return bsp_getsockopt(fd, level, optname, optval, optlen);
}

int mdrv_setsockopt(int fd, int level, int optname, char *optval, unsigned int optlen)
{
    if (sock_esl_version_check()) {
        return bsp_esl_setsockopt(fd, level, optname, optval, optlen);
    }

    return bsp_setsockopt(fd, level, optname, optval, optlen);
}

int mdrv_ioctl(int fd, int cmd, unsigned int arg)
{
    if (sock_esl_version_check()) {
        return bsp_esl_ioctl(fd, cmd, arg);
    }

    return bsp_ioctl(fd, cmd, arg);
}

int mdrv_shutdown(int fd, enum sock_shutdown_cmd how)
{
    if (sock_esl_version_check()) {
        return bsp_esl_shutdown(fd, how);
    }

    return bsp_shutdown(fd, how);
}

unsigned int mdrv_inet_addr(const char *str)
{
    if (sock_esl_version_check()) {
        return bsp_esl_inet_addr(str);
    }

    return bsp_inet_addr(str);
}

#else

int mdrv_socket(int family, int type, int protocol)
{
    return -1;
}
int mdrv_bind(int fd, struct sockaddr *addr, int addrlen)
{
    return 0;
}
int mdrv_listen(int fd, int backlog)
{
    return 0;
}
int mdrv_accept(int fd, struct sockaddr *upeer_sockaddr, int *upeer_addrlen)
{
    return 0;

}

int mdrv_select(int n, sk_fd_set *inp, sk_fd_set *outp, sk_fd_set *exp, struct timespec64 *tvp)
{
    return 0;
}

int mdrv_close(int fd)
{
    return 0;
}
int mdrv_send(int fd, void *buff, size_t len, int flags)
{
    return 0;
}
int mdrv_recv(int fd, void *ubuf, size_t size, int flags)
{
    return 0;
}
int mdrv_sendto(struct hisk_info *hiskinfo, unsigned flags, struct sockaddr *addr, int addr_len)
{
    return 0;
}
int mdrv_getpeername(int fd, struct sockaddr *addr, int *addrlen)
{
    return 0;
}
int mdrv_getsockopt(int fd, int level, int optname, char *optval, int *optlen)
{
    return 0;
}
int mdrv_setsockopt(int fd, int level, int optname, char *optval, unsigned int optlen)
{
    return 0;
}
int mdrv_ioctl(int fd, int cmd, unsigned int arg)
{
    return 0;
}
int mdrv_shutdown(int fd, enum sock_shutdown_cmd how)
{
    return 0;
}
unsigned int mdrv_inet_addr(const char * str)
{
    return 0;
}
#endif

EXPORT_SYMBOL(mdrv_socket);
EXPORT_SYMBOL(mdrv_bind);
EXPORT_SYMBOL(mdrv_close);
EXPORT_SYMBOL(mdrv_listen);
EXPORT_SYMBOL(mdrv_accept);
EXPORT_SYMBOL(mdrv_recv);
EXPORT_SYMBOL(mdrv_send);
EXPORT_SYMBOL(mdrv_select);
EXPORT_SYMBOL(mdrv_sendto);
EXPORT_SYMBOL(mdrv_getpeername);
EXPORT_SYMBOL(mdrv_getsockopt);
EXPORT_SYMBOL(mdrv_setsockopt);
EXPORT_SYMBOL(mdrv_ioctl);
EXPORT_SYMBOL(mdrv_shutdown);
EXPORT_SYMBOL(mdrv_inet_addr);
