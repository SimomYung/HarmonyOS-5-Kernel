/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBDH_LINUX_SECURITY_H__
#define __LIBDH_LINUX_SECURITY_H__

int sec_load_policy(const char *buffer, unsigned long size);
int sec_update_enforce(int value);
int sec_current_task_sid(uint32_t *sid);
int sec_avc_perm(unsigned int tsid, unsigned short tclass, unsigned int requested);
int socket_sockcreate_sid(unsigned int tsid, unsigned short tclass, unsigned int *socksid);
unsigned short socket_type_to_secclass(int domain, int type, int protocol);
int sec_update_cache_threshold(int value);
bool sec_is_context_match_current_sid(const char *context);
#endif /* !__LIBDH_LINUX_SECURITY_H__ */
