#include <iostream>
#ifndef SORTS_H
#define SORTS_H


template <typename T> void QuickSort(T *a, int size) {
	T pivot, temp;
	int left = 0, right = size - 1;

	if (size <= 1)
		return;

	pivot = a[right];
	do {
		while (left < right && a[left] <= pivot) {
			left++;

		}
		while (left < right && a[right] >= pivot) {
			right--;

		}
		if (left < right) {
			temp = a[left];
			a[left] = a[right];
			a[right] = temp;


		}
	} while (left < right);
	a[size - 1] = a[left];
	a[left] = pivot;

	QuickSort(a, left);
	QuickSort(a + left + 1, size - left - 1);
}

template <typename T> void BubbleSort(T *a, int n,  unsigned long long &nComp, unsigned long long &nAssign) {
	T temp;
	nComp = nAssign = 0;
	int i, j;
	for (i = 1; i < n; i++) {
		for (j = 0; j < n - i; j++) {
			nComp++;
			if (a[j] > a[j + 1]) {
				temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;
				nAssign += 3;
			}
		}
	}
}


template <typename T> void SelectSort(T *a, int size, unsigned long long &nComp, unsigned long long &nAssign) {
	T temp;
	int i, j, k = 0;
	nComp = nAssign = 0;
	for (i = 1; i < size; i++) {
		for (j = i; j < size; j++) {
			nComp++;
			if (a[j] < a[k])
				k = j;
		}
		if (k != i - 1) {
			temp = a[k];
			a[k] = a[i - 1];
			a[i - 1] = temp;
			nAssign += 3;
			nComp++;
		}
		k = i;
	}
}


template <typename T> void BubbleSortUp(T *a, int n, unsigned long long &nComp, unsigned long long &nAssign) {
	T temp;
	nComp = nAssign = 0;
	int i, j;
	int flags = 0;
	for (i = 1; i < n; i++) {
		for (j = 0; j < n - i; j++) {
			nComp++;
			if (a[j] > a[j + 1]) {
				temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;
				flags = 1;
				nAssign += 3;
			}
		}
		if (!flags)
			return;


	}
}

template <typename T> void SelectSortUp(T *a, int size,	unsigned long long &nComp, unsigned long long &nAssign) {
	int left = 0;
	int right = size - 1;
	int min = left;//存储最小值的下标
	int max = left;//存储最大值的下标
	T temp;
	while (left <= right) {
		min = left;
		max = left;
		for (int i = left; i <= right; ++i) {
			nComp++;
			if (a[i] < a[min]) {
				min = i;
			}
			nComp++;
			if (a[i] > a[max]) {
				max = i;
			}
		}
		temp = a[min];
		a[min] = a[left];
		a[left] = temp;
		nAssign += 3;
		if (left == max)
			max = min;
		temp = a[max];
		a[max] = a[right];
		a[right] = temp;
		nAssign += 3;
		++left;
		--right;
	}
}

template <typename T> void ReSet(T *data, const T *data0, int n) {//重置数据
	for (int i = 0; i < n; i++)
		data[i] = data0[i];
}

template <typename T> bool Check(const T *a, int size) {//检查排序是否正确
	for (int i = 1; i < size; i++)
		if (a[i - 1] > a[i])
			return false;
	return true;
}

template <typename T> void Copy(T *data0, const T *data, int n) {//复制数据
	for (int i = 0; i < n; i++)
		data0[i] = data[i];
}
#endif