//
// МОДУЛЬ: DATETIME.CPP
//
// ОПИСАНИЕ: собственная структура хранения даты и времени, с возможностью
//           перевода в структуру SYSTEMTIME из WinAPI
//
// Copyright [C] 2018 Alex Kondratenko krolmail@list.ru
//
#include "datetime.h"

//
// ФУНКЦИЯ: DateTimeT OnlyDateT(DateTimeT)
//
// НАЗНАЧЕНИЕ: вернуть структуру без времени
//
DateTimeT OnlyDateT(DateTimeT dt) {
	DateTimeT r = dt;
	r.minute = 0;
	r.hour = 0;
	return r;
}



//
// ФУНКЦИЯ: DateTimeT OnlyTimeT(DateTimeT)
//
// НАЗНАЧЕНИЕ: вернуть структуру без даты
//
DateTimeT OnlyTimeT(DateTimeT dt) {
	DateTimeT r = dt;
	r.day = 0;
	r.month = 0;
	r.year = 0;
	return r;
}



//
// ФУНКЦИЯ: SYSTEMTIME ConvertDateTimeT(DateTimeT)
//
// НАЗНАЧЕНИЕ: Конвертировать из DateTimeT в SYSTEMTIME
//
SYSTEMTIME ConvertDateTimeT(DateTimeT dt) {
	SYSTEMTIME st;
	st.wMilliseconds = 0;
	st.wSecond = 0;
	st.wDayOfWeek = 0;
	st.wMinute = dt.minute;
	st.wHour = dt.hour;
	st.wDay = dt.day;
	st.wMonth = dt.month;
	st.wYear = dt.year;
	return st;
}



//
// ФУНКЦИЯ: DateTimeT DeconvertDateTimeT(SYSTEMTIME)
//
// НАЗНАЧЕНИЕ: Конвертировать из SYSTEMTIME в DateTimeT
//
DateTimeT UnconvertDateTimeT(SYSTEMTIME st) {
	DateTimeT dt;
	dt.minute = st.wMinute;
	dt.hour = st.wHour;
	dt.day = st.wDay;
	dt.month = st.wMonth;
	dt.year = st.wYear;
	return dt;
}



//
// ФУНКЦИЯ: SYSTEMTIME ConvertUDateTimeT(UDateTimeT)
//
// НАЗНАЧЕНИЕ: Конвертировать из UDateTimeT в SYSTEMTIME
//
SYSTEMTIME ConvertUDateTimeT(UDateTimeT udt) {
	return ConvertDateTimeT(udt.value);
}



//
// ФУНКЦИЯ: UDateTimeT DeconvertUDateTimeT(SYSTEMTIME)
//
// НАЗНАЧЕНИЕ: Конвертировать из SYSTEMTIME в UDateTimeT
//
UDateTimeT UnconvertUDateTimeT(SYSTEMTIME st) {
	UDateTimeT udt;
	udt.value = UnconvertDateTimeT(st);
	return udt;
}



//
// ФУНКЦИЯ: DateTimeT LoadDateTime(UINT, UINT, UINT, UINT, UINT);
//
// ВЕРНУТЬ: переменную типа DateTimeT с данными значениями
//
DateTimeT LoadDateTime(UINT day, UINT month, UINT year, UINT hour, UINT minute) {
	DateTimeT value;
	value.year = year;
	value.month = month;
	value.day = day;
	value.hour = hour;
	value.minute = minute;
	return value;
}



//
// ФУНКЦИЯ: bool CheckDateTime(UINT, UINT, UINT, UINT, UINT);
//
// ВЕРНУТЬ: проверить границы значений, без учёта высокосности года
//
bool CheckDateTime(UINT day, UINT month, UINT year, UINT hour, UINT minute) {
	return ((FIRSTYEAR<=year) && (year<=LASTYEAR)) &&
	       ((1<=month) && (month<=12)) &&
	       ((1<=day) && (day<=31)) &&
	       ((0<=hour) && (hour<=23)) &&
	       ((0<=minute) && (minute<=59));
}