//
// ЧАСТЬ МОДУЛЯ: SPECIAL.HPP
//
// ОПИСАНИЕ: методы для работы с БД (массивом)
//
// Copyright [C] 2018 Alex Kondratenko krolmail@list.ru
//

//
// МЕТОД: UINT PhonesDB::length()
//
// ВОЗВРАТИТЬ: количество записей БД
//
UINT PhonesDB::length() {
	return size;
}



//
// МЕТОД: void PhonesDB::clear()
//
// НАЗНАЧЕНИЕ: очистить БД
//
void PhonesDB::clear() {
	realloc(0);
	makeflist();
	makeslist();
}



//
// МЕТОД: void PhonesDB::add(PhoneRecT &)
//
// НАЗНАЧЕНИЕ: добавить запись
//
void PhonesDB::add(PhoneRecT &rec) {
	realloc(size+1);
	data[size-1] = rec;
	makeflist();
	makeslist();
}



//
// МЕТОД: PhoneRecT *PhonesDB::get(UINT)
//
// НАЗНАЧЕНИЕ: получить значение записи
//
PhoneRecT *PhonesDB::get(UINT idx) {
	PhoneRecT *t = NULL;
	if (idx < size) {
		t = new PhoneRecT;
		*t = data[idx];
	}
	return t;
}



//
// МЕТОД: bool PhonesDB::set(UINT, PhoneRecT &)
//
// НАЗНАЧЕНИЕ: установить значение записи
//
bool PhonesDB::set(UINT idx, PhoneRecT &rec) {
	if (idx < size)
		data[idx] = rec;
	return (idx < size);
}



//
// МЕТОД: bool PhonesDB::del(UINT idx)
//
// НАЗНАЧЕНИЕ: удалить запись
//
bool PhonesDB::del(UINT idx) {
	if (idx < size) {
		for (UINT i=idx; i<size-1; i++)
			data[i] = data[i+1];
		realloc(size-1);
		makeflist();
		makeslist();
	}
	return (idx < size);
}



////////////////////////////////////////////////////////////////////////////////
// Работа с CheckBox-ами
//

//
//	МЕТОД: UINT PhonesDB::checked()
//
//	ВОЗВРАТИТЬ: количество выбранных записей
//
UINT PhonesDB::checked() {
	UINT r=0;
	for (UINT i=0; i < size; i++)
		if (ischecked(i))
			r++;
	return r;
}



//
//	МЕТОД: bool PhonesDB::ischecked(UINT)
//
//	ВОЗВРАТИТЬ: флаг выбора записи
//
bool PhonesDB::ischecked(UINT idx) {
	if (idx < size)
		return data[idx].Checked;
	else
		return false;
}



//
//	МЕТОД: void PhonesDB::setchecked(UINT, bool)
//
//	НАЗНАЧЕНИЕ: установить флаг выбора записи
//
void PhonesDB::setchecked(UINT idx, bool f) {
	if (idx < size)
		data[idx].Checked = f;
}

