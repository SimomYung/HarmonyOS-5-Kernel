#include <wctype.h>

/* Consider all legal codepoints as printable except for:
 * - C0 and C1 control characters
 * - U+2028 and U+2029 (line/para break)
 * - U+FFF9 through U+FFFB (interlinear annotation controls)
 * The following code is optimized heavily to make hot paths for the
 * expected printable characters. */

int iswprint(wint_t wc)
{
	return (unsigned)wc-0x20 < 0x5f;
}

int __iswprint_l(wint_t c, locale_t l)
{
	return iswprint(c);
}

weak_alias(__iswprint_l, iswprint_l);
