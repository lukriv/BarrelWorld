#include "consoleWindow.h"

#include <iostream>

#include <windows.h>
#include <wincon.h>

using namespace std;


HWND console = GetConsoleWindow();



bool ConsoleWindowWrapper::Initialize(int width, int height)
{
	COORD conFontSize;
	if(!GetConsoleFontSize(console, conFontSize))
	{
		return false;
	}
	
	cout << "font size: "<< conFontSize.X << "; " << conFontSize.Y << endl;
	
	return true;
	
}
