#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include "commonfucs.cpp"
using namespace std;

struct buyer {
	buyer* nxtbuyer;
	string buyerid;
	string name;
	int goods_cnt;
	vector<long long> goodsids; //所有的商品编号
};

void print_buyer(buyer* first){
	buyer* now = first;
	while(now != NULL) {
		cout << now->buyerid << " ";
		cout << now->name << " ";
		for(int i = 0; i < now->goods_cnt-1; ++i){
			printf("%d,",now->goodsids[i]);
		}
		if (now->goods_cnt != 0)
			printf("%d\n",now->goodsids[now->goods_cnt-1]);
		else
			printf("\n");

		now = now -> nxtbuyer;
	}
}

void init_buyer_file () {
	long long cnt = 0;
	long long zero = 0;
	FILE * pw = NULL;
	pw = fopen("buyer.bin","wb");
	fwrite (&cnt ,sizeof(long long), 1, pw);
	fwrite (&zero ,sizeof(long long), 1, pw);
	fclose(pw);
}

buyer* get_buyer_from_file(){
	FILE * pr = NULL;
	pr = fopen("buyer.bin","rb");
	long long items,reserved;
	fread(&items,sizeof(long long),1,pr);
	fread(&reserved,sizeof(long long),1,pr);
	char ch;
	for (int i = 0; i < reserved; ++i){
		fread(&ch,sizeof(char),1,pr);
	}
	buyer* first = NULL;
	buyer* last = NULL;
	for (int i = 0; i < items; ++i){
		buyer* now = new buyer;
		now -> nxtbuyer = NULL;
		
		if(first == NULL){
			first = now;
		}
		
		read_string(pr,now->buyerid);
		
		read_string(pr,now->name);

		fread(&(now->goods_cnt),sizeof(int),1,pr);
		
		// now->goodsids = new long long[now->goods_cnt];
		// fread(now->goodsids,sizeof(long long),now->goods_cnt,pr);
		now->goodsids.resize(now->goods_cnt);
		long long tmpid;
		for (int i = 0; i < now->goods_cnt; ++i){
			fread(&tmpid,sizeof(long long),1,pr);
			now->goodsids[i] = tmpid;
		}
		
		
		if(last != NULL) {
			last -> nxtbuyer = now;
		}
		last = now;
	}

	fclose (pr);
	return first;
}

buyer* input_buyer(){
	buyer* p = new buyer;
	p -> nxtbuyer = NULL;
	
	string buyerid;
	string name;
	
	cin >> buyerid >> name;
	
	p -> buyerid = buyerid;
	p -> name = name;
	
	p -> goods_cnt = 0;
	long long id=0;char ch;
	// vector<long long> v;  //使用vector暂存所有的货物编号
	
	while(true){
		ch = getchar();
		if (ch == ' ' || ch == '	') continue;
		else if (ch == '\n') {
			p -> goods_cnt++;
			p -> goodsids.push_back(id);
			id = 0;
			break;
		} else if (ch >= '0' && ch <= '9'){
			id = id*10+(ch-'0');
		} else if (ch == ','){  //以逗号分隔
			p -> goods_cnt++;
			p -> goodsids.push_back(id);
			id = 0;
		}
	}
	// p -> goodsids = new long long[p -> goods_cnt];
	// for (int i = 0; i < p -> goods_cnt; ++i){
	// 	*( (p -> goodsids)+i ) = v[i];  //将vector中暂存的数组写入struct中
	// }

	return p;
}

void write_buyer_info_file(buyer* first){
	FILE * pw = NULL;
	pw = fopen("buyer.bin","wb");
	long long buyers,reserved;
	buyers = 0; reserved = 0;
	
	buyer* now = first;
	while(now != NULL) {
		now = now -> nxtbuyer;
		buyers++;
	}
	
	fwrite(&buyers,sizeof(long long),1,pw);
	fwrite(&reserved,sizeof(long long),1,pw);
	
	now = first;
	while(now != NULL){
		
		write_string(pw,now->buyerid);

		write_string(pw,now->name);
		
		fwrite(&(now->goods_cnt),sizeof(int),1,pw);
		
		// fwrite(now->goodsids,sizeof(long long),now->goods_cnt,pw);
		for (int i = 0; i < now->goods_cnt; ++i){
			fwrite(&(now->goodsids)[i],sizeof(long long),1,pw);
		}

		now = now -> nxtbuyer;
	}
	fclose (pw);
}

buyer* input_multi_buyer(int k){
	buyer* first = NULL;
	buyer* last = NULL;
	buyer* tmp = NULL;
	while(k--) {
		tmp = input_buyer();
		if (first == NULL){
			first = tmp;
		}	
		if (last != NULL) {
			last -> nxtbuyer = tmp;
		}
		last = tmp;
	}
	return first;
}
