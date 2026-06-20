#ifndef commuonfucs
#define commuonfucs

#include <cstdio>
#include <cstring>
using namespace std;

void read_string(FILE * pr,string& s){
	int len;
	fread(&len,sizeof(int),1,pr);
	char ch;
	s.resize(len);
	for (int i = 0; i < len; ++i) {
		fread(&ch,sizeof(char),1,pr);
		s[i] = ch;
	}
}

void write_string(FILE* pw, string s){
	int len = s.length();
	fwrite(&len,sizeof(int),1,pw);
	char ch;
	for (int i = 0; i < len; ++i) {
		ch = s[i];
		fwrite(&ch,sizeof(char),1,pw);
	}
}

#endif
