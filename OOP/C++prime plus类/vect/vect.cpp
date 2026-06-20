// vect.cpp
#include "vect.h"
#include<cmath>
using namespace std;

const double Rad_to_deg=45.0/atan(1.0);//弧度转化为角度

void Vector::set_mag(){
	mag=sqrt(x*x+y*y);
}

void Vector::set_ang(){
	ang=atan2(y,x);
}

void Vector::set_x(){
	x=mag*cos(ang);
}

void Vector::set_y(){
	y=mag*sin(ang);
}

Vector::Vector(){
	x=y=mag=ang=0.0;
	mode=RECT;
}
//初始化对象
Vector::Vector(double n1,double n2,Mode form){
	mode=form;
	if(mode==RECT){
		x=n1;
		y=n2;
		set_mag();
		set_ang();
	}
	else if(mode==POL){
		mag=n1;
		ang=n2/Rad_to_deg;
		set_x();
		set_y();
	}
	else{
		cout<<"ERROR! "<<"Vector set to 0"<<endl;
		x=y=ang=mag=0.0;
		mode=RECT;
	}
}
//改变对象中的参数值
void Vector::reset(double n1,double n2,Mode form){
	mode=form;
	if(mode==RECT){
		x=n1;
		y=n2;
		set_mag();
		set_ang();
	}
	else if(mode==POL){
		mag=n1;
		ang=n2/Rad_to_deg;
		set_x();
		set_y();
	}
	else{
		cout<<"ERROR! "<<"Vector set to 0"<<endl;
		x=y=ang=mag=0.0;
		mode=RECT;
	}
}

Vector::~Vector(){
}

void Vector::polar_mode(){
	mode=POL;
}

void Vector::rect_mode(){
	mode=RECT;
}

Vector Vector::operator+(const Vector&b) const{
	return Vector(x+b.x,y+b.y);
}

Vector Vector::operator-(const Vector&b)const{
	return Vector(x-b.x,y-b.y);
}

Vector Vector::operator-()const{
	return Vector(-x,-y);
}

Vector Vector::operator*(double n) const{
	return Vector(x*n,y*n);
}

Vector operator*(double n,const Vector&a){
	return a*n;
}

double Vector::operator*(const Vector&b)const{
    return x*b.x+y*b.y;
}

std::ostream&operator<<(std::ostream&os,const Vector&v){
	if(v.mode==Vector::RECT)
		os<<"(x,y)=("<<v.x<<","<<v.y<<")";
	else if(v.mode==Vector::POL)
		os<<"(mag,ang)=("<<v.mag<<","<<v.ang*Rad_to_deg<<")";
	else
		os<<"Invaild!";
	return os;
}