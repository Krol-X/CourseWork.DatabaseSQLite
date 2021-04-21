//
// ����� ������: SPECIAL.HPP
//
// ��������: ������ ��� ������ � �� (��������)
//
// Copyright [C] 2018 Alex Kondratenko krolmail@list.ru
//

//
// �����: UINT PhonesDB::length()
//
// ����������: ���������� ������� ��
//
UINT PhonesDB::length() {
	return size;
}



//
// �����: void PhonesDB::clear()
//
// ����������: �������� ��
//
void PhonesDB::clear() {
	realloc(0);
	makeflist();
	makeslist();
}



//
// �����: void PhonesDB::add(PhoneRecT &)
//
// ����������: �������� ������
//
void PhonesDB::add(PhoneRecT &rec) {
	realloc(size+1);
	data[size-1] = rec;
	makeflist();
	makeslist();
}



//
// �����: PhoneRecT *PhonesDB::get(UINT)
//
// ����������: �������� �������� ������
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
// �����: bool PhonesDB::set(UINT, PhoneRecT &)
//
// ����������: ���������� �������� ������
//
bool PhonesDB::set(UINT idx, PhoneRecT &rec) {
	if (idx < size)
		data[idx] = rec;
	return (idx < size);
}



//
// �����: bool PhonesDB::del(UINT idx)
//
// ����������: ������� ������
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
// ������ � CheckBox-���
//

//
//	�����: UINT PhonesDB::checked()
//
//	����������: ���������� ��������� �������
//
UINT PhonesDB::checked() {
	UINT r=0;
	for (UINT i=0; i < size; i++)
		if (ischecked(i))
			r++;
	return r;
}



//
//	�����: bool PhonesDB::ischecked(UINT)
//
//	����������: ���� ������ ������
//
bool PhonesDB::ischecked(UINT idx) {
	if (idx < size)
		return data[idx].Checked;
	else
		return false;
}



//
//	�����: void PhonesDB::setchecked(UINT, bool)
//
//	����������: ���������� ���� ������ ������
//
void PhonesDB::setchecked(UINT idx, bool f) {
	if (idx < size)
		data[idx].Checked = f;
}

