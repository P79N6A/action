//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//C++11标准：
//1如果一个类定义了多个构造函数，在定义一个对象时，这些构造函数之间可以相互调用，如下面的leiA。

class leiA
{
public:
	leiA():leiA(1,2){}//调用第三个构造函数
	leiA(int i):leiA(i,3.14){}//调用第三个构造函数
	leiA(double=2.7){}//指定参数默认值
	leiA(int i,double d,char c='A'){}//部分指定参数默认值
};
class leiB:public leiA
{
public:
	//using leiA::leiA;//可以直接使用基类的构造函数
	//该句在自己配置的MinGW 4.7.2的gcc中编译尚不通过，包括下面的书中示例程序也不通过
};
int main()
{
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//下面是书《Primer Plus C++》6thP1182的程序
//class BS
//{
//int q;
//double w;
//public:
//BS() : q(0), w(0) {}
//BS(int k) : q(k), w(100) {}
//BS(double x) : q(-1), w(x) {}
//BS(int k, double x) : q(k), w(x) {}
//void Show() const {std::cout << q <<", " << w << '\n';}
//};
//class DR : public BS
//{
//short j;
//public:
//using BS::BS;
//DR() : j(-100) {} // DR needs its own default constructor
//DR(double x) : BS(2*x), j(int(x)) {}
//DR(int i) : j(-2), BS(i, 0.5* i) {}
//void Show() const {std::cout << j << ", "; BS::Show();}
//};
//int main()
////New Class Features 1183
//{
//DR o1; // use DR()
//DR o2(18.81); // use DR(double) instead of BS(double)
//DR o3(10, 1.8); // use BS(int, double)
//}


