#include <iostream>
#include "operate_goods.cpp"
#include "operate_buyer.cpp"
#include "operate_seller.cpp"
#include <vector>
#include <cstring>
using namespace std;


enum UserType {
	BUYER,
	SELLER,
	ADMIN
};
seller* newseller(long long sellerid=-1){
	seller* aseller = new seller;
	aseller -> nxtseller = NULL;

	if (sellerid == -1){
		cout << "请输入您的卖家编号\n";
		cin >> aseller -> sellerid;
	} else {
		aseller -> sellerid = sellerid;
	}
	cout << "请输入您的姓名\n";
	cin >> aseller->name;
	cout << "请输入您的联系方式\n";
	cin >> aseller->contact;
	aseller -> goods_cnt = 0;
	return aseller;
}

buyer* newbuyer(string buyerid){
	buyer* abuyer = new buyer;
	abuyer -> nxtbuyer = NULL;

	abuyer -> buyerid = buyerid;
	
	cout << "请输入您的姓名\n";
	cin >> abuyer->name;
	abuyer -> goods_cnt = 0;
	return abuyer;
}

void DisplayMenu(UserType user){
	cout << "\n您现在的身份是：";
	if(user == BUYER){
		cout << "BUYER\n";
	} else if (user == SELLER){
		cout << "SELLER\n";
	} else {
		cout << "ADMIN\n";
	}
	cout << "请输入数字以执行操作\n";
	cout << "0.退出系统\n2.选择用户身份权限\n3.买家查询商品\n4.买家购买商品\n5.卖家发布商品\n6.卖家修改商品\n7.卖家删除商品\n8.管理员删除买家信息\n9.管理员删除卖家信息\n10.保存信息到文件\n";
}

UserType getUserType() {
	// func2
	int choice;
	cin >> choice;


	switch (choice) {
		case 1:
			return BUYER;
		case 2:
			return SELLER;
		case 3:
			return ADMIN;
		default:
			cout << "无效的选择。请重新输入：" << endl;
			return getUserType();
	}
}
UserType SelectUserType(bool init=false) {
	// func1
	if (init==true) {cout << "欢迎使用系统，请选择用户身份：" << endl;}
	else {cout << "请选择用户身份：" << endl;}
	cout << "1. 买家 （查询商品/购买商品）" << endl;
	cout << "2. 卖家 （分布商品/修改商品/删除商品）" << endl;
	cout << "3. 管理员 （删除买家信息、卖家信息/保存信息到文件）" << endl;
	cout << "请输入相应的数字以选择身份：" << endl;
	return getUserType();
}
long long str2ll(string s){
	long long minf = -9223372036854774808;
	if (s.size() == 0){
		return minf;
	}
	long long r = 0;
	int sgn = 1;
	int i = 0;
	if (s[0] == '-' && s.size() != 1){
		sgn = -1;
		i++;
	}
	for (; i < s.size(); ++i) {
		if (s[i] >= '0' && s[i] <= '9'){
			r = r*10+s[i]-'0';
		} else {
			return minf;
		}
	}
	return r;
}

