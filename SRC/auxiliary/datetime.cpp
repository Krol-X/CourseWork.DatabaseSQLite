//
// ������: DATETIME.CPP
//
// ��������: ����������� ��������� �������� ���� � �������, � ������������
//           �������� � ��������� SYSTEMTIME �� WinAPI
//
// Copyright [C] 2018 Alex Kondratenko krolmail@list.ru
//
#include "datetime.h"

//
// �������: DateTimeT OnlyDateT(DateTimeT)
//
// ����������: ������� ��������� ��� �������
//
DateTimeT OnlyDateT(DateTimeT dt) {
	DateTimeT r = dt;
	r.minute = 0;
	r.hour = 0;
	return r;
}



//
// �������: DateTimeT OnlyTimeT(DateTimeT)
//
// ����������: ������� ��������� ��� ����
//
DateTimeT OnlyTimeT(DateTimeT dt) {
	DateTimeT r = dt;
	r.day = 0;
	r.month = 0;
	r.year = 0;
	return r;
}



//
// �������: SYSTEMTIME ConvertDateTimeT(DateTimeT)
//
// ����������: �������������� �� DateTimeT � SYSTEMTIME
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
// �������: DateTimeT DeconvertDateTimeT(SYSTEMTIME)
//
// ����������: �������������� �� SYSTEMTIME � DateTimeT
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
// �������: SYSTEMTIME ConvertUDateTimeT(UDateTimeT)
//
// ����������: �������������� �� UDateTimeT � SYSTEMTIME
//
SYSTEMTIME ConvertUDateTimeT(UDateTimeT udt) {
	return ConvertDateTimeT(udt.value);
}



//
// �������: UDateTimeT DeconvertUDateTimeT(SYSTEMTIME)
//
// ����������: �������������� �� SYSTEMTIME � UDateTimeT
//
UDateTimeT UnconvertUDateTimeT(SYSTEMTIME st) {
	UDateTimeT udt;
	udt.value = UnconvertDateTimeT(st);
	return udt;
}



//
// �������: DateTimeT LoadDateTime(UINT, UINT, UINT, UINT, UINT);
//
// �������: ���������� ���� DateTimeT � ������� ����������
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
// �������: bool CheckDateTime(UINT, UINT, UINT, UINT, UINT);
//
// �������: ��������� ������� ��������, ��� ����� ������������ ����
//
bool CheckDateTime(UINT day, UINT month, UINT year, UINT hour, UINT minute) {
	return ((FIRSTYEAR<=year) && (year<=LASTYEAR)) &&
	       ((1<=month) && (month<=12)) &&
	       ((1<=day) && (day<=31)) &&
	       ((0<=hour) && (hour<=23)) &&
	       ((0<=minute) && (minute<=59));
}