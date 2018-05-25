#include<iostream>
using namespace std;
class A
{
public:
	A(){a=100;}
	int a;
};
int main()
{
	A A;
	cout<<A.a<<endl;
	return 0;
}