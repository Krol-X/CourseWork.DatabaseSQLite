//
// ЗАГОЛОВОК МОДУЛЯ: DATETIME.CPP
//
// ОПИСАНИЕ: собственная структура хранения даты и времени, с возможностью
//           перевода в структуру SYSTEMTIME из WinAPI
//
// Copyright [C] 2018 Alex Kondratenko krolmail@list.ru
//

#ifndef datetime_h_
#define datetime_h_

#include <windows.h>

#define FIRSTYEAR 1752
#define LASTYEAR 3799

struct DateTimeT {
	unsigned day:5;
	unsigned month:5;
	unsigned year:11; // 1752..3799 (первый минимальный год из DateTimePicker32)
	unsigned minute:6;
	unsigned hour:5;
};

union UDateTimeT {
	DateTimeT value;
	unsigned int packed; // UINT32
};

DateTimeT OnlyDateT(DateTimeT);
DateTimeT OnlyTimeT(DateTimeT);

SYSTEMTIME ConvertDateTimeT(DateTimeT);
DateTimeT  UnconvertDateTimeT(SYSTEMTIME);
SYSTEMTIME ConvertUDateTimeT(UDateTimeT);
UDateTimeT UnconvertUDateTimeT(SYSTEMTIME);

DateTimeT LoadDateTime(UINT, UINT, UINT, UINT, UINT);
bool      CheckDateTime(UINT, UINT, UINT, UINT, UINT);

#endif

