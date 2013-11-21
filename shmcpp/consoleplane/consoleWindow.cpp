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

//screen buffer definition
struct ScreenBuffers {
	HANDLE m_inputBuffer;
	HANDLE m_outScreenBuffer[2];
	COORD m_bufferSize[2];
	unsigned int m_activeBuffer;
	
	ScreenBuffers() 
	{
		m_inputBuffer = GetStdHandle(STD_INPUT_HANDLE);
		m_outScreenBuffer[0] = GetStdHandle(STD_OUTPUT_HANDLE);
		m_activeBuffer = 0;
	}
};

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


void ConsoleWindowWrapper::WriteConsoleInfo()
{
	string consoleModeStr;
	
	if(GetConsoleInputModeInfo(consoleModeStr))
	{
		cout << "console input mode: "<< consoleModeStr << endl;	
	}
	
	consoleModeStr.clear();
	
	cout << "-------------------------" << endl; 
	if(GetConsoleOutputModeInfo(consoleModeStr))
	{
		cout << "console output mode: "<< consoleModeStr << endl;	
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
	
	cout << "Console max size:" << GetLargestConsoleWindowSize(m_pScreenBuffer->m_outScreenBuffer[0]).X << "; " << GetLargestConsoleWindowSize(m_pScreenBuffer->m_outScreenBuffer[0]).Y << endl;
	
}

bool ConsoleWindowWrapper::GetConsoleBufferInfo(std::string& outputString)
{
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
	stringstream ostr;
	string str;
	if(!GetConsoleScreenBufferInfo(m_pScreenBuffer->m_outScreenBuffer[0], &screenBufferInfo))
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
	ConsoleAttrFlagsToString(screenBufferInfo.wAttributes, str);
	ostr << "attributes: 0x" << std::hex << screenBufferInfo.wAttributes;
	ostr << "attributes: " << str;
	
	
	
	outputString = ostr.str();
	return true;
	
}

bool ConsoleWindowWrapper::GetConsoleInputModeInfo(std::string& outputString)
{
	long unsigned int consoleMode = 0;
	if(!GetConsoleMode(m_pScreenBuffer->m_inputBuffer, &consoleMode))
	{
		return false;
	}
	
	ConsoleInputBufferModeFlagsToString(consoleMode, outputString);
	
	return true;
}

bool ConsoleWindowWrapper::GetConsoleOutputModeInfo(std::string& outputString)
{
	long unsigned int consoleMode = 0;
	if(!GetConsoleMode(m_pScreenBuffer->m_outScreenBuffer[0], &consoleMode))
	{
		return false;
	}
	
	ConsoleOutputBufferModeFlagsToString(consoleMode, outputString);
	
	return true;
}


bool ConsoleWindowWrapper::GetConsoleFontInfo(std::string& outputString)
{
	CONSOLE_FONT_INFO consoleFontInfo;
	stringstream ostr;
	
	if(!GetCurrentConsoleFont(m_pScreenBuffer->m_outScreenBuffer[0], true, &consoleFontInfo))
	{
		cout << "GetCurrentConsoleFont failed" << endl;
		return false;
	}
	
	ostr << "fontnumber: " << consoleFontInfo.nFont << "\n";
	ostr << "fontsize: " << consoleFontInfo.dwFontSize.X << "; " << consoleFontInfo.dwFontSize.Y << "\n";
	ostr << "fontsize: " << GetConsoleFontSize(m_pScreenBuffer->m_outScreenBuffer[0], consoleFontInfo.nFont).X << "; " << GetConsoleFontSize(m_pScreenBuffer->m_outScreenBuffer[0], consoleFontInfo.nFont).Y << "\n";

	outputString = ostr.str();
	
	return true;	
	
}


bool ConsoleWindowWrapper::SetConsoleWindowSize(short unsigned int width, short unsigned int height)
{
	SMALL_RECT winRect;

	if(!((width > 0)&&(height > 0)))
	{
		return false;
	}
	
	winRect.Left = 0;
	winRect.Top = 0;
	winRect.Right = (SHORT) width - 1;
	winRect.Bottom = (SHORT) height - 1;

	if(!SetConsoleWindowInfo(m_pScreenBuffer->m_outScreenBuffer[0], true, &winRect))
	{
		cout << "SetConsoleWindowInfo failed" << endl;
		return false;
	}		
	
	
	return true;
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
	
	if(!GetConsoleScreenBufferInfo(m_pScreenBuffer->m_outScreenBuffer[0], &screenBufferInfo))
	{
		cout << "GetConsoleScreenBufferInfo failed" << endl;
		return false;
	}
	
	size.X = (SHORT) width;
	size.Y = (SHORT) height;
	
	if(!SetConsoleScreenBufferSize(m_pScreenBuffer->m_outScreenBuffer[0], size))
	{
		cout << "SetConsoleScreenBufferSize failed" << endl;
		return false;
	}
	
	//set actual size
	m_pScreenBuffer->m_bufferSize[0] = size;
	
	return true;
}

bool ConsoleWindowWrapper::SetFontSize(ConsoleFontSize size)
{
	const RasterSize *pFontSize = GetFontRasterSize(size);
	
	if(pFontSize == NULL) return false;
	
	for(DWORD i = 0; i < GetNumberOfConsoleFonts(); i++)
	{
		COORD fontSize = GetConsoleFontSize(m_pScreenBuffer->m_outScreenBuffer[0], i);
		if ((fontSize.X == pFontSize->m_sizeX)&&(fontSize.Y == pFontSize->m_sizeY))
		{
			//we found correct font type and size
			if(SetConsoleFont(m_pScreenBuffer->m_outScreenBuffer[0], i))
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
	width = m_pScreenBuffer->m_bufferSize[0].X;
	height = m_pScreenBuffer->m_bufferSize[0].Y;
	
	return true;
	
}

bool ConsoleWindowWrapper::GetConsoleWindowSize(unsigned int& width, unsigned int& height)
{
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
	if(!GetConsoleScreenBufferInfo(m_pScreenBuffer->m_outScreenBuffer[0], &screenBufferInfo))
	{
		cout << "GetConsoleScreenBufferInfo failed" << endl;
		return false;
	}
	
	width = screenBufferInfo.srWindow.Right - screenBufferInfo.srWindow.Left + 1;
	height = screenBufferInfo.srWindow.Bottom - screenBufferInfo.srWindow.Top + 1;
	
	return true;
}

void ConsoleWindowWrapper::ClearBuffer()
{
}

bool ConsoleWindowWrapper::Initialize(short unsigned int width, short unsigned int height, bool doubleBuffer, ConsoleFontSize font)
{
	COORD bufferSize;
	COORD windowSize;
	COORD newWindowSize;
	COORD minWindowSize;
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;

	if(m_pScreenBuffer == NULL) return false;

	if(!((width > 0)&&(height > 0)))
	{
		return false;
	}
	
	if(font != CONSOLE_FONT_SIZE_UNDEF)
	{
		//set font
		if(!SetFontSize(font))
		{
			cout << "SetFontSize failed" << endl;
			return false;
		}
	}

	// get screen buffer info
	if(!GetConsoleScreenBufferInfo(m_pScreenBuffer->m_outScreenBuffer[0], &screenBufferInfo))
	{
		cout << "GetConsoleScreenBufferInfo failed" << endl;
		return false;
	}
	
	bufferSize = screenBufferInfo.dwSize;
	windowSize.X = screenBufferInfo.srWindow.Right - screenBufferInfo.srWindow.Left + 1;
	windowSize.Y = screenBufferInfo.srWindow.Bottom - screenBufferInfo.srWindow.Top + 1;
	
	// check if sizes are not 
	newWindowSize.X = (width > screenBufferInfo.dwMaximumWindowSize.X) ? screenBufferInfo.dwMaximumWindowSize.X : width;
	newWindowSize.Y = (height > screenBufferInfo.dwMaximumWindowSize.Y) ? screenBufferInfo.dwMaximumWindowSize.Y : height;
	
	minWindowSize.X = (width < windowSize.X) ? width : windowSize.X;
	minWindowSize.Y = (height < windowSize.Y) ? height : windowSize.Y;
	
	ConvertConsoleAttributesToColor(screenBufferInfo.wAttributes, m_foreGroundColor, m_backGroundColor);
	
	// set window size to minimum
	if(!SetConsoleWindowSize(minWindowSize.X, minWindowSize.Y))
	{
		cout << "SetConsoleWindowSize(minWindowSize.X, minWindowSize.Y) failed" << endl;
		return false;
	}
	
	if(!SetConsoleMode(m_pScreenBuffer->m_outScreenBuffer[0], ENABLE_PROCESSED_OUTPUT))
	{
		cout << "SetConsoleMode(m_pScreenBuffer->m_outScreenBuffer[0], ENABLE_PROCESSED_OUTPUT) failed" << endl;
		return false;
	}
	
	// set buffersize
	if(!SetConsoleBufferSize(width, height))
	{
		cout << "SetConsoleBufferSize(width, height) failed" << endl;
		return false;
	}
	
	// set window size to correct size
	if(!SetConsoleWindowSize(newWindowSize.X, newWindowSize.Y))
	{
		cout << "SetConsoleWindowSize(newWindowSize.X, newWindowSize.Y) failed" << endl;
		return false;
	}
	
	return true;

}

bool ConsoleWindowWrapper::ReadInput(std::string& inputStr)
{
	return false;
}

bool ConsoleWindowWrapper::WriteChar(wchar_t c, short unsigned int x, short unsigned int y)
{
	if((x >= m_pScreenBuffer->m_bufferSize[0].X)||(y >= m_pScreenBuffer->m_bufferSize[0].Y))
	{
		return false;
	}
	
	DWORD writtenChars = 0;
	COORD position;
	position.X = (SHORT) x;
	position.Y = (SHORT) y;
		
	
	
	if(!SetConsoleCursorPosition(m_pScreenBuffer->m_outScreenBuffer[0], position))
	{
		return false;
	}
	
	if(!WriteConsoleW(m_pScreenBuffer->m_outScreenBuffer[0], &c, 1, &writtenChars, NULL))
	{
		return false;
	}
	
	return true;
}

ConsoleWindowWrapper::ConsoleWindowWrapper()
{
	// set buffer size to actual window size
	m_pScreenBuffer = new ScreenBuffers();
}

ConsoleWindowWrapper::~ConsoleWindowWrapper()
{
	if(m_pScreenBuffer)
	{
		delete m_pScreenBuffer;
	}
}




bool ConsoleWindowWrapper::WriteChar(wchar_t c, const ConsoleCoord& coord)
{
	return WriteChar(c, coord.x, coord.y);
}

bool ConsoleWindowWrapper::WriteChar(wchar_t c, const ConsoleCoord &coord, unsigned int foreGroundColor, unsigned int backGroundColor)
{
	return false;
}

void ConsoleWindowWrapper::SetBackGroundColor(unsigned int color)
{
}

void ConsoleWindowWrapper::SetForeGroundColor(unsigned int color)
{
}

bool ConsoleWindowWrapper::WriteRect(const wchar_t* buffer, const ConsoleCoord& position, const ConsoleCoord& rectSize)
{
	if(((position.x + rectSize.x) >= m_pScreenBuffer->m_bufferSize[0].X)
		||((position.y + rectSize.y) >= m_pScreenBuffer->m_bufferSize[0].Y))
	{
		return false;
	}
	
	DWORD writtenChars = 0;
	COORD origin, buffSize;
	SMALL_RECT rect;
	origin.X = (SHORT) position.x;
	origin.Y = (SHORT) position.y;
	
	buffSize.X = (SHORT) rectSize.x;
	buffSize.Y = (SHORT) rectSize.y;
		
	rect.Top = (SHORT) position.y;
	rect.Left = (SHORT) position.x;
	rect.Right = (SHORT) (position.x + rectSize.x);
	rect.Bottom = (SHORT) (position.y + rectSize.y);
	
	if(!WriteConsoleOutputW(m_pScreenBuffer->m_outScreenBuffer[0],
						buffer,
						buffSize,
						origin,
						&rect))
	{
		return false;
	}
	
	return true;
}
//////////////////////////////////////////////////////
// static methods
//////////////////////////////////////////////////////
unsigned int ConsoleWindowWrapper::ConvertColorToBackgroundColor(int color)
{
	unsigned int result = 0;
	if(color & CONSOLE_COLOR_BLUE)
	{
		result |= BACKGROUND_BLUE;
	}
	
	if(color & CONSOLE_COLOR_GREEN)
	{
		result |= BACKGROUND_GREEN;
	}
	
	if(color & CONSOLE_COLOR_BLUE)
	{
		result |= BACKGROUND_RED;
	}
	
	if(color & CONSOLE_COLOR_INTENSITY)
	{
		result |= BACKGROUND_INTENSITY;
	}
	
	return result;
}

unsigned int ConsoleWindowWrapper::ConvertColorToForegroundColor(int color)
{
	unsigned int result = 0;
	if(color & CONSOLE_COLOR_BLUE)
	{
		result |= FOREGROUND_BLUE;
	}
	
	if(color & CONSOLE_COLOR_GREEN)
	{
		result |= FOREGROUND_GREEN;
	}
	
	if(color & CONSOLE_COLOR_BLUE)
	{
		result |= FOREGROUND_BLUE;
	}
	
	if(color & CONSOLE_COLOR_INTENSITY)
	{
		result |= FOREGROUND_INTENSITY;
	}
	
	return result;
}

void ConsoleWindowWrapper::ConvertConsoleAttributesToColor(short unsigned int conAttr, unsigned int &foreColor, unsigned int &backColor)
{
	unsigned int foreColor = 0;
	unsigned int backColor = 0;
	if(conAttr & BACKGROUND_BLUE)
	{
		backColor |= CONSOLE_COLOR_BLUE;
	}
	
	if(conAttr & BACKGROUND_GREEN)
	{
		backColor |= CONSOLE_COLOR_GREEN;
	}
	
	if(conAttr & BACKGROUND_RED)
	{
		backColor |= CONSOLE_COLOR_BLUE;
	}
	
	if(conAttr & BACKGROUND_INTENSITY)
	{
		backColor |= CONSOLE_COLOR_INTENSITY;
	}
	
	if(conAttr & FOREGROUND_BLUE)
	{
		foreColor |= CONSOLE_COLOR_BLUE;
	}
	
	if(conAttr & FOREGROUND_GREEN)
	{
		foreColor |= CONSOLE_COLOR_GREEN;
	}
	
	if(conAttr & FOREGROUND_BLUE)
	{
		foreColor |= CONSOLE_COLOR_BLUE;
	}
	
	if(conAttr & FOREGROUND_INTENSITY)
	{
		foreColor |= CONSOLE_COLOR_INTENSITY;
	}
	
}

void ConsoleWindowWrapper::ConsoleInputBufferModeFlagsToString(long unsigned int consoleMode, std::string& outputString)
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

void ConsoleWindowWrapper::ConsoleOutputBufferModeFlagsToString(long unsigned int consoleMode, std::string& outputString)
{
	bool slashWrite = false;
	if(consoleMode & ENABLE_PROCESSED_OUTPUT)
	{
		outputString.append("ENABLE_PROCESSED_OUTPUT");
		slashWrite = true;
	}
	
	if(consoleMode & ENABLE_WRAP_AT_EOL_OUTPUT)
	{
		if(slashWrite) outputString.append(" | ");
		outputString.append("ENABLE_WRAP_AT_EOL_OUTPUT");
		slashWrite = true;
	}
}

void ConsoleWindowWrapper::ConsoleAttrFlagsToString(short unsigned int attr, std::string& outputString)
{	
	bool slashWrite = false;
	outputString.clear();
	if(conAttr & BACKGROUND_BLUE)
	{
		outputString.append("BACKGROUND_BLUE");
		slashWrite = true;
	}
	
	if(conAttr & BACKGROUND_GREEN)
	{
		if(slashWrite) outputString.append(" | ");
		outputString.append("BACKGROUND_GREEN");
		slashWrite = true;
	}
	
	if(conAttr & BACKGROUND_RED)
	{
		if(slashWrite) outputString.append(" | ");
		outputString.append("BACKGROUND_RED");
		slashWrite = true;
	}
	
	if(conAttr & BACKGROUND_INTENSITY)
	{
		if(slashWrite) outputString.append(" | ");
		outputString.append("BACKGROUND_INTENSITY");
		slashWrite = true;
	}
	
	if(conAttr & FOREGROUND_BLUE)
	{
		if(slashWrite) outputString.append(" | ");
		outputString.append("FOREGROUND_BLUE");
		slashWrite = true;
	}
	
	if(conAttr & FOREGROUND_GREEN)
	{
		if(slashWrite) outputString.append(" | ");
		outputString.append("FOREGROUND_GREEN");
		slashWrite = true;
	}
	
	if(conAttr & FOREGROUND_BLUE)
	{
		if(slashWrite) outputString.append(" | ");
		outputString.append("FOREGROUND_BLUE");
		slashWrite = true;
	}
	
	if(conAttr & FOREGROUND_INTENSITY)
	{
		if(slashWrite) outputString.append(" | ");
		outputString.append("FOREGROUND_INTENSITY");
		slashWrite = true;
	}
}
