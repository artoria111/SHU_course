// Stock.h
#ifndef MY_STOCK_H
#define MY_STOCK_H

#include <string>
#include<iostream>
class Stock{
private:
	std::string company;//公司名称
	long shares;//股票数量
	double share_val;//每股的价格
	double total_val;//股票的总价格
	void set_tot(){total_val=shares*share_val;}
public:
	Stock();
	Stock(const std::string&co,long n=0,double pr=0.0);
	~Stock(){std::cout<<"Bye,"<<company<<"!\n";};
	const Stock&topval(const Stock&s)const;
	bool operator<(const Stock&s)const;//尝试对<进行重载
	void buy(long num,double price);
	void sell(long num, double price);
	void update(double price);
	void show();
};

#endif
