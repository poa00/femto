#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "fCommon.h"

typedef enum fLangIds
{
	flangHELP_GENERAL,
	flangHELP_INDENDATION,
	flangHELP_AESTHETHICS,
	flangHELP_MISCELLANEOUS,


	flang_size

} fLangIds_e;

extern const char * flang_strings[flang_size];

#define flang_get(id)  (flang_strings[id])


#define FEMTO_HELP_GENERAL_DEF \
L"\nGeneral:\n\n" \
"  --h                 Shows this help\n" \
"  --h=[value]         Shows help for a keyword\n" \
"  --file=[file]       Specifies file name to open\n" \
"  --setting=[file]    Specifies custom settings file explicitly\n"

#define FEMTO_HELP_INDENDATION_DEF \
L"\nIndendation:\n\n" \
"  --tabsToSpaces=[value]    true/false, 1/0\n" \
"  --tabWidth=[value]        Number from 1-32 (inclusive)\n" \
"  --autoIndent=[value]      true/false, 1/0\n"

#define FEMTO_HELP_AESTHETICS_DEF \
L"\nAesthetics:\n\n" \
"  --whitespaceVisible=[value]    true/false, 1/0\n" \
"  --whitespaceCh=[value]         Specifies 'visible' whitespace character\n" \
"  --whitespaceCol=[value]        Specifies whitespace color, 0-255\n" \
"  --lineNumRelative=[value]      true/false, 1/0\n" \
"  --lineNumCol=[value]           Specifies line number color, 0-255\n"

#define FEMTO_HELP_MISCELLANEOUS_DEF \
L"\nMiscellaneous:\n\n" \
"  --log=[value] Specifies the use of logging mode\n"


#endif
