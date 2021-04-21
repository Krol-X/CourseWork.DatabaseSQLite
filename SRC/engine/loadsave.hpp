//
// ЧАСТЬ МОДУЛЯ: SPECIAL.HPP
//
// ОПИСАНИЕ: методы загрузки/сохранения данных
//
// Copyright [C] 2018 Alex Kondratenko krolmail@list.ru
//

//
//	МЕТОД: bool PhonesDB::load(PhonesDB)
//
//	НАЗНАЧЕНИЕ: загрузить записи из другой БД
//
void PhonesDB::load(PhonesDB &db) {
	clear();
	for (UINT i=0; i<db.length(); i++)
		add(*db.get(i));
}



//
//	ФУНКЦИЯ: bool SQLexec(sqlite3 *, const char *, sqlite3_callback, void *)
//
//	НАЗНАЧЕНИЕ: упрощает вызов комманды SQL
//
bool SQLexec(sqlite3 *fdb, const char *str, sqlite3_callback callback=NULL,
             void *param = NULL) {
	TCHAR buf[MAX_PATH];
	char *err = 0;
	bool r = sqlite3_exec(fdb, str, callback, param, &err);
	if (r) {
		_stprintf(buf, TEXT("SQL Error: %sn"), err);
		MessageBox(NULL, buf, TEXT("Error!"), MB_OK | MB_ICONERROR);
		sqlite3_free(err);
	}
	return !r;
}

//
//	ФУНКЦИЯ: static int callback(void, int, char **, char **)
//
//	НАЗНАЧЕНИЕ: используется для загрузки записей
//
static int callback(void *_db, int argc, char **argv, char **azColName) {
	PhonesDB *db = (PhonesDB *) _db;
	PhoneRecT *data;
	data = new PhoneRecT;
	for(int i=0; i<argc; i++) {
		int arg = atoi(argv[i]);
		TCHAR *sarg = (TCHAR *) argv[i]; // ?
		if (strcmp(azColName[i], "num") == 0)
			data->Num = arg;
		else if (strcmp(azColName[i], "fullname") == 0)
			_tcscpy(data->FullName, sarg);
		else if (strcmp(azColName[i], "street") == 0)
			_tcscpy(data->Street, sarg);
		else if (strcmp(azColName[i], "house") == 0)
			data->House = arg;
		else if (strcmp(azColName[i], "apartment") == 0)
			data->Apartment = arg;
		else if (strcmp(azColName[i], "date") == 0) {
			UDateTimeT t;
			t.packed = arg;
			data->Date = t;
			data->Checked = false;
			db->add(*data);
			data = new PhoneRecT;
		}
	}
	delete data;
	return 0;
}

//
//	МЕТОД: bool PhonesDB::load(LPTSTR)
//
//	НАЗНАЧЕНИЕ: загрузить записи из файла
//
bool PhonesDB::load(LPTSTR name) {
	TCHAR buf[MAX_PATH];
	sqlite3 *fdb = NULL;
#ifndef UNICODE
	LPWSTR wname = CodePageToUnicode(1251, (char *) name);
	sqlite3_open16(wname, &fdb);
#else
	sqlite3_open16(name, &fdb);
#endif
	if( fdb==0 || SQLITE_OK!=sqlite3_errcode(fdb) ) {
		_stprintf(buf, TEXT("Opening Error: %s\nFile: %s"), sqlite3_errmsg(fdb), name);
		MessageBox(NULL, buf, TEXT("Error!"), MB_OK | MB_ICONERROR);
		return false;
	}
	clear();
	bool f = SQLexec(fdb, "SELECT * FROM PHDB", callback, this);
	sqlite3_close(fdb);
	return f;
}

//
//	МЕТОД: bool PhonesDB::save(LPTSTR)
//
//	НАЗНАЧЕНИЕ: сохранить записи в файл
//
bool PhonesDB::save(LPTSTR name) {
	TCHAR buf[MAX_PATH];

	const char *cm = "\
CREATE TABLE if not exists PHDB (\n\
num INT NOT NULL,\n\
fullname CHARACTER(30) NOT NULL,\n\
street CHARACTER(15) NOT NULL,\n\
house INT NOT NULL,\n\
apartment INT NOT NULL,\n\
date INT NOT NULL);";

	TCHAR *s = &name[_tcslen(name)-5];
	if (_tcscmp(s, TEXT(".phdb")))
		_tcscat(name, TEXT(".phdb"));
	remove((char *) name);
	sqlite3 *fdb = NULL;
#ifndef UNICODE
	LPWSTR wname = CodePageToUnicode(1251, name);
	sqlite3_open16(wname, &fdb);
#else
	sqlite3_open16(name, &fdb);
#endif
	if( fdb==0 || SQLITE_OK!=sqlite3_errcode(fdb) ) {
		_stprintf(buf, TEXT("Saving Error: %s\nFile: %s"), sqlite3_errmsg(fdb), name);
		MessageBox(NULL, buf, TEXT("Error!"), MB_OK | MB_ICONERROR);
		return false;
	}
	bool f;
	if ((f = SQLexec(fdb, cm)) == true) {
		for (UINT i=0; i < size; i++) {
			PhoneRecT *data = get(i);
			UDateTimeT t;
			t = data->Date;
			_stprintf(buf, TEXT("INSERT INTO PHDB values(%d, \"%s\", \"%s\", %d, %d, %d);"),
			        data->Num, data->FullName, data->Street,
			        data->House, data->Apartment, t.packed);
			SQLexec(fdb, (char *) buf); // ?
		}
	}
	sqlite3_close(fdb);
	return f;
}
