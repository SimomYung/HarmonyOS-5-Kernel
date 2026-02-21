/*
* Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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

#include <net/tcp.h>

#include <linux/sched.h>
#include <linux/rcupdate.h>

#include "pid_socket.h"
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14,0 )
#include <linux/sched/task.h>
#endif
#include <net/sock.h>

#include <log/log_usertype.h>
#ifdef DUBAI_WATCH_WATCH_NETWORK_SOCKET_EVENT
#include <huawei_platform/log/hwlog_kernel.h>
#endif

void print_process_pid_name(struct inet_sock *inet)
{
	int pid = 0;
	int uid = 0;
	unsigned short source_port = 0;
#if defined(CONFIG_LOG_EXCEPTION) || defined(CONFIG_LOG_USERTYPE)
	unsigned int user_type = get_logusertype_flag();
	if (user_type != BETA_USER && user_type != OVERSEA_USER)
		return;
#endif

#if defined(CONFIG_HUAWEI_KSTATE)
	if (NULL == inet || NULL == inet->sk.sk_socket) {
		return;
	}

	pid = inet->sk.sk_socket->pid;
#else
	if (NULL == inet) {
		return;
	}

	pid = task_tgid_vnr(current);
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 10)
	uid = sock_i_uid(&inet->sk).val;
#else
	uid = sock_i_uid(&inet->sk);
#endif

	source_port = inet->inet_sport;

	source_port = htons(source_port);
	printk("%s: uid:%d, pid:%d, port:%u-%u, fam:%u, ptl:%u, fwmark:%#x, bound_if:%d\n", __func__, uid, pid, source_port,
		ntohs(inet->inet_dport), inet->sk.sk_family, inet->sk.sk_protocol, inet->sk.sk_mark, inet->sk.sk_bound_dev_if);

#if defined(DUBAI_WATCH_WATCH_NETWORK_SOCKET_EVENT)
	HWDUBAI_LOGE("DUBAI_TAG_WATCH_NETWORK_SOCKET_EVENT", "uid=%d pid=%d name=%s port=%d",
			uid, pid, __func__, source_port);
#endif
}

/*20150114 add,get pid and process name of the app who used connect function.*/
