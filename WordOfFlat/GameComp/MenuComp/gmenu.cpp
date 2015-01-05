#include "gmenu.h"

#include <CEGUI/CEGUI.h>

GameMenu::GameMenu(GameMenuSystem *pMenuRes, GameLogger *pLogger) : GameMenuBase(pMenuRes, pLogger) 
					, m_isInitialized(false)
					, m_pButtonExit(nullptr)
					, m_pButtonSwitch(nullptr)
					, m_pCallback(nullptr) {}


GameErrorCode GameMenu::Initialize(GameMenuCallback* pCallback)
{
	GameErrorCode result = FWG_NO_ERROR;
	
	if(!pCallback)
	{
		FWGLOG_ERROR(wxT("Callback pointer cannot be nullptr"), GetLogger());
		return FWG_E_INVALID_PARAMETER_ERROR;
	} else {
		m_pCallback = pCallback;
	}
	
	try {
	
		// check if the right scheme is already loaded and alternatively load
		if( !CEGUI::SchemeManager::getSingleton().isDefined( "test" ) )
		{
			CEGUI::SchemeManager::getSingleton().createFromFile( "test.scheme" );
		}
		
		if( FWG_FAILED( result = CreateRootWindow(wxString("mainMenu"))))
		{
			FWGLOG_ERROR_FORMAT(wxT("CreateRootWindow failed: 0x%08x"), GetLogger(), result, FWGLOG_ENDVAL);
			return result;
		}
		
		m_pButtonExit = CEGUI::WindowManager::getSingleton().createWindow( "Test/Button", "mainMenu/ExitButton");
		m_pButtonExit->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1,0),CEGUI::UDim(0.1,0)));
		m_pButtonExit->setSize(CEGUI::USize (CEGUI::UDim(0,200), CEGUI::UDim(0,75)));
		m_pButtonExit->setText("Exit");
		
		m_pButtonExit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameMenu::ExitEvent, this));
		
		m_pButtonSwitch = CEGUI::WindowManager::getSingleton().createWindow( "Test/Button", "mainMenu/SwitchButton");
		m_pButtonSwitch->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1,0),CEGUI::UDim(0.3,0)));
		m_pButtonSwitch->setSize(CEGUI::USize (CEGUI::UDim(0,200), CEGUI::UDim(0,75)));
		m_pButtonSwitch->setText("Switch");
		
		m_pButtonSwitch->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameMenu::SwitchEvent, this));
		
		// set buttons to root window
		GetRootWindow()->addChild(m_pButtonExit);
		GetRootWindow()->addChild(m_pButtonSwitch);
		
	} catch (Ogre::Exception &ex) {
		FWGLOG_ERROR_FORMAT(wxT("Ogre exception raised: '%s'"), GetLogger()
														, wxString::FromUTF8(ex.what()).GetData().AsInternal()
														, FWGLOG_ENDVAL);
		return FWG_E_NO_RESOURCES_ERROR;
	} catch (CEGUI::Exception &ex) {
		FWGLOG_ERROR_FORMAT(wxT("CEGUI exception raised: '%s'"), GetLogger()
														, wxString::FromUTF8(ex.what()).GetData().AsInternal()
														, FWGLOG_ENDVAL);
		return FWG_E_MENU_SYSTEM_ERROR;
	}
	
	// load 
	return FWG_NO_ERROR;
}

GameMenu::~GameMenu()
{
	if(m_pButtonExit)
	{
		m_pButtonExit->removeAllEvents();
		CEGUI::WindowManager::getSingleton().destroyWindow(m_pButtonExit);
		m_pButtonExit = nullptr;
	}
	
	if(m_pButtonSwitch)
	{
		m_pButtonSwitch->removeAllEvents();
		CEGUI::WindowManager::getSingleton().destroyWindow(m_pButtonSwitch);
		m_pButtonSwitch = nullptr;
	}
	
}

GameErrorCode GameMenu::Show()
{
	if((m_pMenuRes)&&(m_pMenuRes->GetGuiSystem())&&(GetRootWindow()))
	{
		m_pMenuRes->GetGuiSystem()->getDefaultGUIContext().setRootWindow(GetRootWindow());
		m_pMenuRes->GetGuiSystem()->getDefaultGUIContext().setDefaultFont("DejaVuSans-10");
		m_pMenuRes->GetGuiSystem()->getDefaultGUIContext().getMouseCursor().setDefaultImage("Test/Cursor");
	}
	
	return FWG_NO_ERROR;
}

bool GameMenu::ExitEvent(const CEGUI::EventArgs&)
{
	if(m_pCallback)
	{
		m_pCallback->OnExitEvent();
	}
	
	return true;
}

bool GameMenu::SwitchEvent(const CEGUI::EventArgs&)
{

	if(m_pButtonExit->isVisible())
	{
		m_pButtonExit->setVisible(false);
	} else {
		m_pButtonExit->setVisible(true);
	}
	
	return true;
}
