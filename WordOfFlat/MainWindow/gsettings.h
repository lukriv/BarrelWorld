#ifndef __GAME_ENGINE_SETTINGS_H__
#define __GAME_ENGINE_SETTINGS_H__

#include <wx/string.h>
#include <wx/defs.h>

struct GameEngineSettings {
	wxDword m_screenWidth;
	wxDword m_screenHeight;
	wxString m_screenTitle;
	wxString m_worldName;

	GameEngineSettings() :
		m_screenWidth(800),
		m_screenHeight(600) {}
};


#endif // __GAME_ENGINE_SETTINGS_H__