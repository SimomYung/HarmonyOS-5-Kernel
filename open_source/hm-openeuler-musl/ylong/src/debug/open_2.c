/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: __open_2
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int __open_2(const char *file, int oflag)
{
    if ((oflag & O_CREAT) == O_CREAT || (oflag & O_TMPFILE) == O_TMPFILE) {
        printf("*** invalid open call: O_CREAT or O_TMPFILE without mode ***: terminated\n");
        abort();
    }

    return open(file, oflag);
}
