/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Test whether xom is enabled.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 12 17:20:09 2022
 */

/*
 * The generated code should contain section that only have executable flag.
 * Tester can examin the result of `readelf -l <binary-file>` to judge this.
 * '  E'(character 'E' with two leading spaces).
 */
extern int func(void);
int func(void)
{
	return 0;
}
