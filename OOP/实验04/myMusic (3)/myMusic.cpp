// myMusic.cpp
#include "myMusic.h"
#include<iostream>
MUS::MUS() {
    song_name = singer = "Noname";
}

MUS::MUS(const string Song_name, const string Singer) {
    song_name = Song_name;
    singer = Singer;
}

void MUS::set(const string Song_name, const string Singer) {
    song_name = Song_name;
    singer = Singer;
}

ostream& operator<<(ostream& out, const MUS& b) {
    out << "Song: " << b.song_name << "\t" << "Singer: " << b.singer << endl;
	return out;
}

istream & operator>>(istream &in, MUS &b){

	char str[80];
	string song_name,singer;
	in.getline(str, 80,':');
	in>>song_name; 
	in.getline(str, 80,':');
	in>>singer;
	b.set(song_name,singer);
	in.getline(str, 80); 
	return in;
}

bool MUS::operator!=(const MUS& b)const {
    if (song_name != b.song_name)
        return true;

    return false;
}

User::User() {
    id = "1883";
    name = "Noname";
}

User::User(const string ID, const string Name) {
    id = ID;
    name = Name;
}

void User::ReSet(const string ID, const string Name) {
    id = ID;
    name = Name;
}

void User::AppendMUS(const MUS& b) {
    music_link.Append(b);
}

void User::DeleteMUS(const MUS &b) {
    Node<MUS>* found = music_link.Locate(b, true);
    if (found == NULL) {
        cout << "未找到指定的歌曲。" << endl;
    } else {
        music_link.DeleteCurNode();
    }
}

void User::m_Reverse() {
    music_link.Reverse();
}

int User::m_Research(const MUS& b) {
    music_link.Locate(b, true);
    return music_link.CurPos();
}

void User::m_Show(ostream& out) const {
    music_link.PutList(out);
}

void User::m_Get(istream &in){
	music_link.GetList(in);
}

void User::u_Show(ostream& out) const {
    out << "User:" << name << "  " << "ID:" <<  id ;
}

bool User::operator!=(const User& b)const {
    if (name != b.name || id != b.id)
        return true;

    return false;
}

void User::Research2(string str, int i)
{
    Node<MUS>* current = music_link.GoTop(); // 获取链表的头结点
    bool found = false; // 标识是否找到匹配项
    if (i == 1)
    {
        // 遍历整个链表
        while (current != nullptr)
        {
            // 获取当前节点的歌曲数据
            MUS current_music = current->GetData();

            // 比较输入的字符串与歌曲名称或歌手名称
            if (current_music.song_name == str)
            {
                // 输出匹配的歌曲信息
                cout << "找到歌曲: " << current_music << endl;
                found = true; // 标识为已找到匹配项
            }

            try {
                current = music_link.Skip(1); // 移动到下一个节点
            }
            catch (int) {
                break; // 到达链表尾部，结束循环
            }
        }

        if (!found)
        {
            cout << "未找到与 '" << str << "' 匹配的歌曲。" << endl;
        }
    }
    else
    {
        // 遍历整个链表
        while (current != nullptr)
        {
            // 获取当前节点的歌曲数据
            MUS current_music = current->GetData();

            // 比较输入的字符串与歌曲名称或歌手名称
            if (current_music.singer == str)
            {
                // 输出匹配的歌曲信息
                cout << "找到歌曲: " << current_music << endl;
                found = true; // 标识为已找到匹配项
            }

            try {
                current = music_link.Skip(1); // 移动到下一个节点
            }
            catch (int) {
                break; // 到达链表尾部，结束循环
            }
        }

        if (!found)
        {
            cout << "未找到与 '" << str << "' 匹配的歌手。" << endl;
        }
    }

}

ostream& operator<<(ostream& out, const User& u) {
    u.u_Show(out);
    u.m_Show(out);
    return out;
}

istream & operator>>(istream &in, User &u){
	char str[80];
	string id,name;
	in.getline(str, 80,':');
	in>>name; 
	in.getline(str, 80,':');
	in>>id;
	u.ReSet(id,name);
	return in;
}