void BuyerSearch(UserType user,goods* pgoods) {
	// func3

	if (user != BUYER){
		cout << "您不是买家\n";
		return ;
	}

	char a = cin.get();
	goods *p;
	string iname;
	int flag = 0;
	cout << "请输入需要查找的商品名字或是商品编号或是商品的描述，也可以按A查询所有商品信息->";
	cin >> iname;
	if (iname == "A" && pgoods != NULL)
		print_goods(pgoods);
	else if (iname == "A" && pgoods == NULL)
		cout << "当前没有商品！" << endl;
	else {
		cout << "以下是可能符合您要求的商品：\n";
		for (goods* now = pgoods; now != NULL; now = now->nxtgoods){
			if (now->goodsid == str2ll(iname) || now->name.find(iname) != string::npos || now->description.find(iname) != string::npos){
				cout << "商品编号为：" << now->goodsid;
				cout << "\t商品名称为：" << now->name;
				cout << "\t商品描述为：" << now->description;
				cout << "\t商品件数为：" << now->pieces;
				cout << "\t商品价格为：" << now->price;
				cout << endl;
			}

		}

	}
}
void BuyerBuy(UserType user,buyer* &pbuyers,goods* pgoods) {
	// func4

	if (user != BUYER){
		cout << "您不是买家\n";
		return ;
	}
	string ibuyerid;
	cout << "请输入您的买家编号：";
	cin >> ibuyerid;

	bool found_buyer = false;
	buyer* nowbuyer = pbuyers;
	for(;nowbuyer != NULL; nowbuyer = nowbuyer->nxtbuyer){
		if (ibuyerid.compare(nowbuyer->buyerid) == 0){
			found_buyer = true;
			break;
		}
	}

	if (found_buyer == false){
		cout << "检测到买家编号不存在，系统自动帮您新建一个买家，请完成初始化\n";
		nowbuyer = newbuyer(ibuyerid);

		nowbuyer -> nxtbuyer = pbuyers;
		pbuyers = nowbuyer; //头插链表
	}
	long long gid;
	cout << "请输入要购买的商品编号：  ";
	cin >> gid;
	bool found_goods = false;
	for (goods* nowgoods = pgoods; nowgoods != NULL; nowgoods = nowgoods -> nxtgoods){
		if (nowgoods -> goodsid == gid){
			found_goods = true;

			if (nowgoods -> pieces == 0){
				cout << "抱歉，当前没有该商品库存" << endl;
			} else {
				cout << "购买成功！" << endl;
				nowgoods->pieces--;
				nowbuyer -> goods_cnt++;
				nowbuyer -> goodsids.push_back(gid);
			}
			break;
		}
	}
	if (!found_goods){
		cout << "该编号的商品不存在!\n";
	}

}

int findgoods(goods *head,long long gnum) {//gnum为用户输入的编号   判断商品编号是否存在
	goods *p=head;
	while(p!=NULL) {
		if(p->goodsid==gnum)
			return 1;
		p=p->nxtgoods;
	}
	return -1;
} 
void SellerAddGoods(UserType user,goods *&goodshead,seller* &sellerhead){ //卖家发布商品
	// func5

	if (user != SELLER){
		cout << "您不是卖家\n";
		return ;
	}

	long long sellerid,gnum; 
	cout<<"请输入您的商家编号: "<<endl;
	cin>>sellerid;
	cout<<"请输入您想发布的商品的编号： "<<endl;
	cin>>gnum;
	int ret=findgoods(goodshead,gnum);
	if(ret==1){ //编号存在 
		cout << "该商品编号已存在\n";
	}
	else { //编号不存在
		if (sellerhead == NULL){
			cout << "检测到卖家编号不存在，系统自动帮您新建一个卖家，请完成初始化\n";
			sellerhead= newseller(sellerid);
		}
		seller* nowseller = sellerhead;
		while(true){
			if (nowseller -> sellerid = sellerid){
				break;
			}
			if (nowseller -> nxtseller == NULL){
				cout << "检测到卖家编号不存在，系统自动帮您新建一个卖家，请完成初始化\n";
				seller* aseller = newseller(sellerid);
				nowseller -> nxtseller = aseller;
				break;
			}
			nowseller = nowseller->nxtseller;
		}
		goods * agoods = new goods;
		agoods -> nxtgoods = NULL;
		cout<<"正在添加新的商品"<<endl;
				
		//商品编号
				
		cout<<"请输入商品编号:" <<endl; 
		cin >> agoods->goodsid;
				
		//商品名称
		cout<<"请输入商品名称:" <<endl; 
		cin >> agoods->name;
				
		//商品描述 
		cout<<"请输入商品描述:" <<endl; 
		cin >> agoods->description;
				
		//商品价格
		cout<<"请输入商品价格:" <<endl; 
		cin >> agoods->price;
		
		//商品库存 
		cout<<"请输入商品库存:" <<endl; 
		cin >> agoods->pieces;
				
		//售卖商家编号 
		cout<<"请输入售卖商家编号:" <<endl; 
		cin >> agoods->sellerid;
		
		if(goodshead==NULL) goodshead = agoods;
		else {
			goods* now;
			for (now = goodshead; now != NULL; now = now -> nxtgoods){}
			now -> nxtgoods = agoods;
		}

		nowseller = sellerhead;
		while(true){
			if (nowseller -> sellerid = sellerid){
				nowseller -> goods_cnt++;
				nowseller -> goodsids.push_back(agoods->goodsid);
				break;
			}
			if (nowseller -> nxtseller == NULL) {
				cout << "FIND SELLER ERROE: EXPECTED SELLER IN CHAIN\n";
				break;
			}
			nowseller = nowseller->nxtseller;
		}
		
		cout<<"添加成功"<<endl;

		// system("pause");
		// system("cls");
		// return goodshead;
	}
}

