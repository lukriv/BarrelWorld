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

enum ConsoleAttr {
	CONSOLE_FOREGROUND_BLUE				= 0x0001,
	CONSOLE_FOREGROUND_GREEN			= 0x0002,
	CONSOLE_FOREGROUND_RED				= 0x0004,
	CONSOLE_FOREGROUND_INTENSITY		= 0x0008,
	CONSOLE_BACKGROUND_BLUE 			= 0x0010,
	CONSOLE_BACKGROUND_GREEN 			= 0x0020,
	CONSOLE_BACKGROUND_RED 				= 0x0040,
	CONSOLE_BACKGROUND_INTENSITY		= 0x0080,
	//CONSOLE_COMMON_LVB_LEADING_BYTE		= 0x0100,
	//CONSOLE_COMMON_LVB_TRAILING_BYTE	= 0x0200,
	//CONSOLE_COMMON_LVB_GRID_HORIZONTAL	= 0x0400,
	//CONSOLE_COMMON_LVB_GRID_LVERTICAL	= 0x0800,
	//CONSOLE_COMMON_LVB_GRID_RVERTICAL	= 0x1000,
	//CONSOLE_COMMON_LVB_REVERSE_VIDEO	= 0x4000,
	//CONSOLE_COMMON_LVB_UNDERSCORE		= 0x8000,
};


struct CharObject {
	wchar_t uniChar;
	unsigned short Attr;
};

struct ScreenBuffers;


class IConsoleEventCallback {
public:
	
};


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
	short unsigned int m_globalConsoleAttr;
	
public:
	ConsoleWindowWrapper();
	~ConsoleWindowWrapper();
	
	bool Initialize(short unsigned int width, short unsigned int height, bool doubleBuffer = false, ConsoleFontSize font = CONSOLE_FONT_SIZE_UNDEF);
	
	void ClearBuffer();
	
	void SetGlobalAttr(short unsigned int attr);
	bool EnableCursor(bool enable);
	
	bool SetCursorPosition(short unsigned int x, short unsigned int y);
	
	bool RegisterEventCallback();
	
	bool WriteChar(wchar_t c, short unsigned int x, short unsigned int y);
	bool WriteChar(wchar_t c, const ConsoleCoord &coord);
	
	bool WriteChar(wchar_t c, const ConsoleCoord &coord, short unsigned int consoleAttr);
	
	bool WriteString(const wchar_t* str, const ConsoleCoord &coord);
	bool WriteString(const std::string &string, const ConsoleCoord &coord);
	
	bool WriteString(const wchar_t* str, const ConsoleCoord &coord, unsigned short int consoleAttr);
	bool WriteString(const std::string &string, const ConsoleCoord &coord, unsigned short int consoleAttr);
	
	bool WriteRect(const CharObject* buffer, const ConsoleCoord &position, const ConsoleCoord &rectSize);
	
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
	static void ConsoleAttrFlagsToString(short unsigned int attr, std::string& outputString);
};



#endif //__CONSOLE_WINDOW_H__