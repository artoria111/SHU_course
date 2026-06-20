//vectmain.cpp
#include"vect.h"
using namespace std;
int main()
{
	cout << "======向量类模板示例======"<<endl;
	cout << "请输入您要计算的向量1的维数：";
	int s1;
	while (true) {
	    cin >> s1;
	    if (s1 < 0) {
	        cout << "向量维度不能为负数，请重新输入：";
	    } else {
	        break;
	    }
	}
	cout << "请输入您要计算的向量1: ";
	Vector<int> v1(s1);
	cin >> v1;
	
	cout << "请输入您要计算的向量2的维数：";
	int s2;
	while (true) {
	    cin >> s2;
	    if (s2 < 0) {
	        cout << "向量维度不能为负数，请重新输入：";
	    } else {
	        break;
	    }
	}
	cout << "请输入您要计算的向量2: ";
	Vector<int> v2(s2);
	cin >> v2;
	
	cout << "======向量类模板示例======"<<endl;
	cout << "1  向量相加" << endl;
	cout << "2  向量相减" << endl;
	cout << "3  向量相乘" << endl;
	cout << "4  向量扩大n倍" << endl;
	cout << "5  向量维数缩放" << endl;
	cout << "6  反转向量" << endl;
	cout << "7  尾端插入数据" << endl;
	cout << "8  删除指定位置向量元素" << endl;
	cout << "9  在指定位置插入向量元素" << endl;
	cout << "10 修改指定位置向量元素值" << endl;
	cout << "11 查询向量" << endl;
	cout << "12 查询向量维数" << endl;	
	cout << "13 重新输入向量" << endl;
	cout << "0  退出" << endl;
	cout << "请输入您要执行的操作：";
	int a;
	cin >> a;
	
	while (true)
	{
		switch (a)
		{
		case 1:
		{
			cout << "v1 : " << v1 << endl;
			cout << "v2 : " << v2 << endl;
			cout << "v1 + v2 : " << v1 + v2 << endl;
			break;
		}
		case 2:
		{
			cout << "v1 : " << v1 << endl;
			cout << "v2 : " << v2 << endl;
			cout << "v1 - v2 : " << v1 - v2 << endl;
			break;
		}
		case 3:
		{
		        if (v1.getdim() != v2.getdim())
                {
                    cout << "维度不匹配，无法相乘" << endl;
                    break; // 返回主菜单
                }
                else
                {
                    cout << "v1 : " << v1 << endl;
                    cout << "v2 : " << v2 << endl;
                    cout << "v1 * v2 : " << v1 * v2 << endl;
                    break;
                }
		}
		case 4:
		{
			cout << "请输入您想要操作的向量(1或2)：";
			int i;
			cin >> i;
			cout << "请输入您想要扩大的倍数：";
			int n;
			cin >> n;
			if (i == 1)
			{
				cout << "扩大前 : " << v1 << endl;
				v1 = v1 * n;
				cout << "扩大后 : " << v1 << endl;
			}
			else
			{
				cout << "扩大前 : " << v2 << endl;
				v2 = v2 * n;
				cout << "扩大后 : " << v2 << endl;
			}
			break;
		}
		case 5:
		{
			cout << "请输入您想要缩放的向量(1或2)：" << endl;
			int s;
			cin >> s;
			cout << "请输入您想要缩放到的维数：" << endl;
			int i;
			cin >> i;
			if (s == 1)
			{
				cout << "缩放前 : " << v1 << endl;
				v1.redim(i);
				cout << "缩放后 : " << v1 << endl;
			}
			else
			{
				cout << "缩放前 : " << v2 << endl;
				v2.redim(i);
				cout << "缩放后 : " << v2 << endl;
			}
			break;
		}
		case 6:
		{
			cout << "请输入您想要反转的向量(1或2)：" << endl;
			int s;
			cin >> s;
			if (s == 1)
			{
				cout << "反转前 : " << v1 << endl;
				cout << "反转后 : " << -v1 << endl;
			}
			else
			{
				cout << "反转前 : " << v2 << endl;
				cout << "反转后 : " << -v2 << endl;
			}
			break;
		}
		case 7:
		{
			cout << "请输入您想要操作的向量(1或2)：";
			int i, s;
			cin >> i;
			cout << "请输入您想要添加的元素：";
			cin >> s;
			if (i == 1)
			{
				cout << "添加前 : " << v1<<endl;
				v1.push_back(s);
				cout << "添加前 : " << v1<<endl;
			}
			else
			{
				cout << "添加前 : " << v2<<endl;
				v2.push_back(s);
				cout << "添加后 : " << v2<<endl;
			}
			break;
		}
		case 8:
		{
			cout << "请输入您想要操作的向量(1或2)：";
			int i, s;
			cin >> i;
			cout << "请输入您要删除第几个元素：";
			cin >> s;
			if (i == 1) 
			{
				cout << "删除前 : " << v1 << endl;
				v1.erase(s);
				cout << "删除后 : " << v1 << endl;
			}
			else
			{
				cout << "删除前 : " << v2 << endl;
				v2.erase(s);
				cout << "删除后 : " << v2 << endl;
			}
			break;
		}
		case 9:
		{
			cout << "请输入您想要操作的向量(1或2)：";
			int i, s;
			cin >> i;
			cout << "请输入您要增加元素的位置：";
			cin >> s;
			if (i == 1)
			{
				cout << "增加前 : " << v1 << endl;
				v1.add(s);
				cout << "增加后 : " << v1 << endl;
			}
			else
			{
				cout << "增加前 : " << v2 << endl;
				v2.add(s);
				cout << "增加后 : " << v2 << endl;
			}
			break;
		}
		case 10:
		{
			cout << "请输入您想要操作的向量(1或2)：";
			int i, s;
			cin >> i;
			cout << "请输入您要修改元素的位置和值：";
			cin >> s;
			if (i == 1)
			{
				cout << "增加前 : " << v1 << endl;
				v1.reset(s);
				cout << "增加后 : " << v1 << endl;
			}
			else
			{
				cout << "增加前 : " << v2 << endl;
				v2.reset(s);
				cout << "增加后 : " << v2 << endl;
			}
			break;
		}
		
		case 11:
		{
			cout << "向量1：" << v1 << endl;
			cout << "向量2：" << v2 << endl;
			break;

		}		
		case 12:
		{
			cout << "向量1的维数为：" << v1.getdim() << endl;
			cout << "向量2的维数为：" << v2.getdim() << endl;
			break;

		}
		case 13:
		{
			cout << "请重新输入您要计算的向量1的维数：";
			int s1;
			cin >> s1;
			cout << "请重新输入您要计算的向量1:" << endl;
			Vector<int> v3(s1);
			cin >> v3;
			cout << "请重新输入您要计算的向量2的维数：";
			int s2;
			cin >> s2;
			cout << "请重新输入您要计算的向量2:" << endl;
			Vector<int> v4(s2);
			cin >> v4;
			v1 = v3;
			v2 = v4;
			
			cout << "向量1：" << v1 << endl;
			cout << "向量2：" << v2 << endl;
			break;
		}
		case 0:
			return 0;
		}
		cout << "请输入您要执行的操作：";
		cin >> a;
	}
	return 0;
}

