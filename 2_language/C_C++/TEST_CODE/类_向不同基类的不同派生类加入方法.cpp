//1测试将某个函数应用于由不同基类派生的不同类的几种方法
//一：在基类中定义方法：将该方法定义在一个类中并作为基类，添加到需要方法的派生类的继承类中
//二：在普通函数中定义方法并调用需要类：定义一个普通函数，该普通函数的参数类型是定义方法的这个基类
//三：直接在需要方法的类中定义方法函数
//四：定义一个普通函数，并将其作为需求类（需要方法的类）的友元函数
//理念：方法自已定义（成员函数）；普通函数定义（调用友元函数，被普通函数调用）；
//基类继承；
#include<iostream>
#include<string>
using namespace std;
class newWay
{
public:
	newWay(){name="newWay";}
	virtual void show1(){cout<<name<<endl;}
	virtual void show2(){cout<<getName()<<endl;}
	virtual string getName(){return name;}
	void show3(){cout<<name<<endl;}
	string  name;

	virtual void change(){this->name="chage";}
};
class A
{
public:
	A(){name="class A";}
	string name;
};
class AA:public A
{
public:
	AA(){name="class AA";}
	virtual string getName(){return name;}
	string name;
};
class AB:public A,public newWay
{
public:
	AB(){name="class AB";}
	virtual string getName(){return name;}
	//必须实现getName函数，如果只是在基类中定义getName那么派生类调用返回的仍是基类的name
	string name;
};
class B
{
public:
	B(){name="class B";}
	string name;
};
class BA:public B
{
public:
	BA(){name="class BA";}
	virtual string getName(){return name;}
	string name;
};
class BB:public A,public newWay
{
public:
	BB(){name="class BB";}
	virtual string getName(){return name;}
	string name;
};
void display(newWay&nw)
{
	cout<<"display called:"<<nw.getName()<<endl;
}
int main()
{
	AA aa;
	//aa.show();//报错：没有这个函数
	AB ab;newWay *p=&ab;
	ab.show1();//输出的是基类的字符串"newWay"
	ab.show2();//输出的是派生类的字符串
	ab.show3();//输出的是基类的字符串"newWay"
	p->show1();
	p->show2();
	p->show3();
	//BB bb;
	//bb.show1();
	//bb.show2();
	cout<<"-------------------------------------------"<<endl;
	BB bb;BA ba;
	//display(aa);//不能调用，原因是它没有定义该接口
	display(ab);
	//display(ba);//不能调用，原因是它没有定义该接口
	display(bb);
	cout<<"////////////////////////////////////////////"<<endl;
	{
		BB a;
	    cout<<a.getName()<<endl;
		a.change();
		cout<<a.getName()<<'\t'<<a.newWay::name<<'\t'<<a.name<<endl;
		newWay*p=&a;
		p->change();
		cout<<a.getName()<<endl;
	}
}