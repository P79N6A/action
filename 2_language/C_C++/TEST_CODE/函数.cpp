//1函数有三种，一种是全局普通函数，二是局部lambda函数，三是用类打包的重载了operator()的函数。
#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
const int SIZE=100000;
bool modThree1(int x){return x%3==0;}
void fun(bool (*f)(int))
{
	static int count=0;
	cout<<"fun called the "<<++count<<"th time with &count="<<&count<<endl;
	f(3);
}
int main()
{
	vector<int> v(SIZE);
	generate(v.begin(),v.end(),rand);
	int r=0;
	r=count_if(v.begin(),v.end(),modThree1);
	cout<<"count using ordinary function is:"<<r<<endl;
	class modThree2
	{
	public:
		bool operator()(int x){return x%3==0;}
		//operator (bool(*f)(int)()){return this;}
	};
	r=count_if(v.begin(),v.end(),modThree2());
	cout<<"coutnt using Functor is:"<<r<<endl;
	auto modThree3=[](int x){return x%3==0;};
	r=count_if(v.begin(),v.end(),modThree3);
	cout<<"count using lambda function is:"<<r<<endl;
	///////////////////////////////////////////////////////////////////////////////////////////
	cout<<"-----------------------------------------------------------------------"<<endl;
	bool(*f)(int)=&modThree1;
	fun(modThree1);
	//fun(modThree2());//在fun定义为非模板的情况下，不能使用fun来统一调用这三个函数，如果使用模板或打包function<double(double)>(modThree2)再代入一定可以。如果将fun定义为模板，那么这三种函数形式将属于不同的类型，所以这个静态量将始终是count＝1；&count 不变。《C++PrimerPlus》6th P1191
	fun(modThree3);
	return 0;
}