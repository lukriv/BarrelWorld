#include <stdio.h>

#include <iostream>
#include "consoleWindow.h"
#include "engine.h"

#define WIDTH 	52
#define HEIGHT 	52

using namespace std;

int main(int argc, char **argv)
{
	char c;
	ConsoleWindowWrapper console;
	ConsoleGameEngine engine;
	//console.WriteConsoleInfo();
	
	if(!console.Initialize(WIDTH,HEIGHT,false,CONSOLE_FONT_SIZE_16x12))
	{
		cout << "ConsoleWindowWrapper::Initialize error" << endl;
		return 1;
	}
	
	if(!engine.WriteMargin(console, ConsoleCoord(0,0), ConsoleCoord(50,50)))
	{
		cout << "engine.WriteMargin error" << endl;
		return 1;		
	}
	
	cin >> c;
	
	console.ClearBuffer();
	
	cin >> c;
	
	return 0;
}
