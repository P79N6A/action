///1如果某个函数参数类型有const修饰符，那么在该函数中只能使用类中的const函数和数据成员（数据成员const非const均可）
//因为const函数一定可以保证不改变类中的值，数据成员只要不作为左值就可以保证不改变，所以加不加const均可
#include<iostream>
using namespace std;
class lei
{
public:
	lei(){x=100;}
	int get_x()const{return x;}//如果不加const编译出错：error C2662: “lei::get_x”: 不能将“this”指针从“const lei”转换为“lei &”
//1>        转换丢失限定符
	int x;
};
void fun(const lei& cl)
{
	cout<<"cl.get_x():"<<cl.get_x()<<endl;
	cout<<"cl.x:"<<cl.x<<endl;
}
int main()
{
	{
		lei a;
		fun(a);
	}
	return 0;
}