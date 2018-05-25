#include<iostream>
#include<io.h>
#include<string>
#include<locale>
using namespace std;
int main()
{ 
	{
		locale loc("chs");//用vs2005编译没问题，使用gcc编译出错
		//wcout.imbue(loc);
	}

	{
		//wstring a=wstring(L"马年到");
	}
	return 0;
}