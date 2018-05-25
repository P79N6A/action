
#include<iostream>
using namespace std;
class leiA
{
public:
	leiA(){a=0;e=2.71828;}
	int *a;
	double e;
};
int main()
{
	cout<<"-----------------------------------------------------------------"<<endl;
	{
		cout<<"=>“.*”和“->.”是成员指针运算符"<<endl;
		leiA a;
		//cout<<*(a.a)<<endl;//访问一个空指针会使得程序运行崩溃，如同下面这句
		//double *d=0;cout<<*d<<endl;
		
		double leiA::*pe=&leiA::e;//这个指针是一个相对地址，和起了别名一样
		//是在类中的一个相对地址
		leiA b;
		cout<<b.*pe<<endl;//相当于b.e

	}
	return 0;
}