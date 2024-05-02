#include "fLang.h"

const char * flang_strings[flang_size] = {
	[flangHELP_GENERAL] 		= FEMTO_HELP_GENERAL_DEF,	
	[flangHELP_INDENDATION] 	= FEMTO_HELP_INDENDATION_DEF,
	[flangHELP_AESTHETHICS] 	= FEMTO_HELP_AESTHETHICS_DEF,
	[flangHELP_MISCELLANEOUS] 	= FEMTO_HELP_MISCELLANEOUS_DEF,

};

static char * s_flang_string[flang_size] = { 0 };
