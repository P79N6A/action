//测试转义字符(esape sequences)效果
#include<iostream>
int main(void)
{
	using std::cin;
	using std::cout;
	using std::endl;
	cout<<"\???\n";//\n与endl等价
	for(int i=0;i<3;i++)
	{
		cout<<'\a'<<endl;//这个转义字符会发出声音，有些系统可能不会发出声音
	}
	return 0;
}