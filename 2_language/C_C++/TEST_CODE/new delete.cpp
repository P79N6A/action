//1不能使用一个delete一次删除多个变量
//2释放一段已经被释放过的内存会使程序处于死循环中Another disturbing symptom is that attempting to delete the same memory twice can
//cause the program to abort. Microsoft Visual C++ 2010 (debug mode), for example, displays
//an error message window saying “Debug Assertion Failed!”, and g++ 4.4.1 on Linux
//reports “double free or corruption” and aborts. Other systems might provide different
//messages or even no message, but the same evil lurks within the programs
//3对于一个动态分配数组的内存，对于非数组使用delete[]会导致运行崩溃，对于数组使用delete和delete[]运行均不崩溃，
//估计不会把内存
//都释放。不管怎样，规范操作，对数组使用delete[]，对于非数组使用delete;
//4如果使用int *p=new(buffer) lei;，使用replacement new（即使用new在已分配的内存buffer上分配空间）在buffer
//上分配的类对象，必须显式地调用析构函数，否则如果类中有动态分配的内存，会造成内存泄露，因为类对象
//动态分配的内存可能不在buffer中，而在其它地方
#include<iostream>
using namespace std;
class leiA
{
public:
	leiA(){a=new double(3.14);}
	~leiA(){delete a;}
	double *a;
};
int main()
{
	{
		char buffer1[512];//在本地栈上分配512B的内存，不需要用delete释放
		char *buffer2=new char[1024];//动态分配1024B的内存
		leiA *a=new(buffer1) leiA;
		leiA *b=new(buffer2) leiA;
		b->~leiA();//如果leiA中存在动态分配的内存，在delete b之间一定要显式调用~leiA()，因为a在buffer2上，
		//而*a不在buffer2上
		delete b;
		a->~leiA();
		delete []buffer2;
	}
	cout<<"----------------------------------------------------"<<endl;
	{
		int *a=new int;
		int *b=new int;
		*a=1;*b=2;
		cout<<*a<<"            "<<*b<<endl;
		delete a,b;//编译不会出错，实际运行结果，a指向其它数，而b仍指向原值，表明b没有成功删除
		cout<<*a<<"            "<<*b<<endl;
	}
	cout<<"----------------------------------------------------"<<endl;
	{
		int *a=new int(1);
		int *b;
		b=a;
		delete a;
		//delete b;删除已经被删除过的地址，会使程序死循环
	}
	cout<<"----------------------------------------------------"<<endl;
	{
		int a[2]={1,2};
		a[5]=4;
		cout<<a[5]<<endl;//可以越界赋值输出，但可能会影响系统或其它程序在内存中的数据
		int*b=new int[2];
		b[5]=100;
		cout<<b[5]<<endl;
		delete []b;
	}
	cout<<"----------------------------------------------------"<<endl;
	{
		int *a=new int[2];
		//delete a;//使用它会使程序崩溃
		delete[]a;
	}
	cout<<"------------------------------------------------------";
	{
		char words[15] = "bad idea";
		char * p1= words;
		char * p2 = new char;
		char * p3;
		char *p4=new char[4];
		//delete [] p1; // undefined, so don't do it，因为p1所指向的内存不是动态分配的，程序运行会崩溃
		//delete [] p2; // undefined, so don't do it，p2指向的不是数组，不起作用，程序运行崩溃，
		//如果是char*p2=new char[1];起作用
		delete p2;
		//delete [] p3; // undefined, so don't do it，指向一个无意义的地址，可能地址受保护，程序运行崩溃
		char*p5=&p4[1];
		delete p4;//程序运行不崩溃
		//delete p5;//程序运行崩溃
		//delete []p4;
		int *a=new int[5];
		delete a;
	}
	return 0;
}