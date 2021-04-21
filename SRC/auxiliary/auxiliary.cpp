//
// МОДУЛЬ: AUXILIARY.CPP
//
// ОПИСАНИЕ: вспомогательные функции данного проекта
//
// Copyright [C] 2018 Alex Kondratenko krolmail@list.ru
//
#include "auxiliary.h"

//
// ФУНКЦИЯ: int _tcStrСmp(TCHAR *, TCHAR *)
//
// НАЗНАЧЕНИЕ: сравнить две строки (возможно разной длины)
//
int _tcStrCmp(TCHAR *a, TCHAR *b) {
	int lena=_tcslen(a);
	int lenb=_tcslen(b);
	if (lena && !lenb)
		return 1;
	if (!lena && lenb)
		return -1;
	if (lena<lenb)
		return _tcsncmp(a, b, lena);
	if (lena>lenb)
		return _tcsncmp(a, b, lenb);
	else
		return _tcscmp(a, b);
}

//
// ФУНКЦИЯ: bool _tcTemplate(TCHAR *, TCHAR *)
//
// НАЗНАЧЕНИЕ: сравнить строку c шаблоном
//
bool _tcTemplate(TCHAR *str, const TCHAR *pat) {
	if (*str == '\0' && *pat == '\0')
		return true;
	if (*pat == '\0')
		return false;
	if (*pat == '*') {
		if (*(pat+1) == '\0')
			return true;
		if (*(pat+1) == '*') {
			if (*str == '*')
				return _tcTemplate(str+1, pat+2);
		} else {
			for (int i=0; i<=_tcslen(str); i++) {
				if ( (*(str+i) == *(pat+1)) ||
				        (*(pat+1) == '?') ||
				        (*(pat+1) == '#') ||
				        (*(pat+1) == '$') )
					if (_tcTemplate(str+i, pat+1))
						return true;
			}
		}
	} else {
		if (*str == '\0')
			return false;
		switch (*pat) {
			case '#':
				if (!('0' <= *str && *str <= '9'))
					break;
				return _tcTemplate(str+1, pat+1);
			case '$':
				if (!(('a' <= *str && *str <= 'z') ||
				        ('A' <= *str && *str <= 'Z')))
					break;
				return _tcTemplate(str+1, pat+1);
			case '?':
				if (*(pat+1) == '?') {
					if (*str != '?')
						break;
				}
			default:
				if (*str == *pat)
					return _tcTemplate(str+1, pat+1);
		}
	}
	return false;
}
