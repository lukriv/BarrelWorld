#include <stdio.h>

#include <iostream>
#include "consoleWindow.h"

#define WIDTH 	80
#define HEIGHT 	80

using namespace std;

int main(int argc, char **argv)
{
	ConsoleWindowWrapper console;
	if(!console.Initialize())
	{
		cout << "ConsoleWindowWrapper::Initialize error" << endl;
		return 1;
	}
	
	if(!console.SetFontSize(CONSOLE_FONT_SIZE_16x12)) 
	{
		cout << "ConsoleWindowWrapper::SetFontSize error" << endl;
		return 1;
	}
	
	if(!console.SetConsoleBufferSize(WIDTH, HEIGHT))
	{
		cout << "ConsoleWindowWrapper::SetConsoleBufferSize error" << endl;
		return 1;		
	}
	
	if(!console.SetConsoleWindowSize(WIDTH, HEIGHT))
	{
		cout << "ConsoleWindowWrapper::SetConsoleWindowSize error" << endl;
		return 1;		
	}
	
	console.WriteConsoleInfo();
	
	return 0;
}
