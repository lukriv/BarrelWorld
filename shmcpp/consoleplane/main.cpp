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
	
	 
	
	engine.Play(console);
	
	console.ClearBuffer();
	
	return 0;
}
