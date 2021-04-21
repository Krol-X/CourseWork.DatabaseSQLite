//
// ����� ������: LISTOBJ.HPP
//
// ��������: ���������� ������� ������ ListObj
//
// Copyright [C] 2018 Alex Kondratenko krolmail@list.ru
//

//
// ���������: ������� ������ �������
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
// �����������: ListObj::ListObj()
//
ListObj::ListObj() {
	first = last = NULL;
	size = 0;
}



//
// ����������: ListObj::~ListObj()
//
ListObj::~ListObj() {
	if (size)
		clear();
}



//
// �������: UINT ListObj::length()
//
// ����������: ���������� ������� ������
//
UINT ListObj::length() {
	return size;
}



//
// �����: void ListObj::clear()
//
// ����������: �������� ������
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
// �����: void ListObj::add(PhoneRecT *)
//
// ����������: �������� ������� � ������
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
// �����: PhoneRecT *ListObj::get(UINT)
//
// ����������: �������� �������� �������� ������
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
// �����: bool ListObj::set(UINT, UINT)
//
// ����������: ���������� �������� �������� ������
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
// �����: bool ListObj::del(UINT)
//
// ����������: ������� �������� ������
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
// �����: UINT ListObj::next(UINT);
//
// ����������: ����� idx �� ������ ������ �������
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

