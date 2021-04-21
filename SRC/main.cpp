//
// ОСНОВНОЙ МОДУЛЬ: MAIN.CPP
//
// ОПИСАНИЕ ПРОГРАММЫ:
//   Программа представляет собой однотабличную базу данных, содержащую
// сведения о домашних телефонах.
//
// ЦЕЛЕВАЯ ПЛАТФОРМА: WIN32
//
// Copyright [C] 2018 Alex Kondratenko krolmail@list.ru
//

#include "include.h"
#include "resource.h"
#include "engine\engine.h"
#include "main.h"

//
// Глобальные константы
//
const TCHAR *szNull = TEXT("");
const TCHAR *szFileMask = TEXT("*.phdb");
const TCHAR *szDateTemplate = TEXT("%02u.%02u.%04u");
const TCHAR *szDotDot = TEXT("..");
const TCHAR *szSlash = TEXT("\"");

//
// Глобальные переменные
//
PhonesDB db;                           // Основной экземпляр класса базы данных
PhoneRecT *rec;                        // Временное поле записи
FilterT search;                        // Текущий шаблон поиска
FilterT filter;                        // Текущий шаблон фильтра
FilterT old;                           // Временный шаблон
HINSTANCE hInst;                       // Экземпляр приложения
HACCEL hAccelTable;                    // Таблица акселераторов
HWND hListView;                        // Элемент управления ListView
HWND hStatusBar;                       // Элемент управления StatusBar
TCHAR szWindowClass[MAX_LOADSTRING];   // Имя класса главного окна
TCHAR szAbout[MAX_LOADSTRING];         // Строка из диалога "О программе..."
TCHAR szAboutTitle[MAX_LOADSTRING];    // Заголовок диалога "О программе..."
TCHAR szError[MAX_LOADSTRING];         // Сообщение об ошибке
TCHAR szFilePath[MAX_PATH];            // Путь до текущего файла базы данных
INT dlgfocus;                          // Фокус для элементов диалога

enum TemplateT {
    Search, Filter
} Template;                            // Идентификация редактируемого шаблона


//
//   ФУНКЦИЯ: int _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
//
//   НАЗНАЧЕНИЕ: точка входа для приложения
//
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine, int       nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	// Приложение не будет обрабатывать командую строку:
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 0. Инициализация глобальных переменных, загрузка некоторых ресурсов
	LoadString(hInstance, IDS_WNDCLASS, szWindowClass, MAX_LOADSTRING);
	LoadString(hInstance, IDS_ABOUTTITLE, szAboutTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDS_ABOUT, szAbout, MAX_LOADSTRING);
	LoadString(hInstance, IDS_ERROR, szError, MAX_LOADSTRING);
	hAccelTable = LoadAccelerators(hInstance, TEXT("APP_ACCELERATORS"));

	// 1. Проверяем, не было ли это приложение запущено ранее
	HWND hWnd = FindWindow(szWindowClass, NULL);
	if (hWnd) {
		if(IsIconic(hWnd)) // Окно приложения было свернуто в пиктограмму?
			ShowWindow(hWnd, SW_RESTORE); // Восстанавливаем его
		SetForegroundWindow(hWnd); // Выдвигаем окно приложения на передний план
		return false;
	}

	// 2. Регистрация класса окна
	if (!RegisterWndClass())
		return false;

	// 3. Создание окна и элементов управления
	hWnd = InitWnd();
	if (!hWnd)
		return false;

	// 4. Проверка коммандной строки
	while (_tcslen(lpCmdLine)>0 && lpCmdLine[0] == ' ')
		lpCmdLine++;
	if (lpCmdLine[0] == '"') {
		lpCmdLine++;
		TCHAR *t = _tcschr(lpCmdLine, '\"');
		if (t)
			t[0] = '\0';
	} else {
		TCHAR *t = _tcschr(lpCmdLine+1, ' ');
		if (t)
			t[0] = '\0';
	}
	if (_tcslen(lpCmdLine)>0) {
		PhonesDB *tdb = new PhonesDB;
		if (tdb->load(lpCmdLine)) {
			db.load(*tdb);
			_tcscpy(szFilePath, lpCmdLine);
			search.column = 0;
			filter.column = 0;
			RefreshWindow(hWnd);
		}
		delete tdb;
	}

	MSG msg;
	// 5. Цикл обработки сообщений приложения:
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(hWnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int) msg.wParam;
}



