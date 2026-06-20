// StockMain.cpp
#include <iostream>
#include <iomanip>
#include "Stock.h"
using namespace std;

int main()
{
	Stock soyo("SoyoTokyo",20,12.50);
	//Stock soyo=Stock("AnonTokyo",20,12.50);
	//Stock *soyo=new Stock("AnonTokyo",20,12.50);
	soyo.show();
	soyo.buy(15,18.125);
	soyo.show();
	soyo.sell(400,20.00);
	soyo.show();
	soyo.buy(300000,40.125);
	soyo.show();
	soyo.sell(300000,0.125);
	soyo.show();
	
	Stock anon("AnonTokyo",1,2);
	anon.show();
	
	Stock top;
	top=soyo.topval(anon);
	top.show();//≤‚ ‘topval
	
	if(anon<soyo){//≤‚ ‘÷ÿ‘ÿ‘ÀÀ„∑˚<
		cout<<"soyo,win!"<<endl;
	}else{
		cout<<"anon,win!"<<endl;
	}
		
	return 0;
}
