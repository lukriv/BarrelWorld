#include "consoleWindow.h"

#include <iostream>
#include <sstream>

#include <windows.h>
#include <wincon.h>

//Additional windows console functions 
#ifdef __cplusplus
extern "C" {
#endif
BOOL WINAPI GetCurrentConsoleFont(HANDLE hConsoleOutput,BOOL bMaximumWindow,PCONSOLE_FONT_INFO lpConsoleCurrentFont);
COORD WINAPI GetConsoleFontSize( HANDLE hConsoleOutput, DWORD nFont);
DWORD WINAPI GetNumberOfConsoleFonts();
BOOL WINAPI SetConsoleFont(HANDLE, DWORD);
#ifdef __cplusplus
}
#endif


//Raster font table sizes


const ConsoleWindowWrapper::RasterSize ConsoleWindowWrapper::RasterTable[] = {
			{CONSOLE_FONT_SIZE_4x6,		4,	6},
			{CONSOLE_FONT_SIZE_6x8, 	6,	8},
			{CONSOLE_FONT_SIZE_8x8, 	8,	8},
			{CONSOLE_FONT_SIZE_16x8,   16,  8},
			{CONSOLE_FONT_SIZE_5x12, 	5, 12},
			{CONSOLE_FONT_SIZE_7x12, 	7, 12},
			{CONSOLE_FONT_SIZE_8x12, 	8, 12},
			{CONSOLE_FONT_SIZE_16x12,  16, 12},
			{CONSOLE_FONT_SIZE_12x16,  12, 16},
			{CONSOLE_FONT_SIZE_10x18,  10, 18}
		};

#define SIZE_OF_TABLE(x) (sizeof(x)/sizeof(x[0]))

using namespace std;


HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

bool ConsoleWindowWrapper::Initialize()
{
	if(!SetFontSize(CONSOLE_FONT_SIZE_8x8))
	{
		return false;
	}
	
	
	
	return true;
}
	


void ConsoleWindowWrapper::WriteConsoleInfo()
{
	string consoleModeStr;
	
	if(GetConsoleModeInfo(consoleModeStr))
	{
		cout << "console mode: "<< consoleModeStr << endl;	
	}
	
	consoleModeStr.clear();
	
	cout << "-------------------------" << endl; 
	if(GetConsoleBufferInfo(consoleModeStr))
	{
		cout << "console screen buffer info :" << endl;
		cout << consoleModeStr << endl;
	}
	
	consoleModeStr.clear();
	
	cout << "-------------------------" << endl;
	
	if(GetConsoleFontInfo(consoleModeStr))
	{
		cout << "console font info :" << endl;
		cout << consoleModeStr << endl;
	}
	
	cout << "-------------------------" << endl;
	
	consoleModeStr.clear();
	
	cout << "Console max size:" << GetLargestConsoleWindowSize(console).X << "; " << GetLargestConsoleWindowSize(console).Y << endl;
	
}

bool ConsoleWindowWrapper::GetConsoleBufferInfo(std::string& outputString)
{
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
	stringstream ostr;
	if(!GetConsoleScreenBufferInfo(console, &screenBufferInfo))
	{
		cout << "GetConsoleScreenBufferInfo failed" << endl;
		return false;
	}
	
	ostr << "buffer size: " << screenBufferInfo.dwSize.X << "; " << screenBufferInfo.dwSize.Y << "\n";
	ostr << "cursor position: " << screenBufferInfo.dwCursorPosition.X << "; " << screenBufferInfo.dwCursorPosition.Y << "\n";
	ostr << "displayed area: left = " << screenBufferInfo.srWindow.Left <<  "; top = " << screenBufferInfo.srWindow.Top 
		  <<  "; right = " << screenBufferInfo.srWindow.Right << "; bottom = " << screenBufferInfo.srWindow.Bottom << "\n";
	ostr << "displayed area size: " << screenBufferInfo.srWindow.Right - screenBufferInfo.srWindow.Left << "; " 
		  << screenBufferInfo.srWindow.Bottom - screenBufferInfo.srWindow.Top << "\n";
 	ostr << "maximum size: " << screenBufferInfo.dwMaximumWindowSize.X << "; " << screenBufferInfo.dwMaximumWindowSize.Y << "\n";
	ostr << "attributes: 0x" << std::hex << screenBufferInfo.wAttributes;
	
	outputString = ostr.str();
	return true;
	
}

