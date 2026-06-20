#include <iostream>
#include "Sort.h"
#include "DataCreate.cpp"
#include "TimeTest.cpp"
#include <stdlib.h>
#include <iomanip>
using namespace std;

string str[4] = {"正序", "逆序", "均值分布", "正态分布"};

template <typename T> void GetMemory(T *&data, T *&data0, int n) {//为数组提前分配内存
	if (data0 != NULL)
		delete [] data0;
	if (data != NULL)
		delete [] data;
	data0 = new T[n];
	data = new T[n];
}

template <typename T> void FreeMemory(T *&data, T *&data0) {//释放内存
	if (data0 != NULL)
		delete [] data0;
	if (data != NULL)
		delete [] data;
	data0 = data = NULL;
}

template <typename T> void Result(T *data, T *data0, int scale) {//输出排序结果，包括时间，比较次数，赋值次数等

	double t1, t2, t3;
	bool b1, b2, b3;

	unsigned long long comp1, comp2, assign1, assign2;//comp为比较次数，assign为赋值次数
	comp1 = comp2  = assign1 = assign2  = 0;

	gettime(1);
	BubbleSort(data, scale, comp1, assign1);
	t1 = gettime(0);
	b1 = Check(data, scale);
	ReSet(data, data0, scale);

	gettime(1);
	SelectSort(data, scale, comp2, assign2);
	t2 = gettime(0);
	b2 = Check(data, scale);
	ReSet(data, data0, scale);

	gettime(1);
	QuickSort(data, scale);
	t3 = gettime(0);
	b3 = Check(data, scale);

	cout << setw(10) << t1 << setw(10) << t2 << setw(10) << t3;
	cout << setw(15) << comp1 << setw(10) << comp2  ;
	cout << setw(10) << assign1 << setw(10) << assign2  ;
	if (!b1)
		cout << "冒泡错误" << endl;
	if (!b2)
		cout << "选择错误" << endl;
	if (!b3)
		cout << "快速错误" << endl;
	if (b1 && b2 && b3) {
		cout << '\t' << "排序成功！" << endl;
	}



}


template <typename T> void ResultUp(T *data, T *data0,
                                    int scale) {//输出优化后的排序结果，包括时间，比较次数，赋值次数等

	double t1, t2;
	bool b1, b2;

	unsigned long long comp1, comp2, assign1, assign2;
	comp1 = comp2  = assign1 = assign2  = 0;

	gettime(1);
	BubbleSortUp(data, scale, comp1, assign1);
	t1 = gettime(0);
	b1 = Check(data, scale);
	ReSet(data, data0, scale);

	gettime(1);
	SelectSortUp(data, scale, comp2, assign2);
	t2 = gettime(0);
	b2 = Check(data, scale);
	ReSet(data, data0, scale);


	cout << setw(10) << t1 << setw(10) << t2;
	cout << setw(10) << comp1 << setw(10) << comp2  ;
	cout << setw(10) << assign1 << setw(10) << assign2  ;
	if (!b1)
		cout << "冒泡错误" << endl;
	if (!b2)
		cout << "选择错误" << endl;
	if (b1 && b2) {
		cout << '\t' << "排序成功！" << endl;
	}

}




void TestScore(int scale) {//测试结构体
	Score *data = NULL;
	Score *data0 = NULL;
	GetMemory(data, data0, scale);
	ScoreCreate(data, scale);//调用函数生成结构体数据
	Copy(data0, data, scale); //备份data
	cout << setw(5) << scale;
	Result(data, data0, scale);
	FreeMemory(data, data0);
}

void TestData(int scale, int x) {//测试数据
	int *data = NULL;
	int *data0 = NULL;
	GetMemory(data, data0, scale);
	CreateData(data, scale, x); //调用函数生成数据
	Copy(data0, data, scale); //备份data
	cout << setw(5) << scale ;
	Result(data, data0, scale);
	FreeMemory(data, data0);
}

void TestString(int scale) {//测试字符串数据
	string *data = NULL;
	string *data0 = NULL;
	GetMemory(data, data0, scale);
	StringCreate(data, scale); //调用函数生成数据
	Copy(data0, data, scale); //备份data
	cout << setw(5) << scale ;
	Result(data, data0, scale);
	FreeMemory(data, data0);
}

void TestDataUp(int scale, int x) {//优化排序后测试数据
	int *data = NULL;
	int *data0 = NULL;
	GetMemory(data, data0, scale);
	CreateData(data, scale, x); //调用函数生成数据
	Copy(data0, data, scale); //备份data
	cout << setw(5) << scale;
	ResultUp(data, data0, scale);
	FreeMemory(data, data0);
}







int main() {
	int a, b;
	a = 2000;//数据规模
	b = 32000;
	cout << "结构体数据" << endl << "数据规模   冒泡排序  选择排序  快速排序    比较次数1  比较次数2  赋值次数1  赋值次数2"
	     << endl;
	for (int i = a; i <= b; i = i * 2)
		TestScore(i);

	cout << endl;
	for (int j = 0; j < 4; j++) {
		cout << str[j] << "数据" << endl <<
		     "数据规模   冒泡排序  选择排序  快速排序    比较次数1  比较次数2  赋值次数1  赋值次数2" <<
		     endl;
		for (int i = a; i <= b; i = i * 2) {
			TestData(i, j);
		}
		cout << endl;
	}

	cout << "字符串数据" << endl <<
	     "数据规模   冒泡排序  选择排序    快速排序    比较次数1  比较次数2  赋值次数1  赋值次数2" << endl;
	for (int i = a; i <= b; i = i * 2)
		TestString(i);

	cout << endl;
	cout << "优化排序算法后" << endl << endl;

	for (int j = 0; j < 4; j++) {
		cout << str[j] << "数据" << endl << "数据规模   冒泡排序   选择排序    比较次数1  比较次数2  赋值次数1  赋值次数2" <<
		     endl;
		for (int i = a; i <= b; i = i * 2) {
			TestDataUp(i, j);
		}
		cout << endl;
	}
	system("pause");
	return 0;
}
