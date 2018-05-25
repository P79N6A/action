//1在头文件namespace中只能写函数原型，类类型定义和const变量定义，而不能写具体变量，否则
//会在链接时出现重复定义的情况（因为文件中分别编译再链接，所以用条件编译在编译阶段
//没有问题，而在链接时就会出现问题，不同文件中的变量名或者说是namespace名重复）,声明
//可以有多次，而定义只能有一次
//
namespace test3
{
	//double e=2.71828;//链接出错：error LNK2005: "double test3::e" (?e@test3@@3NA) 已经在 namespace.obj 中定义
	extern double e;//这里做一个声明，e在其它文件中定义
}