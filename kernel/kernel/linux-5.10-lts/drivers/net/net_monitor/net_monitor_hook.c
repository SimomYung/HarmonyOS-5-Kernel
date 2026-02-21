#include <linux/module.h>
#include <linux/net_monitor_hook.h>

static void (*g_nm_func_table[NM_ENUM_BOT])(void **, int) = { 0 };

void net_monitor_trace(const char *name, enum nm_enum_table e, void **para_stack, int size)
{
    if (e >= NM_ENUM_BOT)
        return;

    if (g_nm_func_table[e] == NULL)
        return;

    g_nm_func_table[e](para_stack, size);
}
EXPORT_SYMBOL_GPL(net_monitor_trace);

int net_monitor_hook_register(enum nm_enum_table e, void (*func)(void **, int))
{
    if (e >= NM_ENUM_BOT)
        return -1;

    if (func == NULL)
        return -1;

    g_nm_func_table[e] = func;

    return 0;
}
EXPORT_SYMBOL_GPL(net_monitor_hook_register);