//
// ФУНКЦИЯ: ATOM RegisterWndClass()
//
// НАЗНАЧЕНИЕ: регистрирует класс окна
//
// ВОЗВРАТИТЬ: флаг успеха выполнения операции
//
ATOM RegisterWndClass() {
	// Регистрируем класс окна
	WNDCLASSEX  wcex;
	ATOM        aReturn;

	wcex.cbSize          = sizeof(WNDCLASSEX);
	wcex.style           = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc     = (WNDPROC)WndProc;
	wcex.cbClsExtra      = 0;
	wcex.cbWndExtra      = 0;
	wcex.hInstance       = hInst;
	wcex.hCursor         = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground   = (HBRUSH) (COLOR_WINDOW + 1);
	wcex.lpszMenuName    = MAKEINTRESOURCE(IDM_MAINMENU);
	wcex.lpszClassName   = szWindowClass;
	wcex.hIcon           = LoadIcon(hInst, IDI_APPLICATION);
	wcex.hIconSm         = (HICON) LoadImage(hInst,
	                       IDI_APPLICATION, IMAGE_ICON,
	                       16, 16, 0);
	aReturn = RegisterClassEx(&wcex);
	// Поддержка для предыдущих версий Windows(NT 4.5? =)
	if (!aReturn) {
		WNDCLASS wc;
		wc.style          = 0;
		wc.lpfnWndProc    = (WNDPROC)WndProc;
		wc.cbClsExtra     = 0;
		wc.cbWndExtra     = 0;
		wc.hInstance      = hInst;
		wc.hIcon          = LoadIcon(hInst, IDI_APPLICATION);
		wc.hCursor        = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground  = (HBRUSH) (COLOR_WINDOW + 1);
		wc.lpszMenuName   = MAKEINTRESOURCE(IDM_MAINMENU);
		wc.lpszClassName  = szWindowClass;
		aReturn = RegisterClass(&wc);
	}
	return aReturn;
}

//
// ФУНКЦИЯ: WHND InitWnd()
//
// НАЗНАЧЕНИЕ: создаёт окно и элементы управление, отображает окно
//
// ВОЗВРАТИТЬ: идетификатор окна / NULL
//
HWND InitWnd() {
	HWND hWnd = CreateWindow(szWindowClass, szFilePath,
	                         WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,	0,
	                         640, 480, NULL, NULL, hInst, NULL);
	if (hWnd) {
		if (!InitListView(hWnd))
			return NULL;
		if (!InitStatusBar(hWnd))
			return NULL;
		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);
		RefreshWindow(hWnd);
		return hWnd;
	} else
		return NULL;
}



//
// ФУНКЦИЯ: bool InitListView(HWND)
//
// НАЗНАЧЕНИЕ: создаёт элемент управления ListView
//
// ВОЗВРАТИТЬ: флаг успеха выполнения операции
//
bool InitListView(HWND hWnd) {
	INT dwStyle = WS_TABSTOP |
	              WS_CHILD |
	              WS_VISIBLE |
	              LVS_SHOWSELALWAYS |
	              LVS_SINGLESEL |
	              LVS_REPORT |
	              LVS_OWNERDATA;
	hListView = CreateWindowEx(WS_EX_CLIENTEDGE,
	                           WC_LISTVIEW,
	                           (PCTSTR) NULL,
	                           dwStyle,
	                           0, 0, 0, 0,
	                           hWnd,
	                           (HMENU) IDC_LISTVIEW,
	                           hInst,
	                           NULL);
	if (hListView) {
		ListView_SetExtendedListViewStyle (
		    hListView,
		    LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES
		);
		LV_COLUMN lvColumn;
		TCHAR buf[MAX_LOADSTRING];
		lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvColumn.fmt = LVCFMT_LEFT;
		lvColumn.cx = 100;
		lvColumn.pszText = buf;
		for (UINT i = 0; i < IDS_COL_num; i++) {
			LoadString(hInst, IDS_COL+i, buf, MAX_LOADSTRING);
			ListView_InsertColumn(hListView, i, &lvColumn);
		}
	}
	return (hListView != NULL);
}



//
// ФУНКЦИЯ: bool InitStatusBar(HWND)
//
// НАЗНАЧЕНИЕ: создаёт элемент управления StatusBar
//
// ВОЗВРАТИТЬ: флаг успеха выполнения операции
//
bool InitStatusBar(HWND hWnd) {
	INT dwStyle = WS_CHILD |
	              WS_VISIBLE;
	hStatusBar = CreateWindowEx(0,
	                            STATUSCLASSNAME,
	                            (PCTSTR) NULL,
	                            dwStyle,
	                            0, 0, 0, 0,
	                            hWnd,
	                            (HMENU) IDC_STATUSBAR,
	                            hInst,
	                            NULL);
	if (hStatusBar) {

	}
	return (hStatusBar != NULL);
}



