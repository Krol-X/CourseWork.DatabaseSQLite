//
// МОДУЛЬ: ENGINE.CPP
//
// ОПИСАНИЕ: средство для работы с PhonesDB
//
// КОММЕНТАРИЙ: база данных -> фильтр -> сортировка -> таблица
//
// Copyright [C] 2018 Alex Kondratenko krolmail@list.ru
//
#include "engine.h"

//
// КОНСТРУКТОР: PhonesDB::PhonesDB()
//
PhonesDB::PhonesDB() {
	sortMode = 0;
	clear();
}



//
// ДЕСТРУКТОР: PhonesDB::~PhonesDB()
//
PhonesDB::~PhonesDB() {
	clear();
}



#include "listobj.hpp"  // Реализация методов класса ListObj
#include "db.hpp"       // Методы для работы с БД (массивом)
#include "list.hpp"     // Методы для работы со списком
#include "special.hpp"  // Специальные методы
#include "loadsave.hpp" // Методы загрузки/сохранения данных

