#include <stdio.h>
#include <iostream>

using namespace std;
// program zjisti, zda je zadana hodnota a rovna nebo mensi nez 5
int main(int argc, char **argv)
{
	int a = 0;
	cout << "Zadej hodnotu a :" << endl;
	cin >> a;
	cout << "Hodnota a = " << a << endl;
	if(a <= 5)
	{
		cout << "Cislo a je mensi nebo rovno 5" << endl;
	} else {
		cout << "Cislo a je vetsi nez 5" << endl;
	}
	return 0;
}