//
// ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
// НАЗНАЧЕНИЕ: обрабатывает сообщения в главном окне
//
// WM_COMMAND	- обработка меню приложения
// WM_NOTIFY	- обработка запросов отображения базы данных в ListView
// WM_SIZE      - обновить отображение окна
// WM_DESTROY	- ввести сообщение о выходе и вернуться
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
	TCHAR buf[MAX_LOADSTRING*2], buf2[MAX_LOADSTRING];
	switch (message) {
		case WM_COMMAND:
			int wmId;
			wmId = LOWORD(wParam);
			UINT i;
			LPTSTR tname;
			switch (wmId) {
				case IDM_NEW:
					db.clear();
					szFilePath[0] = '\0';
					search.column = 0;
					filter.column = 0;
					RefreshWindow(hWnd);
					break;

				case IDM_OPEN:
					PhonesDB *tdb;
					tdb = new PhonesDB;
					LoadString(hInst, IDDS_OPEN_, buf, MAX_LOADSTRING);
					tname = OpenFileDialog(buf, szFileMask, hWnd);
					if (tname && _tcslen(tname)>0) {
						if (tdb->load(tname)) {
							db.load(*tdb);
							_tcscpy(szFilePath, tname);
							search.column = 0;
							filter.column = 0;
						}
					}
					delete tdb;
					RefreshWindow(hWnd);
					break;

				case IDM_SAVE:
					if (szFilePath[0]!='\0') {
						db.save(szFilePath);
						RefreshWindow(hWnd);
						break;
					}

				case IDM_SAVEAS:
					LoadString(hInst, IDDS_SAVEAS_, buf, MAX_LOADSTRING);
					tname = SaveFileDialog(buf, szFileMask, hWnd);
					if (tname && _tcslen(tname)>0) {
						if (db.save(tname)) {
							_tcscpy(szFilePath, tname);
						}
						search.column = 0;
						filter.column = 0;
					}
					RefreshWindow(hWnd);
					break;

				case IDM_QUIT:
					DestroyWindow(hWnd);
					break;

				case IDM_ADDREC:
					dlgfocus = 0;
					rec = NULL;
					if (DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_EDITDLG),
					                   hWnd, EditDlgProc, IDDS_ADDINGREC)) {
						rec->Checked = false;
						db.add(*rec);
						db.makeflist(&filter);
						db.makeslist(&search);
					}
					RefreshWindow(hWnd);
					break;

				case IDM_EDITREC:
					dlgfocus = 0;
					if ((i = LV_Selection()) < db.FList->length()) {
						LV_EditItem(hWnd, i);
					}
					RefreshWindow(hWnd);
					break;

				case IDM_DELETEREC:
					LoadString(hInst, IDS_DELETE, buf, MAX_LOADSTRING);

					if ((i = db.checked()) > 1) {
						_itot(i, buf2, 10);
						_tcscat(buf, buf2);
						LoadString(hInst, IDS_ITEMS_, buf2, MAX_LOADSTRING);
					} else if (
					    (db.checked()==1) || ((i=LV_Selection()) < db.FList->length())
					) {
						LoadString(hInst, IDS_ITEM, buf2, MAX_LOADSTRING);
					} else
						break;

					_tcscat(buf, buf2);
					LoadString(hInst, IDS_CONFIRM, buf2, MAX_LOADSTRING);

					if (MessageBox(hWnd, buf2, buf, MB_YESNO) == IDYES) {
						if (db.checked() <= 1)
							db.del(i);
						else
							for (i=0; i<db.FList->length(); i++)
								if (db.ischecked(i)) {
									db.del(i);
									i--;
								}
						db.makeflist(&filter);
						db.makeslist(&search);
					}
					RefreshWindow(hWnd);
					break;

				case IDM_REFRESH:
					RefreshWindow(hWnd);
					break;

				case IDM_CLEAR:
					db.clear();
					RefreshWindow(hWnd);
					break;

				case IDM_FINDTEMPL:
					Template = Search;
					old = search;
					if (DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_TEMPLDLG),
					                   hWnd, TemplDlgProc, IDDS_SEARCHRECS)) {
						db.makeslist(&search);
						ListView_SetSelectionMark(hListView, 0);
						LV_Next();
					} else
						search = old;
					RefreshWindow(hWnd);
					break;

				case IDM_FINDRANGE:
					Template = Search;
					old = search;
					if (DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_RANGEDLG),
					                   hWnd, RangeDlgProc, IDDS_SEARCHRECS)) {
						db.makeslist(&search);
						ListView_SetSelectionMark(hListView, 0);
						LV_Next();
					} else
						search = old;
					RefreshWindow(hWnd);
					break;

				case IDM_FINDNEXT:
					LV_Next();
					RefreshWindow(hWnd);
					break;

				case IDM_FILTEROFF:
					filter.column = 0;
					if (!filter.R && filter.cmpdata.templ)
						delete[] filter.cmpdata.templ;
					filter.cmpdata.templ = NULL;
					db.makeflist(&filter);
					db.makeslist(&search);
					RefreshWindow(hWnd);
					break;

				case IDM_NFILTERTEMPL:
					Template = Filter;
					old = filter;
					if (DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_TEMPLDLG),
					                   hWnd, TemplDlgProc, IDDS_FILTERRECS)) {
						db.makeflist(&filter);
						db.makeslist(&search);
					} else
						filter = old;
					RefreshWindow(hWnd);
					break;

				case IDM_NFILTERRANGE:
					Template = Filter;
					old = filter;
					if (DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_RANGEDLG),
					                   hWnd, RangeDlgProc, IDDS_FILTERRECS)) {
						db.makeflist(&filter);
						db.makeslist(&search);
					} else
						filter = old;
					RefreshWindow(hWnd);
					break;

				case IDM_FILTERTEMPL:
					Template = Filter;
					old = filter;
					if (DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_TEMPLDLG),
					                   hWnd, TemplDlgProc, IDDS_FILTERRECS)) {
						db.makeflist(&filter, db.FList);
						db.makeslist(&search);
					} else
						filter = old;
					RefreshWindow(hWnd);
					break;

				case IDM_FILTERRANGE:
					Template = Filter;
					old = filter;
					if (DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_RANGEDLG),
					                   hWnd, RangeDlgProc, IDDS_FILTERRECS)) {
						db.makeflist(&filter, db.FList);
						db.makeslist(&search);
					} else
						filter = old;
					RefreshWindow(hWnd);
					break;

				case IDM_INFODB:
					LoadString(hInst, IDS_TOTALRECS, buf, MAX_LOADSTRING);
					_itot(db.length(), buf2, 10);
					_tcscat(buf, buf2);
					LoadString(hInst, IDS_VISIBLERECS, buf2, MAX_LOADSTRING);
					_tcscat(buf, buf2);
					_itot(db.FList->length(), buf2, 10);
					_tcscat(buf, buf2);
					LoadString(hInst, IDS_DBINFO, buf2, MAX_LOADSTRING);
					MessageBox(hWnd, buf, buf2, MB_OK);
					break;

				case IDM_ABOUT:
					MessageBox(hWnd, szAbout, szAboutTitle, MB_OK);
					RefreshWindow(hWnd);
					break;
			}
			break;

		case WM_NOTIFY:
			LV_DISPINFO *lpdi;
			lpdi = (LV_DISPINFO*) lParam;
			NMLVKEYDOWN *lpNMKEYDOWN;
			lpNMKEYDOWN = (NMLVKEYDOWN*) lParam;
			NMITEMACTIVATE *lpNMITEMACTIVATE;
			lpNMITEMACTIVATE = (NMITEMACTIVATE*) lParam;
			UINT itemid;
			itemid = lpdi->item.iItem;

			switch (lpdi->hdr.code) {
				case LVN_GETDISPINFO:
					// Нужна информация о значениях?
					if (lpdi->item.mask & LVIF_TEXT) {
						TCHAR buf[20];
						PhoneRecT *data = db.listget(itemid);
						switch(lpdi->item.iSubItem) {
							case 0:
								_stprintf(buf, TEXT("%06u"), data->Num);
								lpdi->item.pszText = buf;
								break;
							case 1:
								lpdi->item.pszText = data->FullName;
								break;
							case 2:
								lpdi->item.pszText = data->Street;
								break;
							case 3:
								_itot(data->House, buf, 10);
								lpdi->item.pszText = buf;
								break;
							case 4:
								_itot(data->Apartment, buf, 10);
								lpdi->item.pszText = buf;
								break;
							case 5:
								DateTimeT dt;
								dt = data->Date.value;
								_stprintf(buf, szDateTemplate, dt.day,
								          dt.month, dt.year+FIRSTYEAR);
								lpdi->item.pszText = buf;
								break;
						}
					}
					// Нужна информация об изображениях/флажках?
					if (lpdi->item.mask & LVIF_IMAGE) {
						lpdi->item.iImage=0;

						lpdi->item.mask |= LVIF_STATE;
						lpdi->item.stateMask = LVIS_STATEIMAGEMASK;

						if(db.ischecked(itemid)) {
							lpdi->item.state = INDEXTOSTATEIMAGEMASK(2);
						} else {
							lpdi->item.state = INDEXTOSTATEIMAGEMASK(1);
						}
					}
					break;

				case LVN_KEYDOWN:
					switch ( lpNMKEYDOWN->wVKey ) {
						case VK_SPACE:
							LV_ToggleCheckBox(LV_Selection());
							break;
					}
					RefreshWindow(hWnd);
					break;

				case NM_CLICK:
					if (LV_HitOnItem(lpNMITEMACTIVATE) == LV_StateClick)
						LV_ToggleCheckBox(lpdi->item.mask);
					RefreshWindow(hWnd);
					break;

				case NM_DBLCLK:
					if ((LV_HitOnItem(lpNMITEMACTIVATE) == LV_ItemClick) &&
					        (itemid = LV_Selection()) < db.FList->length()) {
						dlgfocus = lpdi->item.iItem;
						LV_EditItem(hWnd, itemid);
					}
					RefreshWindow(hWnd);
					break;

				case LVN_COLUMNCLICK:
					LV_FlushSortColumn();
					db.sort(itemid + 1);
					RefreshWindow(hWnd);
					break;
			} // switch (lpdi->hdr.code)
			break;
		case WM_SIZE:
			RefreshWindow(hWnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}



