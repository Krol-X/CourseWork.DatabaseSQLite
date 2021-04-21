//
// ЧАСТЬ МОДУЛЯ: SPECIAL.HPP
//
// ОПИСАНИЕ: специальные методы
//
// Copyright [C] 2018 Alex Kondratenko krolmail@list.ru
//

//
// МЕТОД: void PhonesDB::realloc(UINT)
//
// НАЗНАЧЕНИЕ: изменить размер массива записей
//
// КОММЕНТАРИЙ: аналог realloc() из <stdlib.h>
//
void PhonesDB::realloc(UINT newsize) {
	PhoneRecT *newdata;
	if (newsize)
		newdata = new PhoneRecT[newsize];
	else
		newdata = NULL;
	UINT i=0;
	if (size > 0) {
		for (; (i < size)&&(i < newsize); i++)
			newdata[i] = data[i];
	}
	if (data)
		delete[] data;
	data = newdata;
	size = newsize;
}



////////////////////////////////////////////////////////////////////////////////
// Работа со списками
//

//
// МЕТОД: ListObj *PhonesDB::makelist(FilterT *)
//
// НАЗНАЧЕНИЕ: создать список по фильтру
//
ListObj *PhonesDB::makelist(FilterT *f, ListObj *src) {
	if (!src) {
		src = new ListObj;
		for (UINT i=0; i < size; i++)
			src->add(i);
	}
	if (f->column == 0)
		return src;
	ListObj *List = new ListObj;

	for (UINT i=0; i<src->length(); i++) {
		UINT idx = *src->get(i);
		PhoneRecT *t = get(idx);
		switch (f->R) {
			case false: // Строка-шаблон
				TCHAR buf[20];
				switch (f->column) {
					case 1:
						_itot(t->Num, buf, 10);
						if (_tcTemplate(buf, f->cmpdata.templ) != 0)
							List->add(idx);
						break;
					case 2:
						if (_tcTemplate(t->FullName, f->cmpdata.templ) != 0)
							List->add(idx);
						break;
					case 3:
						if (_tcTemplate(t->Street, f->cmpdata.templ) != 0)
							List->add(idx);
						break;
					case 4:
						_itot(t->House, buf, 10);
						if (_tcTemplate(buf, f->cmpdata.templ) != 0)
							List->add(idx);
						break;
					case 5:
						_itot(t->Apartment, buf, 10);
						if (_tcTemplate(buf, f->cmpdata.templ) != 0)
							List->add(idx);
						break;
					case 6:
						DateTimeT date = t->Date.value;
						_stprintf(buf, TEXT("%u.%u.%u"),
						          date.day, date.month, date.year);
						if (_tcTemplate(buf, f->cmpdata.templ) != 0)
							List->add(idx);
						break;
				}
				break;
			case true: // Диапазон
				switch (f->column) {
					case 1:
						if ((f->cmpdata.range.left <= t->Num) &&
						        (t->Num <= f->cmpdata.range.right))
							List->add(idx);
						break;
					case 2: // TODO: Диапазон для строки?
						List->add(idx);
						break;
					case 3: // TODO: Диапазон для строки?
						List->add(idx);
						break;
					case 4:
						if ((f->cmpdata.range.left <= t->House) &&
						        (t->House <= f->cmpdata.range.right))
							List->add(idx);
						break;
					case 5:
						if ((f->cmpdata.range.left <= t->Apartment) &&
						        (t->Apartment <= f->cmpdata.range.right))
							List->add(idx);
						break;
					case 6:
						/* TODO (_KROL#2#): Филтрация даты в диапазоне? */
						List->add(idx);
						break;
				}
				break;
		}
	}
	return List;
}



static FilterT *lastFilter = NULL;

//
// МЕТОД: void PhonesDB::makeflist(FilterT *)
//
// НАЗНАЧЕНИЕ: переопределить список фильтрации
//
void PhonesDB::makeflist(FilterT *f, ListObj *l) {
	if (f)
		lastFilter = f;
	if (!lastFilter) {
		lastFilter = new FilterT;
		lastFilter->column = 0;
	}
	if (l)
		FList = makelist(lastFilter, l);
	else
		FList = makelist(lastFilter);
}



static FilterT *lastSearch = NULL;

//
// МЕТОД: void PhonesDB::makeslist(FilterT *)
//
// НАЗНАЧЕНИЕ: переопределить список поиска
//
void PhonesDB::makeslist(FilterT *f) {
	if (f)
		lastSearch = f;
	if (!lastSearch) {
		lastSearch = new FilterT;
		lastSearch->column = 0;
	}
	SList = makelist(lastSearch);
}



////////////////////////////////////////////////////////////////////////////////
// Сортировка
//

//
//	ФУНКЦИЯ: int ___compare(const void *, const void *)
//
//	НАЗНАЧЕНИЕ: функция сравнения записей
//
INT ___sortMode;
int ___compare(const void *_x1, const void *_x2) {
	PhoneRecT *x1 = (PhoneRecT*)_x1, *x2 = (PhoneRecT*)_x2;
	BOOL bSortAscending = (___sortMode > 0);
	INT result = 0;
	if (x1==x2)
		return 0;
	else
		switch (abs(___sortMode)) {
			case 1:
				result = bSortAscending ?
				         (x1->Num - x2->Num) : (x2->Num - x1->Num);
				break;
			case 2:
				result = bSortAscending ? _tcStrCmp(x1->FullName, x2->FullName) :
				         _tcStrCmp(x2->FullName, x1->FullName);
				break;
			case 3:
				result = bSortAscending ? _tcStrCmp(x1->Street, x2->Street) :
				         _tcStrCmp(x2->Street, x1->Street);
				break;
			case 4:
				result = bSortAscending ?
				         (x1->House - x2->House) : (x2->House - x1->House);
				break;
			case 5:
				result = bSortAscending ? (x1->Apartment - x2->Apartment) :
				         (x2->Apartment - x1->Apartment);
				break;
			case 6:
				result = bSortAscending ? x1->Date.packed - x2->Date.packed :
				         x2->Date.packed - x1->Date.packed;
				break;
		}
	if (result == 0)
		result = 1;
	return result;
}



//
// МЕТОД: int PhonesDB::sort()
//
// НАЗНАЧЕНИЕ: получить текущий режим сортировки
//
int PhonesDB::sort() {
	return sortMode;
}



//
//	МЕТОД: void PhonesDB::sort(int)
//
//	НАЗНАЧЕНИЕ: сортировать записи списка
//
void PhonesDB::sort(int mode) {
	if (mode == sortMode)
		sortMode*= -1;
	else if (mode == -sortMode)
		sortMode = 0;
	else
		sortMode = mode;
	___sortMode = sortMode;
	mySort(data, size, sizeof(PhoneRecT), ___compare);
	makeflist();
	makeslist();
}

