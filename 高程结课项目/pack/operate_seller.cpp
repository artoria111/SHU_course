#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include "commonfucs.cpp"
using namespace std;

struct seller {
	seller* nxtseller;
	long long sellerid;
	string name;
	string contact;  //联系方式
	int goods_cnt;
	vector<long long> goodsids; //所有的商品编号
};

void print_seller(seller* first){
	seller* now = first;
	while(now != NULL) {
		printf("%d ",now->sellerid);
		cout << now->name << " ";
		cout << now->contact << " ";
		for(int i = 0; i < now->goods_cnt-1; ++i){
			printf("%d,",now->goodsids[i]);
		}
		if (now->goods_cnt != 0)
			printf("%d\n",now->goodsids[now->goods_cnt-1]);
		else
			printf("\n");
		now = now -> nxtseller;
	}
}

void init_seller_file () {
	long long cnt = 0;
	long long zero = 0;
	FILE * pw = NULL;
	pw = fopen("seller.bin","wb");
	fwrite (&cnt ,sizeof(long long), 1, pw);
	fwrite (&zero ,sizeof(long long), 1, pw);
	fclose(pw);
}

seller* get_seller_from_file(){
	FILE * pr = NULL;
	pr = fopen("seller.bin","rb");
	long long items,reserved;
	fread(&items,sizeof(long long),1,pr);
	fread(&reserved,sizeof(long long),1,pr);
	char ch;
	for (int i = 0; i < reserved; ++i){
		fread(&ch,sizeof(char),1,pr);
	}
	seller* first = NULL;
	seller* last = NULL;
	for (int i = 0; i < items; ++i){
		seller* now = new seller;
		now -> nxtseller = NULL;
		
		if(first == NULL){
			first = now;
		}
		

		fread(&(now->sellerid),sizeof(long long),1,pr);
		
		read_string(pr,now->name);

		read_string(pr,now->contact);

		fread(&(now->goods_cnt),sizeof(int),1,pr);
		
		// now->goodsids = new long long[now->goods_cnt];
		now->goodsids.resize(now->goods_cnt);
		long long tmpid;
		for (int i = 0; i < now->goods_cnt; ++i){
			fread(&tmpid,sizeof(long long),1,pr);
			now->goodsids[i] = tmpid;
		}
		
		if(last != NULL) {
			last -> nxtseller = now;
		}
		last = now;
	}

	fclose (pr);
	return first;
}

seller* input_seller(){
	seller* p = new seller;
	p -> nxtseller = NULL;
	
	long long sellerid;
	string name;
	string contact;
	
	cin >> sellerid >> name >> contact;
	
	p -> sellerid = sellerid;
	p -> name = name;
	p -> contact = contact;
	
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

void write_seller_info_file(seller* first){
	FILE * pw = NULL;
	pw = fopen("seller.bin","wb");
	long long sellers,reserved;
	sellers = 0; reserved = 0;
	
	seller* now = first;
	while(now != NULL) {
		now = now -> nxtseller;
		sellers++;
	}
	
	fwrite(&sellers,sizeof(long long),1,pw);
	fwrite(&reserved,sizeof(long long),1,pw);
	
	now = first;
	while(now != NULL){

		fwrite(&(now->sellerid),sizeof(long long),1,pw);
		
		write_string(pw,now->name);
		
		write_string(pw,now->contact);
		
		fwrite(&(now->goods_cnt),sizeof(int),1,pw);
		
		// fwrite(now->goodsids,sizeof(long long),now->goods_cnt,pw);
		for (int i = 0; i < now->goods_cnt; ++i){
			fwrite(&now->goodsids[i],sizeof(long long),1,pw);
		}

		now = now -> nxtseller;
	}
	fclose (pw);
}

seller* input_multi_seller(int k){
	seller* first = NULL;
	seller* last = NULL;
	seller* tmp = NULL;
	while(k--) {
		tmp = input_seller();
		if (first == NULL){
			first = tmp;
		}	
		if (last != NULL) {
			last -> nxtseller = tmp;
		}
		last = tmp;
	}
	return first;
}
