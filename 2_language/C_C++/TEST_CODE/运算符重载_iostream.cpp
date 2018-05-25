//1对类重载输入流时，一定不能将istream类用const修饰，因为istream类对象数据成员要使用cin赋值，如果声明为const，
//程序会崩溃
///2fstream类派生于iostream类，可以用iostream类声明一个一个函数的形参，用同样的函数将文本分别输出到窗口
//或者文件
#include<iostream>
#include<fstream>
using namespace std;
int main()
{
	return 0;
}