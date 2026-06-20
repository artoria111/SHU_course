#include <stdio.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
using namespace std;

struct Score {
	char id[9];
	double Chinese, Math, English, Total;
	bool operator< (const Score &s) const {
		return Total <  s.Total;    // 重载6个关系运算符函数
	}
	bool operator<=(const Score &s) const {
		return Total <= s.Total;    // 使得两个结构体变量之间可以直接进行比较
	}
	bool operator> (const Score &s) const {
		return Total >  s.Total;    // 从这6个函数体可见，实际上是比较总分Total
	}
	bool operator>=(const Score &s) const {
		return Total >= s.Total;
	}
	bool operator==(const Score &s) const {
		return Total == s.Total;
	}
	bool operator!=(const Score &s) const {
		return Total != s.Total;
	}
};

double Uniform(double a, double b) {		// 区间[a, b]上的均匀分布
	return a + rand() * (b - a) / RAND_MAX;			// 经过两点(0,a)和(RAND_MAX,b)的直线方程
}

double Gaussian(double mean, double variance) {	// mena:均值、数学期望，variance方差
	static double u, v;
	static int phase = 0;
	double z;

	if (phase == 0) {
		u = (rand() + 1.0) / (RAND_MAX + 1.0);	// 避免u或v为0时参与对数log计算
		v = (rand() + 1.0) / (RAND_MAX + 1.0);
		z = sqrt(-2.0 * log(u)) * sin(2.0 * M_PI * v);
	} else {
		z = sqrt(-2.0 * log(u)) * cos(2.0 * M_PI * v);
	}
	phase = 1 - phase;
	return mean + z * sqrt(variance);
}


//按正态分布生成成绩数据
void ScoreCreate(Score *data, int n) {
	double a, b;
	int i;
	a = 72, b = 9; //控制均值和方差
	for (i = 0; i < n; i++) {
		sprintf(data[i].id, "%08d", i + 1);	// 请自学 sprintf 函数。请注意：原始数据中，学号是完全顺序排列的！
		data[i].Total  = data[i].Chinese   = int(0.5 + Gaussian(a, b));	// 加0.5后取整即小数部分"四舍五入"
		data[i].Total += data[i].Math      = int(0.5 + Gaussian(a, b));
		data[i].Total += data[i].English   = int(0.5 + Gaussian(a, b));
	}
}

void ShowScore(const Score *data, int n) {
	cout << "语文 数学 英语 总分" << endl;
	for (int i = 0; i < n; i++) {
		cout << data[i].Chinese << " " << data[i].Math << " " << data[i].English << " "
		     << data[i].Total << endl;
	}
}

template <typename T> void CreateData(T *data, int scale, int flag) {
	int a, b, i;
	switch (flag) {
		case 1: {
			for (i = 0; i < scale; i++)
				data[i] = i;         //正序从小到大
			break;
		}
		case 2: {
			for (i = 0; i < scale; i++)
				data[i] = scale - i;    //逆序从大到小
			break;
		}
		case 3: {
			a = 9, b = 90; //控制均匀分布的下限和上限
			for (i = 0; i < scale; i++)
				data[i] = (T)Uniform(a, b);
			break;
		}
		case 4: {
			a = 68, b = 12; //控制正态分布的均值和方差
			for (i = 0; i < scale; i++)
				data[i] = (T)Gaussian(a, b);
			break;
		}
	}

}

string rand_str(const int len) {
	string str;
	char c;
	int idx;
	for (idx = 0; idx < len; idx ++) {
		c = 'a' + rand() % 26;
		str.push_back(c);
	}
	return str;

}


void StringCreate(string *data, int scale) {
	for (int i = 0; i < scale; i++) {
		data[i] = rand_str(10);
	}
}//生成随机字符串

void ShowString(string *data, int scale) {
	for (int i = 0; i < scale; i++) {
		cout << data[i] << endl;
	}

}

template <typename T> void ShowData(T *data, int n, int flag) {

	for (int i = 0; i < n; i++) {
		cout << data[i] << " ";
		if ((i + 1) % 7 == 0)
			cout << endl;
	}
}
