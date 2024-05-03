#include "fLang.h"
#include "femto.h"

const fCLangArr8_t fLang_tokens = {
	[flangHELP_GENERAL] 		= "general",
	[flangHELP_INDENDATION] 	= "indent",
	[flangHELP_AESTHETICS] 		= "aesthetics",
	[flangHELP_MISCELLANEOUS] 	= "misc",
	[flangUNTITLED] 			= "untitled"
};

static const fCLangArr_t s_fLang_defStrings = {
	[flangHELP_GENERAL] 		= FEMTO_HELP_GENERAL_DEF,	
	[flangHELP_INDENDATION] 	= FEMTO_HELP_INDENDATION_DEF,
	[flangHELP_AESTHETICS] 		= FEMTO_HELP_AESTHETICS_DEF,
	[flangHELP_MISCELLANEOUS] 	= FEMTO_HELP_MISCELLANEOUS_DEF,
	[flangUNTITLED] 			= FEMTO_UNTITLED_NAME,
};

fCLangArr_t fLang_strings = {
	[flangHELP_GENERAL] 		= FEMTO_HELP_GENERAL_DEF,	
	[flangHELP_INDENDATION] 	= FEMTO_HELP_INDENDATION_DEF,
	[flangHELP_AESTHETICS] 		= FEMTO_HELP_AESTHETICS_DEF,
	[flangHELP_MISCELLANEOUS] 	= FEMTO_HELP_MISCELLANEOUS_DEF,
	[flangUNTITLED] 			= FEMTO_UNTITLED_NAME,
};


static fLangArr_t * s_fLang_stringsArr = NULL;

static wchar ** s_langNames = NULL;
static usize s_numLanguages = 0;
static isize s_langIdx = -1;



bool fLang_init(void)
{
	if ((s_numLanguages == 0) || (s_langNames == NULL) ||
		(s_fLang_stringsArr == NULL) || (s_langIdx >= (isize)s_numLanguages) )
	{
		return false;
	}

	fLangArr_t * const strings = (s_langIdx == ((isize)-1)) ? NULL : &s_fLang_stringsArr[s_langIdx];

	for (usize i = 0; i < flang_size; ++i)
	{
		fLang_strings[i] = ((strings == NULL) || ((*strings)[i] == NULL)) ? s_fLang_defStrings[i] : (*strings)[i];
	}

	return true;
}
isize fLang_setLang8(const char * restrict language)
{
	wchar * lang16 = NULL;
	if (language != NULL)
	{
		femto_toutf16(language, (int)strlen(language) + 1, &lang16, NULL);
		if (lang16 == NULL)
		{
			return -1;
		}
	}
	isize ret = fLang_setLang(lang16);
	free(lang16);
	return ret;
}
isize fLang_setLang(const wchar * restrict language)
{
	if (language == NULL)
	{
		s_langIdx = -1;
		return s_langIdx;
	}

	// search for language in list
	const usize langLen = wcslen(language);
	if ((s_numLanguages != 0) && (s_langNames != NULL))
	{
		for (usize i = 0; i < s_numLanguages; ++i)
		{
			assert(s_langNames[i] != NULL);
			if (wcsncmp(s_langNames[i], language, langLen) == 0)
			{
				s_langIdx = (isize)i;
				return s_langIdx;
			}
		}
	}

	// add langauge to list, it wasn't found
	usize tempsz = s_numLanguages + 1;
	wchar ** templan = realloc(s_langNames, sizeof(wchar *) * tempsz);
	if (templan == NULL)
	{
		return -1;
	}
	s_langNames    = templan;
	s_numLanguages = tempsz;

	// resize s_fLang_string
	fLangArr_t * tempStr = realloc(s_fLang_stringsArr, sizeof(fLangArr_t) * s_numLanguages);
	if (tempStr == NULL)
	{
		--s_numLanguages;
		if (!s_numLanguages)
		{
			free(s_langNames);
			s_langNames = NULL;
		}
		else
		{
			s_langNames = realloc(s_langNames, sizeof(wchar *) * s_numLanguages);
		}
		return -1;
	}
	s_fLang_stringsArr = tempStr;

	for (usize i = 0; i < flang_size; ++i)
	{
		s_fLang_stringsArr[s_numLanguages - 1][i] = NULL;
	}

	s_langNames[s_numLanguages - 1] = wcsdup(language);
	s_langIdx = (isize)s_numLanguages - 1;

	return s_langIdx;
}
bool fLang_addKeyword8(isize langId, fLangId_e kwId, const char * restrict keyword)
{
	wchar * kw16 = NULL;
	if (keyword != NULL)
	{
		femto_toutf16(keyword, (int)strlen(keyword) + 1, &kw16, NULL);
		if (kw16 == NULL)
		{
			return -1;
		}
	}
	isize ret = fLang_addKeyword(langId, kwId, kw16);
	free(kw16);
	return ret;
}
bool fLang_addKeyword(isize langId, fLangId_e kwId, const wchar * restrict keyword)
{
	if ((langId >= (isize)s_numLanguages) || (kwId >= flang_size) || (keyword == NULL))
	{
		return false;
	}

	wchar * kwcopy = wcsdup(keyword);
	if (kwcopy == NULL)
	{
		return false;
	}

	s_fLang_stringsArr[langId][kwId] = kwcopy;

	return true;
}

void fLang_destroy(void)
{
	for (usize j = 0; j < s_numLanguages; ++j)
	{
		for (usize i = 0; i < flang_size; ++i)
		{
			if (s_fLang_stringsArr[j][i] != NULL)
			{
				free(s_fLang_stringsArr[j]);
			}
		}
		free(s_fLang_stringsArr[j]);
		free(s_langNames[j]);
	}

	free(s_fLang_stringsArr);
	free(s_langNames);
}
