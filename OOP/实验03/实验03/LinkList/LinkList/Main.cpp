// Main.cpp
#include "LinkList.h"
#include "Banking.h"
#include <cstring> 
#include <conio.h>
#include <string>
void test1(),test2(), test3(), AddressBookTest(), BankingTest();
class AddressBook
{
public:
	AddressBook(int Id=0, const char *Name="NoName", const char *Phone="", const char *Email="") : id(Id)
	{
		strncpy(name, Name, sizeof(name));		// 为了安全保障，指定长度拷贝字符串
		name[sizeof(name)-1] = '\0';			// 对可能超长的字符串进行截断
		strncpy(phone, Phone, sizeof(phone));
		phone[sizeof(phone)-1] = '\0';
		email = Email;	// 相当于 email = string(Email); 将C-字符串Email转换成string然后再赋值。本句可移至冒号语法处
	}
	friend ostream & operator<<(ostream &out, const AddressBook &a)
	{
		out << setfill('0') << setw(8) << a.id << '\t' << a.name << '\t' 
			<< setfill(' ') << setw(11) << a.phone << '\t' << a.email;
		return out;
	}
	friend istream & operator>>(istream &in, AddressBook &a)
	{
		char str[80];
		in.getline(str, 80, '\t');	a.id = atoi(str);
		in.getline(str, 80, '\t');	strncpy(a.name, str, sizeof(a.name));	a.name[sizeof(a.name)-1] = '\0';
		in.getline(str, 80, '\t');	strncpy(a.phone, str, sizeof(a.phone));	a.phone[sizeof(a.phone)-1] = '\0';
		in.getline(str, 80);		a.email = str;
		return in;
	}
	operator int()		const { return id; 			}	// 类型转换运算符，将本对象转换为 int（指编号）
	operator string()	const { return name;		}	// 类型转换运算符，将本对象转换为 C++字符串string（指姓名）
	operator long long()const { return atoll(phone);}	// 类型转换运算符，将本对象转换为 long long（指手机号码）
private:
	int id;
	char name[21], phone[12];
	string email;
};

template <typename T>
void Search(LinkList<AddressBook> &link, const T &t)
{
	int n=0;
	Node<AddressBook> *p;
	p = link.Locate(t, true);
	while(p!=NULL)
	{
		n++;
		cout << link.CurData() << endl;
		p = link.Locate(t);
	}
	cout << "找到 " << n << " 个满足条件的结点" << endl;
}

void Find(LinkList<AddressBook> &link, int flag)
{
	char str[80];
	switch(flag)
	{
	case '1':	cout << "请输入查询的编号: ";
				cin.getline(str, 80);
				Search(link, atoi(str));				break;
				
	case '2':	cout << "请输入查询的姓名: ";
				cin.getline(str, 80);
				Search(link, string(str));				break;
				
	case '3':	cout << "请输入查询的手机号码: ";
				cin.getline(str, 80);
				Search(link, atoll(str));				break;
	}
}

