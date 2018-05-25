#include<iostream>
using namespace std;
double& fenpei(void)
{
	return *(new double(3.14));//返回一个动态分配的数据
}
int main()
{
	int *p=new int(100);
	int& a=*p;//为动态分配的变量起了一个别名
	cout<<a<<endl;
	delete &a;//释放内存
	cout<<endl<<"---------------------------------------------"<<endl;
	double &pi=fenpei();//必须定义一个别名来作为左值，要会造成内存泄漏（无法删除）。
	//不是别名就是造成一个拷贝
	cout<<pi<<endl;
	delete &pi;
	return 0;
}