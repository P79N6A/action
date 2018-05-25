#include<iostream>
#include<cmath>
using namespace std;
int main()
{
	{
		cout<<atan(1.0)<<endl;//atan只能计算一四象限
		cout<<atan2f(1,-1)<<endl;//atan2f算出的角度分布在四个象限
		
		cout<<atan2f(0.0,0.0)<<endl;//都为0时输出0
		cout<<atan2f(1,0)<<endl;//计算出的是pi/2
	}
	return 0;
}