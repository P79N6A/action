//1重载运算符()可以把类对象以函数形式来使用，它有函数的特点，当然也可以直接调用
//它包含的诸多成员函数
//2重载其它运算符可以把类对象当成一个普通变量来使用，当然也可以通过.或->来调用成员
//变量
//3如果一个重载的运算符需要对多个类都能使用，则使用友元或普通函数重载运算符，可以避免在
//每个类中分别定义，相当于每个类均用到这个功能，如果很多类都要借用到某个类中的
//某个功能（函数），可以将这个类成员函数声明为它们的友元（私有成员会暴露），或者
//是定义一个接口来接收该成员函数的指针来进行特定的操作（某些数据不可以访问，带
//监督）
//4operator()只能重载为类的成员函数，而不能重载为普通函数，而+-等是可以重载为普通函数
//5基类的重载运算符可以被派生类继承，为了不引起岐义和限定使用重载运算符的类型范围
//使用继承或直接类内重载，而不是友元函数或普通函数重载运算符更安全些
//6重载运算符的功能可以定义一个函数代替，但重载之后看上去更加简洁，可以重载地像
//普通变量一样运算，其本质仍是一个函数
#include<iostream>
#include<string>
using namespace std;
class jilei
{
public:
	//virtual string operator()(string s){return getString()+s;}//基类的重载运算符可以被继承
	string operator()(string s){return getString()+s;}//在派生类中调用的getString是派生类自己的getString()
	virtual string getString(){return string();};
	void display(){cout<<"非virtual函数可以被派生类调用"<<endl;}
};
class leiA:public jilei
{
public:
	leiA(){a="leiA";}
	string getString(){return a;}
private:
	string a;
};
class leiB
{
public:
	leiB(){b="leiB";}
	string getString(){return b;}
	string getC(){return clC.getString();}
private:
	leiA clA;//类中使用已经在外部定义的类
	class leiC//为中定义类
	{
	public:
		leiC(){c="leiC";}
		string getString(){return c;}
	private:
		string c;
	};
	leiC clC;
	string b;
};
//string operator()(string any)//编译出错，说重载operator()必须是非静态成员，因为在main函数外定义
////的，它相当于把operator()声明为普通函数是不可以的，也可以理解，因为普通函数的调用
////是fun()，不能重载()后变成()()，这可能要出问题，可能会大大增加编译器复杂度
////这时，把重载的operator()函数写到一个基类中再利用C++多年继承将该类加到父类后面
//{
//	//return any+getString();
//}
//template<class T=leiA>
//ostream& operator<<(ostream&output,T& te)
////1把类对象当成一个普通字符一样输出类中一些内容
////2不用声明为友元函数，编译器会自动寻找类型匹配的对象
////3上面既然使用的模板，也就是<<可以接受任何类型的参数，但是<<同样已被cout重载
////在编译时应该会引起混淆，但是多数情况下，没有混淆，如main()中，cout可以输出endl
////数，但是在cout<<"----------……时出现了问题，编译错误是ambiguous，在不使用模板或显式
////使用string把它包进来时，它才可以正常输出，一个比较理想的情况是，T可以限定使用
////的类型，但是在C++模板技术中我还没有见到这样的技术（在不使用模板时，
////一个好的做法是把基类声明
////为形参类型，这样所有基类对象和派生类对象都会被重载，如果要使不同基类的某些派生类
////对象具有某个重载性质，可以定义一个基类，类似接口，利用多重继承，派生到这些派生
////类中，然后将重载运算符函数（可以声明基类的成员函数，也可以定义为普通函数），
////的形参类型定义为该基类对象，这样就完成了重载运算符的类型限定，当然如果需要重载
////的类不多，也可以直接对他们重载，每个类型都写一个函数
////
//{
//	output<<te.getString();
//	return output;
//}
ostream& operator<<(ostream&output,jilei& cl)//
//在这里参数定义为基类，那么由基类或者其派生类对象都会被重载，调用它们各自的函数
//(因为定义的是虚函数)
{
	output<<cl.getString();
	return output;
}
int main()
{
	leiA a;
	//cout<<a<<endl;
	jilei b;
	{//培养用{}划分模块的习惯
		cout<<b.operator()("success")<<endl;
		cout<<a.operator ()(":success")<<endl;
		cout<<b("当函数使用")<<endl;
		cout<<a("当函数使用")<<endl;
	}
	a.display();
	cout<<2.718281828459<<endl;
	cout<<"---------------------------------------------"<<endl;
	//cout<<string("---------------------------------------------")<<endl;
	leiB c;
	cout<<c.getC()<<endl;
	return 0;
}


