/*
 * callee:
 * x0 -> pointer to argc
 * x1 -> dynamic section address
 */

__asm__(
".text \n"
".global " START "\n"
".type " START ",%function\n"
START ":\n"
#ifdef CONFIG_ARM64_BTI_INST
"	bti c\n"
#endif
"	mov x29, #0\n"
"	mov x30, #0\n"
"	mov x0, sp\n"
".weak _DYNAMIC\n"
".hidden _DYNAMIC\n"
"	adrp x1, _DYNAMIC\n"
"	add x1, x1, #:lo12:_DYNAMIC\n"
"	and sp, x0, #-16\n"
"	b " START "_c\n"
);
