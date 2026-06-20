// Experiment1.cpp
#include "Experiment1.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;

double detaCau(double a, double b, double c) {
	double m;
	m = b * b - 4 * a * c;
	return m;
}

bool solve(double a, double b, double c, double &x1, double &x2) {
	double deta = detaCau(a, b, c);
	if (deta >= 0) {
		x1 = (-b + sqrt(deta)) / (2 * a);//求根公式
		x2 = (-b - sqrt(deta)) / (2 * a);
		return 1;
	}
	return 0;
}

void output(int flag, double x1, double x2) {
	if (flag == 1) {
		if (x1 != x2) 
		{
			cout << "方程的两个根分别为" << x1 << "和" << x2 << endl;
		} 
		else 
		{
			cout << "方程的唯一根为" << x1 << endl;//重根的情况
		}
	} 
	else 
	{
		cout << "方程无解" << endl;
	}
}

void test(double a[][3],int n){
	for(int i=0;i<n;i++)
	{
		double x1,x2;
		int flag=solve(a[i][0],a[i][1],a[i][2],x1,x2);
    	output(flag,x1,x2);
    }
	
}

int randdata(double data[][3],int scale){
	srand(static_cast<unsigned>(time(nullptr)));
	for(int i=0;i<scale;i++)
	{
	   for(int j=0;j<3;j++)
	   {
	      data[i][j]=rand()%9+1;//确保生成的10以内随机数不为0
	   }	
	}
	return scale;
}



