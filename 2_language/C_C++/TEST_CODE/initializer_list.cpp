//1initializer_list是一个模板类，它可以与几乎所有类型如基本类型，自定义类型进行构造时的运算
#include <initializer_list>
double sum(std::initializer_list<double> il);
int main()
{
double total = sum({2.5,3.1,4}); // 4 converted to 4.0
//...
}
double sum(std::initializer_list<double> il)
{
double tot = 0;
for (auto p = il.begin(); p !=il.end(); p++)
tot += *p;
return tot;
}