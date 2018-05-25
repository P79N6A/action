//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//C++11标准：
//1If you do provide a destructor or a copy constructor or a copy assignment operator, the compiler does not automatically provide a move constructor or a move assignment operator. If you do provide a move constructor or a move assignment operator, the compiler does not automatically provide a copy constructor or a copy assignment operator.
//2Also the defaulted move constructor and defaulted move assignment operator work
//similarly to their copy counterparts, doing memberwise initialization and copying for
//built-in types. For members that are class objects, constructors and assignment operators
//for those classes are used as if the parameters were rvalues.This, in turn, invokes move
//constructors and assignment operators, if defined, and copy constructors and assignment
//operators otherwise, if defined.
//3转移构造函数和转移赋值函数并不会自动地把被再被使用的指针值赋值为nullptr，所以它和普通的复制构造函数和复制赋值函数一样都是简单地值复制。所以一旦存在动态分配的内存，所有这些复制构造函数，转移构造函数，复制赋值函数，转移赋值函数如果需要都要自己定义，不能使用默认，否则会造成运行崩溃。（其实编译器完全可以自己针对这些自动分配的内存提供正确的操作，不知道为什么不这样做，非得让程序员自己写）
#include<iostream>
using namespace std;
const int N=1000000;
class leiA
{
private:
	char *pc;
public:
	leiA(){pc=new char[N];}
	~leiA(){cout<<"~leiA() called with pc="<<(void*)pc<<endl;delete []pc;}
	leiA(const leiA&)=default;
	leiA(leiA&&)=default;//不能声明为const ，因为要改变其中的指针值为nullptr;
	leiA&operator=(const leiA&)=default;
	leiA&operator=(leiA&&)=default;//不能声明为const ，因为要改变其中的指针值为nullptr;
};
int main()
{
	int i=1;
	//while(i++)
	{
		leiA a;
		//leiA b(a);//有动态分配的内存时不能使用默认的复制构造函数和默认的复制赋值函数。
		//如果写成leiA c=a则相当于leiA c(a)
		//leiA c;c=a;

		//leiA d(leiA());//使用了默认的转移构造函数和转移赋值函数。程序运行崩溃，因为相当于重复删除同一段内存。
		//leiA e;e=leiA();
	}
	return 0;
}