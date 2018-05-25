//1goto可以实现函数内跳跃，而try catch除可以抛出异常外可以实现函数间的跳跃
//只能由try block内某个函数的某个位置或某个位置跳跃到能捕获到的catch
//2try catch是一种组合，和do while一样，不能分开，这两个block中间不能有其它语句
//一个try后面可以连续跟多个catch block
//3每一个catch只能捕获由它前面try或前面try所包含的try发出的throw，不能捕获其
//它地方try发出的throw
//4如果throw这样用throw;后面没有任何参数，那么它会rethrow上次的内容
//5如果catch的参数类型为引用，最后得到的变量也不是原值，而是一个
//copy值，原值会在其作用域完后被销毁。
//6将catch参数设置成类引用有如下好处：references have another important property:
//A base-class reference can also refer to derived-class objects. Suppose you have a
//collection of exception types that are related by inheritance. In that case, the exception
//specification need only list a reference to the base type, and that would also match any
//derived objects thrown.
//Suppose you have a class hierarchy of exceptions and you want to handle the different
//types separately. A base-class reference can catch all objects of a family, but a derived-class
//object can only catch that object and objects of classes derived from that class.A thrown
//object is caught by the first catch block that matches it.This suggests arranging the
//catch blocks in inverse order of derivation。《C++PrimerPlus》6th P915
//7catch(...)使用省略号ellipsis可以使用catch捕获所有前面try的任何异常类型
#include<iostream>
using namespace std;
void fun1()
{
	try{cout<<"throw in fun1 of try"<<endl;throw 1;}
	catch(double d){cout<<"catch called in fun1"<<endl;}
}
void fun2()
{
	try{cout<<"throw in fun2 of try"<<endl;throw 1.1;}
	catch(int i){cout<<"catch called in fun2"<<endl;}
}
int main()
{
	try
	{
		{//block zero
			fun1();
			fun2();//不能捕获fun1中抛出的异常
			try{cout<<"throw block zero of try"<<endl;throw 1.1;}
			catch(int i){cout<<"catch called in block zero"<<endl;}//不能捕获fun1中抛出的异常
		}
	}
	catch(int i)
	{cout<<"catch called in main block"<<endl;}
	{//block one
		try{
			cout<<"try called in block one"<<endl;
			throw 1;
		}
		catch(int i){
			cout<<"catch called in block one after try"<<endl;
		}
		{//block two
			//catch(int i){//error C2318: 没有与该 catch 处理程序关联的 Try 块
			//	cout<<"catch called in block two";
			//}
		}
		//catch(int i){//error C2318: 没有与该 catch 处理程序关联的 Try 块
		//	cout<<"catch called in block one";
		//}
	}
	//catch(int i){//error C2318: 没有与该 catch 处理程序关联的 Try 块
	//	cout<<"catch called in main block";
	//}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
};