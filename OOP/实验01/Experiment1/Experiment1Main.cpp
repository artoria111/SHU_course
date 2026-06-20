// Experiment1Main.cpp
#include <iostream>
#include <iomanip>
#include "Experiment1.h"
using namespace std;

int main()
{
	double a[][3]={{1,2,-3},{1,2,1},{-1,-2,-3}};
	int n=sizeof(a)/sizeof(*a);
	test(a,n);
	
	cout<<endl;
	
	double data[10][3];
	int scale=randdata(data,10);
	test(data,scale);
	return 0;
}
