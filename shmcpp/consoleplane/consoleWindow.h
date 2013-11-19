#ifndef __CONSOLE_WINDOW_WRAPPER_H__
#define __CONSOLE_WINDOW_WRAPPER_H__

#include <string>

struct ConsoleCoord {
	unsigned int x;
	unsigned int y;
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
	
	bool WriteChar(char c);
	bool WriteChar(char c, unsigned int x, unsigned int y);
	
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

};



#endif //__CONSOLE_WINDOW_H__