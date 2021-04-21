//
// ������: ENGINE.CPP
//
// ��������: �������� ��� ������ � PhonesDB
//
// �����������: ���� ������ -> ������ -> ���������� -> �������
//
// Copyright [C] 2018 Alex Kondratenko krolmail@list.ru
//
#include "engine.h"

//
// �����������: PhonesDB::PhonesDB()
//
PhonesDB::PhonesDB() {
	sortMode = 0;
	clear();
}



//
// ����������: PhonesDB::~PhonesDB()
//
PhonesDB::~PhonesDB() {
	clear();
}



#include "listobj.hpp"  // ���������� ������� ������ ListObj
#include "db.hpp"       // ������ ��� ������ � �� (��������)
#include "list.hpp"     // ������ ��� ������ �� �������
#include "special.hpp"  // ����������� ������
#include "loadsave.hpp" // ������ ��������/���������� ������

