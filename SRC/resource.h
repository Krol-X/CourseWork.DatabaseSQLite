//
// ЗАГОЛОВОК: RESOUCRE.H
//
// ОПИСАНИЕ: содержит определения идентификаторов ресурсов и элементов окна
//
// Copyright [C] 2018 Alex Kondratenko krolmail@list.ru
//

// Для "статических" ресусов, события от которых можно не отслеживать
#ifndef resouce_h_
#define resouce_h_

#ifndef IDC_STATIC
#define IDC_STATIC                      -1
#endif

#define IDC_LISTVIEW                    1
#define IDC_STATUSBAR                   2

#define IDS_WNDCLASS                    10
#define IDS_ABOUTTITLE                  11
#define IDS_ABOUT                       12
#define IDDS_ADDINGREC                  13
#define IDDS_EDITINGREC                 14
#define IDS_DELETE                      15
#define IDS_ITEM                        16
#define IDS_ITEMS_                      17
#define IDS_CONFIRM                     18
#define IDS_ERROR                       19
#define IDDS_OPEN_                      20
#define IDDS_SAVEAS_                    21
#define IDDS_SEARCHRECS                 22
#define IDDS_FILTERRECS                 23

#define IDS_DBINFO                      30
#define IDS_TOTALRECS                   31
#define IDS_VISIBLERECS                 32
#define IDS_ONFIELD                     33
#define IDS_NO                          34
#define IDS_FILTER_                     35
#define IDS_SEARCH_                     36

#define IDS_ERR0                        40
#define IDS_ERR1                        41
#define IDS_ERR2                        42
#define IDS_ERR3                        43
#define IDS_ERR4                        44
#define IDS_ERR5                        45
#define IDS_ERR6                        46
#define IDS_ERR7                        47
#define IDS_DATEFORMAT                  49

#define IDS_COL                         50
#define IDS_COL_num                     6
#define IDS_COL_NUM                     50
#define IDS_COL_FULLNAME                51
#define IDS_COL_STREET                  52
#define IDS_COL_HOUSE                   53
#define IDS_COL_APPART                  54
#define IDS_COL_DATE                    55

// Главное меню
#define IDM_MAINMENU                    100
// Файл
#define IDM_NEW                         101
#define IDM_OPEN                        102
#define IDM_SAVE                        103
#define IDM_SAVEAS                      104
#define IDM_QUIT                        105
// Список
#define IDM_ADDREC                      110
#define IDM_EDITREC                     111
#define IDM_DELETEREC                   112
#define IDM_REFRESH                     113
#define IDM_CLEAR                       114
// Инструменты
#define IDM_FINDTEMPL                   120
#define IDM_FINDRANGE                   121
#define IDM_FINDNEXT                    122
#define IDM_FILTEROFF                   123
#define IDM_NFILTERTEMPL                124
#define IDM_NFILTERRANGE                125
#define IDM_FILTERTEMPL                 126
#define IDM_FILTERRANGE                 127
#define IDM_INFODB                      128
// Справка
#define IDM_ABOUT                       130

// EditDlg
#define IDD_EDITDLG                     500
#define IDD1_CAPTION                    501
#define IDD1_NUM                        502
#define IDD1_FULLNAME                   503
#define IDD1_STREET                     504
#define IDD1_HOUSE                      505
#define IDD1_APART                      506
#define IDD1_DATE                       507
#define IDD1_OK                         508
#define IDD1_CANCEL        	            509

// TemplDlg
#define IDD_TEMPLDLG                    600
#define IDD2_CAPTION                    601
#define IDD2_FIELD                      602
#define IDD2_TEMPL                      603
#define IDD2_OK                         604
#define IDD2_CANCEL                     605

// RangeDlg
#define IDD_RANGEDLG                    700
#define IDD3_CAPTION                    701
#define IDD3_FIELD                      702
#define IDD3_LEFT                       703
#define IDD3_RIGHT                      704
#define IDD3_OK                         705
#define IDD3_CANCEL                     706

#endif
