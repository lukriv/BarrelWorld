#include <stdio.h>

#include <iostream>
#include "consoleWindow.h"

#define WIDTH 	60
#define HEIGHT 	60

using namespace std;

int main(int argc, char **argv)
{
	char c;
	ConsoleWindowWrapper console;
	
	console.WriteConsoleInfo();
	
	if(!console.Initialize(WIDTH,HEIGHT,false,CONSOLE_FONT_SIZE_16x12))
	{
		cout << "ConsoleWindowWrapper::Initialize error" << endl;
		return 1;
	}
	
	console.WriteConsoleInfo();
	
	cin >> c;
	
	return 0;
}