void AddressBookTest()
{
	int Pos(char ch, const char *str);						// 函数声明
	int Choice(const char *prompt, const char *options="");	// 函数声明
	LinkList<AddressBook> link;
	char filename[80] = "AddressBook.txt";
	AddressBook x;
	int key, k, n, yn;
	n = link.Load(filename);
	cout << "\n已从 " << filename << " 文件中读取了 " << n << " 个结点的数据" << endl;

	while(true)
	{
		cout << "\n\n========= 通讯录 =========" << endl;
		cout << "  1 ---- 输 出 通讯录 (P)" << endl;
		cout << "  2 ---- 插入新的结点 (I)" << endl;
		cout << "  3 ---- 追加新的结点 (A)" << endl;
		cout << "  4 ---- 查        询 (S)" << endl;
		cout << "  5 ---- 倒        置 (V)" << endl;
		cout << "  6 ---- 排        序 (O)" << endl;
		cout << "  7 ---- 数据写入文件 (W)" << endl;
		cout << "  8 ---- 读取数据文件 (R)" << endl;
		cout << "  9 ---- 释放所有结点 (F)" << endl;
		cout << "  0 ---- 退      出 (ESC)" << endl;
		cout << "==========================" << endl;
		key = Choice("请选择", "0123456789\x1bpPiIaAsSvVoOwWrRfF");
		cout << "\n\n";
		if(key==27 || key=='0')		// '\x1b'等于27，指ESC键
			break;
		switch(key)
		{
		case '1':
		case 'p':
		case 'P':	cout << link << endl;							break;
		case '2':
		case 'i':
		case 'I':	cout << "插入一个结点到链表首结点前。" << endl;
					cout << "请输入数据域数据(编号Tab姓名Tab手机号码TabEmail换行): ";
					cin >> x;
					link.Insert(x);									break;
		case '3':
		case 'a':
		case 'A':	cout << "追加一个结点(至尾结点后)。";
					cout << "请输入数据域数据(编号Tab姓名Tab手机号码TabEmail换行): ";
					cin >> x;
					link.Append(x);									break;
		case '4':
		case 's':
		case 'S':	cout << "根据数据域数据查询。可根据如下三种方式查询。" << endl;
					cout << "1 序号，2 姓名，3 手机号码。请选择: ";
					k = getche();
					Find(link, k);									break;
		case '5':
		case 'v':
		case 'V':	cout << "链表所有结点倒置。倒置之前\n" << link << endl;
					link.Reverse();
					cout << "倒置之前\n" << link << endl;			break;
		case '6':
		case 'o':
		case 'O':	cout << "排序根据 1按序号，2按姓名，3按手机号码。请选择: ";
					k = getche();
					switch(k)
					{
					case '1':	link.Sort(int(0));					// 第一个参数传递的数值不重要，重要的是数据类型
								cout << "\n按序号（升序）排序结果\n" << link << endl;
								link.Sort(int(0), false);
								cout << "按序号（降序）排序结果\n" << link << endl;	break;
					case '2':	link.Sort(string(""));
								cout << "\n按姓名（升序）排序结果\n" << link << endl;
								link.Sort(string(""), false);
								cout << "按姓名（降序）排序结果\n" << link << endl;	break;
					case '3':	link.Sort((long long)(0));
								cout << "\n按手机号码（升序）排序结果\n" << link << endl;
								link.Sort((long long)(0), false);
								cout << "按手机号码（降序）排序结果\n" << link << endl;	break;
					}
					break;
		case '7':
		case 'w':
		case 'W':	n = link.Save(filename);
					cout << "将 " << n << " 个结点的数据写入 "
						 << filename << " 文件。" << endl;			break;
		case '8':
		case 'r':
		case 'R':	cout << "读取数据文件。是否先释放已有的所有结点(y/[N])? ";
					yn = getche();
					if(yn=='y' || yn=='Y')
						link.FreeList();	
					n = link.Load(filename);
					cout << "从 " << filename << " 文件中读取 "
						 << n << " 个数据。现有 "
						 << link.NumNodes() << " 个结点" << endl;	break;
		case '9':
		case 'f':
		case 'F':	cout << "释放所有结点。是否继续(y/[N])? ";
					yn = getche();
					if(yn=='y' || yn=='Y')				// 默认 No
						link.FreeList();							break;
		}
	}
	if(link.NumNodes())
	{
		cout << "\n即将返回主函数，链表数据是否存盘([Y]/n)? ";
		yn = getche();
		if(yn!='n' && yn!='N')							// 默认 Yes
		{
			filename[Pos('.', filename)] = '\0';		// 去掉文件扩展名.txt
			strcat(filename, "(1).txt");				// 拼接成新的文件名
			n = link.Save(filename);
			cout << "\n已将 " << n << " 个结点的数据存入 " << filename << " 文件。" << endl;
		}
	}
	cout << "\n自动调用析构函数释放所有结点，返回主函数。\n" << endl;
}