//
// ФУНКЦИЯ: char *makefilterstr(FilterT &)
//
// НАЗНАЧЕНИЕ: создать информативную строку для фильтра
//
TCHAR *makefilterstr(FilterT &f) {
	TCHAR *buf = new TCHAR[MAX_LOADSTRING], buf2[MAX_LOADSTRING];
	buf[0] = '\0';
	if (f.column) {
		if (f.R) {
			if (f.column == 6) {
				UDateTimeT fdt, ldt;
				fdt.packed = f.cmpdata.range.left;
				ldt.packed = f.cmpdata.range.right;
				_stprintf(buf2, szDateTemplate,
				          fdt.value.day, fdt.value.month, fdt.value.year);
				_tcscat(buf, buf2);
				_tcscat(buf, szDotDot);
				_stprintf(buf2, szDateTemplate,
				          ldt.value.day, ldt.value.month, ldt.value.year);
				_tcscat(buf, buf2);
			} else {
				_itot(f.cmpdata.range.left, buf2, 10);
				_tcscat(buf, buf2);
				_tcscat(buf, szDotDot);
				_itot(f.cmpdata.range.right, buf2, 10);
				_tcscat(buf, buf2);
			}

		} else {
			_tcscat(buf, szSlash);
			_tcscat(buf, f.cmpdata.templ);
			_tcscat(buf, szSlash);
		}
		LoadString(hInst, IDS_ONFIELD, buf2, MAX_LOADSTRING);
		_tcscat(buf, buf2);
		int col = abs(f.column) - 1;
		LoadString(hInst, IDS_COL+col, buf2, MAX_LOADSTRING);
		_tcscat(buf, buf2);
		_tcscat(buf, szSlash);
	} else {
		LoadString(hInst, IDS_NO, buf, MAX_LOADSTRING);
	}
	return buf;
}



