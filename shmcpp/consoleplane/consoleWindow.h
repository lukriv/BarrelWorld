#ifndef __CONSOLE_WINDOW_WRAPPER_H__
#define __CONSOLE_WINDOW_WRAPPER_H__

#include <string>

struct ConsoleCoord {
	short unsigned int x;
	short unsigned int y;
	
	ConsoleCoord(): x(0), y(0) {}
	ConsoleCoord(short unsigned int in_x, short unsigned int in_y) : x(in_x), y(in_y) {}
};

enum ConsoleFontSize {
	CONSOLE_FONT_SIZE_UNDEF	= 0,
	CONSOLE_FONT_SIZE_4x6 	= 1,
	CONSOLE_FONT_SIZE_6x8 	= 2,
	CONSOLE_FONT_SIZE_8x8 	= 3,
	CONSOLE_FONT_SIZE_16x8 	= 4,
	CONSOLE_FONT_SIZE_5x12 	= 5,
	CONSOLE_FONT_SIZE_7x12 	= 6,
	CONSOLE_FONT_SIZE_8x12 	= 7,
	CONSOLE_FONT_SIZE_16x12 = 8,
	CONSOLE_FONT_SIZE_12x16 = 9,
	CONSOLE_FONT_SIZE_10x18 = 10
};

enum ConsoleColor {
	CONSOLE_COLOR_BLUE 		= 1,
	CONSOLE_COLOR_GREEN		= 2,
	CONSOLE_COLOR_RED		= 3,
	CONSOLE_COLOR_INTENSITY = 4
};


struct ScreenBuffers;

class ConsoleWindowWrapper {
private:
	struct RasterSize {
		ConsoleFontSize m_type;
		short unsigned int m_sizeX;
		short unsigned int m_sizeY;
	};

	static const ConsoleWindowWrapper::RasterSize RasterTable[];
private:
	ScreenBuffers* m_pScreenBuffer;
public:
	ConsoleWindowWrapper();
	~ConsoleWindowWrapper();
	
	bool Initialize(short unsigned int width, short unsigned int height, bool doubleBuffer = false, ConsoleFontSize font = CONSOLE_FONT_SIZE_UNDEF);
	
	void ClearBuffer();
	
	void SetForeGroundColor(ConsoleColor color);
	void SetBackGroundColor(ConsoleColor color);
	
	bool WriteChar(wchar_t c, short unsigned int x, short unsigned int y);
	bool WriteChar(wchar_t c, const ConsoleCoord &coord);
	
	bool WriteChar(wchar_t c, const ConsoleCoord &coord, ConsoleColor foreGroundColor, ConsoleColor backGroundColor);
	
	bool ReadInput(std::string &inputStr);
	
	void WriteConsoleInfo();

protected:	
	bool GetConsoleWindowSize(unsigned int &width, unsigned int &height);
	bool GetConsoleBufferSize(unsigned int &width, unsigned int &height);
	
	bool SetConsoleWindowSize(short unsigned int width, short unsigned int height);
	bool SetConsoleBufferSize(short unsigned int width, short unsigned int height);
	
	bool SetFontSize(ConsoleFontSize size);
	
//Get console information	
private:
	bool GetConsoleBufferInfo(std::string& outputString);
	bool GetConsoleInputModeInfo(std::string& outputString);
	bool GetConsoleOutputModeInfo(std::string& outputString);
	bool GetConsoleFontInfo(std::string& outputString);
	
	const RasterSize* GetFontRasterSize(ConsoleFontSize fontSize);
	
//static convert mehtods
private:
	static void ConsoleOutputBufferModeFlagsToString(long unsigned int consoleMode, std::string& outputString);
	static void ConsoleInputBufferModeFlagsToString(long unsigned int consoleMode, std::string& outputString);
	static unsigned int ConvertColorToForegroundColor(int color);
	static unsigned int ConvertColorToBackgroundColor(int color);

};



#endif //__CONSOLE_WINDOW_H__