#include<iostream>
#include<vector>
using namespace std;
int main()
{
	const std::vector<int> v1 = { 1, 2, 3, 5, 7, 11, 13, 17, 21 };
	for(auto a:v1)
		cout<<a<<' ';
	cout<<endl<<"constexpr测试================================================================="<<endl;
	const int ba=1;
	constexpr int bb=2;//C++0x中constexpr可以声明那些在编译期间不发生变化的常量
	//而const则可以保证在运行时产生的const量不被其它函数表达式改变，当然包括编译期间，它
	//也相当于一个操作权限说明符
	return 0;
}