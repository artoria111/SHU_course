// TimeMain.cpp
#include <iostream>
#include <iomanip>
#include "Time.h"
using namespace std;

int main()
{
	Time time_plan(2,40);
	Time time_fix(5,55);
	time_fix.Reset(6,55);//改变对象中的参数只能用函数
//	Time time_fix(4,55);不合法，因为已经有一个time_fix了
	Time total_time;
	cout<<"time_plan: ";
	time_plan.Show();
	cout<<time_plan;//对<<进行重载，用于输出
	cout<<"time_fix: ";
	time_fix.Show();
	
	total_time=time_plan*10-time_fix*3+time_plan+3*time_plan;
	cout<<"total_time: ";
	total_time.Show();
	return 0;
}
