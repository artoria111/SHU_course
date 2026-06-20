#include <iostream>
#include <cstdio>
#include <cstring>
#include "commonfucs.cpp"
using namespace std;

struct goods {
	goods* nxtgoods;
	long long goodsid;
	string name;
	string description;
	double price;
	long long pieces;
	long long sellerid;
};

void print_goods(goods* first){
	goods* now = first;
	while(now != NULL) {
		printf("%d ",now->goodsid);
		cout << now->name << " ";
		cout << now->description << " ";
		printf("%.2f ",now->price);	
		printf("%d ",now->pieces);
		printf("%d\n",now->sellerid);
		now = now -> nxtgoods;
	}
}

void init_goods_file () {
	long long cnt = 0;
	long long zero = 0;
	FILE * pw = NULL;
	pw = fopen("goods.bin","wb");
	fwrite (&cnt ,sizeof(long long), 1, pw);
	fwrite (&zero ,sizeof(long long), 1, pw);
	fclose(pw);
}

goods* get_goods_from_file(){
	FILE * pr = NULL;
	pr = fopen("goods.bin","rb");
	long long items,reserved;
	fread(&items,sizeof(long long),1,pr);
	fread(&reserved,sizeof(long long),1,pr);
	char ch;
	for (int i = 0; i < reserved; ++i){
		fread(&ch,sizeof(char),1,pr);
	}
	goods* first = NULL;
	goods* last = NULL;
	for (int i = 0; i < items; ++i){
		goods* now = new goods;
		now -> nxtgoods = NULL;
		
		if(first == NULL){
			first = now;
		}
		
		int name_len,des_len;
		double price;

		fread(&(now->goodsid),sizeof(long long),1,pr);
		
		read_string(pr,now->name);

		read_string(pr,now->description);

		fread(&(now->price),sizeof(double),1,pr);
		
		fread(&(now->pieces),sizeof(long long),1,pr);
		
		fread(&(now->sellerid),sizeof(long long),1,pr);
		
		if(last != NULL) {
			last -> nxtgoods = now;
		}
		last = now;
	}

	fclose (pr);
	return first;
}

goods* input_goods(){
	goods* p = new goods;
	p -> nxtgoods = NULL;
	
	long long goodsid;
	string name;
	string description;
	double price;
	long long pieces;
	long long sellerid;
	
	cin >> goodsid >> name >> description >> price >> pieces >> sellerid;
	
	p -> goodsid = goodsid;
	p -> name = name;
	p -> description = description;
	p -> price = price;
	p -> pieces = pieces;
	p -> sellerid = sellerid;
	return p;
}

void write_goods_info_file(goods* first){
	FILE * pw = NULL;
	pw = fopen("goods.bin","wb");
	long long items,reserved;
	items = 0; reserved = 0;
	
	goods* now = first;
	while(now != NULL) {
		now = now -> nxtgoods;
		items++;
	}
	
	fwrite(&items,sizeof(long long),1,pw);
	fwrite(&reserved,sizeof(long long),1,pw);
	
	now = first;
	while(now != NULL){

		fwrite(&(now->goodsid),sizeof(long long),1,pw);
		
		write_string(pw,now->name);
		
		write_string(pw,now->description);
		
		fwrite(&(now->price),sizeof(double),1,pw);
		
		fwrite(&(now->pieces),sizeof(long long),1,pw);
		
		fwrite(&(now->sellerid),sizeof(long long),1,pw);
		now = now -> nxtgoods;
	}
	fclose (pw);
}

goods* input_multi_goods(int k){
	goods* first = NULL;
	goods* last = NULL;
	goods* tmp = NULL;
	while(k--) {
		tmp = input_goods();
		if (first == NULL){
			first = tmp;
		}	
		if (last != NULL) {
			last -> nxtgoods = tmp;
		}
		last = tmp;
	}
	return first;
}
