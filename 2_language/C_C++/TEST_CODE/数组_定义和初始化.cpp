//1数组使用空的初始化列表，可以使得数组每个元素被初始化为0，（无论是动态分配，还是非动态分配的数组都是如此）
//2如果在{}中加入初始化数，其中包含几个数，数组就会对前几个数初始化这几个数，如果非动态数组，其它数会被初始化为0，而对于动态分配的数组，其它数仍为乱码
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//C++11编译
#include<iostream>
using namespace std;
template<typename T>
void arrayShow(const T*const p,const int N=0)
{
	cout<<endl;
	for(int i=0;i<N;i++)
		cout<<p[i]<<'\t';
}
int main()
{
	{
		int a[4]={1,2,3,4};//四个数据依次被赋值
		arrayShow(&a[0],4);//等同于下面
		//arrayShow(a,4);
		int b[4]={4};//只是第一个数被初始化为4，其它的都是0
		arrayShow(b,4);
		int c[4]={};//只要使用初始化列表，其中所有的数据都会初始化为0
		arrayShow(c,4);
		int d[4];//直接定义的临时数组，其中的内容是乱码，所有数据都是随机的
		arrayShow(d,4);
	}
	cout<<endl<<"-------------------------------------"<<endl;
	{//必须使用C++11编译
		int N=5;
		int *a=new int[N]{1};//第一个数是1，其它乱码
		arrayShow(a,N);
		double *b=new double[N]{1,2};//前两个数是1，2，其它是乱码
		arrayShow(b,N);
		//long *c=new long[N]={1,2};//编译出错：error: lvalue required as left operand of assignment
		//不能对一个右值初始化
		double *c=new double[N]{};//c中的数字会被全部初始化为0
		arrayShow(c,N);
	}
	cout<<endl<<"/////////////////////////////////////////////////////////////////////////"<<endl;
	{
		cout<<"=>可以把一个一维数组当二维数组使用"<<endl;
		int a[9];
		int n=3;
		//则a[i*n+j];相当于二维数组的a[i][j]；
	}
	return 0;
}