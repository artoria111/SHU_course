// AbstractMain.cpp
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include "Abstract.h"
using namespace std;


void testVector() {
    int size;
    cout << "请输入向量大小: ";
    cin >> size;
    cout << endl;

    Vector<int> vec(size);
    cout << "请输入向量元素: ";
    for (int i = 0; i < size; ++i) {
        int value;
        cin >> value;
        vec[i] = value;
    }

    cout << "输入的向量是: " << vec << endl;

    int index;
    cout << "请输入要访问的下标: ";
    cin >> index;
    cout << "访问的元素: " << vec[index] << endl << endl;

    Vector<int> vec2(size);
    cout << "请输入第二个向量元素: ";
    for (int i = 0; i < size; ++i) {
        int value;
        cin >> value;
        vec2[i] = value;
    }

    int index2;
    cout << "请输入要访问的下标: ";
    cin >> index2;
    cout << "访问的元素: " << vec2[index2] << endl << endl;

    Abstract<int>* result = vec + vec2;
    cout << "两个向量的和是: " << *result << endl;

    try {
        int result = vec * vec2;
        cout << "两个向量的内积是: " << result << endl << endl;
    }
    catch (const char* e) {
        cout << e << endl;
    }

    delete result;
}

void testString() {
    cout << "请输入字符串: ";
    char input1[100];
    cin.ignore(); // 忽略之前读取的换行符
    cin.getline(input1, 100);

    String str1(input1);

    cout << "输入的字符串是: " << str1 << endl;

    int index;
    cout << "请输入要访问的字符下标: ";
    cin >> index;
    cout << "访问的字符: " << str1[index] << endl << endl;

    cout << "请输入第二个字符串: ";
    char input2[100];
    cin.ignore();
    cin.getline(input2, 100);

    String str2(input2);

    cout << "输入的第二个字符串是: " << str2 << endl;

    int index2;
    cout << "请输入要访问的字符下标: ";
    cin >> index2;
    cout << "访问的字符: " << str2[index2] << endl << endl;

    if (str1 == str2) {
        cout << "两个字符串相等。" << endl;
    } else {
        cout << "两个字符串不相等。" << endl;
    }

    if (str1 < str2) {
        cout << "\"" << str1 << "\" 小于 \"" << str2 << "\"" << endl << endl;
    } else if (str1 > str2) {
        cout << "\"" << str1 << "\" 大于 \"" << str2 << "\"" << endl << endl;
    } else {
        cout << "比较不适用（相等）" << endl << endl;
    }

    int resizeChoice;
    cout << "是否调整字符串大小？(1. 是 0. 否): ";
    cin >> resizeChoice;
    if (resizeChoice == 1) {
        int newSize;
        cout << "请输入新的大小: ";
        cin >> newSize;
        str1.resize(newSize);
        cout << "调整后的字符串: " << str1 << endl;
    }

    char reverseChoice;
    cout << "是否反转字符串？(y. 是 n. 否): ";
    cin >> reverseChoice;
    if (reverseChoice == 'y' || reverseChoice == 'Y') {
        str1.reverse();
        cout << "反转后的字符串: " << str1 << endl;
    }
}

void test() {
    int size1;
    cout << "请输入第一个向量的大小: ";
    cin >> size1;

    Vector<int> vec1(size1);
    cout << "请输入第一个向量的元素: ";
    for (int i = 0; i < size1; ++i) {
        int value;
        cin >> value;
        vec1[i] = value;
    }

    int size2;
    cout << "\n请输入第二个向量的大小: ";
    cin >> size2;

    Vector<int> vec2(size2);
    cout << "请输入第二个向量的元素: ";
    for (int i = 0; i < size2; ++i) {
        int value;
        cin >> value;
        vec2[i] = value;
    }

    Abstract<int>* result = vec1 + vec2;
    cout << "\n两个向量的和是: " << *result << endl << endl;

    cout << "\n请输入第一个字符串: ";
    char input1[100];
    cin >> input1;

    String str1(input1);

    cout << "请输入第二个字符串: ";
    char input2[100];
    cin >> input2;

    String str2(input2);

    Abstract<char>* resultStr = str1 + str2;
    cout << "\n两个字符串的合并是: " << *resultStr << endl << endl;

    // 展示两个类的交互功能
    cout << "\n假设结果向量的第一个元素作为索引，寻找（该元素+1）个字符: " << endl;
    int index = (*result)[0]; // 假设结果向量的第一个元素作为索引
    if (index >= 0 && index < resultStr->size()) {
        cout << "在合并后的字符串中，第" << index + 1 << " 个字符是: " << (*resultStr)[index] << endl;
    } else {
        cout << "索引超出范围，无法访问合并后的字符串。" << endl;
    }
}


int main()
{	
	while (1)
	{
		cout << "抽象向量类模板及其派生类测试菜单\n"
			 << "1.测试向量类模板\n"
			 << "2.测试字符串类\n"
			 << "3.联合测试\n"
			 << "0.退出\n"
			 << "请选择要进行的操作: ";
		int choice;
		cin >> choice;
		cout << endl;
		switch (choice)
		{
		case 1:
			testVector();
			break;
		case 2:
			testString();
			break;
		case 3:
			test();
			break;
		case 0:
			choice = 0;
			return 0;
		default:
			cout << "输入无效！\n";
			break;
		}
	}
	return 0;
}
