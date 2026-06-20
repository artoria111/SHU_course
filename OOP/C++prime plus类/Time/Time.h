// Time.h
#ifndef MY_TIME_H
#define MY_TIME_H
#include <iostream>
class Time
{
private:
	int hours;
	int minutes;
public:
	Time();
	Time(int h,int m=0);
	void AddMin(int m);
	void AddHr(int h);
	void Reset(int h=0,int m=0);
	Time operator+(const Time &t)const;//重载运算符
	Time operator*(double mult)const;
	Time operator-(const Time&t)const;
	friend Time operator*(double m,const Time&t);
	friend std::ostream& operator<<(std::ostream&os,const Time&t);//友元常用于将非类的项作为其第一个操作数，反转顺序
	void Show()const;
};
#endif
