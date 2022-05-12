#include "femtoSyntax.h"
#include "femtoLine.h"


const char * fSyntaxName(enum femtoSyntax fs)
{
	static const char * syntaxes[fstxSIZE] = {
		"None",
		"C",
		"C++",
		"Markdown"
	};

	assert(fs <= fstxSIZE);
	return syntaxes[fs];
}
bool fSyntaxParseAutoAlloc(femtoLineNode_t * restrict node)
{
	assert(node != NULL);

	void * mem = realloc(node->syntax, sizeof(WORD) * node->lineEndx);
	if (mem == NULL)
	{
		free(node->syntax);
		node->syntax = NULL;
		return false;
	}
	node->syntax = mem;

	return true;
}

static void checkCToken(femtoLineNode_t * restrict node, uint32_t start, uint32_t lasti)
{
	writeProfiler("checkCToken", "start: %u, last: %u", start, lasti);

	if ((lasti - start) < 1)
	{
		return;
	}

	struct keyword
	{
		uint8_t len;
		const wchar_t ** words;
	};

	static const wchar_t * keywordsLen2[] = {
		L"do",
		L"if"
	};
	static const wchar_t * keywordsLen3[] = {
		L"for",
		L"int"
	};
	static const wchar_t * keywordsLen4[] = {
		L"auto",
		L"case",
		L"char",
		L"else",
		L"enum",
		L"long",
		L"void",
		L"goto",

		L"bool"
	};
	static const wchar_t * keywordsLen5[] = {
		L"break",
		L"const",
		L"float",
		L"short",
		L"union",
		L"while",

		L"_Bool"
	};
	static const wchar_t * keywordsLen6[] = {
		L"double",
		L"extern",
		L"inline",
		L"return",
		L"signed",
		L"sizeof",
		L"static",
		L"struct",
		L"switch"
	};
	static const wchar_t * keywordsLen7[] = {
		L"default",

		L"_Atomic",

		L"alignas",
		L"alignof",
		L"complex",
		L"typedef"
	};
	static const wchar_t * keywordsLen8[] = {
		L"continue",
		L"register",
		L"restrict",
		L"unsigned",
		L"volatile",

		L"_Alignas",
		L"_Alignof",
		L"_Complex",
		L"_Generic",

		L"noreturn"
	};
	static const wchar_t * keywordsLen9[] = {
		L"_Noreturn",

		L"imaginary"
	};
	static const wchar_t * keywordsLen10[] = {
		L"_Decimal64",
		L"_Decimal32",
		L"_Imaginary"
	};
	static const wchar_t * keywordsLen11[] = {
		L"_Decimal128"
	};
	static const wchar_t * keywordsLen12[] = {
		L"thread_local"
	};
	static const wchar_t * keywordsLen13[] = {
		L"_Thread_local",

		L"static_assert"
	};
	static const wchar_t * keywordsLen14[] = {
		L"_Static_assert"
	};

	static const struct keyword words[] = {
		[2]  = { .len = ARRAYSIZE(keywordsLen2),  .words = keywordsLen2  },
		[3]  = { .len = ARRAYSIZE(keywordsLen3),  .words = keywordsLen3  },
		[4]  = { .len = ARRAYSIZE(keywordsLen4),  .words = keywordsLen4  },
		[5]  = { .len = ARRAYSIZE(keywordsLen5),  .words = keywordsLen5  },
		[6]  = { .len = ARRAYSIZE(keywordsLen6),  .words = keywordsLen6  },
		[7]  = { .len = ARRAYSIZE(keywordsLen7),  .words = keywordsLen7  },
		[8]  = { .len = ARRAYSIZE(keywordsLen8),  .words = keywordsLen8  },
		[9]  = { .len = ARRAYSIZE(keywordsLen9),  .words = keywordsLen9  },
		[10] = { .len = ARRAYSIZE(keywordsLen10), .words = keywordsLen10 },
		[11] = { .len = ARRAYSIZE(keywordsLen11), .words = keywordsLen11 },
		[12] = { .len = ARRAYSIZE(keywordsLen12), .words = keywordsLen12 },
		[13] = { .len = ARRAYSIZE(keywordsLen13), .words = keywordsLen13 },
		[14] = { .len = ARRAYSIZE(keywordsLen14), .words = keywordsLen14 }
	};
	static const uint32_t wordLens[] = {
		14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2
	};
	static const size_t nwordLens = ARRAYSIZE(wordLens);

	wchar_t kwBuf[MAX_KWBUF];
	kwBuf[MAX_KWBUF - 1] = L'\0';

	uint32_t filled = 0;
	for (int32_t i = MAX_KWBUF - 2, j = (int32_t)lasti, starti = (int32_t)start; (i >= 0) && (j >= 0) && (j >= starti);)
	{
		if (((j - (int32_t)node->freeSpaceLen) == ((int32_t)node->curx - 1)) && (node->freeSpaceLen > 0))
		{
			j -= (int32_t)node->freeSpaceLen;
			continue;
		}
		kwBuf[i] = node->line[j];
		--i;
		--j;
		++filled;
	}

	for (uint32_t i = 0; i < nwordLens; ++i)
	{
		const uint32_t n = wordLens[nwordLens - i - 1];
		if ((n > filled) || ((filled - n) > 2))
		{
			continue;
		}
		for (uint32_t j = 0; j < words[n].len; ++j)
		{
			writeProfiler("checkCToken", "str: \"%S\"", kwBuf + (MAX_KWBUF - 1) - n);
			if (wcscmp(kwBuf + (MAX_KWBUF - 1) - n, words[n].words[j]) == 0)
			{
				int32_t l = (lasti > node->curx) ? (int32_t)(lasti - node->freeSpaceLen) : (int32_t)lasti;
				for (uint32_t k = 0; k < n; --l, ++k)
				{
					node->syntax[l] = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED;
				}
				return;
			}
		}
	}
}
static void checkCPPToken(femtoLineNode_t * restrict node, uint32_t start, uint32_t lasti)
{
	writeProfiler("checkCToken", "start: %u, last: %u", start, lasti);

	if ((lasti - start) < 1)
	{
		return;
	}

	struct keyword
	{
		uint8_t len;
		const wchar_t ** words;
	};

	static const wchar_t * keywordsLen2[] = {
		L"do",
		L"if",
		L"or"
	};
	static const wchar_t * keywordsLen3[] = {
		L"for",
		L"int",
		L"and",
		L"asm",
		L"new",
		L"not",
		L"xor",
		L"try"
	};
	static const wchar_t * keywordsLen4[] = {
		L"auto",
		L"case",
		L"char",
		L"else",
		L"enum",
		L"long",
		L"void",
		L"goto",
		L"this",
		L"bool",
		L"true"
	};
	static const wchar_t * keywordsLen5[] = {
		L"break",
		L"const",
		L"float",
		L"short",
		L"union",
		L"while",
		L"bitor",
		L"class",
		L"catch",
		L"compl",
		L"false",
		L"or_eq",
		L"throw",
		L"using",
		L"final"
	};
	static const wchar_t * keywordsLen6[] = {
		L"double",
		L"extern",
		L"inline",
		L"return",
		L"signed",
		L"sizeof",
		L"static",
		L"struct",
		L"switch",
		L"and_eq",
		L"bitand",
		L"delete",
		L"export",
		L"friend",
		L"not_eq",
		L"public",
		L"typeid",
		L"xor_eq",
		L"import",
		L"module"
	};
	static const wchar_t * keywordsLen7[] = {
		L"alignas",
		L"alignof",
		L"char8_t",
		L"concept",
		L"default",
		L"mutable",
		L"nullptr",
		L"private",
		L"typedef",
		L"virtual",
		L"wchar_t"
	};
	static const wchar_t * keywordsLen8[] = {
		L"char16_t",
		L"char32_t",
		L"continue",
		L"co_await",
		L"co_yield",
		L"decltype",
		L"explicit",
		L"noexcept",
		L"operator",
		L"register",
		L"requires",
		L"template",
		L"typename",
		L"unsigned",
		L"volatile",
		L"override"
	};
	static const wchar_t * keywordsLen9[] = {
		L"consteval",
		L"constexpr",
		L"constinit",
		L"co_return",
		L"namespace",
		L"protected"
	};
	static const wchar_t * keywordsLen10[] = {
		L"const_cast"
	};
	static const wchar_t * keywordsLen11[] = {
		L"static_cast"
	};
	static const wchar_t * keywordsLen12[] = {
		L"dynamic_cast",
		L"thread_local"
	};
	static const wchar_t * keywordsLen13[] = {
		L"static_assert"
	};
	static const wchar_t * keywordsLen16[] = {
		L"reinterpret_cast"
	};

	static const struct keyword words[] = {
		[2]  = { .len = ARRAYSIZE(keywordsLen2),  .words = keywordsLen2  },
		[3]  = { .len = ARRAYSIZE(keywordsLen3),  .words = keywordsLen3  },
		[4]  = { .len = ARRAYSIZE(keywordsLen4),  .words = keywordsLen4  },
		[5]  = { .len = ARRAYSIZE(keywordsLen5),  .words = keywordsLen5  },
		[6]  = { .len = ARRAYSIZE(keywordsLen6),  .words = keywordsLen6  },
		[7]  = { .len = ARRAYSIZE(keywordsLen7),  .words = keywordsLen7  },
		[8]  = { .len = ARRAYSIZE(keywordsLen8),  .words = keywordsLen8  },
		[9]  = { .len = ARRAYSIZE(keywordsLen9),  .words = keywordsLen9  },
		[10] = { .len = ARRAYSIZE(keywordsLen10), .words = keywordsLen10 },
		[11] = { .len = ARRAYSIZE(keywordsLen11), .words = keywordsLen11 },
		[12] = { .len = ARRAYSIZE(keywordsLen12), .words = keywordsLen12 },
		[13] = { .len = ARRAYSIZE(keywordsLen13), .words = keywordsLen13 },
		[16] = { .len = ARRAYSIZE(keywordsLen16), .words = keywordsLen16 }
	};
	static const uint32_t wordLens[] = {
		16, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2
	};
	static const size_t nwordLens = ARRAYSIZE(wordLens);

	wchar_t kwBuf[MAX_KWBUF];
	kwBuf[MAX_KWBUF - 1] = L'\0';

	uint32_t filled = 0;
	for (int32_t i = MAX_KWBUF - 2, j = (int32_t)lasti, starti = (int32_t)start; (i >= 0) && (j >= 0) && (j >= starti);)
	{
		if (((j - (int32_t)node->freeSpaceLen) == ((int32_t)node->curx - 1)) && (node->freeSpaceLen > 0))
		{
			j -= (int32_t)node->freeSpaceLen;
			continue;
		}
		kwBuf[i] = node->line[j];
		--i;
		--j;
		++filled;
	}

	writeProfiler("checkCPPToken", "Filled: %u, str: %S", filled, kwBuf + (MAX_KWBUF - 1) - filled);

	for (uint32_t i = 0; i < nwordLens; ++i)
	{
		const uint32_t n = wordLens[nwordLens - i - 1];
		if ((n > filled) || ((filled - n) > 2))
		{
			continue;
		}

		for (uint32_t j = 0; j < words[n].len; ++j)
		{
			writeProfiler("checkCPPToken", "str: \"%S\"", kwBuf + (MAX_KWBUF - 1) - n);
			if (wcscmp(kwBuf + (MAX_KWBUF - 1) - n, words[n].words[j]) == 0)
			{
				int32_t l = (lasti > node->curx) ? (int32_t)(lasti - node->freeSpaceLen) : (int32_t)lasti;
				for (uint32_t k = 0; k < n; --l, ++k)
				{
					node->syntax[l] = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED;
				}
				return;
			}
		}
	}
}

