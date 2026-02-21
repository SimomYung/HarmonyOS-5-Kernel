SECTIONS
{
	/* Kernel symbol table: Normal symbols */
	__ksymtab : {
		__start___ksymtab = .;
		KEEP(*(SORT(___ksymtab+*)))
		__stop___ksymtab = .;
	}
	/* Kernel symbol table: GPL-only symbols */
	__ksymtab_gpl : {
		__start___ksymtab_gpl = .;
		KEEP(*(SORT(___ksymtab_gpl+*)))
		__stop___ksymtab_gpl = .;
	}
	/* Kernel symbol table: GPL-future-only symbols */
	__ksymtab_gpl_future : {
		__start___ksymtab_gpl_future = .;
		KEEP(*(SORT(___ksymtab_gpl_future+*)))
		__stop___ksymtab_gpl_future = .;
	}
	/* Kernel symbol table: Normal symbols */
	__kcrctab : {
		__start___kcrctab = .;
		KEEP(*(SORT(___kcrctab+*)))
		__stop___kcrctab = .;
	}
	/* Kernel symbol table: GPL-only symbols */
	__kcrctab_gpl : {
		__start___kcrctab_gpl = .;
		KEEP(*(SORT(___kcrctab_gpl+*)))
		__stop___kcrctab_gpl = .;
	}
	/* Kernel symbol table: GPL-future-only symbols */
	__kcrctab_gpl_future : {
		__start___kcrctab_gpl_future = .;
		KEEP(*(SORT(___kcrctab_gpl_future+*)))
		__stop___kcrctab_gpl_future = .;
	}
	/* Kernel symbol table: strings */
	__ksymtab_strings : {
		*(__ksymtab_strings)
	}
}
INSERT BEFORE .data;
