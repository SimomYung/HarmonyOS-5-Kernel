#include <wctype.h>

int iswalpha(wint_t wc)
{
	return ((unsigned)wc|32)-'a' < 26;
}

int __iswalpha_l(wint_t c, locale_t l)
{
	return iswalpha(c);
}

weak_alias(__iswalpha_l, iswalpha_l);
