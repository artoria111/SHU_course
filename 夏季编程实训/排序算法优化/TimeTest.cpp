#include <ctime>

double gettime(int restart) {				// 参数带默认值，非零表示重新计时
	// 否则累计计时
	const double c = 1.0 / CLOCKS_PER_SEC;
	static clock_t t = clock();				// 静态局部变量。第一次调用时，确定计时起点
	if (restart)
		t = clock();				// 根据实参决定是否重新确定计时起点
	return c * (clock() - t);					// 从上一计时点到现在所经历的时间
}
