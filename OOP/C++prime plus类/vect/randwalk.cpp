// randwalk.cpp
//不断将随机方向固定长度的向量相加，直到达到目标长度
#include <iostream>
#include <iomanip>
#include<cstdlib>
#include<ctime>
#include "vect.h"
using namespace std;

//在初始化或者重设置向量的时候，使用角度制单位输入，在程序内部会被隐式转换为弧度制。
int main()
{
	srand(time(0));
	double direction;//前进的方向
	Vector step;//前进的向量
	Vector result(0.0,0.0);//当前的位置
	unsigned long step_sum=0;//总步数
	double target;//目标位置的距离
	double dstep;//每步的长度
	cout<<"输入目标位置距离原点的距离(输入q以退出): ";
	while(cin>>target){
		cout<<"输入每步的长度: ";
		if(!(cin>>dstep))
			break;
		
		while(result.magval()<target){
			direction=rand()%360;
			step.reset(dstep,direction,Vector::POL);
			result=result+step;
			step_sum++;
		}
		
		cout<<"在"<<step_sum<<"步之后，目标在位置："<<result<<endl;
		step_sum=0;
		result.reset(0.0,0.0);
		cout<<"输入目标位置距离原点的距离(输入q以退出): ";
	}
	return 0;
}
