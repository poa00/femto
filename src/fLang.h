#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "fCommon.h"


typedef enum fLangId
{
	flangHELP_GENERAL,
	flangHELP_INDENDATION,
	flangHELP_AESTHETICS,
	flangHELP_MISCELLANEOUS,
	flangHELP_CLUE,
	flangHELP_KEYWORD,
	flangCATEGORIES,
	flangUSAGE,

	flangFILE,
	flangUNTITLED,
	flangUNSAVED,
	flangIS_UNSAVED,
	flangPRESS,
	flangCONFIRM_CLOSE,
	flangCONFIRM_EXIT,
	flangCLOSED_TAB,
	flangOPT_FILE,
	flangNOTHING_NEW,
	flangOPEN_ERROR,
	flangWRITE_ERROR,
	flangMEM_ERROR,
	flangWROTE,
	flangBYTES,
	flangBYTES_TO,
	flangUNKNOWN_COMB,
	flangCAPS_ON,
	flangCAPS_OFF,

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

#define FLANG_HELP_GENERAL_DEF \
L"\nGeneral:\n\n" \
"  --h                 Shows this help\n" \
"  --h=[value]         Shows help for a keyword\n" \
"  --file=[file]       Specifies file name to open\n" \
"  --setting=[file]    Specifies custom settings file explicitly\n"

#define FLANG_HELP_INDENDATION_DEF \
L"\nIndendation:\n\n" \
"  --tabsToSpaces=[value]    true/false, 1/0\n" \
"  --tabWidth=[value]        Number from 1-32 (inclusive)\n" \
"  --autoIndent=[value]      true/false, 1/0\n"

#define FLANG_HELP_AESTHETICS_DEF \
L"\nAesthetics:\n\n" \
"  --whitespaceVisible=[value]    true/false, 1/0\n" \
"  --whitespaceCh=[value]         Specifies 'visible' whitespace character\n" \
"  --whitespaceCol=[value]        Specifies whitespace color, 0-255\n" \
"  --lineNumRelative=[value]      true/false, 1/0\n" \
"  --lineNumCol=[value]           Specifies line number color, 0-255\n"

#define FLANG_HELP_MISCELLANEOUS_DEF \
L"\nMiscellaneous:\n\n" \
"  --log=[value] Specifies the use of logging mode\n"

#define FLANG_HELP_KEYWORD L"Help keyword"
#define FLANG_HELP_CLUE  L"To show all available commands type"
#define FLANG_CATEGORIES L"By categories"
#define FLANG_USAGE  L"Usage:"


#define FLANG_FILE L"File"
#define FLANG_UNTITLED_NAME  L"untitled"
#define FLANG_UNSAVED  L"Unsaved file(s)"
#define FLANG_IS_UNSAVED L"is unsaved"
#define FLANG_PRESS  L"Press"
#define FLANG_CONFIRM_CLOSE  L"to confirm closing"
#define FLANG_CONFIRM_EXIT  L"to confirm exit"
#define FLANG_CLOSED_TAB L"Closed tab"
#define FLANG_OPT_FILE  L"[options] [file]"
#define FLANG_NOTHING_NEW L"Nothing new to save"
#define FLANG_OPEN_ERROR L"File open error!"
#define FLANG_WRITE_ERROR L"The file is write-protected!"
#define FLANG_MEM_ERROR L"Memory allocation error!"
#define FLANG_WROTE L"Wrote"
#define FLANG_BYTES L"bytes"
#define FLANG_BYTES_TO L"bytes to"
#define FLANG_UNKNOWN_COMB L"Unknown key combination!"
#define FLANG_CAPS_ON   L"'CAPS' On"
#define FLANG_CAPS_OFF  L"'CAPS' Off"


#endif
