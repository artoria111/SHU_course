// Abstract.h
#ifndef MY_ABSTRACT_H
#define MY_ABSTRACT_H

#include <vector>
#include <iostream>
#include <cstring>
using namespace std;
//从Abstract类派生出Vector模板类和字符串类，把这两个类共有的方法放在基类中，并且定义为纯虚函数。
template <typename T>
class Abstract {
public:
    virtual ~Abstract() {}
    virtual int size() const = 0;
    virtual T& operator[](int index) = 0;//修改
    virtual const T& operator[](int index) const = 0;//只读
    virtual Abstract<T> *operator+(const Abstract<T> &other) = 0;
    virtual void print(ostream& out) const = 0;
    virtual Abstract<T> *operator=(const Abstract<T> &other) = 0;
    
    friend std::ostream& operator<<(std::ostream& out, const Abstract<T>& other) {
        other.print(out);// 调用派生类的打印函数
        return out;
    }
};

template <typename T>
class Vector : public Abstract<T> {
private:
    vector<T> data;
public:
    Vector(int size) : data(size) {}
    Vector(const Vector<T>& other) : data(other.data) {}
    T size() const override { return data.size(); }
    T& operator[](int index) override {
        try {
            if (index >= data.size())
                throw "下标越界";
        }
        catch (const char* s) {
            cout << "下标越界,返回Vector中最后一位" << endl;
            index = data.size() - 1;
        }
        return data[index];
    }
    const T& operator[](int index) const override {
        try {
            if (index >= data.size())
                throw "下标越界";
        }
        catch (const char* s) {
            cout << "下标越界,返回Vector中最后一位" << endl;
            index = data.size() - 1;
        }
        return data[index];
    }
//加法
    Abstract<T> *operator+(const Abstract<T> &other) override {
       	Vector<T> *result;
	try{
		if(this->size()!=other.size())
			throw "Error";
		else 
			throw 1;
	}
	catch(const int x){
		result = new Vector<T>(this->size());
		for (int i = 0; i < this->size(); ++i) {
			(*result)[i] = (*this)[i] + other[i];
		}
	}
	catch(const char*s){
		cout << "维度数不相同,无法运算,返回第一个向量" << endl;
		result = this;
	}
	return result;
    }
//输出
    void print(ostream& out) const override {
        out << "{";
        for (int i = 0; i < data.size(); i++) {
            out << data[i];
            if (i < data.size() - 1)
                out << ",";
        }
        out << "}";
    }
//内积
    T operator*(const Abstract<T> &other) const {
     	T result=0;
	for(int i=0;i<other.size();i++)
		result+=other[i]*(*this)[i];
		
	return result;
    }
//赋值
    Abstract<T> *operator=(const Abstract<T> &other) override {
	Vector<T> *result = new Vector<T>(other.size());
	for (int i = 0; i < this->size(); ++i) {
			(*result)[i] = other[i];
		}
	
	return result;
    }
    // 新增 resize 方法
    void resize(int newSize) {
        data.resize(newSize);
    }
};

class String : public Abstract<char> {
private:
    char* str;
    int len;
    static int num_strings;// 记录字符串的总数量，在所有对象中共享
public:
// 用字符串初始化String对象，提供默认参数
    String(const char* s = "") {
        len = strlen(s);
        str = new char[len + 1];
        strcpy(str, s);
        num_strings++;
    }
// 用String对象初始化String对象
    String(const String& st) {
        len = st.len;
        str = new char[len + 1];
        strcpy(str, st.str);
        num_strings++;
    }
//析构函数
    ~String() {
        num_strings--;
        delete[] str;
    }
// 下标运算符，用于修改
    char& operator[](int i) override {
        try {
            if (i >= len)
                throw "下标越界";
        }
        catch (const char* s) {
            cout << "下标越界,返回最后一位字符" << endl;
            i = len - 1;
        }
        return str[i];
    }
// 下标运算符，用于读取
    const char& operator[](int i) const override {
        try {
            if (i >= len)
                throw "下标越界";
        }
        catch (const char* s) {
            cout << "下标越界,返回最后一位字符" << endl;
            i = len - 1;
        }
        return str[i];
    }
    int size() const override { return len; }
//输出
    void print(ostream& out) const override {
        out << str;
    }
//赋值
    Abstract<char> *operator=(const Abstract<char> &other) override {
	const String& otherStr = dynamic_cast<const String&>(other);
    	if (this != &otherStr) {
       	 delete[] str;
       	 len = otherStr.len;
      	  str = new char[len + 1];
      	  strcpy(str, otherStr.str);
   	 }
   	 return this;
    }
//字符串合并
    Abstract<char> *operator+(const Abstract<char> &other) override {
	const String& otherStr = dynamic_cast<const String&>(other);
	char* result=new char[len+otherStr.len+1];
	strcpy(result, str);
    	strcat(result, otherStr.str);
    
	return new String(result);
    }
    bool operator==(const String& other) const {
        return strcmp(str, other.str) == 0;
    }
    bool operator!=(const String& other) const {
        return !(*this == other);
    }
    bool operator<(const String& other) const {
        return strcmp(str, other.str) < 0;
    }
    bool operator>(const String& other) const {
        return strcmp(str, other.str) > 0;
    }
    static int HowMany() {
        return num_strings;
    }
    // 新增 reverse 方法
    void reverse() {
        for (int i = 0; i < len / 2; ++i) {
            char temp = str[i];
            str[i] = str[len - 1 - i];
            str[len - 1 - i] = temp;
        }
    }
    // 新增 resize 方法
    void resize(int newSize) {
        if (newSize > len) {
            char* tempStr = new char[newSize + 1];
            strncpy(tempStr, str, len);
            tempStr[len] = '\0';
            delete[] str;
            str = tempStr;
            len = newSize;
        } else if (newSize < len) {
            str[newSize] = '\0';
            len = newSize;
        }
    }
};

int String::num_strings = 0;

#endif 