int Pos(char ch, const char *str)			// 返回指定字符ch在字符串str中的下标。不存在时返回-1
{
	int i;
	for(i=0; str[i]!='\0'; i++)
		if(ch==str[i])
			return i;
	return -1;
}

int Choice(const char *prompt, const char *options="");	// 函数声明
int Choice(const char *prompt, const char *options)		// 函数定义。输出提示信息prompt，输入选择的字符并返回。
{
	int key;
	cout << prompt << "{";
	for(int i=0; options[i]!='\0'; i++)
	{
		if(' ' < (options[i] & 0x7f) && (options[i] & 0x7f) < 127)	// 选项options中的可打印字符
			cout << options[i] << ' ';
		else														// 选项options中的部分控制字符
		{
			switch(options[i])
			{
			case '\t':   cout << "[Tab] ";   break;					// 选项中的Tab字符'\t'（即ASCII编码9）
			case '\x1b': cout << "[Esc] ";   break;					// 选项中的Esc字符'\x1b'（即ASCII编码27）
			case ' ':    cout << "[Space] "; break;					// 选项中的空格字符' '（即ASCII编码32）
			}
		}
	}
	cout << "\b}: ";					// '\b'退格。先退一格，在输出右花括号
	do
	{
		key = getch();
	}while(options[0]!='\0' && Pos(key, options)<0); // 若options为空长度为0的字符串，则输入无限制；否则输入项应该在optins中。
	cout << endl;
	return key;
}


void BankingTest()
{
	LinkList<User> uLink;
	User xUser;
	Node<User> *pU;
	BWB xBWB;
	char id[80], name[80], str[80];
	int choice, year, month, day;
	double money;

	while(true)
	{
		cout << "\n请输入账号(n账号--开户，直接回车则退出)：";
		cin.getline(id, 80);
		if(id[0] == '\0')
			break;
		if(id[0]=='N' || id[0]=='n')
		{
			cout << "请输入户名：";
			cin.getline(name, 80);
			xUser.Set(id+1, name);
			uLink.Append(xUser);
		}
		else
			xUser.Set(id, "");
		pU = uLink.Locate(string(xUser), true);
		if(pU==NULL)
		{
			cout << "账号输入错误！" << endl;
			continue;
		}

		while(true)
		{
			cout << endl;
			uLink.CurData().Show(cout);
			cout << "1 -- 存入（正数）/支取（负数）" << endl;
			cout << "2 -- 查询" << endl;
			cout << "0 -- 返回" << endl;
			cout << "请选择：";
			choice = getche() - '0';
			cout << endl;
			if(choice<=0) break;
			switch(choice)
			{
			case 1:
					cout << "请输入（年 月 日 金额）: ";
					cin >> year >> month >> day >> money;
					cin.getline(str, 80);					// 处理换行字符
					xBWB.Set(year, month, day, money);
					uLink.CurData().AppendBWB(xBWB);
					break;
			case 2:	cout << uLink.CurData() << endl; break;
			}
		}
	}
	cout << "\n自动调用析构函数释放所有结点，返回主函数。\n" << endl;
}

int Pause(const char* prompt = "按任意键继续……")
{
	cout << prompt;
	return getch();
}

