//
// ЧАСТЬ МОДУЛЯ: LISTOBJ.HPP
//
// ОПИСАНИЕ: реализация методов класса ListObj
//
// Copyright [C] 2018 Alex Kondratenko krolmail@list.ru
//

//
// СТРУКТУРА: элемент списка выборки
//
struct ListItemT {
	UINT data;
	struct ListItemT* next;
	ListItemT(UINT d) {
		data = d;
		next = NULL;
	}
};


//
// КОНСТРУСТОР: ListObj::ListObj()
//
ListObj::ListObj() {
	first = last = NULL;
	size = 0;
}



//
// ДЕСТРУКТОР: ListObj::~ListObj()
//
ListObj::~ListObj() {
	if (size)
		clear();
}



//
// ФУНКЦИЯ: UINT ListObj::length()
//
// ВОЗВРАТИТЬ: количество записей списка
//
UINT ListObj::length() {
	return size;
}



//
// МЕТОД: void ListObj::clear()
//
// НАЗНАЧЕНИЕ: очистить список
//
void ListObj::clear() {
	ListItemT *c = first;
	while (c) {
		ListItemT *t = c->next;
		delete c;
		c = t;
	}
	first = last = NULL;
	size = 0;
}



//
// МЕТОД: void ListObj::add(PhoneRecT *)
//
// НАЗНАЧЕНИЕ: добавить элемент в список
//
void ListObj::add(UINT data) {
	ListItemT *t = new ListItemT(data);
	if (!first) {
		first = t;
		last = t;
	} else {
		last->next = t;
		last = t;
	}
	size++;
}



//
// МЕТОД: PhoneRecT *ListObj::get(UINT)
//
// НАЗНАЧЕНИЕ: получить значение элемента списка
//
UINT *ListObj::get(UINT idx) {
	if (idx >= size)
		return NULL;
	ListItemT *t = first;
	for (UINT i=0; i<idx && t; i++)
		t = t->next;
	return &(t->data);
}



//
// МЕТОД: bool ListObj::set(UINT, UINT)
//
// НАЗНАЧЕНИЕ: установить значение элемента списка
//
bool ListObj::set(UINT idx, UINT data) {
	if (idx < size) {
		ListItemT *t = first;
		for (UINT i=1; i<idx && t; i++)
			t = t->next;
		t->data = data;
	}
	return (idx < size);
}



//
// МЕТОД: bool ListObj::del(UINT)
//
// НАЗНАЧЕНИЕ: удалить элемента списка
//
bool ListObj::del(UINT idx) {
	if (idx < size) {
		ListItemT *t = first, *k = first;
		for (UINT i=1; i<idx && t; i++) {
			k = t;
			t = t->next;
		}
		k->next = t->next;
		k = t;
		t = t->next;
		k->next = NULL;
		delete[] k;
		size--;
	}
	return (idx < size);
}



//
// МЕТОД: UINT ListObj::next(UINT);
//
// НАЗНАЧЕНИЕ: найти idx из списка больше данного
//
UINT ListObj::next(UINT id, UINT len) {
	ListItemT *t = first;
	UINT i;
	if (id+1 < len)
		for (i=0; t; i++) {
			UINT c = t->data;
			t = t->next;
			if (c == id)
				break;
		}
	if (!t)
		return (first->data);
	return (t->data);
}

