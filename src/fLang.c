#include "fLang.h"

const wchar * flang_strings[flang_size] = {
	[flangHELP_GENERAL] 		= FEMTO_HELP_GENERAL_DEF,	
	[flangHELP_INDENDATION] 	= FEMTO_HELP_INDENDATION_DEF,
	[flangHELP_AESTHETICS] 		= FEMTO_HELP_AESTHETICS_DEF,
	[flangHELP_MISCELLANEOUS] 	= FEMTO_HELP_MISCELLANEOUS_DEF,

};

static wchar * s_flang_string[flang_size] = { 0 };
