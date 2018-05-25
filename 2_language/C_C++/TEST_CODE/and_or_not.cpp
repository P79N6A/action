//程序中使用and not or 与使用&& ! ||是等价的，and or not 都是关键字
#include<iostream>
using namespace std;
int main()
{
	int a(1),b(2),c(3);
	if(a<b and a<c)
		cout<<"can use and "<<endl;
	if(a<b or b>c)
		cout<<"can us or"<<endl;
	if(not 0)
		cout<<"not also can be used"<<endl;
	return 0;
}