//
// ФУНКЦИЯ: RefreshWindow(HWND)
//
// НАЗНАЧЕНИЕ: обновляет элементы главного окна
//
void RefreshWindow(HWND hWnd) {
	TCHAR buf[MAX_LOADSTRING*2], buf2[MAX_LOADSTRING];
	const TCHAR *szArrowUp = TEXT("/\\ "),
	             *szArrowDown = TEXT("\\/ "),
	              *szHDash = TEXT(" - "),
	               *szVDash = TEXT("   |   ");
	// 0. Показать стрелку направления сортировки
	if (db.sort()) {
		LV_COLUMN *lvColumn = new LV_COLUMN;
		int col = abs(db.sort()) - 1;
		LoadString(hInst, IDS_COL+col, buf2, MAX_LOADSTRING);
		ZeroMemory(lvColumn, sizeof(LV_COLUMN));
		if (db.sort()>0)
			_tcscpy(buf, szArrowUp);
		else
			_tcscpy(buf, szArrowDown);
		_tcscat(buf, buf2);
		lvColumn->mask = LVCF_TEXT;
		lvColumn->pszText = buf;
		ListView_SetColumn(hListView, col, lvColumn);
		delete lvColumn;
	}
	// 1. Устанавливаем количество записей ListView
	ListView_SetItemCount(hListView, db.FList->length());

	// 2. Изменяем размеры ListView и StatusBar
	RECT rc, sbrc;
	GetClientRect(hWnd, &rc);
	GetClientRect(hStatusBar, &sbrc);
	LONG sbheight = (sbrc.bottom - sbrc.top);
	MoveWindow(hListView,
	           rc.left,
	           rc.top,
	           rc.right - rc.left,
	           rc.bottom - rc.top - sbheight,
	           true);
	SendMessage(hStatusBar, WM_SIZE, 0, 0);
	for (int i=0; i<IDS_COL_num; i++)
		ListView_SetColumnWidth(hListView, i, LVSCW_AUTOSIZE_USEHEADER);
	// 3. Обновляем меню
	HMENU hMenu = GetMenu(hWnd);
	if (filter.column == 0)
		EnableMenuItem(hMenu, IDM_FILTEROFF, MF_DISABLED);
	else
		EnableMenuItem(hMenu, IDM_FILTEROFF, MF_ENABLED);
	// 4. Устанавливаем заголовок окна
	LoadString(hInst, IDS_WNDCLASS, buf, MAX_LOADSTRING);
	_tcscat(buf, szHDash);
	_tcscat(buf, szFilePath);
	SetWindowText(hWnd, buf);
	// 5. Устанавливаем текст в StatusBar
	LoadString(hInst, IDS_FILTER_, buf2, MAX_LOADSTRING);
	_tcscpy(buf, buf2);
	_tcscat(buf, makefilterstr(filter));
	_tcscat(buf, szVDash);
	LoadString(hInst, IDS_SEARCH_, buf2, MAX_LOADSTRING);
	_tcscat(buf, buf2);
	_tcscat(buf, makefilterstr(search));
	SetWindowText(hStatusBar, buf);
}



//
// ФУНКЦИЯ: LPTSTR OpenFileDialog(LPTSTR, LPTSTR, HWND)
//
// НАЗНАЧЕНИЕ: обработать диалог открытия файла
//
LPTSTR OpenFileDialog(LPTSTR Title, LPCTSTR Filter, HWND hWnd) {
	OPENFILENAME ofn;
	LPTSTR buffer = (LPTSTR) operator new(MAX_PATH);
	ZeroMemory(&ofn, sizeof(ofn));
	ZeroMemory(buffer, MAX_PATH);
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = buffer;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = TEXT("Phones Database File(*.phdb)\0*.phdb\0\0");
	ofn.nFilterIndex = 1;
	ofn.hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
	    ofn.hwndOwner = hWnd;
	ofn.lpstrTitle = Title;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	GetOpenFileName(&ofn);
	return buffer;
}



//
// ФУНКЦИЯ: LPTSTR SaveFileDialog(LPTSTR, LPTSTR, HWND)
//
// НАЗНАЧЕНИЕ: обработать диалог сохранения файла
//
LPTSTR SaveFileDialog(LPTSTR Title, LPCTSTR Filter, HWND hWnd) {
	OPENFILENAME ofn;
	LPTSTR buffer = (LPTSTR) operator new(MAX_PATH);
	ZeroMemory(&ofn, sizeof(ofn));
	ZeroMemory(buffer, MAX_PATH);
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = buffer;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = TEXT("Phones Database File(*.phdb)\0*.phdb\0\0");
	ofn.nFilterIndex = 1;
	ofn.hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
	    ofn.hwndOwner = hWnd;
	ofn.lpstrTitle = Title;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	GetSaveFileName(&ofn);
	return buffer;
}



