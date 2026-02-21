/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Provide a interface for handling the shortcuts
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 13 16:33:24 2019
 */

#ifndef TTY_KEYBOARD_H
#define TTY_KEYBOARD_H

#include "tty.h"

#define __HANDLE_NOTHING	0x0
#define __HANDLE_INPUT		0x1
#define __HANDLE_ECHO		(0x1 << 1)

#define SHORTCUT_KEY_DISCARD	__HANDLE_NOTHING
#define SHORTCUT_KEY_ONLY_ECHO	__HANDLE_ECHO
#define SHORTCUT_KEY_NORMAL	(__HANDLE_INPUT | __HANDLE_ECHO)

#define SHORTCUT_NEED_ECHO(ret) ((ret) & __HANDLE_ECHO)
#define SHORTCUT_NEED_INPUT(ret) ((ret) & __HANDLE_INPUT)

int tty_handle_shortcuts_key(char shortcuts_key, struct tty_struct *tty);

#endif
