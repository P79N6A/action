//1自己定义的非复制构造函数不会覆盖编译器创建的复制构造函数，但是
//自己定义的复制构造函数会覆盖编译器创建的非复制构造函数
#include<iostream>
using namespace std;
class leiA
{
public:
	leiA(leiA&a){num=a.num;}
	int num;
};
class leiB
{
public:
	leiB(int i=1){num=i;}
	int num;
};
int main()
{
	//leiA a;//error C2512: “lei”: 没有合适的默认构造函数可用
	leiB b(10);
	leiB c(b);//使用默认的复制构造函数
	cout<<c.num<<endl;
	return 0;
}