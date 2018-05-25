// limits.cpp -- some integer limits
#include <iostream>
#include <climits> // use limits.h for older systems
int main()
{
using namespace std;
int n_int = INT_MAX; // initialize n_int to max int value
short n_short = SHRT_MAX; // symbols defined in climits file
long n_long = LONG_MAX;
long long n_llong = LLONG_MAX;
// sizeof operator yields size of type or of variable
cout << "int is " << sizeof (int) << " bytes." << endl;
cout << "short is " << sizeof n_short << " bytes." << endl;
cout << "long is " << sizeof n_long << " bytes." << endl;
cout << "long long is " << sizeof n_llong << " bytes." << endl;
cout << endl;
cout << "Maximum values:" << endl;
cout << "int: " << n_int << endl;
cout << "short: " << n_short << endl;
cout << "long: " << n_long << endl;
cout << "long long: " << n_llong << endl << endl;
cout << "Minimum int value = " << INT_MIN << endl;
cout << "Bits per byte = " << CHAR_BIT << endl;
return 0;
}
//Table 3.1 Symbolic Constants from climits
//Symbolic Constant Represents
//CHAR_BIT Number of bits in a char
//CHAR_MAX Maximum char value
//CHAR_MIN Minimum char value
//SCHAR_MAX Maximum signed char value
//SCHAR_MIN Minimum signed char value
//UCHAR_MAX Maximum unsigned char value
//SHRT_MAX Maximum short value
//SHRT_MIN Minimum short value
//USHRT_MAX Maximum unsigned short value
//INT_MAX Maximum int value
//INT_MIN Minimum int value
//UINT_MAX Maximum unsigned int value
//LONG_MAX Maximum long value
//LONG_MIN Minimum long value
//ULONG_MAX Maximum unsigned long value
//LLONG_MAX Maximum long long value
//LLONG_MIN Minimum long long value
//ULLONG_MAX Maximum unsigned long long value