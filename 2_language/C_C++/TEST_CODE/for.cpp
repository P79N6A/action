#include<iostream>
#include<functional>
using namespace std;
int main(void)
{
	{
		int a[5]{1,2,3,4,5};
		//auto display=[&](int (*pa)[5])//这里lambda函数名前面的类型是指函数类型，（已经把函数当成一个
			//独立的个体看待），所以用auto很方便，而不是函数的类型
		function<void(int(*)[5])> display=[&](int (*pa)[5])//lambda函数完整写应该是这样
		{
			cout<<"调用display:"<<endl;
			for (int x:*pa)
				cout<<x<<" ";
			cout<<endl;
		};//lambda函数后面一定要打分号
		display(&a);
cout<<"----------------------------------------------"<<endl;
		auto display2=[&](int *p)
		{
			cout<<"调用display2:"<<endl;
			//for(int x:p)//编译出错，可能因为传来的p并不包含数组完整的信息，
			//所以这里不可以用这种for结构
			//	cout<<x<<" ";
		};
		display2(a);
cout<<"----------------------------------------------"<<endl;

		for(int x:a)//x并不能改变数组中的值
			x=0;
		display(&a);

		for(int&x:a)//要想改变数组中的值，这其中的x必须定义为引用
			x=7;
		display(&a);
	}
	return 0;
}
