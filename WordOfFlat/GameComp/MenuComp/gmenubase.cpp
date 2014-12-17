#include "gmenubase.h"
#include "gmenumgr.h"

/////////////////////
//  GameMenuBase   //
/////////////////////


GameMenuBase::~GameMenuBase()
{
	if(m_pRootWindow)
	{
		m_pRootWindow->destroy();
		m_pRootWindow = nullptr;
	}
}

GameMenuBase::GameMenuBase(GameMenuSystem *pMenuRes, GameLogger *pLogger) : m_spLogger(pLogger)	
																,m_pMenuRes(pMenuRes) {}


GameErrorCode GameMenuBase::CreateRootWindow(const wxString& windowName)
{
	if(!m_pRootWindow)
	{
		m_pRootWindow = CEGUI::WindowManager::getSingleton().createWindow( "DefaultWindow", windowName.ToStdString() );	
	}
	
	return FWG_NO_ERROR;	
}