void test1()				// 测试类模板LinkList的 构造函数、输出运算符（包括文件I/O）、析构函数，链表的链表测试等
{
	int n, iArray[] = {900, 800, 500};					// 准备好int型数组
	int iN = sizeof(iArray)/sizeof(*iArray);
	
	LinkList<int> iLink0, iLink1(2), iLink2(iN, iArray), iLArray[] = {iLink0, iLink1, iLink2};
			// 创建3条链表（结点数分别为0, 2, 3），以及一个链表数组（元素个数为3，即三条链表组成的数组）
	int nLL = sizeof(iLArray)/sizeof(*iLArray);
	LinkList<LinkList<int> > iLLink(nLL, iLArray);
			// 创建链表的链表，即以链表作为数据域数据

	cout << "\n\n(1.1) 输出链表（第一种格式）" << endl;
	cout << "\tcout << iLink0 << endl;" << endl;	cout << iLink0 << endl;
	cout << "\tcout << iLink1 << endl;" << endl;	cout << iLink0 << endl;
	cout << "\tcout << iLink2 << endl;" << endl;	cout << iLink0 << endl;
	Pause();
	
	cout << "\n\n(1.2) 输出链表数组元素" << endl;
	for(int i=0; i<nLL; i++)
		cout << iLArray[i] << endl;
	Pause();

	cout << "\n\n(1.3) 输出链表（另一种格式）" << endl;
	cout << "\tiLink0.ShowList();" << endl;			iLink0.ShowList();
	cout << "\tiLink1.ShowList();" << endl;			iLink1.ShowList();
	cout << "\tiLink2.ShowList();" << endl;			iLink2.ShowList();
	Pause();

	cout << "\n\n(1.4) 输出链表的链表（两种格式）" << endl;
	cout << "\tcout << iLLink << endl;" << endl;	cout << iLLink << endl;
	cout << "\tiLLink.ShowList();" << endl;			iLLink.ShowList();
	Pause();

	cout << "\n\n(1.5) 链表的链表存盘" << endl;
	cout << "\tn = iLLink.Save(\"LLink.txt\");" << endl;
	n = iLLink.Save("LLink.txt");
	cout << "\t" << n << " 个结点数据写入文件 \"LLink.txt\" 中" << endl;
	Pause();

	cout << "\n\n(1.6) 从文件中读取数据时构造链表。先释放链表中的所有结点" << endl;
	cout << "\tiLLink.FreeList(); iLLink.ShowList();" << endl;
	iLLink.FreeList();
	iLLink.ShowList();
	cout << "\tn = iLLink.Load(\"LLink.txt\");" << endl;
	n = iLLink.Load("LLink.txt");
	cout << "\t读取 " << n << " 个结点数据，构造链表。读取结果如下" << endl;
	cout << iLLink << endl;
	Pause();

	double dArray[] = {1.414, 2.718, 3.142};					// 准备好double型数组
	int dN = sizeof(dArray)/sizeof(*dArray);
	LinkList<double> dLink0, dLink1(2), dLink2(dN, dArray);		// 创建3条链表（结点数分别为0, 2, 3）

	cout << "\n\n(1.7) 数据域为double型数据测试" << endl;
	dLink0.ShowList();	cout << dLink0 << endl;
	dLink1.ShowList();	cout << dLink1 << endl;
	dLink2.ShowList();	cout << dLink2 << endl;
	Pause();

	n = dLink2.Save("dLink.txt");
	cout << "\n\n(1.8) 将链表数据（共 " << n << " 个结点数据写入文件 \"dLink.txt\" 中" << endl;
}

