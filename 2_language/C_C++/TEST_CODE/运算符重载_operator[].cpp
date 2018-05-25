//1运算符[]，()，=，->只能重载为非静态成员
#include<iostream>
#include<string>
using namespace std;
class leiA
{
public:
	leiA(string s){str=s;}
	char& operator[](int i)//重载为成员函数
	{
		return str[i];
	}
	string str;
};
class leiB
{
public:
	leiB(string s){str=s;}
	string str;
};
//char& operator[](const leiB&cl,int i)//编译出错：error C2801: “operator []”必须是非静态成员
//{
//	return cl.str[i];
//}
int main()
{
	{
		leiA a("zhongyong");
		cout<<a[1];
	}
	return 0;
}