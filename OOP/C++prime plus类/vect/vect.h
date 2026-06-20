// vect.h
#ifndef MY_VECT_H
#define MY_VECT_H
#include<iostream>

	class Vector{
		public:
			enum Mode{RECT, POL};//RECT代表直角坐标系，POL代表极坐标系
		private:
			double x;
			double y;
			double mag;//向量模
			double ang;//极坐标角
			Mode mode;
			void set_mag();
			void set_ang();
			void set_x();
			void set_y();
		public:
			Vector();
			Vector(double n1,double n2,Mode form=RECT);
			void reset(double n1,double n2,Mode form=RECT);
			~Vector();
			//输出参数
			double xval()const{return x;}
			double yval()const{return y;}
			double magval()const{return mag;}
			double angval()const{return ang;}
			void polar_mode();
			void rect_mode();
			//运算符重载
			Vector operator+(const Vector&b)const;
			Vector operator-(const Vector&b)const;
			Vector operator-()const;
			Vector operator*(double n)const;
			double operator*(const Vector&b)const;
			friend Vector operator*(double n,const Vector&a);
			friend std::ostream&operator<<(std::ostream&os,const Vector&V);
	};


#endif
