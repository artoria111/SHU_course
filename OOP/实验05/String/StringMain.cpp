#include <iostream>
#include "String.h"

using namespace std;

int main() {
    int choice;
    do {
        cout << "字符串类测试菜单：" << endl;
        cout << "1. 测试构造函数" << endl;
        cout << "2. 测试赋值运算符" << endl;
        cout << "3. 测试下标运算符" << endl;
        cout << "4. 测试比较运算符" << endl;
        cout << "5. 测试输入输出运算符" << endl;
        cout << "6. 测试长度函数" << endl;
        cout << "7. 退出" << endl;
        cout << "请输入您的选择：";
        cin >> choice;
        cin.ignore(); // 忽略换行符
        switch (choice) {
            case 1:
                cout << "测试构造函数：" << endl;
                {
                    cout << "创建一个字符串对象 s1，初始化为 \"HELLO\"" << endl;
                    String s1("HELLO");
                    cout << "s1: " << s1 << ", 长度: " << s1.length() << endl;
                    cout << endl;
                    cout << "创建一个空字符串对象 s2" << endl;
                    String s2;
                    cout << "s2: " << s2 << ", 长度: " << s2.length() << endl;
                    cout << endl;
                    cout << "创建一个字符串对象 s3 :" << endl;
                    String s3;
                    cin >> s3;
                    cout << "s3: " << s3 << ", 长度: " << s3.length() << endl;
                    cout << endl;
                }
                break;
            case 2:
                cout << "测试赋值运算符：" << endl;
                {
                    cout << "创建一个字符串对象 s1，初始化为 \"WORLD\"" << endl;
                    String s1("WORLD");
                    cout << "创建一个空字符串对象 s2 并赋值其为 s1" << endl;
                    String s2 = s1;
                    cout << "s2 赋值后: " << s2 << endl;
                    cout << endl; 
                    cout << "创建一个字符串对象 s3 :";
                    String s3;
                    cin >> s3;
                    cout << "创建一个空字符串对象 s4 并赋值其为 s3" << endl;
                    String s4 = s3;
                    cout << "s4 赋值后: " << s3 << endl;
                    cout << endl; 
                }
                break;
            case 3:
                cout << "测试下标运算符：" << endl;
                {
                    cout << "创建一个字符串对象 s1，初始化为 \"HELLO WORLD\"" << endl;
                    String s1("HELLO WORLD");
                    try {
                        cout << "访问 s1 的第 1 个字符：" << s1[0] << endl;
                        cout << "访问 s1 的第 7 个字符：" << s1[6] << endl;
                        cout << "将 s1 的第 5 个字符改为小写 'y'" << endl;
                        s1[4] = 'y';
                        cout << "s1 修改后: " << s1 << endl;
                        cout << endl;
                    } catch (const exception& e) {
                        cout << "异常: " << e.what() << endl;
                    }
                    cout << "创建一个字符串对象 s2，初始化为  \"HI\"" << endl;
                    String s2("HI");
                    try {
                        cout << "访问 s2 的第 1 个字符：" << s2[0] << endl;
                        cout << "访问 s2 的第 7 个字符：" << s2[6] << endl;
                        cout << "将 s2 的第 5 个字符改为小写 'y'" << endl;
                        s2[4] = 'y';
                        cout << "s2 修改后: " << s2 << endl;
                        cout << endl;
                    } catch (const exception& e) {
                        cout << "异常: " << e.what() << endl;
                    }
                    cout << "创建一个字符串对象 s3 :";
                    String s3;
                    cin >> s3;
                    try {
                    	int n=0,m=0,k=0;
                    	char s;
                        cout << "访问 s3 的第 n 个字符(请输入n)：";
						cin >> n;
						cout << "第 " << n << " 个字符为："<< s3[n-1] << endl;
                        cout << "访问 s3 的第 m 个字符(请输入m)：" ;
						cin >> m;
						cout << "第 " << m << " 个字符为："<< s3[m-1] << endl;
                        cout << "将 s3 的第 k 个字符修改(请输入k)：";
                        cin >> k;
                        cout << "将该字符修改为：";
                        cin >> s;
                        s3[k-1] = s;
                        cout << "s3 修改后: " << s3 << endl;
                        cout << endl;
                    } catch (const exception& e) {
                        cout << "异常: " << e.what() << endl;
                    }
                }
                break;
            case 4:
                cout << "测试比较运算符：" << endl;
                {
                    cout << "创建两个字符串对象 s1 和 s2，分别初始化为 \"HELLO WORLD\" 和 \"AI\"" << endl;
                    String s1("HELLO WORLD");
                    String s2("AI");
                    cout << "比较 s1 和 s2：" << endl;
                    cout << "s1 < s2: " << (s1 < s2 ? "真" : "假") << endl;
                    cout << "s1 == s2: " << (s1 == s2 ? "真" : "假") << endl;
                    cout << "s1 > s2: " << (s1 > s2 ? "真" : "假") << endl;
                    cout << endl;
                    cout << "创建两个字符串对象 s3 和 s4，分别初始化为 \"HELLO WORLD\" 和 \"HELLO WORLD\"" << endl;
                    String s3("HELLO WORLD");
                    String s4("HELLO WORLD");
                    cout << "比较 s3 和 s4：" << endl;
                    cout << "s3 < s4: " << (s3 < s4 ? "真" : "假") << endl;
                    cout << "s3 == s4: " << (s3 == s4 ? "真" : "假") << endl;
                    cout << "s3 > s4: " << (s3 > s4 ? "真" : "假") << endl;
                    cout << endl;
                    cout << "创建字符串对象 s5 :";
                    String s5;
                    cin >> s5;
                    cout << "创建字符串对象 s6 :";
                    String s6;
                    cin >> s6;
                    cout << "比较 s5 和 s6：" << endl;
                    cout << "s5 < s6: " << (s5 < s6 ? "真" : "假") << endl;
                    cout << "s5 == s6: " << (s5 == s6 ? "真" : "假") << endl;
                    cout << "s5 > s6: " << (s5 > s6 ? "真" : "假") << endl;
                    cout << endl;
                }
                break;
            case 5:
                cout << "测试输入输出运算符：" << endl;
                {
                    cout << "请输入一个字符串：";
                    String s;
                    cin >> s; // 使用重载的 >> 运算符
                    cout << "您输入的是: " << s << endl;
                    cout << endl;
                }
                break;
            case 6:
                cout << "测试长度函数：" << endl;
                {
                    cout << "创建一个字符串对象 s1，初始化为 \"HELLO WORLD\"" << endl;
                    String s1("HELLO WORLD");
                    cout << "s1 的长度: " << s1.length() << endl;
                    cout << endl;
                    cout << "创建一个字符串对象 s2 :";
                    String s2;
                    cin >> s2;
                    cout << "s2 的长度: " << s2.length() << endl;
                    cout << endl;
                }
                break;
            case 7:
                cout << "正在退出..." << endl;
                break;
            default:
                cout << "无效选择，请重新输入。" << endl;
        }
    } while (choice != 7);

    return 0;
}

