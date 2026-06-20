#include "String.h"

int String::num_strings = 0;

String::String(const char* s) {// 用字符串初始化String对象，提供默认参数
    len = strlen(s);
    str = new char[len + 1];
    strcpy(str, s);
    num_strings++;
}

String::String(const String& st) {// 用String对象初始化String对象
    len = st.len;
    str = new char[len + 1];
    strcpy(str, st.str);
    num_strings++;
}

String::~String() {// 析构函数
    --num_strings;
    delete[] str;
}

String& String::operator=(const String& st) {// 使用字符串对象进行赋值
    if (this != &st) {
        delete[] str;
        len = st.len;
        str = new char[len + 1];
        strcpy(str, st.str);
    }
    return *this;
}

String& String::operator=(const char* s) {//直接使用字符串赋值
    if (s != str) {
        delete[] str;//清除被赋值对象的str
        len = strlen(s);
        str = new char[len + 1];
        strcpy(str, s);
    }
    return *this;
}

char& String::operator[](int i){//下标运算符，用于修改
	try{
		if(i>=len)
	   		throw "下标越界";
       }
    catch(const char*s){
    	cout<<"下标越界,返回最后一位字符"<<endl;
    	i=len-1; 
	}
	    return str[i];
}

const char&String::operator[](int i)const{// 下标运算符，用于读取
	try{
		if(i>=len) 
	   		throw "下标越界";
       }
    catch(const char*s){
    	cout<<"下标越界,返回最后一位字符"<<endl;
    	i=len-1;
	}
	    return str[i];
}
bool operator<(const String& st1, const String& st2) {
    return strcmp(st1.str, st2.str) < 0;
}

bool operator>(const String& st1, const String& st2) {
    return strcmp(st1.str, st2.str) > 0;
}

bool operator==(const String& st1, const String& st2) {
    return strcmp(st1.str, st2.str) == 0;
}

ostream& operator<<(ostream& os, const String& st) {
    os << st.str;
    return os;
}

istream&operator>>(istream&is,String &st){
	char temp[String::CINLIM];
	is.get(temp,String::CINLIM);
	if(is)
		st=temp;
	while(is&&is.get()!='\n')
		continue;
	return is;
}

int String::HowMany() {// 返回字符串的总数量
    return num_strings;
}