void SellerModifyGoods(UserType user,goods *head) {
	// func6

	if (user != SELLER){
		cout << "您不是卖家\n";
		return ;
	}

	long long sellerid,snum;
	cout << "请输入您的卖家编号: "<<endl;
	cin >> sellerid;
	cout << "请输入您想修改的商品的编号： "<<endl;
	cin >> snum;
	int ret=findgoods(head,snum);
	if(ret==1){
		goods *now=head;
		while(now!=NULL) {
			if(now->goodsid==snum){ 
				if (now->sellerid==sellerid){  //判断该商品是否属于该商家 
					cout<<"请修改商品的信息"<<endl;
					
					cout<<"请输入商品名称:" <<endl;
					cin>>now->name;
					
					cout<<"请输入商品描述:" <<endl;
					cin>>now->description;
					
					cout<<"请输入商品价格:" <<endl;
					cin>>now->price;
			
					cout<<"请输入商品库存:" <<endl;
					cin>>now->pieces;
					
					cout<<"修改成功,以下是当前商品信息："<<endl;
					cout<<"商品编号:"<<now->goodsid<<"\t商品名称："<<now->name<<"\t商品描述："<<now->description<<"\t商品价格："<<now->price<<"\t商品库存："<<now->pieces<<"\t售卖商家编号："<<now->sellerid<<endl;
				}
				else {
					cout<<"这不是您所售卖的商品"<<endl;
				}
				break;
			}
			now=now->nxtgoods;
		}
	}
	else {
		cout<<"该商品不存在"<<endl;
	}
	
}

void SellerDeleteGoods(UserType user,buyer* buyerhead,goods *&goodshead,seller* sellerhead) {

	if (user != SELLER){
		cout << "您不是卖家\n";
		return ;
	}

	if(goodshead==NULL)
		cout<<"当前无可删除的商品"<<endl;
	else {
		long long sellerid,goodsid;
		cout << "请输入您的卖家编号: "<<endl;
		cin >> sellerid;
		cout << "请输入您想删除的商品的编号： "<<endl;
		cin >> goodsid;
		int ret = findgoods(goodshead,goodsid);
		if(ret==1) {
			bool del_success = false;
			goods *now=goodshead,*last=goodshead;
			if(goodshead->goodsid == goodsid && goodshead->sellerid==sellerid) {
				// 假如删除的商品在首位
				goodshead = goodshead->nxtgoods;
				delete now;
				cout<<"删除成功"<<endl;
				del_success = true;
			}
			else {
				while(now!=NULL) {
					
					if(now->goodsid==goodsid){
						if (now->sellerid == sellerid){
							last->nxtgoods=now->nxtgoods;
							delete now;
							del_success = true;
							cout<<"删除成功"<<endl;
						}
						else
							cout<<"这不是您所售卖的商品"<<endl;
						break;
					}
					last = now;
					now=now->nxtgoods;
				}
			}

			if (del_success == true){ // 删除对应买家，卖家的商品信息
				for (buyer* nowb = buyerhead; nowb != NULL; nowb = nowb -> nxtbuyer){
					for (int i = 0; i < nowb->goods_cnt; ++i){
						if (nowb->goodsids[i] == goodsid){
							nowb->goodsids.erase(nowb->goodsids.begin()+i);
							i--;
							nowb->goods_cnt--;
						}
					}
				}


				seller* nowseller = sellerhead;
				for (; nowseller != NULL; nowseller = nowseller->nxtseller){
					bool found_seller = false;
					if (nowseller->sellerid == sellerid){
						for(int i = 0; i < nowseller->goods_cnt; ++i){
							if (nowseller -> goodsids[i] == goodsid){
								nowseller->goodsids.erase(nowseller->goodsids.begin()+i);
								nowseller->goods_cnt--;
								found_seller = true;
								break;
							}
						}
					}
					if (found_seller){break;}
				}
			}


		}


		else {
			cout<<"该商品不存在"<<endl;
		}
	
	}

}

