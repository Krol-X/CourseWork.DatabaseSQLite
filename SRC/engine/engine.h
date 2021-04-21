//
// ��������� ������: ENGINE.CPP
//
// ����������: �������� ��� ������ � PhoneDB
//
// �����������: ���� ������ -> ������ -> ���������� -> �������
//
// Copyright [C] 2018 Alex Kondratenko krolmail@list.ru
//

#ifndef engine_h_
#define engine_h_

#include "..\include.h"
#include "..\sqlite3.h"

//
// ���������: ������ ��
//
struct PhoneRecT {
	UINT Num;
	TCHAR FullName[30];
	TCHAR Street[15];
	UINT House, Apartment;
	UDateTimeT Date;
	bool Checked;
};



//
// ���������: ������� ������ �������
//
struct ListItemT;



//
// �����: ������
//
class PhonesDB;
class ListObj {
	private:
		ListItemT *first, *last;         // ������ � ��������� �������� ������
		UINT size;                       // ���������� ������� ������
	public:
		ListObj();
		~ListObj();

		UINT length();                   // ���������� ���������� ������� ������
		void clear();                    // �������� ������
		void add(UINT);                  // �������� ������� � ������
		UINT *get(UINT);                 // �������� �������� �������� ������
		bool set(UINT, UINT);            // ���������� �������� �������� ������
		bool del(UINT);                  // ������� �������� ������
		UINT next(UINT, UINT);           // ����� idx �� ������ ������ �������
};



//
// ���������: ������� �������
//
struct FilterT {
	int column;
	bool R;
	union {
		struct {
			UINT left, right;
		} range;
		TCHAR *templ;
	} cmpdata;
};



//
// �����: ��
//
// �����������: DB -> FList (filter) -> SList (search) -> sort
//
class PhonesDB {
	private:
		PhoneRecT *data;                 // ������ �������
		UINT size;                       // ���������� ������� ��
		int sortMode;                    // ����� ����������

		// ����������� ������:
		void realloc(UINT);              // �������� ������ ������� �������
		ListObj *makelist(FilterT *, ListObj *src = NULL); // ������� ������
		// ...�� ������� (�� ������ ������� ������)
	public:
		ListObj *FList;                  // ������ ��������������� �������
		ListObj *SList;                  // ������ ��������� �������

		PhonesDB();
		~PhonesDB();

		// ������ ��� ������ � �� (��������):
		UINT length();                   // ���������� ������� � ��
		void clear();                    // �������� ��
		void add(PhoneRecT &);           // �������� ������
		PhoneRecT *get(UINT);            // �������� �������� ������
		bool set(UINT, PhoneRecT &);     // ���������� �������� ������
		bool del(UINT);                  // ������� ������
		// ������ � CheckBox-���:
		UINT checked();                  // ���������� ��������� �������
		bool ischecked(UINT);            // ���� ������ ������
		void setchecked(UINT, bool);     // ���������� ���� ������ ������

		// ������ ��� ������ � �� ��������� ������ ����������:
		PhoneRecT *listget(UINT);        // �������� �������� ������
		// (��������� ������ ����������)
		bool listset(UINT, PhoneRecT &); // ���������� �������� ������
		// (��������� ������ ����������)
		bool listdel(UINT);              // ������� ������
		// (��������� ������ ����������)

		// ����������� ������:
		void makeflist(FilterT *f = NULL, ListObj *l = NULL); // ��������������
		// ...������ ���������� (�� ������ ������� ������)
		void makeslist(FilterT *f=NULL); // �������������� ������ ������
		int sort();                      // �������� ������� ����� ����������
		void sort(int);                  // ����������� ������ ������

		// ������ ��������/���������� ������:
		void load(PhonesDB &);           // ��������� ������ �� ������ ��
		bool load(LPTSTR);               // ��������� ������ �� �����
		bool save(LPTSTR);               // ��������� ������ � ����
};


#endif