void test2()	// 测试插入，查找/继续查找，修改（替换）数据，倒置、排序等成员函数
{
	int iArray[] = {1, 2, 3, 3, 5, 6, 7, 8, 8, 3};
	int n = sizeof(iArray)/sizeof(*iArray);
	LinkList<int> iLink(n, iArray);
	
	cout << "\n原始链表" << endl;
	iLink.ShowList();
	cout << "\n\n(2.1) 将 0 插入到链首结点前，成为新的链首结点" << endl;
	iLink.Insert(0);
	iLink.ShowList();
	Pause();

	cout << "\n\n(2.2) 将 100 插入到链尾结点后，成为新的链尾结点" << endl;
	iLink.Append(100);
	iLink.ShowList();
	Pause();

	cout << "\n\n(2.3) 将当前结点移至第3个结点（从0开始）";
	iLink.Go(3);
	iLink.ShowList();
	Pause();

	cout << "\n\n(2.4) 在当前结点前插入 30" << endl;
	iLink.InsertBefore(30);
	iLink.ShowList();
	Pause();

	cout << "\n\n(2.5) 在当前结点后插入 40" << endl;
	iLink.InsertAfter(40);
	iLink.ShowList();
	Pause();

	cout << "\n\n(2.6) 将链表各结点倒置" << endl;
	iLink.Reverse();
	iLink.ShowList();
	Pause();

	cout << "\n\n(2.7) 链表排序（升序）" << endl;
	iLink.Sort(int(0));
	iLink.ShowList();
	Pause();

	cout << "\n\n(2.8) 链表排序（降序）" << endl;
	iLink.Sort(int(0), false);
	iLink.ShowList();
	Pause();

	cout << "\n\n(2.9) 测试异常。链表中当前结点绝对定位成员函数Go(n)，序号n可能超出结点数" << endl;
	for (int i = iLink.NumNodes() + 1; i > -2; i--)
	{
		cout << "Go(" << i << "): ";
		iLink.Go(i);		// 可能使cur_node为NULL
		try
		{
			cout << iLink.CurData() << endl;
		}
		catch (int)
		{
			cout << "\t捕捉到异常：超出结点数范围" << endl;
		}
	}
	Pause();

	int x;
	cout << "\n\n(2.10) 在链表中查找并修改（替换）数据，请输入要查询的数据: ";
	cin >> x;
	Node<int> *p;
	p = iLink.Locate(x, true);
	n = 0;
	while(p!=NULL)
	{
		cout << "找到第 " << iLink.CurPos() << " 结点(编号从0开始)：\t" << iLink.CurData() << endl;
		cout << "修改为: ";
		cin >> iLink.CurData();				// 函数调用表达式 iLink.CurData() 做左值，上面曾做右值
		n++;
		p = iLink.Locate(x);
	}
	cout << "\t共找到 " << n << " 个结点满足条件" << (n>0? "，并进行了修改。":"。") << endl;
	cout << "\n将当前结点设置为首结点" << endl;
	iLink.GoTop();
	iLink.ShowList();
}

void test3()								// 测试当前结点绝对定位、相对定位，删除当前结点函数
{
	int n, interval, k = 0, m;
	cout << "\n约瑟夫(Josephus)问题" << endl;
	cout << "有n个小孩(编号1~n)围成一圈，从第1个小孩开始按顺时针方向从1~m循环报数，报到m的小孩出圈。"
		<< "最后剩下的小孩为胜利者。" << endl;
	cout << "请输入小孩人数 间隔数(如 10 3 ): ";
	cin >> n >> interval;

	if (interval <= 1)
	{
		cout << "间隔数应该大于1，现调整为2。" << endl;
		interval = 2;
	}
	LinkList<int> link;
	for (int i = 1; i <= n; i++)
		link.Append(i);
	link.GoTop();
	cout << "初始态\t";
	link.ShowList();
	while (link.NumNodes() > 1)
	{
		if (++k == interval)
		{
			m = link.DeleteCurNode();
			cout << m << " 出圈\t";
			if (link.CurNode() == NULL)
				link.GoTop();
			k = 1;
			Pause("");
		}
		else
			cout << "\t";
		link.Skip();
		if (link.CurNode() == NULL)
			link.GoTop();
		link.ShowList();
	}
	cout << "胜利者: " << link.CurData() << " 号。" << endl;
}

int main()
{
	int key;
	while(true)
	{
		cout << "\n\n\t单向链表类模板测试" << endl;
		cout << "1 --- 链表对象（包括链表的链表）构造、析构、标准I/O、文件I/O" << endl;
		cout << "2 --- 插入结点、查找/继续查找、倒置、排序、异常处理、修改(替换)数据" << endl;
		cout << "3 --- 约瑟夫(Josephus)问题(测试当前结点定位方法，环绕处理)" << endl;
		cout << "4 --- 通讯录类型测试" << endl;
		cout << "5 --- 模拟存储款业务" << endl;
		key = Choice("\n请选择", "12345\x1b");
		cout << "\n\n";
		if(key==27)		// '\x1b'等于27，指ESC键
			break;
		switch(key)
		{
		case '1':	test1();			break;
		case '2':	test2();			break;
		case '3':	test3();			break;
		case '4':	AddressBookTest();	break;
		case '5':	BankingTest();		break;
		}
	}
	return 0;
}