#ifndef NET_MONITOR_HOOK_H
#define NET_MONITOR_HOOK_H

#define NM_VAR1(v1) (void *)&v1
#define NM_VAR2(v2, ...) (void *)&v2, NM_VAR1(__VA_ARGS__)
#define NM_VAR3(v3, ...) (void *)&v3, NM_VAR2(__VA_ARGS__)
#define NM_VAR4(v4, ...) (void *)&v4, NM_VAR3(__VA_ARGS__)
#define NM_VAR5(v5, ...) (void *)&v5, NM_VAR4(__VA_ARGS__)

#define NM_ENUM(f) NM_ENUM##f

enum nm_enum_table {
    NM_ENUM(__sys_socket),
    NM_ENUM(__sys_shutdown),
    NM_ENUM(__sys_setsockopt),
    NM_ENUM(__sys_getsockopt),
    NM_ENUM(compat_sock_ioctl),
    NM_ENUM(___sys_sendmsg),
    NM_ENUM(___sys_recvmsg),
    NM_ENUM_BOT
};

void net_monitor_trace(const char *name, enum nm_enum_table e, void **para_stack, int size);

int net_monitor_hook_register(enum nm_enum_table e, void (*func)(void **, int));

#define NM_PARA_STACK1(v1) \
    void *stack[] = {NM_VAR1(v1)}

#define NM_PARA_STACK2(v1, v2) \
    void *stack[] = {NM_VAR2(v1, v2)}

#define NM_PARA_STACK3(v1, v2, v3) \
    void *stack[] = {NM_VAR3(v1, v2, v3)}

#define NM_PARA_STACK4(v1, v2, v3, v4) \
    void *stack[] = {NM_VAR4(v1, v2, v3, v4)}

#define NM_PARA_STACK5(v1, v2, v3, v4, v5) \
    void *stack[] = {NM_VAR5(v1, v2, v3, v4, v5)}

#define NM_TRACE(f, x, ...) \
    { \
        NM_PARA_STACK##x(__VA_ARGS__); \
        net_monitor_trace(__func__, NM_ENUM(f), stack, sizeof(stack)); \
    } \


#endif