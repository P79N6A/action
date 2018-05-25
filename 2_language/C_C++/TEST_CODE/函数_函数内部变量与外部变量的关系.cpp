#include<iostream>
#include<string>
using namespace std;
class lei
{
private:
	string str;
public:
	lei(string s="default string"):str(s){cout<<"constructor lei(string) called"<<endl;}
	lei(const lei&l){str=l.str;cout<<"copy constructor lei(const lei&) called"<<endl;}
	~lei(){cout<<"destructor ~lei() called"<<endl;}
	friend lei operator+(const lei&,const lei&);
	lei& operator=(const lei&l)
	{
		cout<<"operator= called"<<endl;
		str=l.str;
		return*this;
	}
};
lei operator+(const lei&a,const lei&b)
{
	return lei(a.str+b.str);
}
int main()
{
	{
		lei a("successfull"),b("man");
		lei c=a+b;
		//相当于lei c(temp);,operator+函数中创建的临时变量temp会直接当作复制构造函数的参数来构造lei c。
	}
	cout<<"--------------------------------------------------"<<endl;
	{
		lei a("thinking"),b("plan");
		lei c;
		c=a+b;
		//operator+函数中创建的临时变量temp1会通过copy constructor赋给一个新的临时变量temp2(temp1)，接着有c=temp2;这个temp2又通过赋值运算符函数对已经定义的lei c赋值。
	}
	return 0;
}