//
// ЗАГОЛОВОК МОДУЛЯ: ENGINE.CPP
//
// НАЗНАЧЕНИЕ: средство для работы с PhoneDB
//
// КОММЕНТАРИЙ: база данных -> фильтр -> сортировка -> таблица
//
// Copyright [C] 2018 Alex Kondratenko krolmail@list.ru
//

#ifndef engine_h_
#define engine_h_

#include "..\include.h"
#include "..\sqlite3.h"

//
// СТРУКТУРА: запись БД
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
// СТРУКТУРА: элемент списка выборки
//
struct ListItemT;



//
// КЛАСС: список
//
class PhonesDB;
class ListObj {
	private:
		ListItemT *first, *last;         // первый и последний элементы списка
		UINT size;                       // количество записей списка
	public:
		ListObj();
		~ListObj();

		UINT length();                   // Возвратить количество записей списка
		void clear();                    // Очистить список
		void add(UINT);                  // Добавить элемент в список
		UINT *get(UINT);                 // Получить значение элемента списка
		bool set(UINT, UINT);            // Установить значение элемента списка
		bool del(UINT);                  // Удалить элемента списка
		UINT next(UINT, UINT);           // Найти idx из списка больше данного
};



//
// СТРУКТУРА: элемент фильтра
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
// КЛАСС: БД
//
// КОММЕНТАРИЙ: DB -> FList (filter) -> SList (search) -> sort
//
class PhonesDB {
	private:
		PhoneRecT *data;                 // массив записей
		UINT size;                       // количество записей БД
		int sortMode;                    // режим сортировки

		// Специальные методы:
		void realloc(UINT);              // изменить размер массива записей
		ListObj *makelist(FilterT *, ListObj *src = NULL); // создать список
		// ...по фильтру (на основе другого списка)
	public:
		ListObj *FList;                  // список отфильтрованных записей
		ListObj *SList;                  // список найденных записей

		PhonesDB();
		~PhonesDB();

		// Методы для работы с БД (массивом):
		UINT length();                   // количество записей в БД
		void clear();                    // очистить БД
		void add(PhoneRecT &);           // добавить запись
		PhoneRecT *get(UINT);            // получить значение записи
		bool set(UINT, PhoneRecT &);     // установить значение записи
		bool del(UINT);                  // удалить запись
		// Работа с CheckBox-ами:
		UINT checked();                  // количество выбранных записей
		bool ischecked(UINT);            // флаг выбора записи
		void setchecked(UINT, bool);     // установить флаг выбора записи

		// Методы для работы с БД используя список фильтрации:
		PhoneRecT *listget(UINT);        // получить значение записи
		// (используя список фильтрации)
		bool listset(UINT, PhoneRecT &); // установить значение записи
		// (используя список фильтрации)
		bool listdel(UINT);              // удалить запись
		// (используя список фильтрации)

		// Специальные методы:
		void makeflist(FilterT *f = NULL, ListObj *l = NULL); // переопределить
		// ...список фильтрации (на основе другого списка)
		void makeslist(FilterT *f=NULL); // переопределить список поиска
		int sort();                      // получить текущий режим сортировки
		void sort(int);                  // сортировать записи списка

		// Методы загрузки/сохранения данных:
		void load(PhonesDB &);           // загрузить записи из другой БД
		bool load(LPTSTR);               // загрузить записи из файла
		bool save(LPTSTR);               // сохранить записи в файл
};


#endif
