//1如果一个类重载了operator[]，那么对于单纯的符号m[i]，即可以表示一个类中的某元素，也可以表示
//一个类数组中的某个类，这取决于m类义的是一个类数组还是一个类对象。
#include<iostream>
using namespace std;
class lei
{
public:
	int&operator[](int i){return a[i];}
	int a[10];
};
int main()
{
	{
		lei m[5];
		//m[3]//单纯地看这个符号，既能代表一个类，又能代表类中数组一个元素
		m[3][3]=3;//对第4个类对象的第4个数进行赋值
		cout<<m[3][3]<<endl;
	}
	return 0;
}