// String.h
#ifndef MY_STRING_H
#define MY_STRING_H
#include <iostream>
using namespace std;

class String{
private:
	char*str;
	int len;
	static int num_strings;//记录字符串的总数量，在所有对象中共享
	static const int CINLIM=80;//输入的长度限制
public:
	String(const char*s);
	String();
	String(const String&);
	~String();
	int length() const {return len;}
	
	String &operator=(const String&);
	String &operator=(const char*);
	char&operator[](int i);//"&"用于减少复制结构所需要的时间
	const char&operator[](int i)const;
	
	friend bool operator<(const String&st,const String &st2);
	friend bool operator>(const String&st,const String &st2);
	friend bool operator==(const String&st,const String &st2);
	friend ostream& operator<<(ostream &os, const String &st);
	friend istream& operator>>(istream &is, String &st);
	
	static int HowMany();
};

#endif
