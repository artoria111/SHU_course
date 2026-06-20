//vect.h
#ifndef VECT_H
#define VECT_H
#include <iostream>
#include <stdexcept>
using namespace std;

template <typename T> 
class Vector
{
public:
	Vector(int setdim);//默认构造函数
	Vector(const Vector<T>& v);//向量拷贝构造
	Vector(const T& item,int setdim);//数组拷贝构造
	
	~Vector();//析构函数
	
	Vector<T> operator+(const Vector<T>& v);
	Vector<T> operator-(const Vector<T>& v);
	Vector<T> operator-();//反转向量
	
	T& operator[](int index);
	Vector<T>& operator+=(const Vector<T>& v);
	T operator*(const Vector<T>& v);
	Vector<T> operator*(T n);
	bool operator==(const Vector<T>& v);
	bool operator!=(const Vector<T>& v);
	Vector<T>& operator=(const Vector<T>& v);
	
	int  getdim();//获取类内私有数据,向量维数
	void redim(int setdim);//向量维数缩放,如果是缩，则保留原向量前setdim元素的值，若括，则保留原向量的值，扩的向量维持默认值 
	
	void push_back(const T& item);//在尾插入一个元素
	void erase(int pos);//删除pos位置的元素 
	void add(int pos);//在pos位置的增加一个元素
	void reset(int pos);//重新设置pos位置的元素值

	
	friend istream& operator>>(istream& in, const Vector<T>& v)//在类内写出
	{
		for (int i = 0; i < v.dim; i++)
		{
			in >> v.data[i];
		}
		return in;
	}
	friend ostream& operator<<(ostream& out, const Vector<T>& v)
	{
		out << "{";
		for (int i = 0; i < v.dim; i++)
		{
			out << v.data[i];
			if (i < v.dim - 1)
				out << ",";
		}
		out << "}" << endl;
		return out;
	}
private:
	T* data;//数据
	int dim;//向量维度 
};

//******成员函数******

//默认构造函数
template <typename T>
Vector<T>::Vector(int setdim)
{
	dim = setdim;
	data = new T[dim];
}

//拷贝向量构造函数
template <typename T>
Vector<T>::Vector(const Vector<T>& v)
{
	dim = v.dim;
	data = new T[dim];
	for (int i = 0; i < dim; i++)
	{
		data[i] = v.data[i];
	}
}

//拷贝数组构造函数
template <typename T>
Vector<T>::Vector(const T& item,int setdim)
{
	dim = setdim;
	data = new T[setdim];
	for (int i = 0; i < dim; i++)
	{
		data[i] = item[i];
	}
}

//析构函数
template <typename T>
Vector<T>::~Vector ()
{
	delete[]data;
}

//重载加法运算符
template <typename T>
Vector<T> Vector<T>::operator+(const Vector<T>& v)
{
	try{
		if(dim != v.dim) throw"维度数不相同";
		else throw 1;
	}
	catch(const char*s){
		cout << "维度数不相同,无法运算,返回第一个向量" << endl;
		return *this;
	}
	catch(const int x){
		Vector<T> temp(dim);
		for (int i = 0; i < dim; i++)
		{
			temp.data[i] = data[i] + v.data[i];
		}
	return temp;
	}
}

//重载减法运算符
template <typename T>
Vector<T> Vector<T>::operator-(const Vector<T>& v)
{
	try{
		if(dim != v.dim) throw"维度数不相同";
		else throw 1;
	}
	catch(const char*s){
		cout << "维度数不相同,无法运算,返回第一个向量" << endl;
		return *this;
	}
	catch(const int x){
		Vector<T> temp(dim);
		for (int i = 0; i < dim; i++)
		{
			temp.data[i] = data[i] - v.data[i];
		}
	return temp;
	}
}


//重载反转向量运算符
template <typename T>
Vector<T> Vector<T>::operator-()
{
	Vector<T> temp(dim);
	
	for (int i = 0; i < dim; i++)
	{
		temp.data[i] = -data[i];
	}
	return temp;
}

//重载[]运算符
template <typename T>
T& Vector<T>::operator[](int index)
{
	if (index < 0 || index >= dim)
	{
		throw out_of_range("向量维度错误,返回第一个向量");
	}
	return data[index];
}

