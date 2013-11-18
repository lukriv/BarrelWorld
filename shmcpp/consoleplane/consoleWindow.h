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


enum ConsoleFlags {
	CONSOLE_ECHO_INPUT 			= 1,
	CONSOLE_LINE_INPUT			= 2,
	CONSOLE_MOUSE_INPUT			= 4,
	CONSOLE_WINDOW_INPUT		= 8,
	CONSOLE_INSERT_MODE			= 16,
	CONSOLE_QUICK_EDIT_MODE		= 32,
	CONSOLE_PROCESSED_INPUT		= 64,

};

enum BufferFlags {
	CONSOLE_PROCESSED_OUTPUT	= 1,
	CONSOLE_WRAP_AT_EOL_OUTPUT	= 2	
};


class ConsoleWindowWrapper {
private:
	struct RasterSize {
		ConsoleFontSize m_type;
		short unsigned int m_sizeX;
		short unsigned int m_sizeY;
	};

	static const ConsoleWindowWrapper::RasterSize RasterTable[];
public:
	ConsoleWindowWrapper() {}
	~ConsoleWindowWrapper() {}
	
	bool Initialize();
	
	bool GetConsoleWindowSize(unsigned int &width, unsigned int &height);
	bool GetConsoleBufferSize(unsigned int &width, unsigned int &height);
	
	bool SetConsoleWindowSize(short unsigned int width, short unsigned int height);
	bool SetConsoleBufferSize(short unsigned int width, short unsigned int height);
	
	bool GetMode(unsigned int &flags);
	bool SetMode(unsigned int flags);
	
	bool CreateBuffer(unsigned int flags);
	
	bool SetFontSize(ConsoleFontSize size);
	
	void WriteConsoleInfo();

private:
	

//Get console information	
private:
	bool GetConsoleBufferInfo(std::string& outputString);
	bool GetConsoleModeInfo(std::string& outputString);
	bool GetConsoleFontInfo(std::string& outputString);
	
	const RasterSize* GetFontRasterSize(ConsoleFontSize fontSize);
	
//static convert mehtods
private:
	static void ConsoleModeFlagsToString (long unsigned int consoleMode, std::string& outputString);
	

};



#endif //__CONSOLE_WINDOW_H__