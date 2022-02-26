#ifndef SAFEC_H
#define SAFEC_H

#include <stdint.h>

/**
 * @brief "Safely" duplicate string contents on the heap
 * 
 * @param str Pointer to the string character array
 * @param len String length in characters, you can use SIZE_MAX to copy
 * everything if wstr is null-terminated
 * @return char* Pointer to the newly allocated & copied string
 */
char * strdup_s(const char * restrict str, size_t len);
/**
 * @brief "Safely" duplicate wide string contents on the heap
 * 
 * @param wstr Pointer to the wide string character array
 * @param len Wide string length in characters, you can use SIZE_MAX to copy
 * everything if wstr is null-terminated
 * @return wchar_t* Pointer to the newly allocated & copied wide string
 */
wchar_t * wcsdup_s(const wchar_t * restrict wstr, size_t len);

/**
 * @brief Duplicate string newstr to memory provided by str, reallocates memory
 * if necessary, frees memory allocated by str on memory errors
 * 
 * @param str Pointer to destination memory to use/reallocate, can be NULL, has to be null-terminated
 * @param newstr Pointer to source string to copy, cannot be NULL, has to be null-terminated
 * @return char* Pointer to newly allocated string, NULL on failure
 */
char * strredup(char * restrict str, const char * restrict newstr);
/**
 * @brief Duplicate string newstr to memory provided by str, reallocates memory
 * if necessary, frees memory allocated by str on memory errors
 * 
 * @param str Pointer to destination memory to use/reallocate, can be NULL, has to be null-terminated
 * @param newstr Pointer to source string to copy, cannot be NULL, has to be null-terminated
 * @return wchar_t* Pointer to newly allocated string, NULL on failure
 */
wchar_t * wcsredup(wchar_t * restrict str, const wchar_t * restrict newstr);

char * dynstrcat_s(char ** restrict pstr, size_t * restrict psize, size_t strLen, const char * restrict addStr);
char * dynstrncat_s(char ** restrict pstr, size_t * restrict psize, size_t strLen, const char * restrict addStr, size_t addStrLen);

wchar_t * dynwcscat_s(wchar_t ** restrict pstr, size_t * restrict psize, size_t strLen, const wchar_t * restrict addStr);
wchar_t * dynwcsncat_s(wchar_t ** restrict pstr, size_t * restrict psize, size_t strLen, const wchar_t * restrict addStr, size_t addStrLen);

#endif