bool ConsoleWindowWrapper::GetConsoleModeInfo(std::string& outputString)
{
	long unsigned int consoleMode = 0;
	if(!GetConsoleMode(console, &consoleMode))
	{
		return false;
	}
	
	ConsoleModeFlagsToString(consoleMode, outputString);
	
	return true;
}

bool ConsoleWindowWrapper::GetConsoleFontInfo(std::string& outputString)
{
	CONSOLE_FONT_INFO consoleFontInfo;
	stringstream ostr;
	
	if(!GetCurrentConsoleFont(console, true, &consoleFontInfo))
	{
		cout << "GetCurrentConsoleFont failed" << endl;
		return false;
	}
	
	ostr << "fontnumber: " << consoleFontInfo.nFont << "\n";
	ostr << "fontsize: " << consoleFontInfo.dwFontSize.X << "; " << consoleFontInfo.dwFontSize.Y << "\n";
	ostr << "fontsize: " << GetConsoleFontSize(console, consoleFontInfo.nFont).X << "; " << GetConsoleFontSize(console, consoleFontInfo.nFont).Y << "\n";

	outputString = ostr.str();
	
	return true;	
	
}


bool ConsoleWindowWrapper::SetConsoleWindowSize(short unsigned int width, short unsigned int height)
{
	SMALL_RECT winRect;
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;

	if(!((width > 0)&&(height > 0)))
	{
		return false;
	}
	

	if(!GetConsoleScreenBufferInfo(console, &screenBufferInfo))
	{
		cout << "GetConsoleScreenBufferInfo failed" << endl;
		return false;
	}
	
	winRect.Left = 0;
	winRect.Top = 0;
	winRect.Right = (SHORT) width - 1;
	winRect.Bottom = (SHORT) height - 1;

	if((width > screenBufferInfo.dwSize.X)||(height > screenBufferInfo.dwSize.Y))
	{
		if(!SetConsoleBufferSize(width, height))
		{
			cout << "SetConsoleBufferSize failed" << endl;
			return false;
		}
	}

	if(!SetConsoleWindowInfo(console, true, &winRect))
	{
		cout << "SetConsoleWindowInfo failed" << endl;
		return false;
	}		
	
	
	return true;
}



void ConsoleWindowWrapper::ConsoleModeFlagsToString(long unsigned int consoleMode, std::string& outputString)
{
	bool slashWrite = false;
	if(consoleMode & ENABLE_ECHO_INPUT)
	{
		outputString.append("ENABLE_ECHO_INPUT");
		slashWrite = true;
	}
	
	if(consoleMode & ENABLE_INSERT_MODE)
	{
		if(slashWrite) outputString.append(" | ");
		outputString.append("ENABLE_INSERT_MODE");
		slashWrite = true;
	}
	
	if(consoleMode & ENABLE_LINE_INPUT)
	{
		if(slashWrite) outputString.append(" | ");
		outputString.append("ENABLE_LINE_INPUT");
		slashWrite = true;
	}

	if(consoleMode & ENABLE_MOUSE_INPUT)
	{
		if(slashWrite) outputString.append(" | ");
		outputString.append("ENABLE_MOUSE_INPUT");
		slashWrite = true;
	}

	if(consoleMode & ENABLE_PROCESSED_INPUT)
	{
		if(slashWrite) outputString.append(" | ");
		outputString.append("ENABLE_PROCESSED_INPUT");
		slashWrite = true;
	}

	if(consoleMode & ENABLE_QUICK_EDIT_MODE)
	{
		if(slashWrite) outputString.append(" | ");
		outputString.append("ENABLE_QUICK_EDIT_MODE");
		slashWrite = true;
	}

	if(consoleMode & ENABLE_WINDOW_INPUT)
	{
		if(slashWrite) outputString.append(" | ");
		outputString.append("ENABLE_WINDOW_INPUT");
		slashWrite = true;
	}		
}

const ConsoleWindowWrapper::RasterSize* ConsoleWindowWrapper::GetFontRasterSize(ConsoleFontSize fontSize)
{
	for (size_t i = 0; i < SIZE_OF_TABLE(RasterTable); i++)
	{
		if(RasterTable[i].m_type == fontSize)
		{
			return (RasterTable + i);
		}
	}
	
	return NULL;
}

