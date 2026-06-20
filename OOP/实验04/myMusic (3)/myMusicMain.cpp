// myMusicMain.cpp
#include <iostream>
#include <iomanip>
#include <conio.h>
#include "myMusic.h"

using namespace std;

int main() {
	LinkList<User> LUser;
	while (1) {
		cout << "欢迎使用歌单管理系统" << endl;
		cout << "1.创建/删除用户" << endl;
		cout << "2.添加/删除歌曲" << endl;
		cout << "3.搜索歌曲或歌手" << endl;
		cout << "4.展示歌单" << endl;
		cout << "5.倒置歌单" << endl;
		cout << "6.从本地读取账户与歌单" << endl;
		cout << "7.将数据保存至本地" << endl;
		cout << "q.退出系统" << endl;
		cout << "  请输入[1 2 3 4 5 6 7 q]->";
		char ch = getche();

		if (ch == 'q')
			break;

		switch (ch) {
			case '1':
				Create_Delete(LUser);
				break;
			case '2':
				Add_Delete(LUser);
				break;
			case '3':
				Research(LUser);
				break;
			case '4':
				Show(LUser);
				break;
			case '5':
				Reverse(LUser);
				break;
			case '6':
				Download(LUser);
				break;
			case '7':
				Upload(LUser);
				break;
			default:
				cout << endl << "ERROR!";
		}


		cout << endl << endl;
	}

	cout << endl << "Bye!" << endl;

	return 0;
}

/////////////////////////////////////////////////////////////

void Add_Delete(LinkList<User> &LUser) {
	cout << endl;
	cout << "请输入用户的名字与ID(用空格分开)：";
	string user_name, user_id;
	cin >> user_name >> user_id;
	User user_x(user_id, user_name);

 	Node<User>* found = LUser.Locate(user_x, true);
    if (found == NULL) {
        cout << "未找到该用户。" << endl;
        return;
    }


	while (1) {
		cout << endl << endl << "1.添加歌曲" << endl;
		cout << "2.删除歌曲" << endl;
		cout << "q.退出" << endl;
		cout << "  请输入[1 2 q]->";
		char ch1 = getche();
		if (ch1 == '1') {
			cout << endl;
			string song_name, singer;
			cout << "请输入歌曲名称与歌手姓名(用空格分开)";
			cin >> song_name >> singer;
			cout << endl;

			MUS new_song(song_name, singer);
			LUser.Locate(user_x, true)->data.AppendMUS(new_song);
		} else if (ch1 == '2') {
			cout << endl;
			string song_name, singer;
			cout << "请输入歌曲名称与歌手姓名(用空格分开)";
			cin >> song_name >> singer;
			cout << endl;

			MUS new_song(song_name, singer);
			LUser.Locate(user_x, true)->data.DeleteMUS(new_song);
		} else if (ch1 == 'q')
			break;
		else
			cout << endl << "ERROR!";

	}

}

void Create_Delete(LinkList<User> &LUser) {
    while (1) {
        cout << endl << endl << "1.创建用户" << endl;
        cout << "2.删除用户" << endl;
        cout << "q.退出" << endl;
        cout << "  请输入[1 2 q]->";
        char ch1 = getche();
        if (ch1 == '1') {
            cout << endl;
            string name, id;
            cout << "请输入您的名称与ID号(四位任意数字，默认为1883，用空格分开)：";
            cin >> name >> id;
            cout << endl;

            if (id.length() != 4 || !isdigit(id[0]) || !isdigit(id[1]) || !isdigit(id[2]) || !isdigit(id[3])) {
                cout << "非法ID，请输入四位数字作为ID。" << endl;
                continue;
            }

            User new_man(id, name);
            LUser.Insert(new_man);
            cout << "创建成功！" << endl;

        } else if (ch1 == '2') {
            cout << endl << "请输入要删除的用户名称与ID号(用空格分开)：";
            string name, id;
            cin >> name >> id;
            cout << endl;

            User new_man(id, name);
            Node<User>* found = LUser.Locate(new_man, 1);
            if (found == NULL) {
                cout << "未找到该用户。" << endl;
            } else {
                LUser.DeleteCurNode();
                cout << "删除成功！" << endl;
            }
        } else if (ch1 == 'q')
            break;
        else
            cout << endl << "ERROR!";

        cout << "当前用户有:" << endl;
        LUser.PutList(cout);
    }
}