//
// ФУНКЦИЯ: INT_PTR CALLBACK EditDlgProc(HWND, UINT, WPARAM, LPARAM)
//
// НАЗНАЧЕНИЕ: обработать диалог добавления/редактирования записи
//
INT_PTR CALLBACK EditDlgProc(HWND hDlg, UINT msg, WPARAM wParam,
                             LPARAM lParam) {
	TCHAR buf[MAX_LOADSTRING];
	BOOL fError;
	HWND hDate = GetDlgItem(hDlg, IDD1_DATE); // хэндл элемента DateTimePicker32
	SYSTEMTIME windate;
	DateTimeT date;
	switch (msg) {
		case WM_INITDIALOG:
			SetFocus(GetDlgItem(hDlg, IDD1_NUM+dlgfocus));

			LoadString(hInst, lParam, buf, MAX_LOADSTRING);
			SetDlgItemText(hDlg, IDD1_CAPTION, buf);

			if (!rec) {
				rec = new PhoneRecT;
				rec->Num = 0;
				rec->FullName[0] = '\0';
				rec->Street[0] = '\0';
				rec->House = 0;
				rec->Apartment = 0;
				rec->Date.packed = 0;
			}

			if (rec->Num)
				_stprintf(buf, TEXT("%06u"), rec->Num);
			else
				buf[0] = '\0';
			SetDlgItemText(hDlg, IDD1_NUM, buf);
			SetDlgItemText(hDlg, IDD1_FULLNAME, rec->FullName);
			SetDlgItemText(hDlg, IDD1_STREET,   rec->Street);
			SetDlgItemInt (hDlg, IDD1_HOUSE,    rec->House, false);
			SetDlgItemInt (hDlg, IDD1_APART,    rec->Apartment, false);
			date = OnlyDateT(rec->Date.value);
			ZeroMemory(&windate, sizeof(windate));
			windate.wDay = date.day;
			windate.wMonth = date.month;
			windate.wYear = date.year + FIRSTYEAR;
			DateTime_SetSystemtime(hDate, GDT_VALID, &windate);
			return false;

		case WM_COMMAND:
			switch(wParam) {

				case IDD1_OK:
					// Num
					GetDlgItemText(hDlg, IDD1_NUM,
					               buf, MAX_LOADSTRING);
					if (!_tcTemplate(buf, TEXT("######"))) {
						LoadString(hInst, IDS_ERR0, buf, MAX_LOADSTRING);
						MessageBox(hDlg, buf, szError, MB_OK);
						SetFocus(GetDlgItem(hDlg, IDD1_NUM));
						return false;
					}
					rec->Num = _ttoi(buf);
					// FullName
					GetDlgItemText(hDlg, IDD1_FULLNAME,
					               buf, MAX_LOADSTRING);
					if (_tcslen(buf)>29) {
						LoadString(hInst, IDS_ERR1, buf, MAX_LOADSTRING);
						MessageBox(hDlg, buf, szError, MB_OK);
						SetFocus(GetDlgItem(hDlg, IDD1_FULLNAME));
						return false;
					}
					_tcscpy(rec->FullName, buf);
					// Street
					GetDlgItemText(hDlg, IDD1_STREET,
					               buf, MAX_LOADSTRING);
					if (_tcslen(buf)>14) {
						LoadString(hInst, IDS_ERR2, buf, MAX_LOADSTRING);
						MessageBox(hDlg, buf, szError, MB_OK);
						SetFocus(GetDlgItem(hDlg, IDD1_STREET));
						return false;
					}
					_tcscpy(rec->Street, buf);
					// House
					rec->House     = GetDlgItemInt(hDlg, IDD1_HOUSE,
					                               &fError, false);
					if (!fError) {
						LoadString(hInst, IDS_ERR3, buf, MAX_LOADSTRING);
						MessageBox(hDlg, buf, szError, MB_OK);
						SetFocus(GetDlgItem(hDlg, IDD1_HOUSE));
						return false;
					}
					// Apartment
					rec->Apartment = GetDlgItemInt(hDlg, IDD1_APART,
					                               &fError, false);
					if (!fError) {
						LoadString(hInst, IDS_ERR4, buf, MAX_LOADSTRING);
						MessageBox(hDlg, buf, szError, MB_OK);
						SetFocus(GetDlgItem(hDlg, IDD1_APART));
						return false;
					}
					// Date
					DateTime_GetSystemtime(hDate, &windate);
					ZeroMemory(&date, sizeof(date));
					date.day = windate.wDay;
					date.month = windate.wMonth;
					date.year = windate.wYear - FIRSTYEAR;
					rec->Date.value = date;
					EndDialog(hDlg, true);
					break;

				case IDD1_CANCEL:
					EndDialog(hDlg, false);
					break;
			}
			return true;
	}
	return false;
}



