//1如果把友元函数的函数体写在类定义内，那么这个类将只能用于本类所属对象间的运算，
//类与其它类型的混合运算将出现不接受类型转换的情况。也就是编译器匹配不到合适的
//转换
#include<iostream>
#include<string>
using namespace std;
class lei
{
public:
	explicit lei(string s="default string"){str=s;}
	lei(char c){str=c;}
	friend lei operator+(lei&a,lei&b)
	{
		return lei(a.str+b.str);
	}
	string str;
	void show(){cout<<str<<endl;}
private:
};
int main()
{
	lei a("take"),b("action");
	(a+b).show();

	//cout<<('A'+lei("BC")).str<<endl;//error C2678: 二进制“+”: 没有找到接受“char”类型的左操作数的运算符(或没有可接受的转换)
	//cout<<(lei("BC")+'A').str<<endl;//error C2679: 二进制“+”: 没有找到接受“char”类型的右操作数的运算符(或没有可接受的转换)

	return 0;
}


