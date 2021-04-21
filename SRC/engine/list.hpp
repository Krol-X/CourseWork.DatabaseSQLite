//
// ����� ������: LIST.HPP
//
// ��������: ������ ��� ������ �� �������
//
// Copyright [C] 2018 Alex Kondratenko krolmail@list.ru
//



//
// �����: PhoneRecT *PhonesDB::listget(UINT)
//
// ����������: �������� �������� ������ (��������� ������ ����������)
//
PhoneRecT *PhonesDB::listget(UINT idx) {
	return get(*(FList->get(idx)));
}



//
// �����: bool PhonesDB::listset(UINT, PhoneRecT &)
//
// ����������: ���������� �������� ������ (��������� ������ ����������)
//
bool PhonesDB::listset(UINT idx, PhoneRecT &rec) {
	return set(*FList->get(idx), rec);
}



//
// �����: bool PhonesDB::listdel(UINT)
//
// ����������: ������� ������ (��������� ������ ����������)
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
