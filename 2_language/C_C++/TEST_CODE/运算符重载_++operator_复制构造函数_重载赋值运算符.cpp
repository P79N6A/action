#include<iostream>
#include<ctime>
//1一旦类对象中有动态分配的数据，如果使用++ -- 复制构造lei b(a)等一定要重新实现复制构造
//函数，默认的复制构造函数不行，如果使用=号（不是lei b=a的情况）时，一定要重载赋值
//运算符
//In that case, the user defines a prefix function(++ --等前置) that works
//by incrementing a value and then returning it. But the postfix version(后置) works by first stashing
//a copy of the value, incrementing the value, and then returning the stashed copy.Thus,
//for classes, the prefix version is a bit more efficient than the postfix version.
//经过测试前置与后置相差非常小，且多数情况下，后置运算时间短于前置，与书中不一致
using namespace std;
class lei
{
public:
	lei()
	{
		num=0;
		//p=new int(100);//旧版本编译器不支持这种写法
		p=new int;*p=100;
	}
	lei(lei&alei)//复制构造函数参数必须为引用类型
	{
		num=alei.num;
		p=new int;*p=*alei.p;
		cout<<"调用了复制构造函数"<<endl;
	}
	lei& operator=(lei&alei)//重载赋值运算符返回类型必须是引用。如果输入类型不是引用，必须定义
//复制构造函数，否则无法拷贝，如果输出类型不是引用，那么在诸如a=b=c的式子中，会产生大量的拷贝，效率很低。
	{
		if(this==&alei)return *this;//如果自己对自己赋值，直接返回
		num=alei.num;
		*p=*alei.p;//如果两个类对象中动态成员部分的长度不同，即指针所指内存块的大小不同
		//则需要删除之前的内存，并重新分配可以容纳右值指针所指内容的空间
		cout<<"调用了=函数"<<endl;
		return *this;
	}
	~lei(){delete p;}
	lei operator++(){num++;return *this;}//前置自加运算符(指把运算符前置)
	lei operator++(int){num++;return *this;}//后置自加运算符
	int num;
	int *p;
};
int main()
{
	lei a;
	cout<<*a.p<<endl;
	//cout<<*(a++).p<<endl;
	//默认的复制构造函数只是执行了简单的值的传递
	//如果没有实现自己的复制构造函数，执行a++会使得a的自加运算得到的
	//新值中的指针指向一个已经销毁的地址。即相当于a=a+1，a.p指向了已经被销毁的a+1的右
	//值的地址，编译没有问题，运行会崩溃
	//cout<<*(++a).p<<endl;
	//cout<<a.p<<":"<<*a.p<<endl;
	cout<<"-------------------------------------------------------------"<<endl;
	*a.p=13;
	//lei b=a;//如果以这种方式定义b，调用的不是重载的赋值运算符，而是复制构造函数
	//原则上讲，赋值运算符与复制构造函数实现的是相同的功能，但是两者相互分离
	//谁也不能替代谁，如果对类对象用"="就必须用到重载赋值运算符，如果要用到++,--等运算
	//符或者在定义的同时如lei b=a，或者是lei b(a)情况下就一定要用到复制构造函数。
	//如果没有定义复制构造函数或者是重载赋值运算符而导致运行崩溃，往往是因为，一个
	//类在调用析构函数释放完内存后，另一个类对象再调用析构函数，试图去释放一个已经
	//被释放内存的空间而导致的，因为之前两个类对象的指针指向的是同一块内存
	lei b;
	b=a;//如果没有重载赋值运算符，这种写法运行崩溃
	cout<<*b.p<<endl;
	lei c(a);//如果没有重载复制构造函数，这种写法运行崩溃
	cout<<"c"<<*c.p<<endl;
	{
		cout<<"测试前置和后置的运算速度============================="<<endl;
		clock_t s;
		int temp=0;
		s=clock();
		for(int i=0;i<1e8;i++)
		{
			temp++;
			if(temp>10000000)
				temp*=-1;
		}
		cout<<"temp++耗时："<<double(clock()-s)/CLOCKS_PER_SEC<<endl;
		
		s=clock();
		for(int i=0;i<1e8;i++)
		{
			++temp;
			if(temp>10000000)
				temp*=-1;
		}
		cout<<"++temp耗时："<<double(clock()-s)/CLOCKS_PER_SEC<<endl;

		s=clock();
		for(int i=0;i<1e8;i++)
		{
			temp=temp+1;
			if(temp>10000000)
				temp*=-1;
		}
		cout<<"temp=temp+1耗时："<<double(clock()-s)/CLOCKS_PER_SEC<<endl;

		s=clock();
		for(int i=0;i<1e8;i++)
		{
			temp+=1;
			if(temp>10000000)
				temp*=-1;
		}
		cout<<"temp+=1耗时："<<double(clock()-s)/CLOCKS_PER_SEC<<endl;
	}

	return 0;
}