// myMusic.h
#ifndef MY_MYMUSIC_H
#define MY_MYMUSIC_H
#include "LinkList.h"
#include <iomanip>

class User;

class MUS//Music;
{
private:
	string song_name;//歌曲名称
	string singer;//歌手
public:
	MUS();//设置默认值
	MUS(const string Song_name,const string Singer);//构造函数
	void set(const string Song_name,const string Singer);//设置歌曲名称，设置歌手名称
	bool operator!=(const MUS&b)const;//重载运算符“!=”
	friend ostream & operator<<(ostream &out, const MUS &b);//重载ostream输出流
	friend istream & operator>>(istream &in, MUS &b);//重载istream输入流
	friend class User;
};

class User//用户
{
public:
	User();//设置默认值
	User(const string ID, const string Name);//构造函数
	void ReSet(const string ID, const string Name);//重置用户ID和名称
	void AppendMUS(const MUS &b);//在尾部添加歌曲
	void DeleteMUS(const MUS &b);//删除指定歌曲
	void m_Show(ostream &out) const;//输出播放列表（到显示屏或者是文件中）
	void u_Show(ostream &out) const;//输出用户信息
	void m_Reverse();//倒置歌单列表
	void Research2(string str,int i);
	int m_Research(const MUS&b);//搜索指定歌曲位置
	friend ostream & operator<<(ostream &out, const User &u);//重载ostream输出流
	friend istream & operator>>(istream &in, User &u);//重载istream输入流
	void m_Get(istream &in); 
	bool operator!=(const User&b)const;
private:
	string id, name;		// 账号、户名
	LinkList<MUS> music_link;		// 链表(该用户的歌单)
};

void Create_Delete(LinkList<User> &LUser);
void Add_Delete(LinkList<User> &LUser);
void Research(LinkList<User> &LUser);
void Show(LinkList<User> &LUser);
void Reverse(LinkList<User> &LUser);
void Upload(LinkList<User> &LUser);
void Download(LinkList<User> &LUser);


#endif

