#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "fCommon.h"


typedef enum fLangId
{
	flangHELP_GENERAL,
	flangHELP_INDENDATION,
	flangHELP_AESTHETICS,
	flangHELP_MISCELLANEOUS,
	flangUNTITLED,

	flang_size

} fLangId_e;

typedef wchar * fLangArr_t[flang_size];
typedef const wchar * fCLangArr_t[flang_size];
typedef const char * fCLangArr8_t[flang_size];

extern const fCLangArr8_t fLang_tokens;
extern fCLangArr_t fLang_strings;

/**
 * @brief Returns pointer to language string
 * 
 */
#define fLang_get(id)  (fLang_strings[id])

bool fLang_init(void);
isize fLang_setLang8(const char * restrict language);
isize fLang_setLang(const wchar * restrict language);
bool fLang_addKeyword8(isize langId, fLangId_e kwId, const char * restrict keyword);
bool fLang_addKeyword(isize langId, fLangId_e kwId, const wchar * restrict keyword);
void fLang_destroy(void);


// Defined defaults

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

#define FEMTO_UNTITLED_NAME  L"untitled"



#endif