void return_bought_goods(buyer* buyer,goods* goods_head){
	goods* nowgoods;
	for (int i = 0; i < buyer -> goods_cnt; ++i){
		for (nowgoods = goods_head; nowgoods != NULL; nowgoods = nowgoods -> nxtgoods){
			if (nowgoods -> goodsid == buyer -> goodsids[i]){
				nowgoods -> pieces++;
				break;
			}
		}

	}
}

void DeleB(buyer *&buyer_head, string inbuyerid,goods* goods_head){
	buyer *p=buyer_head, *q=NULL;
	if(NULL==buyer_head)
		return;
	if(inbuyerid == buyer_head->buyerid){
		return_bought_goods(p,goods_head);
		buyer_head=p->nxtbuyer;
		delete p;
		return;
	}
	q=p->nxtbuyer;
	while(NULL!=q && inbuyerid != q->buyerid){
		p=q;
		q=q->nxtbuyer;
	}
	if(NULL!=q){
		p->nxtbuyer=q->nxtbuyer;
		return_bought_goods(p,goods_head);
		delete q;
		return;
	} 
}
void AdminDeleteBuyer(UserType user,buyer *&pbuyers,goods* goods_head){

	if (user != ADMIN){
		cout << "您不是管理员" << endl;
		return ;
	}

	char a;
	a = cin.get();
	buyer *p; //p用于循环 
	string inbuyerid;
	
	//print_buyer(pbuyers);
	
	if(pbuyers==NULL)
		cout<<"所有买家信息均已删除！"<<endl;
	else{
		cout<<"所有买家信息如下："<<endl; 
		print_buyer(pbuyers);
		
		cout << "请输入要删除的买家编号： ";
		cin >> inbuyerid; //输入要删除的买家的编号 
		
		bool flag_findbuyer=0;//标记是否找到要删除的买家信息 
		
		for (p = pbuyers; p != NULL; p = p->nxtbuyer){
			if (p->buyerid == inbuyerid){
				DeleB(pbuyers,inbuyerid,goods_head);
				flag_findbuyer=1; 
				
				//write_buyer_info_file(pbuyers);
				cout << "该买家信息已删除！"<<endl; 
				
				cout<<"删除操作执行后所有买家信息如下："<<endl;
				print_buyer(pbuyers);
				break;
			}
		}
		if(!flag_findbuyer){
			cout<<"未找到当前买家！"<<endl; 
		}
	}
} 


void DeleGs(goods *&goodshead,long long insellerid){
	goods* tmp;
	while(goodshead != NULL && goodshead->sellerid == insellerid){
		tmp = goodshead;
		goodshead = goodshead -> nxtgoods;
		delete tmp;	
	}
	goods* now = goodshead;
	goods* last = NULL;
	while(now != NULL){
		if (now -> sellerid == insellerid){
			last -> nxtgoods = now -> nxtgoods;
			delete now;
			now = last -> nxtgoods;
		} else {
			last = now;
			now = now -> nxtgoods;
		}
	}
}

