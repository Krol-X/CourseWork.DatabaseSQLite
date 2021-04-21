//
// ЧАСТЬ МОДУЛЯ: LIST.HPP
//
// ОПИСАНИЕ: методы для работы со списком
//
// Copyright [C] 2018 Alex Kondratenko krolmail@list.ru
//



//
// МЕТОД: PhoneRecT *PhonesDB::listget(UINT)
//
// НАЗНАЧЕНИЕ: получить значение записи (используя список фильтрации)
//
PhoneRecT *PhonesDB::listget(UINT idx) {
	return get(*(FList->get(idx)));
}



//
// МЕТОД: bool PhonesDB::listset(UINT, PhoneRecT &)
//
// НАЗНАЧЕНИЕ: установить значение записи (используя список фильтрации)
//
bool PhonesDB::listset(UINT idx, PhoneRecT &rec) {
	return set(*FList->get(idx), rec);
}



//
// МЕТОД: bool PhonesDB::listdel(UINT)
//
// НАЗНАЧЕНИЕ: удалить запись (используя список фильтрации)
//
bool PhonesDB::listdel(UINT idx) {
	UINT *IDX = FList->get(idx);
	if (IDX) {
		del(*IDX);
		FList->del(idx);
		for (UINT i=0; i<FList->length(); i++) {
			UINT *cur = FList->get(i);
			if (*cur>*IDX)
				FList->set(i, *cur-1);
		}
		return true;
	} else
	return false;
}