//重载+=运算符 
template <typename T>
Vector<T>& Vector<T>::operator+=(const Vector<T>& v)
{
	for (int i = 0; i < dim; i++)
	{
		data[i] += v.data[i];
	}
	return *this;
}

// 重载*运算符（向量与向量相乘）
template <typename T>
T Vector<T>::operator*(const Vector<T>& v)
{
	if (dim != v.dim)
	{
		throw invalid_argument("维度不匹配，无法相乘");
	}
	T sum = 0;
	for (int i = 0; i < dim; i++)
		sum += data[i] * v.data[i];
	return sum;
}

//重载*运算符 向量与数相乘
template <typename T>
Vector<T> Vector<T>::operator*(T n)
{
	Vector<T> temp(dim);
	for (int i = 0; i < dim; i++)
	{
		temp.data[i] = data[i] * n;
	}
	return temp;
}

//重载==运算符
template <typename T>
bool Vector<T>::operator==(const Vector<T>& v)
{
	int i = 0;
	for (i = 0; i < dim; i++)
	{
		if (data[i] != v.data[i])
			break;
	}
	if (i >= dim)
		return true;
	else
		return false;
}

//重载！=运算符
template <typename T>
bool Vector<T>::operator!=(const Vector<T>& v)
{
	int i = 0;
	for (i = 0; i < dim; i++)
	{
		if (data[i] != v.data[i])
			break;
	}
	if (i >= dim)
		return false;
	else
		return true;
}

//重载=运算符
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& v)
{
	dim = v.dim;
	if (data)
	{
		delete[]data;
	}
	data = new T[dim];
	for (int i = 0; i < dim; i++)
	{
		data[i] = v.data[i];
	}
	return *this;
}

//获取类内私有数据,向量维数
template <typename T>
int Vector<T>::getdim()
{
	return dim;
}

//向量维数缩放,如果是缩，则保留原向量前setdim元素的值，若括，则保留原向量的值，扩的向量维持默认值 
template <typename T>
void Vector<T>::redim(int setdim)
{
	T q;
	T* tempdata = new T[setdim];
	
	cout << "请输入新的向量数据：" << endl;
	
	for (int i = 0; i < setdim; i++)
	{
		cin >> q;
		tempdata[i] = q;
	}
	
	
	if (data)
	{
		delete[]data;
	}
	
    dim = setdim;
    data=tempdata;
}

//在尾端插入数据
template <typename T>
void Vector<T>::push_back(const T& item)
{
	dim++;
	T* tempdata = new T[dim];
	for (int i = 0; i < dim - 1; i++)
	{
		tempdata[i] = data[i];
	}
	tempdata[dim - 1] = item;
	delete[]data;
	data = tempdata;
}

//删除指定位置的元素
template <typename T>
void Vector<T>::erase(int pos)
{
	if (pos > dim)
	{
		cout << "删除位置超出了本Vector的范围";
		return ;
	}
	for (int i = pos-1; i < dim-1; i++)
	{
		data[i] = data[i+1];
	}
	T* tempData = new T[dim - 1];
	for (int i = 0; i < dim - 1; i++)
	{
		tempData[i] = data[i];
	}
	delete[]data;
	data = tempData;
	dim--;
}

//添加指定位置的元素
template <typename T>
void Vector<T>::add(int pos)
{
	T q;
	
	if (pos > dim+1)
	{
		cout << "增加位置超出了本Vector的范围";
		return;
	}
	cout << "请输入你想要添加的元素值" << endl;
	cin >> q;
	dim++;
	T* tempData = new T[dim];
	int i = 0,j=0;
	for (i = 0; i < pos - 1; i++)
	{
		tempData[i] = data[i];
		j = i;
	}
	tempData[j+1] = q;
	for (i = pos; i < dim; i++)
	{
		tempData[i] = data[i - 1];
	}
	delete[]data;
	data = tempData;
}

//修改指定位置的元素
template <typename T>
void Vector<T>::reset(int pos)
{
	T q;
	if (pos > dim+1)
	{
		cout << "增加位置超出了本Vector的范围";
		return;
	}
	cout << "请输入修改元素值" << endl;
	cin >> q;
	
	data[pos-1]=q;	
}
void readPositiveInt(int& value) ; 
#endif 

