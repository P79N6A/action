//1对ostream重载<<运算符时，<<是双目运算符，如果由定义为普通函数再声明为友元函数的方式来重载且
//第一个参数为ostream对象，编译器会使得
//第一个参数在<<使用时的左边，而第二个参数在使用时的右边，且<<运算符是从左向右结合。如果定义为成员函数或者
//普通函数，且普通函数时，第一个参数为类，第二个参数为ostream类，那么这两种方式等价，函数在使用时
//类对象在左边，ostream对象在右边。因为第一个参数总是在左边，第二个参数总是在右边
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//2cout<<endl;和cout<<flush;会清空缓冲区buffer，为下次输出做准备
//3cout.put(),cout.write()分别对于输出字符和字符串很方便
//4cout<<hex或hex(cout);cout<<dec;或dec(cout);cout<<oct;oct(cout)控制输出进制。
//5 int cout.width(int);设置当前width();返回之前的width;int cout.width(void)返回当前width。它只对下一输出流有作用，之后width就会自动恢复到之前值。
//6 cout.fill(char)填充没去占用的width宽度，默认是使用空格填充
//7 八进制和十六进制数没有负数，如果输出负数也是按unsigned处理（输入的仍然是一个负值，只是把这个负数的补码按unsigned来作后续处理）
//8 cin和cout维持两个不同的缓冲区
//9 cout<<;cout.write;cout.width;cout.fill;cout.precision;cout.setf;cou.unsetf;basefield,floatfield,adjustfield;
//iomanip:setw;setprecision;setfill;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//1使用cout<<setw(n);后只在当前block有效果，程序运行出block后恢复原设置。而cout.width();只对下句有效
//常见写法有
//cout<<1<<' '<<setw(2)<<setfill('0')<<2<<' '<<3<<' '<<endl;;其中setw,setfill只对下一个输出有影响；
#include<iostream>
#include<string>
using namespace std;
class lei
{
public:
	lei(){aim="million";age=25;}
	friend ostream& operator<<(ostream&output,lei&);
    //friend ostream& operator<<(lei&le,ostream&output);//与定义为成员函数的方式等价
	ostream& operator<<(ostream&output)
	{
		output<<"重载为成员函数:"<<endl;
		return output;
	}
private:
	string aim;
	int age;
};
ostream& operator<<(ostream&output,lei& le)
{
	output<<"when I was "<<le.age<<" , I will have "<<le.aim<<endl;
	return output;
}
//ostream& operator<<(lei&le,ostream&output)
//{
//	output<<"Just calm think study and practice before "<<le.age<<endl;
//	return output;
//}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	cout<<"----------------------------------------------------------------------------"<<endl;
	{
		cout<<"=>关于cout的运算符重载"<<endl;
		//cout<<'aa'<<endl;//用单引号括起多字个字符，编译不出错，可以输出，但输出字符错误
		lei a;lei b;
		b<<(a<<cout);//第二种重载方式的输出方法，因为运算符是从左向右结合，所以要括起来
		cout<<a<<endl;
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout<<"----------------------------------------------------------------------------"<<endl;
	{
		cout<<"=>cout基本用法"<<endl;
		cout<<hex;
		int a=-0xa0;int b=0xa0;
		cout<<a<<'\t'<<b<<endl;//十六进制和八进制编译器会始终认为是unsigned
		
	}
	return 0;
}