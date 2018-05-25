#include<iostream>
//#include<thread>
using namespace std;
class A
{
public:
	A(int a=0){
	num=a;
	}
	friend A operator+(A&,A&);
	//friend A operator+(A,A);
	int getNum(){return num;}
private:
	int num;
};
A operator+(A& a1,A& a2)
{
	return A(a1.getNum()+a2.getNum());
}
//A operator+(A a1,A a2)
//{
//	return A(a1.getNum()+a2.getNum());
//}
int main()
{
	A a,b(3),c(4);
	a=b+c;///可以正常相加
	cout<<a.getNum()<<endl;
	//a=a+A(100);//编译出错没有match
	//a=std::ref(a)+std::ref(A(100));//编译出错
	//a=a+100;编译出错无match
	cout<<a.getNum()<<endl;
	return 0;
}