//
// ФУНКЦИЯ: INT_PTR CALLBACK TemplDlgProc(HWND, UINT, WPARAM, LPARAM)
//
// НАЗНАЧЕНИЕ: обработать диалог поиска/фильтрации записи по шаблону
//
INT_PTR CALLBACK TemplDlgProc(HWND hDlg, UINT msg, WPARAM wParam,
                              LPARAM lParam) {
	TCHAR buf[MAX_LOADSTRING];
	switch (msg) {
		case WM_INITDIALOG:
			SetFocus(GetDlgItem(hDlg, IDD2_FIELD));

			LoadString(hInst, lParam, buf, MAX_LOADSTRING);
			SetDlgItemText(hDlg, IDD2_CAPTION, buf);

			for (int i=0; i < IDS_COL_num; i++) {
				LoadString(hInst, IDS_COL+i, buf, MAX_LOADSTRING);
				SendDlgItemMessage(hDlg, IDD2_FIELD,
				                   CB_INSERTSTRING, -1, (LPARAM) buf);
			}
			SendDlgItemMessage(hDlg, IDD2_FIELD, CB_SETCURSEL, 0, 0);
			SetDlgItemText(hDlg, IDD2_TEMPL, szNull);
			return false;

		case WM_COMMAND:
			switch(wParam) {
				case IDD2_OK:
					FilterT *f;
					f = NULL;
					if (Template == Search)
						f = &search;
					if (Template == Filter)
						f = &filter;
					f->column = SendDlgItemMessage(
					                hDlg, IDD2_FIELD, CB_GETCURSEL, 0, 0) + 1;
					if (f->R)
						f->cmpdata.templ = NULL;
					if (!f->cmpdata.templ)
						f->cmpdata.templ = new TCHAR[MAX_LOADSTRING];
					GetDlgItemText(
					    hDlg, IDD2_TEMPL,
					    f->cmpdata.templ,
					    MAX_LOADSTRING);
					f->R = false;
					EndDialog(hDlg, true);
					break;
				case IDD2_CANCEL:
					EndDialog(hDlg, false);
					break;
			}
			return true;
	}
	return false;
}



//
// ФУНКЦИЯ: INT_PTR CALLBACK RangeDlgProc(HWND, UINT, WPARAM, LPARAM)
//
// НАЗНАЧЕНИЕ: обработать диалог поиска/фильтрации записи по диапазону
//
INT_PTR CALLBACK RangeDlgProc(HWND hDlg, UINT msg, WPARAM wParam,
                              LPARAM lParam) {
	TCHAR buf[MAX_LOADSTRING], buf2[MAX_LOADSTRING];
	UINT i, j, col;
	BOOL fError;
	switch (msg) {
		case WM_INITDIALOG:
			SetFocus(GetDlgItem(hDlg, IDD3_FIELD));

			LoadString(hInst, lParam, buf, MAX_LOADSTRING);
			SetDlgItemText(hDlg, IDD3_CAPTION, buf);

			for (i=0; i < IDS_COL_num; i++) {
				LoadString(hInst, IDS_COL+i, buf, MAX_LOADSTRING);
				SendDlgItemMessage(hDlg, IDD3_FIELD,
				                   CB_INSERTSTRING, -1, (LPARAM) buf);
			}
			SendDlgItemMessage(hDlg, IDD3_FIELD, CB_SETCURSEL, 0, 0);
			SetDlgItemText(hDlg, IDD3_LEFT, szNull);
			SetDlgItemText(hDlg, IDD3_RIGHT, szNull);
			return false;

		case WM_COMMAND:
			switch(wParam) {
				case IDD3_OK:
					FilterT *f;
					f = NULL;
					if (Template == Search)
						f = &search;
					if (Template == Filter)
						f = &filter;
					if (!f->R) {
						if (search.cmpdata.templ)
							delete[] search.cmpdata.templ;
						ZeroMemory(&search.cmpdata.range,
						           sizeof(search.cmpdata.range));
					}
					col = SendDlgItemMessage(hDlg, IDD3_FIELD,
					                         CB_GETCURSEL, 0, 0) + 1;
					if (col == 6)	{
						UDateTimeT fdt, ldt;
						UINT year, month, day;
						bool b;
						GetDlgItemText(
						    hDlg, IDD3_LEFT,
						    buf,
						    MAX_LOADSTRING);
						b =	_tcTemplate(buf, TEXT("##.##.####"));
						if (b)
							_stscanf(buf, TEXT("%2u.%2u.%4u"),
							         &day, &month, &year);
						if (!b || !CheckDateTime(day, month, year, 0, 0)) {
							LoadString(hInst, IDS_ERR5, buf, MAX_LOADSTRING);
							LoadString(hInst, IDS_DATEFORMAT, buf2, MAX_LOADSTRING);
							_tcscat(buf, buf2);
							MessageBox(hDlg, buf, szError, MB_OK);
							SetFocus(GetDlgItem(hDlg, IDD3_LEFT));
							return false;
						}
						fdt.value = LoadDateTime(day, month, year, 0, 0);
						GetDlgItemText(
						    hDlg, IDD3_RIGHT,
						    buf,
						    MAX_LOADSTRING);
						b =	_tcTemplate(buf, TEXT("##.##.####"));
						if (b)
							_stscanf(buf, TEXT("%2u.%2u.%4u"),
							         &day, &month, &year);
						if (!b || !CheckDateTime(day, month, year, 0, 0)) {
							LoadString(hInst, IDS_ERR6, buf, MAX_LOADSTRING);
							LoadString(hInst, IDS_DATEFORMAT, buf2, MAX_LOADSTRING);
							_tcscat(buf, buf2);
							MessageBox(hDlg, buf, szError, MB_OK);
							SetFocus(GetDlgItem(hDlg, IDD3_RIGHT));
							return false;
						}
						ldt.value = LoadDateTime(day, month, year, 0, 0);
						i = fdt.packed;
						j = ldt.packed;
					} else {
						i = GetDlgItemInt(hDlg, IDD3_LEFT,
						                  &fError, FALSE);
						if (!fError) {
							LoadString(hInst, IDS_ERR5, buf, MAX_LOADSTRING);
							MessageBox(hDlg, buf, szError, MB_OK);
							SetFocus(GetDlgItem(hDlg, IDD3_LEFT));
							return false;
						}
						j = GetDlgItemInt(hDlg, IDD3_RIGHT,
						                  &fError, FALSE);
						if (!fError) {
							LoadString(hInst, IDS_ERR6, buf, MAX_LOADSTRING);
							MessageBox(hDlg, buf, szError, MB_OK);
							SetFocus(GetDlgItem(hDlg, IDD3_RIGHT));
							return false;
						}
					}
					if (i > j) {
						LoadString(hInst, IDS_ERR7, buf, MAX_LOADSTRING);
						MessageBox(hDlg, buf, szError, MB_OK);
						SetFocus(GetDlgItem(hDlg, IDD3_RIGHT));
						return false;
					}
					f->column = col;
					f->cmpdata.range.left = i;
					f->cmpdata.range.right = j;
					f->R = true;
					EndDialog(hDlg, true);
					break;
				case IDD3_CANCEL:
					EndDialog(hDlg, false);
					break;
			}
			return true;
	}
	return false;
}



