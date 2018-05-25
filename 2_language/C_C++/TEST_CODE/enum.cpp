//1枚举值可以和普通变量值作逻辑比较
#include<iostream>
using namespace std;
int main()
{
	enum e{a=100,b=200};
    if(a==100)cout<<"枚举值可以和普通变量值作逻辑比较"<<endl;
	return 0;
}