bool ConsoleWindowWrapper::SetConsoleBufferSize(short unsigned int width, short unsigned int height)
{
	COORD size;
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
	
	if((width < 0)||(height < 0))
	{
		return false;
	}
	
	if(!GetConsoleScreenBufferInfo(console, &screenBufferInfo))
	{
		cout << "GetConsoleScreenBufferInfo failed" << endl;
		return false;
	}
	
	size.X = (SHORT) width;
	size.Y = (SHORT) height;
	
	if(!SetConsoleScreenBufferSize(console, size))
	{
		cout << "SetConsoleScreenBufferSize failed" << endl;
		return false;
	}
	
	return true;
}

bool ConsoleWindowWrapper::SetFontSize(ConsoleFontSize size)
{
	const RasterSize *pFontSize = GetFontRasterSize(size);
	
	if(pFontSize == NULL) return false;
	
	for(DWORD i = 0; i < GetNumberOfConsoleFonts(); i++)
	{
		COORD fontSize = GetConsoleFontSize(console, i);
		if ((fontSize.X == pFontSize->m_sizeX)&&(fontSize.Y == pFontSize->m_sizeY))
		{
			//we found correct font type and size
			if(SetConsoleFont(console, i))
			{
				return true;
			} else {
				return false;
			}
		}
	}
	
	return false;
	
}

bool ConsoleWindowWrapper::GetConsoleBufferSize(unsigned int& width, unsigned int& height)
{
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
	if(!GetConsoleScreenBufferInfo(console, &screenBufferInfo))
	{
		cout << "GetConsoleScreenBufferInfo failed" << endl;
		return false;
	}
	
	width = screenBufferInfo.dwSize.X;
	height = screenBufferInfo.dwSize.Y;
	
	return true;
	
}

bool ConsoleWindowWrapper::GetConsoleWindowSize(unsigned int& width, unsigned int& height)
{
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
	if(!GetConsoleScreenBufferInfo(console, &screenBufferInfo))
	{
		cout << "GetConsoleScreenBufferInfo failed" << endl;
		return false;
	}
	
	width = screenBufferInfo.srWindow.Right - screenBufferInfo.srWindow.Left + 1;
	height = screenBufferInfo.srWindow.Bottom - screenBufferInfo.srWindow.Top + 1;
	
	return true;
}

bool ConsoleWindowWrapper::GetMode(unsigned int& flags)
{

	
}

bool ConsoleWindowWrapper::SetMode(unsigned int flags)
{
	DWORD consoleFlags = 0;
	DWORD bufferFlags = 0;
	
	//console
	if(flags & CONSOLE_ECHO_INPUT)
	{
		consoleFlags |= ENABLE_ECHO_INPUT;
	}
	
	if(flags & CONSOLE_LINE_INPUT)
	{
		consoleFlags |= ENABLE_LINE_INPUT;
	}
	
	if(flags & CONSOLE_MOUSE_INPUT
	{
		consoleFlags |= ENABLE_MOUSE_INPUT;
	}
	
	if(flags & CONSOLE_WINDOW_INPUT
	{
		consoleFlags |= ENABLE_WINDOW_INPUT;
	}
	
	if(flags & CONSOLE_PROCESSED_INPUT
	{
		consoleFlags |= ENABLE_PROCESSED_INPUT;
	}	
	
	if(flags & CONSOLE_INSERT_MODE)
	{
		consoleFlags |= ENABLE_EXTENDED_FLAGS|ENABLE_INSERT_MODE;
	}
	
	if(flags & CONSOLE_QUICK_EDIT_MODE)
	{
		consoleFlags |= ENABLE_EXTENDED_FLAGS|ENABLE_QUICK_EDIT_MODE;
	}

	
	if(!SetConsoleMode(console, consoleFlags))
	{
		cout << "SetConsoleMode failed" << endl;
		return false;
	}
	
	return true;

}

bool ConsoleWindowWrapper::CreateBuffer (unsigned int flags)
{
	//buffer 
	if(flags & CONSOLE_PROCESSED_OUTPUT)
	{
		bufferFlags |= ENABLE_PROCESSED_OUTPUT;
	}
	
	if(flags & CONSOLE_WRAP_AT_EOL_OUTPUT)
	{
		bufferFlags |= ENABLE_WRAP_AT_EOL_OUTPUT;
	}
	
	return true;
}
