#include<iostream>
using namespace std;
int main()
{
	{
		//template<typename T>//编译出错：模板不能在一个函数内部声明
		//auto fun1=[](T a){cout<<a<<endl;};
	}
	return 0;
}