void Research(LinkList<User>& LUser) 
{
	cout << endl;
	cout << "请输入用户的名字与ID(用空格分开)：";
	string user_name, user_id;
	cin >> user_name >> user_id;
	User user_x(user_id, user_name);
	 
	Node<User>* found = LUser.Locate(user_x, true);
    if (found == NULL) {
        cout << "未找到该用户。" << endl;
        return;
    }
    
	Node<User> *cur_User_node = LUser.Locate(user_x, true);
	User cur_User = cur_User_node->GetData();

	cout << "可以根据以下两种方式查询，请选择：" << endl;
	cout << "1. 歌曲" << endl;
	cout << "2. 歌手" << endl;
	char choice;
	cin >> choice;
	string input;
	if (choice == '1')
	{
		cout << "请输入歌曲" << endl;
		cin>>input;
		cur_User.Research2(input, 1);
	}
	else
	{
		cout << "请输入歌手" << endl;
		cin >> input;
		cur_User.Research2(input, 2);
	}
}

void Show(LinkList<User> &LUser) {
	cout << endl;
	cout << "请输入用户的名字与ID(用空格分开)：";
	string user_name, user_id;
	cin >> user_name >> user_id;
	User user_x(user_id, user_name);
	
	Node<User>* found = LUser.Locate(user_x, true);
    if (found == NULL) {
        cout << "未找到该用户。" << endl;
        return;
    }
	
	Node<User> *cur_User_node = LUser.Locate(user_x, true);
	User cur_User = cur_User_node->data;
	cur_User.u_Show(cout);
	cout << endl;
	cur_User.m_Show(cout);
}

void Reverse(LinkList<User> &LUser) {
	cout << endl;
	cout << "请输入用户的名字与ID(用空格分开)：";
	string user_name, user_id;
	cin >> user_name >> user_id;
	User user_x(user_id, user_name);
	Node<User>* found = LUser.Locate(user_x, true);
	
    if (found == NULL) {
        cout << "未找到该用户。" << endl;
        return;
    }
    
	LUser.Locate(user_x, true)->data.m_Reverse();
	cout << "倒置成功并展示:" << endl;

	LUser.Locate(user_x, true)->data.u_Show(cout);
	cout << endl;
	LUser.Locate(user_x, true)->data.m_Show(cout);
}



void Upload(LinkList<User> &LUser)
{
	char filename[] = "music_data.txt";
	LUser.Save(filename);
	
    int	n = LUser.NumNodes();
	cout <<endl<< "将" << n << " 个结点的数据写入: " << filename << "文件。" << endl;
}

void Download(LinkList<User> &LUser)
{
	int n = 0,curNum=0;
	User tempUser;
	Node<User>* newUser=NULL;
	char str[80];
	char filename[] = "music_data.txt";
	
	curNum = LUser.NumNodes();
	/*由于是多级列表，直接处理文件，与输出配合*/
	ifstream in(filename);
	if(in.fail()){
	   cout<<endl << "读取数据文件打不开 ";	
	   return; 
	}
	
	cout <<endl<< "是否先释放已有的所有结点(y/[N])? "<<endl;
	char yn = getche();
	if(yn=='y' || yn=='Y')
		LUser.FreeList();
	
	/*获取加载用户数*/
	int UserNum=0;
	in.getline(str, 80, '(');					// 首先忽略'('字符及其可能的所有前导字符
	in.getline(str, 80);						// 读取字符至行尾
	UserNum = atoi(str);
	for(int i=0;i<UserNum;i++)
	{
		in>>tempUser;
		newUser=LUser.Insert(tempUser);
		((User*)newUser)->m_Get(in);
	}
	in.close();
	
	cout <<endl<< "从 " << filename << " 文件中读取"<< UserNum << "个数据。现有 "
		 << LUser.NumNodes() << "个结点" <<endl;
}
