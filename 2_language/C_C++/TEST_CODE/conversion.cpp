//1从double到int的类型转换(包括隐式转换和显式转换)不会进行四 舍五入，而是直接截断取整数，
//如果要取得四舍五入的效果，最好是加上0.5之后再进行类型转换
#include<iostream>
using namespace std;
int main()
{
	{
		double a=3.4,b=3.7,d=9.8;
		int c;
		c=a;
		cout<<"c=a:"<<c<<endl;
		c=b;
		cout<<"c=b:"<<c<<endl;
		c=int(d);
		cout<<"int(d):"<<c<<endl;
	}
}