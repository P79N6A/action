//1Automatic type deduction works with a single initializer
//value, not an initialization list.如auto a={f1,f2,f3}//f1到f3是自定义类型
#include<iostream>
using namespace std;
int main(void)
{
	auto a=3;//a是一个int型
	auto b=3.3;//b是一个float型
	auto c='c';//c是一个char型
	auto d=3ll;//d是一个long long 型
	std::cout<<a<<'\n'
		<<b<<'\n'
		<<c<<'\n'
	    <<d<<'\n';
	cout<<"尝试利用auto使同一个函数返回不同的类型=================================="<<endl;
	auto fun(int);//浦可以这样声明函数
	//cout<<fun(1)<<endl;
}
//auto fun(int in)
//{
//	if(int==1)
//		return 1;
//	if(int==2)
//		return "happy";
//	if(int==3)
//		return 'A';
//}