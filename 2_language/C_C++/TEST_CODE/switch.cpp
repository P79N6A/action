//1switch后面的表达式必须是整型（包括字符型）
//2switch中不能有定义部分，如果有必须用{}括起来或者在switch外面定义里面用，否则编译出错
#include<iostream>
using namespace std;
int main()
{
	double a=3.14,c;
    c=a;
	//switch(c)//
	//{
	//case 3.14:cout<<"can use double"<<endl;break;
	//}
	{
		char d='a';
		switch(d)
		{
		case 'a':
			{
				int b=3;
				cout<<b<<endl;
				break;
			}
		case 'b':
			//int m=100;//error C2361:“default”标签跳过“m”的初始化操作
			//在switch 语句中不可以有定义部分。
            //如果有定义，如初始化的时候，必须把定义部分需要用{ } 括起来。像case 'a':那样

			break;
		default:
			break;
		}
	}
	return 0;
}