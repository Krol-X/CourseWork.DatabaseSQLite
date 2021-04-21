//
// МОДУЛЬ: SORT.CPP
//
// ОПИСАНИЕ: собственная функция сортировки
//
// Copyright [C] 2018 Alex Kondratenko krolmail@list.ru
//
#include "sort.h"
#include <memory.h>

//
// ФУНКЦИЯ: void mySort(void *, int, int, int(const void*, const void*))
//
// НАЗНАЧЕНИЕ: сортировать элементы массива используя внешнюю функцию f()
//
void mySort(void *data, int sz, int dsz, int f(const void*, const void*)) {
#define uint unsigned int
#define arr(ptr, dsz, i) ((void *)(ptr+dsz*i))
	if (sz<2)
		return;
	int i=0, j=sz-1, t=sz/2;
	void *q = arr((uint)data, dsz, t);
	void *tmp = operator new(dsz);
	do {
		while ( (i<sz-1) && (f(arr((uint)data, dsz, i), q) < 0) )
			i++;
		while ( (j>0) && (f(arr((uint)data, dsz, j), q) > 0) )
			j--;
		if (i <= j) {
			memcpy(tmp, arr((uint)data, dsz, i), dsz);
			memcpy(arr((uint)data, dsz, i), arr((uint)data, dsz, j), dsz);
			memcpy(arr((uint)data, dsz, j), tmp, dsz);
			i++;
			j--;
		}
	} while (i<=j);
	operator delete (tmp);
	if (0<j)
		mySort(data, j, dsz, f);
	if (i<sz)
		mySort(arr((uint)data, dsz, i), sz-i, dsz, f);
#undef arr
#undef uint
}
