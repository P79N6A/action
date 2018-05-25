#include<iostream>
#include<string>
using namespace std;
union lianhe
{
	int in;
	double dou;
	char c;
};
lianhe funA(int);
//template<typename T>//使用关键字typename和class都可以
template<class T>
T funB(int);
int main()
{
	cout<<"使用union====================================================================="<<endl;
	cout<<funA(1).in<<endl
		<<funA(2).dou<<endl
		<<funA(3).c<<endl;
	cout<<"使用模板===================================================================="<<endl;
	cout<<funB<int>(1)<<endl//通常的函数模板调用是不需要打<int>但这里不打出错，函数判断调用哪
		//个函数主要是从输入参数来看的，如果输入参数无异，就要用到<type>
		<<funB<double>(2)<<endl;
	//cout<<string("成功")<<endl;
	//cout<<funB<string>(3)<<endl;//用string调用失败，不知为什么
	//string sa=funB<string>(3);
	//cout<<sa;
	return 0;
}
lianhe funA(int in)
{
	lianhe ta;
	if(in==1)
		ta.in=1;
	if(in==2)
		ta.dou=3.14f;
	if(in==3)
		ta.c='A';
	return ta;
}
//template<typename T>
template<class T>
T funB(int in)
{
	if(in==1)
		return int(2013);
	if(in==2)
	{
		return double(2.718281828459f);
	}
	cout<<"这部分不会输出"<<endl;
	//if(in==3)

	//	return string("成功");

}