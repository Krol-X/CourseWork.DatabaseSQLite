//
// МОДУЛЬ: UNICODE.CPP
//
// НАЗНАЧЕНИЕ: упростить возможность перевода между кодировками
//             в UNICODE и наоборот средствами WinAPI
//
// Copyright [C] 2018 Alex Kondratenko krolmail@list.ru
// Copyright [C] 2007 Chilkat Software http://chilkatsoft.com/p/p_348.asp
//
#include "unicode.h"
#include <string.h>

//
// ФУНКЦИЯ: wchar_t *CodePageToUnicode(int, char *)
//
// НАЗНАЧЕНИЕ: Конвертировать символ в UNICODE
//
wchar_t *CodePageToUnicode(int codePage, char *src) {
	if (!src) return 0;
	int srcLen = strlen(src);
	if (!srcLen) {
		wchar_t *w = new wchar_t[1];
		w[0] = 0;
		return w;
	}
	int requiredSize = MultiByteToWideChar(codePage, 0,
	                                       src, srcLen, 0, 0);
	if (!requiredSize)
		return 0;
	wchar_t *w = new wchar_t[requiredSize+1];
	w[requiredSize] = 0;
	int retval = MultiByteToWideChar(codePage, 0,
	                                 src, srcLen, w, requiredSize);
	if (!retval) {
		delete [] w;
		return 0;
	}
	return w;
}



//
// ФУНКЦИЯ: char *UnicodeToCodePage(int, wchar_t *))
//
// НАЗНАЧЕНИЕ: Конвертировать символ из UNICODE
//
char *UnicodeToCodePage(int codePage, wchar_t *src) {
	if (!src) return 0;
	int srcLen = wcslen(src);
	if (!srcLen) {
		char *x = new char[1];
		x[0] = '\0';
		return x;
	}
	int requiredSize = WideCharToMultiByte(codePage, 0,
	                                       src, srcLen, 0, 0,
										   0, 0);
	if (!requiredSize)
		return 0;
	char *x = new char[requiredSize+1];
	x[requiredSize] = 0;
	int retval = WideCharToMultiByte(codePage, 0,
	                                 src, srcLen, x, requiredSize,
									 0, 0);
	if (!retval) {
		delete [] x;
		return 0;
	}
	return x;
}

