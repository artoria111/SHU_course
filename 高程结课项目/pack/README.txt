请在mainfuc.cpp里写函数

seller,buyer,goods三个结构体的内容在structs.txt里

输入样例在examples.txt里

已经事先将examples.txt里的样例保存在.bin文件中

文件格式在fileformat中（不需要看懂）

可以直接调用的函数（无注释版在funcs.txt中）

seller* get_seller_from_file()
从seller.bin中获得全部seller信息，返回一个seller指针指向单链表的头部

buyer*  get_buyer_from_file()
从buyer.bin 中获得全部buyer信息，返回一个buyer指针指向单链表的头部

goods*  get_goods_from_file()
从goods.bin 中获得全部goods信息，返回一个goods指针指向单链表的头部


void write_seller_info_file(seller*) 将seller*所表示的单链表中的内容全部写入seller.bin
void write_buyer_info_file(buyer*) 将buyer*所表示的单链表中的内容全部写入buyer.bin
void write_goods_info_file(goods*) 将goods*所表示的单链表中的内容全部写入goods.bin


seller* input_seller() 从命令行中读取输入的seller信息
buyer* input_buyer() 从命令行中读取输入的buyer信息
goods* input_goods() 从命令行中读取输入的goods信息
注：以上三个函数从命令行输入的信息需按格式输入，格式在structs.txt文件内


seller* input_multi_seller(int)
从命令行中读取k个输入的seller信息，以回车分隔，返回一个seller指针指向单链表的头部

buyer* input_multi_buyer(int)
从命令行中读取k个输入的buyer信息，以回车分隔，返回一个buyer指针指向单链表的头部

goods* input_multi_goods(int)
从命令行中读取k个输入的buyer信息，以回车分隔，返回一个goods指针指向单链表的头部


void print_seller(seller*) 将seller*所表示的单链表中的内容全部输出到命令行内
void print_buyer(buyer*) 将buyer*所表示的单链表中的内容全部输出到命令行内
void print_goods(goods*) 将goods*所表示的单链表中的内容全部输出到命令行内


注：慎用以下三个函数
void init_seller_file() 清空seller.bin内的信息
void init_buyer_file() 清空buyer.bin内的信息
void init_goods_file() 清空goods.bin内的信息

