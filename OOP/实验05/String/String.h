#define _CRT_SECURE_NO_WARNINGS
#ifndef MY_STRING_H
#define MY_STRING_H

#include <iostream>
#include <cstring>
using namespace std;

class String {
private:
    char* str;
    int len;
    static int num_strings; // 记录字符串的总数量，在所有对象中共享
    static const int CINLIM = 80; // 输入的长度限制

public:
    String(const char* s = ""); // 用字符串初始化String对象，提供默认参数
    String(const String&); // 用String对象初始化String对象
    ~String(); // 析构函数

    int length() const { return len; } // 获取字符串长度

    String& operator=(const String&); // 使用字符串对象进行赋值
    String& operator=(const char*); // 直接使用字符串赋值
    char& operator[](int i); // 下标运算符，用于修改
    const char& operator[](int i) const; // 下标运算符，用于读取
    
	// 其他成员函数
    const char* c_str() const { return str; } // 返回C-字符串
    static int HowMany(); // 返回字符串的总数量
    
    // 重载运算符
    friend bool operator<(const String&, const String&);
    friend bool operator>(const String&, const String&);
    friend bool operator==(const String&, const String&);
    friend ostream& operator<<(ostream&, const String&);
    friend istream& operator>>(istream&, String&);
};

#endif

