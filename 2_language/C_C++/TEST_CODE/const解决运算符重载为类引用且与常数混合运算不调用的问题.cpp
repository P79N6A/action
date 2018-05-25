//1类实现运算符重载，如果运算只是在类对象之间，那么重载函数的参数类型定义为引用和
//普通都可以，如果运算有类对象和常规类型的混合运算，那么重载函数的参数类型要定义
//为普通或者const引用，定义为普通引用不能通过编译（错误是不能进行类型转换），所以
//如果运算符重载要使用引用一定要加const。如果不使用引用，加不加const都可以
//2Using a const reference allows the function to generate and use a temporary variable appropriately.。参见《C++ Primer Plus》P394
//3友元函数相当于成员函数，可以访问私有成员

#include<iostream>
#include<string>
using namespace std;
class lei
{
public:
	lei(){num=0;}
	lei(int in){num=in;}
	friend lei operator+(lei&,lei&);
	friend lei operator*(lei,lei);
	friend lei operator-(const lei&,const lei&);
	friend lei operator/(const lei,const lei);
    void show()
	{cout<<num<<endl;}
private:
	int num;
};
lei operator+(lei&a,lei&b)
{
	return lei(a.num+b.num);
}
lei operator*(lei a,lei b)
{
	return lei(a.num*b.num);
}
lei operator-(const lei&a,const lei&b)
{
	return lei(a.num-b.num);
}
lei operator/(const lei a,const lei b)
{
	return lei(a.num/b.num);
}
int main()
{
	lei a(1),b(2);
    lei c;
	c=a+b;c.show();//定义为引用时，如果运算的两个量是类，+-*运算均正常
	//c=a+10;//编译出错：没有可用的转换
	c=a*b;c.show();//参数类型为非引用时
	c=a*10;c.show();//与常数运算，可以正常把常数通过构造函数转换为函数要调用的类型
	c=a-b;c.show();//参数类型定义为const引用类型
	c=a-10;c.show();//与常数混合运算正常
	cout<<"--------------------------------------"<<endl;
	c=a-3.14;c.show();//3.14是一个double类型，编译器会自动把这个double转为int再调用构造函数
	//相当于进行了两次转换
	cout<<"-------------------------------------"<<endl;
	c=10/b;c.show();//非引用使用const情况
	return 0;
}