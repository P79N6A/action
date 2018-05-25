#include<iostream>
using namespace std;
class lei
{
public:
	lei(){cout<<"lei() called"<<endl;num=1;}
	~lei(){cout<<"~lei() called"<<endl;}
	int num;
};
int main()
{
	for(int i=0/*这个位置变量定义一次就可以在循环中一直使用*/;i<3;i++)
	{
		int j=5;//每次运行到这，这个j都要重新定义并赋值
		lei a;//这个a会每次在这重新定义，一次循环完了就要被析构，for循环中执行的这部分相当于一个block
		cout<<"a.num"<<a.num<<endl;
		a.num=10;
		//cout<<j<<endl;
		j=10;
	}
	cout<<"----------------------------------------------"<<endl;
	int index=0;
	do{
		if(index==3)continue;//这个continue是跳到while条件判断处继续判断
			cout<<index<<endl;
		cout<<"/////"<<endl;
	}while(index++<5);


	return 0;
}