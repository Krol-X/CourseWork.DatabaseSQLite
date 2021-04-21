//
// ЗАГОЛОВОК ОСНОВНОГО МОДУЛЯ: MAIN.CPP
//
// ОПИСАНИЕ: содержит прототипы функций модуля
//
// Copyright [C] 2018 Alex Kondratenko krolmail@list.ru
//

#ifndef main_h_
#define main_h_

#define LV_ItemClick 1
#define LV_StateClick -1

int  APIENTRY    _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int);
ATOM             RegisterWndClass();
HWND             InitWnd();
bool             InitListView(HWND);
bool             InitStatusBar(HWND);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR *          makefilterstr(FilterT &);
void             RefreshWindow(HWND hWnd);
LPTSTR           OpenFileDialog(LPTSTR, LPCTSTR, HWND);
LPTSTR           SaveFileDialog(LPTSTR, LPCTSTR, HWND);
INT_PTR CALLBACK EditDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK TemplDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK RangeDlgProc(HWND, UINT, WPARAM, LPARAM);
void             LV_EditItem(HWND, UINT);
int              LV_HitOnItem(LPNMITEMACTIVATE);
UINT             LV_Selection();
void             LV_ToggleCheckBox(UINT);
void             LV_FlushSortColumn();
void             LV_Next();

#endif
