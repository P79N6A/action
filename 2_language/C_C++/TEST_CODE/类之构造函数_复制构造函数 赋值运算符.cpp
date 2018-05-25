//1多文件程序，在头文件中如果定义了自己的构造函数，一定要实现它，否则在cpp的实现文件中，如果用到该类
//编译会出错，因为编译时要用到构造函数的具体实现。而其它函数可以只声明就能编译成功。
//2复制构造函数在所以要copy的地方被编译器调用，包括克隆，初始化定义，函数返回等地方，
//而赋值运算符在所有使用“=”地方使用，除过一种情况，在定义的同时使用赋值运算符，如 lei a=b;
///这时调用的是复制构造函数。
//在存在动态分配内存时，才需要自己定义复制构造函数或赋值运算符。
//3永远不要把类中静态变量的初始化放在构造函数中，否则每次定义新类对象时，都会改变全局变量的值。
//4在自己定义的复制构造函数和赋值运算符函数中，仍然需要对非指针变量进行编写和赋值，在复制构造
//函数中可以使用形参的private变量
//5因为类中的静态变量是公有的，如果某个对象被声明为const，其中的静态变量仍然可变。
//6复制构造函数和赋值运算符函数通过，leiA(const leiB&cl)和 leiA& operator=(const leiB&cl)或
//leiA&operator=(double b)等等将任何一种类型转化为另外任何一种类型（包括内置类型和打包类型等）
#include<iostream>
using namespace std;
class leiA
{
public:
	leiA(int n=0,int pri=0){count++;num=n;prinum=pri;}
	leiA(const leiA& cl)
	{
		count++;
		num=cl.num;
		prinum=cl.prinum;///如果不一一赋值，prinum将会是原始值
		cout<<"自定义复制构造函数called"<<endl;
	}
	int getPrivate(){return prinum;}
	int num;
	static int count;//记录同一个类下多少个对象被创建
	static int flag;
private:
	int prinum;
};
int leiA::count=0;
int leiA::flag=20;
int main()
{
	{//用count来记录由某个类定义的对象的个数
		leiA a(10),b;
		cout<<a.count<<endl;
	}
	{//复制构造函数，非指针变量的复制问题
		leiA a(3,100);
		leiA c(a);
		cout<<"a.num:"<<a.num<<'\t'<<"c.num:"<<c.num<<endl;
		cout<<"a=>prinum:"<<a.getPrivate()<<'\t'<<"c=>"<<c.getPrivate()<<endl;
	}
	cout<<"---------------------------------------------------"<<endl;
	{
		const leiA a;
		a.flag=7;//即使是const对象，其中的公有静态变量仍然可以改变
		cout<<"a.flag:"<<a.flag<<endl;
	}
	return 0;
}