bool fSyntaxParseC(femtoLineNode_t * restrict node)
{
	if (!fSyntaxParseAutoAlloc(node))
	{
		return false;
	}

	uint32_t tokenStart = 0;
	bool quoteMode = false, littleQuote = false, skip = false, letter = false,
		isZero = false, hex = false, octal = false, comment = false, blockComment = false, preproc = false;
	
	blockComment = (node->prevNode != NULL) ? node->prevNode->blockComment : false;

	for (uint32_t i = 0, j = 0, previ = 0; i < node->lineEndx; ++i, ++j)
	{
		if ((i == node->curx) && (node->freeSpaceLen > 0))
		{
			i += node->freeSpaceLen;
			--i;
			--j;
			continue;
		}
		node->syntax[j] = FEMTO_DEFAULT_COLOR;


		const wchar_t ch = node->line[i];
		
		if (blockComment)
		{
			if ((ch == L'/') && (j > 0) && (node->line[previ] == L'*'))
			{
				blockComment = false;
				tokenStart = i;
			}
			node->syntax[j] = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | BACKGROUND_INTENSITY;
			previ = i;
			continue;
		}
		else if (comment)
		{
			node->syntax[j] = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE;
			previ = i;
			continue;
		}
		else if (preproc)
		{
			node->syntax[j] = FOREGROUND_INTENSITY | FOREGROUND_RED;
		}
		else if (quoteMode)
		{
			node->syntax[j] = FOREGROUND_RED;
			if (skip)
			{
				node->syntax[j] = FOREGROUND_INTENSITY | FOREGROUND_RED;
				skip = false;
			}
			else if (ch == L'\\')
			{
				node->syntax[j] = FOREGROUND_INTENSITY | FOREGROUND_RED;
				skip = true;
			}
			else if ((!littleQuote && (ch == L'"')) || (littleQuote && (ch == L'\'')))
			{
				quoteMode = false;
				tokenStart = i;
			}
			previ = i;
			continue;
		}
		else if ((ch == L'*') && (j > 0) && (node->line[previ] == L'/'))
		{
			blockComment = true;
			node->syntax[j-1] = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE;
			node->syntax[j] = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE;
			previ = i;
			continue;
		}
		else if ((ch == '/') && (j > 0) && (node->line[previ] == L'/'))
		{
			comment = true;
			node->syntax[j-1] = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE;
			node->syntax[j] = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE;
			previ = i;
			continue;
		}
		else if (isZero)
		{
			isZero = false;
			if (ch == L'x')
			{
				letter = false;
				hex = true;
				node->syntax[j] = FOREGROUND_GREEN | FOREGROUND_RED;
				previ = i;
				continue;
			}
			if ((ch >= L'0') && (ch <= '7'))
			{
				octal = true;
			}
		}
		
		switch (ch)
		{
			case L'(':
			case L')':
			case L'{':
			case L'}':
			case L'[':
			case L']':
			case L'.':
			case L',':
			case L'*':
			case L'+':
			case L'-':
			case L'/':
			case L'&':
			case L'|':
			case L'<':
			case L'>':
			case L'=':
			case L':':
				node->syntax[j] = FOREGROUND_GREEN;
				/* fall through */
			case L' ':
			case L'\t':
				letter = false;
				checkCToken(node, tokenStart, previ);
				tokenStart = i;
				break;
			case L'\'':
				littleQuote = true;
				/* fall through */
			case L'"':
				quoteMode = true;
				letter = false;
				node->syntax[j] = FOREGROUND_RED;
				break;
			case L'#':
				preproc = true;
				break;
			default:
				if (hex)
				{
					const wchar_t lch = (wchar_t)towlower(ch);
					if (!letter && (((ch >= L'0') && (ch <= L'9')) || ((lch >= L'a') && (lch <= L'f'))))
					{
						node->syntax[j] = FOREGROUND_BLUE;
					}
					else
					{
						hex = false;
						tokenStart = i;
					}
				}
				else if (isZero || octal)
				{
					isZero = false;
					if (!letter && ((ch >= L'0') && (ch <= '7')))
					{
						octal = true;
						node->syntax[j] = FOREGROUND_RED;
					}
					else
					{
						octal = false;
						tokenStart = i;
					}
				}
				else if ((ch >= L'0') && (ch <= L'9'))
				{
					if (!letter)
					{
						isZero = (ch == L'0');
						node->syntax[j] = FOREGROUND_BLUE;
						tokenStart = i;
					}
				}
				else
				{
					const wchar_t lch = (wchar_t)towlower(ch);
					letter = ((lch >= L'a') && (lch <= L'z')) || (ch == L'_');
					if (!letter)
					{
						tokenStart = i;
					}
				}
		}

		previ = i;
	}

	node->blockComment = blockComment;

	return true;
}
bool fSyntaxParseCpp(femtoLineNode_t * restrict node)
{
	if (!fSyntaxParseAutoAlloc(node))
	{
		return false;
	}

	uint32_t tokenStart = 0;
	bool quoteMode = false, littleQuote = false, skip = false, letter = false,
		isZero = false, hex = false, octal = false, comment = false, blockComment = false, preproc = false;
	
	blockComment = (node->prevNode != NULL) ? node->prevNode->blockComment : false;

	for (uint32_t i = 0, j = 0, previ = 0; i < node->lineEndx; ++i, ++j)
	{
		if ((i == node->curx) && (node->freeSpaceLen > 0))
		{
			i += node->freeSpaceLen;
			--i;
			--j;
			continue;
		}
		node->syntax[j] = FEMTO_DEFAULT_COLOR;


		const wchar_t ch = node->line[i];
		
		if (blockComment)
		{
			if ((ch == L'/') && (j > 0) && (node->line[previ] == L'*'))
			{
				blockComment = false;
				tokenStart = i;
			}
			node->syntax[j] = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | BACKGROUND_INTENSITY;
			previ = i;
			continue;
		}
		else if (comment)
		{
			node->syntax[j] = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE;
			previ = i;
			continue;
		}
		else if (preproc)
		{
			node->syntax[j] = FOREGROUND_INTENSITY | FOREGROUND_RED;
		}
		else if (quoteMode)
		{
			node->syntax[j] = FOREGROUND_RED;
			if (skip)
			{
				node->syntax[j] = FOREGROUND_INTENSITY | FOREGROUND_RED;
				skip = false;
			}
			else if (ch == L'\\')
			{
				node->syntax[j] = FOREGROUND_INTENSITY | FOREGROUND_RED;
				skip = true;
			}
			else if ((!littleQuote && (ch == L'"')) || (littleQuote && (ch == L'\'')))
			{
				quoteMode = false;
				tokenStart = i;
			}
			previ = i;
			continue;
		}
		else if ((ch == L'*') && (j > 0) && (node->line[previ] == L'/'))
		{
			blockComment = true;
			node->syntax[j-1] = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE;
			node->syntax[j] = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE;
			previ = i;
			continue;
		}
		else if ((ch == '/') && (j > 0) && (node->line[previ] == L'/'))
		{
			comment = true;
			node->syntax[j-1] = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE;
			node->syntax[j] = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE;
			previ = i;
			continue;
		}
		else if (isZero)
		{
			isZero = false;
			if (ch == L'x')
			{
				letter = false;
				hex = true;
				node->syntax[j] = FOREGROUND_GREEN | FOREGROUND_RED;
				previ = i;
				continue;
			}
			if ((ch >= L'0') && (ch <= '7'))
			{
				octal = true;
			}
		}
		
		switch (ch)
		{
			case L'(':
			case L')':
			case L'{':
			case L'}':
			case L'[':
			case L']':
			case L'.':
			case L',':
			case L'*':
			case L'+':
			case L'-':
			case L'/':
			case L'&':
			case L'|':
			case L'<':
			case L'>':
			case L'=':
			case L':':
				node->syntax[j] = FOREGROUND_GREEN;
				/* fall through */
			case L' ':
			case L'\t':
				letter = false;
				checkCPPToken(node, tokenStart, previ);
				tokenStart = i;
				break;
			case L'\'':
				littleQuote = true;
				/* fall through */
			case L'"':
				quoteMode = true;
				letter = false;
				node->syntax[j] = FOREGROUND_RED;
				break;
			case L'#':
				preproc = true;
				break;
			default:
				if (hex)
				{
					const wchar_t lch = (wchar_t)towlower(ch);
					if (!letter && (((ch >= L'0') && (ch <= L'9')) || ((lch >= L'a') && (lch <= L'f'))))
					{
						node->syntax[j] = FOREGROUND_BLUE;
					}
					else
					{
						hex = false;
						tokenStart = i;
					}
				}
				else if (isZero || octal)
				{
					isZero = false;
					if (!letter && ((ch >= L'0') && (ch <= '7')))
					{
						octal = true;
						node->syntax[j] = FOREGROUND_RED;
					}
					else
					{
						octal = false;
						tokenStart = i;
					}
				}
				else if ((ch >= L'0') && (ch <= L'9'))
				{
					if (!letter)
					{
						isZero = (ch == L'0');
						node->syntax[j] = FOREGROUND_BLUE;
						tokenStart = i;
					}
				}
				else
				{
					const wchar_t lch = (wchar_t)towlower(ch);
					letter = ((lch >= L'a') && (lch <= L'z')) || (ch == L'_');
					if (!letter)
					{
						tokenStart = i;
					}
				}
		}

		previ = i;
	}

	node->blockComment = blockComment;

	return true;
}
bool fSyntaxParseMd(femtoLineNode_t * restrict node)
{
	if (!fSyntaxParseAutoAlloc(node))
	{
		return false;
	}

	for (uint32_t i = 0, j = 0, previ = 0; i < node->lineEndx; ++i, ++j)
	{
		if ((i == node->curx) && (node->freeSpaceLen > 0))
		{
			i += node->freeSpaceLen;
			--i;
			--j;
			continue;
		}
		node->syntax[j] = FEMTO_DEFAULT_COLOR;

		

		previ = i;
	}

	return true;
}
