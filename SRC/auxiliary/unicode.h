//
// ��������� ������: UNICODE.CPP
//
// ����������: ��������� ����������� �������� ����� �����������
//             � UNICODE � �������� ���������� WinAPI
//
// Copyright [C] 2018 Alex Kondratenko krolmail@list.ru
//

#ifndef unicode_h_
#define unicode_h_

#include <windows.h>

wchar_t *CodePageToUnicode(int, char *);
char *UnicodeToCodePage(int, wchar_t *);

#endif