void DeleS(seller *&seller_head, long long insellerid){
	seller *p=seller_head, *q=NULL;
	if(NULL==seller_head)
		return;
	if(insellerid == seller_head->sellerid){
		seller_head=seller_head->nxtseller;
		delete p;
		return;
	}
	q=p->nxtseller;
	while(NULL!=q && insellerid != q->sellerid){
		p=q;
		q=q->nxtseller;
	}
	if(NULL!=q){
		p->nxtseller=q->nxtseller;
		delete q;
		return;
	}
}

void AdminDeleteSeller(UserType user,buyer* buyerhead,goods *&goodshead,seller*& psellers){

	if (user != ADMIN){
		cout << "您不是管理员" << endl;
		return ;
	}
	seller *p; //p用于循环
	long long insellerid;
	
	//print_seller(psellers);
	
	if(psellers==NULL)
		cout<<"所有卖家信息均已删除！"<<endl;
	else{
		cout<<"所有卖家信息如下："<<endl; 
		print_seller(psellers);////
	
		cout << "请输入要删除的卖家编号： ";
		cin >> insellerid; //输入要删除的卖家的编号 
		
		bool flag_findseller=0;//标记是否找到要删除的卖家信息

		seller *seller_head = psellers;

		for (p = psellers; p != NULL; p = p->nxtseller){
			if (p->sellerid == insellerid){
				
				for (int i = 0; i < p->goods_cnt; ++i){ //删除买家买的 卖家的商品 的信息
					long long goodsid = p->goodsids[i];
					for (buyer* nowb = buyerhead; nowb != NULL; nowb = nowb -> nxtbuyer){
						for (int i = 0; i < nowb->goods_cnt; ++i){
							if (nowb->goodsids[i] == goodsid){
								nowb->goodsids.erase(nowb->goodsids.begin()+i);
								i--;
								nowb->goods_cnt--;
							}
						}
					}
				}


				DeleGs(goodshead,insellerid);
				DeleS(psellers,insellerid);
				flag_findseller=1; 
				
				cout << "该卖家信息已删除！"<<endl; 
				
				cout<<"删除操作执行后所有卖家信息如下："<<endl;
				print_seller(psellers);
				break;
			}
		}
		if(!flag_findseller){
			cout<<"未找到当前卖家！"<<endl; 
		}
	} 
} 


int main() {
	buyer* buyer_head = get_buyer_from_file();
	goods* goods_head = get_goods_from_file();
	seller* seller_head = get_seller_from_file();
	
	UserType user = SelectUserType(true);
	
	while(true){
		
	
	DisplayMenu(user);
	int operateid;
	cin >> operateid;
	switch (operateid){
		case -17:
			buyer_head = get_buyer_from_file();
			goods_head = get_goods_from_file();
			seller_head = get_seller_from_file();
			break;
		case -1:
			cout << "\nseller------------------------------------------------\n";
			print_seller(seller_head);
			cout << "\nbuyer------------------------------------------------\n";
			print_buyer(buyer_head);
			cout << "\ngoods------------------------------------------------\n";
			print_goods(goods_head);
			break;
		case 0:
			return 0;
		case 2:
			user = SelectUserType(); break ;
		case 3:
			BuyerSearch(user,goods_head); break;
		case 4:
			BuyerBuy(user,buyer_head,goods_head); break;
		case 5:
			SellerAddGoods(user,goods_head,seller_head); break;
		case 6:
			SellerModifyGoods(user,goods_head); break;
		case 7:
			SellerDeleteGoods(user,buyer_head,goods_head,seller_head); break;
		case 8:
			AdminDeleteBuyer(user,buyer_head,goods_head); break;
		case 9:
			AdminDeleteSeller(user,buyer_head,goods_head,seller_head); break;
		case 10:
			write_seller_info_file(seller_head);
			write_buyer_info_file(buyer_head);
			write_goods_info_file(goods_head);
			break;
		default:
			cout << "请输入正确的操作id\n";
			break;
	}

	}

	return 0;
}

