//
// ЗАГОЛОВОК: INCLUDE.H
//
// ОПИСАНИЕ: аналог stdafx.h из MSVS
//
// Copyright [C] 2018 Alex Kondratenko krolmail@list.ru
//

#ifndef include_h_
#define include_h_

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

// Конфигурация некоторых значений
#define MAX_LOADSTRING 64

// Файлы заголовков C RunTime
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>

// Файлы заголовков Windows
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>

// Вспомогательные функции данного проекта
#include "auxiliary\auxiliary.h"

#endif
