#include "gmenubase.h"
#include "gmenumgr.h"

/////////////////////
//  GameMenuBase   //
/////////////////////


CEGUI::MouseButton ConvertMouseButtonId(OIS::MouseButtonID mouseButtonId)
{
	switch(mouseButtonId)
	{
	case OIS::MB_Left:
		return CEGUI::LeftButton;
		
	case OIS::MB_Right:
		return CEGUI::RightButton;
		
	case OIS::MB_Middle:
		return CEGUI::MiddleButton;
		
	case OIS::MB_Button3: 
		return CEGUI::X1Button;
		
	case OIS::MB_Button4:
		return CEGUI::X2Button;
		
	default:
		return CEGUI::NoButton;
	}
}


GameMenuBase::~GameMenuBase()
{
	if(m_pRootWindow)
	{
		m_pRootWindow->destroy();
		m_pRootWindow = nullptr;
	}
}

GameMenuBase::GameMenuBase(GameMenuSystem *pMenuRes, GameInputSystem *pInputSystem, GameLogger *pLogger) : m_pRootWindow(nullptr)
																, m_spLogger(pLogger)	
																, m_pMenuRes(pMenuRes)
																, m_pInputSystem(pInputSystem){}


GameErrorCode GameMenuBase::CreateRootWindow(const wxString& windowName)
{
	if(!m_pRootWindow)
	{
		m_pRootWindow = CEGUI::WindowManager::getSingleton().createWindow( "DefaultWindow", windowName.ToStdString() );	
	}
	
	return FWG_NO_ERROR;	
}


void GameMenuBase::OnMouseMoved(const OIS::MouseState& arg)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(static_cast<float>(arg.X.rel), static_cast<float>(arg.Y.rel));
}

void GameMenuBase::OnMousePressed(const OIS::MouseState& arg, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(ConvertMouseButtonId(id));
}

void GameMenuBase::OnMouseReleased(const OIS::MouseState& arg, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(ConvertMouseButtonId(id));
}