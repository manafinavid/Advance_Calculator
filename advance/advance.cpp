#include <iostream>
#include <vector>
#include <string>
#include "Calculator.h"
using namespace std;
int main()
{
	string a;
	getline(cin, a);
	Calculator test(a);
	Calculator b(a);
	cout<<test.start();
	getchar();
}
