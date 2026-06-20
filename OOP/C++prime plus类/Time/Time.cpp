// Time.cpp
#include "Time.h"
#include<iostream>
Time::Time(){
	hours=minutes=0;
}

Time::Time(int h,int m){
	hours=h;
	minutes=m;
}

void Time::AddMin(int m){
	minutes+=m;
	hours+=minutes/60;
	minutes%=60;
}

void Time::AddHr(int h){
	hours+=h;
}

void Time::Reset(int h,int m){
	hours=h;
	minutes=m;
}

void Time::Show()const{
	std::cout<<hours<<" hours, "<<minutes<<" minutes"<<std::endl;
}

Time Time::operator+(const Time&t) const{//运算符+的重载函数
	Time sum;
	sum.minutes=minutes+t.minutes;
	sum.hours=hours+t.hours+sum.minutes/60;
	sum.minutes%=60;
	return sum;
}

Time Time::operator*(double mult) const{
	Time result;
	long total_minutes=hours*60*mult+minutes*mult;
	result.hours=total_minutes/60;
	result.minutes=total_minutes%60;
	return result;
}

Time Time::operator-(const Time&t) const{
	Time result;
	long tol1=hours*60+minutes;
	long tol2=t.hours*60+t.minutes;
	if(tol1-tol2>=0){
		result.hours=(tol1-tol2)/60;
		result.minutes=(tol1-tol2)%60;
	}
	else{
		result.hours=result.minutes=0;
	}
	return result;
}

Time operator*(double m,const Time&t){
	return t*m;
}

std::ostream& operator<<(std::ostream&os,const Time&t){
	os<<t.hours<<" hours, "<<t.minutes<<" minutes"<<std::endl;
	return os;
}