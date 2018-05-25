//1不能为一个非右值定义右值引用。常规引用通常指左值引用。
//2右值引用主要解决为进一步使用临时变量的值，而多此一举的数据拷贝问题
//3不能为左值定义右值引用（包括非const右值引用和const右值引用），不能为右值
//定义非const左值引用，但可以定义const左值引用。
//4编译器在这两种情况下创建临时变量：一，调用某个与常数参数不匹配，但是进行
//类型转换后就可以调用时，二，对于表达式，会产生临时变量
//参见《C++ Primer Plus》P392
//5使用引用作为函数参数时，函数在实际调用时，实参一定要使用左值，如果使用右值
//则一定要把函数形参用const修饰
//参见《C++ Primer Plus》P391 P392 P393(该页阐述了原由)
//6形参为非const左值时，实参只能为左值，形参为const左值时，实参可以是左值，也可以是右值，形参为右值时，实参必须为右值。如果同时存在右值形参和const左值形参，实参为右值时，优先选择右值形参的函数。
/////////////////////////////////////////////////////////////////////////////////////////////////
//C++11标准进行右值引用测试，因为右值引用是在新标准中才引用的
#include<iostream>
#include<vector>
#include<string>
#define BLOCK 2
using namespace std;
int & fa(int &a)
{
	a++;
	return a;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
int &&fb(int &a)
{
	return a+1000;//返回的是一个临时变量
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void fun1(int&&a)
{
	cout<<"fun1 called with:"<<a<<endl;
}
//-------------------------------------------------------------------------------

double up(double x) { return 2.0* x;}
void r1(const double &rx) {cout << rx << endl;}
void r2(double &rx) {cout << rx << endl;}
void r3(double &&rx) {cout << rx << endl;}
void r(const double &rx) {cout<<"r(const double &rx) called with:"<< rx << endl;}
void r(double &rx) {cout << "r(double &rx) called with:"<< rx << endl;}
void r(double &&rx) {cout <<"r(double &&rx) called with:"<<  rx << endl;}
int main()
{
#if BLOCK==1
	int & fa(int &a);
	cout<<endl<<"使用vs编译器，C++98标准进行测试=============================================================="<<endl;
	int aa=1;
	cout<<fa(aa)<<endl;
	cout<<endl;

	fa(aa)=100;//以fa(aa)当左值没有问题
	cout<<aa<<endl;
	cout<<endl;

#endif

	cout<<"---------------------------------------------------------------------"<<endl;
	{
		cout<<"=>右值定义"<<endl;
		int aa=1;
		int && ab=aa+9;//右值引用直接将临时变量地址作为正式地址，避免了拷贝，提高性能
		//如果定义 int ab=aa+9;则aa+9完得到的临时变量要再次被拷贝到新分配的内存&ab处，且销毁临时变量地址
		cout<<aa<<endl;
		cout<<ab<<endl;
	}
	cout<<"---------------------------------------------------------------------"<<endl;
	{
		cout<<"=>函数返回右值引用"<<endl;
		int aa=1;
		int&& ac=fb(aa);
		cout<<fb(aa)<<endl;//输出1001
		cout<<ac<<endl;//输出乱值，虽然返回的是临时变量的引用，但是由于函数是栈操作
		//所以临时变量仍然是要销毁的
	   //右值引用仅仅是改善数据拷贝的中间环节，但是最好不要把临时变量的引用当正式变量用
	}
	cout<<"---------------------------------------------------------------------"<<endl;
	{
		cout<<"=>不能为一个左值定义右值引用，即拿右值引用被声明为const也不可以"<<endl;
		double a=2.71828;
		//int &&b=a;//不能为一个左值定义右值引用。常规引用通常指左值引用。
		//const double &&c=a;//不能为左值定义const右值引用
		double &&b=a*1;cout<<b<<endl;
		//const double&&c=a;cout<<c<<endl;
		
	}
	cout<<"---------------------------------------------------------------------"<<endl;
	{
		cout<<"=>不能为一个右值定义左值引用，左值引用用const修饰可以接收一个右值"<<endl;
		double a=2.71828;
		//double &b=a+10;//不能为一个右值定义左值引用
		const int &temp=a+100;//可以为一个右值定义const左值引用
		//cout<<"temp:"<<(temp=temp+100)<<endl;//错误：temp被const修饰不能，只能读，不能改变值
		//cout<<b<<endl;
	}
	
	cout<<"---------------------------------------------------------------------"<<endl;
	{
		cout<<"=>使用const左值引用接受一个右值"<<endl;
		double&fc(double&);
		const double&fd(const double&);
		int a=10;
		//double &b=fc(a);//a是一个int型，无法自动类型转换为double&（可以自动类型转换为double）
		//double &b=fc(double(a));//编译出错：编译器不能接收一个非const右值引用
		const double &b=fd(double(a));//这里double(a)对进行转化后，得到的是一个临时变量，这个临时
		//变量作为引用被传到了函数，后被传递回来，又以b为别名，这种为一个临时变量起别
		//名只能是const类型，返回临时变量的左值引用只能为const
		//Using a const reference allows the function to generate and use a temporary variable appropriately.。参见《C++ Primer Plus》P394
		cout<<"b:"<<b<<endl;
		const double &c=fd(a);//运行正常，只要对临时变量使用const修饰的引用就没有问题
		//当然既不是const，也没有引用的普通形参肯定没有问题，这里讨论加不加const只是在
		//一定要使用引用的前提下
		cout<<"c:"<<c<<endl;
		const string&fe(const string&str);
		string as=fe("happy");//函数参数引用类型用const修饰时，才可以以一个右值为参数。
		cout<<as<<endl;
		//string a=ff("new");//非const引用要出错
	}
	cout<<"---------------------------------------------------------------------"<<endl;
	{
		cout<<"=>右值引用可以接受使用函数move(左值)处理过的左值"<<endl;
		cout<<"=>move(左值)是确确实实是为一个左值定义了一个右值引用，这个右值引用和这个左值有相同的地址，能过右值引用可以改变原左值"<<endl;
		cout<<"=>move()不会创建新的变量对象，即不开辟新的内存单元";
		int a=123;
		fun1(std::move(a));//如果要对右值引用为形参的函数代入左值，必须对左值使用move()
		fun1(a+10);//编译器能自动判断出来是右值，从而调用以右值引用为形参的函数
		fun1(std::move(a+10));//可以对右值使用move(右值)
		cout<<endl;
		int&&b=std::move(a);
		cout<<"a的地址："<<&a<<"\tb的地址："<<&b<<endl;
		b=100;
		cout<<"a:"<<a<<"\tb:"<<b<<endl;
		class lei{public:lei(){ cout<<"the new object adress is:"<<this<<endl;}~lei(){cout<<"~lei() called with:"<<this<<endl;}};
		lei cla;
		lei&&clb=std::move(cla);//cla与clb对应的就是同一个object
		cout<<"cla地址："<<&cla<<"\tclb地址："<<&clb<<endl;
		lei&&clc=lei();
		cout<<"clc地址："<<&clc<<endl;
	}
	cout<<"---------------------------------------------------------------------"<<endl;
	{
		cout<<"=>可以定义指向左值引用右值引用的指针"<<endl;
		double &&pi=3.14;
		double *p=&pi;cout<<*p<<endl;
		double e=2.7;
		double&ey=e;
		double*pey=&ey;cout<<*pey<<endl;
	}
	cout<<"---------------------------------------------------------------------"<<endl;
	{
		cout<<"=>实参为右值时，同时存在形参为const左值和形参为右值的函数，优先调用以右值为形参的函数"<<endl;
		double w = 10.0;
		r1(w);//函数r1的实参左值和右值都可以
		r1(w+1);
		r1(up(w));
		cout<<endl;
		//1210 Chapter 18 Visiting with the New C++ Standard
		r2(w);//函数r2的实参只能是左值
		//r2(w+1);
		//r2(up(w));
		cout<<endl;
		//r3(w);//函数r3的实参只能为左值
		r3(w+1);
		r3(up(w));
		cout<<endl;
		r(w);
		r(w+1);
		r(up(w));
	}
	return 0;
}
double &fc(double &dou)
{
	return dou;
}
const double&fd(const double&dou)
{
	return dou;
}
const string&fe(const string&str)
{
	return str;
}
string &ff(string&str)
{
	return str;
}