//
// ФУНКЦИЯ: void LV_EditItem(HWND, UINT)
//
// НАЗНАЧЕНИЕ: редактировать запись ListView
//
void LV_EditItem(HWND hWnd, UINT i) {
	rec = db.get(i);
	// В дилоге нажали OK? Заменяем старую запись
	if (DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_EDITDLG), hWnd, EditDlgProc,
	                   IDDS_EDITINGREC)) {
		db.set(i, *rec);
		db.makeflist(&filter);
		db.makeslist(&search);
	}
}



//
// ФУНКЦИЯ: int LV_HitOnItem(LPNMITEMACTIVATE)
//
// НАЗНАЧЕНИЕ: проверить, на что мы кликнули в ListView
//
int LV_HitOnItem(LPNMITEMACTIVATE lpNMITEMACTIVATE) {
	LVHITTESTINFO hitInfo;
	// Текущие координаты клика
	hitInfo.pt.x = lpNMITEMACTIVATE->ptAction.x;
	hitInfo.pt.y = lpNMITEMACTIVATE->ptAction.y;
	ListView_HitTest(hListView, &hitInfo);
	if ( (hitInfo.flags & LVHT_ONITEM) != LVHT_ONITEM ) {
		if ((hitInfo.flags & LVHT_ONITEMSTATEICON) != 0)
			return LV_StateClick;
		else
			return 0;
	} else
		return LV_ItemClick;
}



//
// ФУНКЦИЯ: UINT LV_Selection()
//
// НАЗНАЧЕНИЕ: получить номер выделенного элемента
//
UINT LV_Selection() {
	INT index;
	index = ListView_GetNextItem(hListView,
	                             -1, LVNI_ALL | LVNI_SELECTED);
	if (index == -1)
		index = db.FList->length();
	return (UINT) index;
}



//
// ФУНКЦИЯ: void LV_ToggleCheckBox(UINT)
//
// НАЗНАЧЕНИЕ: переключить состояние CheckBox-а
//
void LV_ToggleCheckBox(UINT i) {
	db.setchecked(i, !db.ischecked(i));
	ListView_RedrawItems(hListView, i, i);
}



//
// ФУНКЦИЯ: void LV_FlushSortColumn()
//
// НАЗНАЧЕНИЕ: вернуть начальный текст заголовка сортируемой колонки
//
void LV_FlushSortColumn() {
	if (db.sort()) {
		LV_COLUMN *lvColumn = new LV_COLUMN;
		int col = abs(db.sort()) - 1;
		TCHAR buf[MAX_LOADSTRING];
		ZeroMemory(lvColumn, sizeof(LV_COLUMN));
		LoadString(hInst, IDS_COL+col, buf, MAX_LOADSTRING);
		lvColumn->mask = LVCF_TEXT;
		lvColumn->pszText = buf;
		ListView_SetColumn(hListView, col, lvColumn);
		delete lvColumn;
	}
}



//
// ФУНКЦИЯ: void LV_Next()
//
// НАЗНАЧЕНИЕ: выделить следующий элемент из списка поиска
//
void LV_Next() {
	if (db.SList->length()) {
		UINT sel = LV_Selection();
		UINT i;
		i = db.SList->next(sel, db.length());
		ListView_SetItemState(hListView, i, LVIS_SELECTED, -1);
	}
}
