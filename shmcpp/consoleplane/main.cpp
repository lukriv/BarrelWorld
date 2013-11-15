#include <stdio.h>

#include <iostream>
#include "consoleWindow.h"

using namespace std;

int main(int argc, char **argv)
{
	ConsoleWindowWrapper console;
	if(!console.Initialize(0,0))
	{
		cout << "ConsoleWindowWrapper::Initialize error" << endl;
		return 1;
	}
	
	return 0;
}
