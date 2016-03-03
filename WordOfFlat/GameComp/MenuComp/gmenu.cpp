#include "gmenu.h"

#include <CEGUI/CEGUI.h>

GameMenu::GameMenu(GameMenuSystem *pMenuRes, GameInputSystem *pInputSystem, GameLogger *pLogger) : GameMenuBase(pMenuRes, pInputSystem, pLogger) 
					, m_isInitialized(false)
					, m_pButtonExit(nullptr)
					, m_pButtonSwitch(nullptr)
					, m_pButtonDebug(nullptr)
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
		m_pButtonExit->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05,0),CEGUI::UDim(0.05,0)));
		m_pButtonExit->setSize(CEGUI::USize (CEGUI::UDim(0,200), CEGUI::UDim(0,75)));
		m_pButtonExit->setText("Exit");
		
		m_pButtonExit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameMenu::ExitEvent, this));
		
		m_pButtonSwitch = CEGUI::WindowManager::getSingleton().createWindow( "Test/Button", "mainMenu/SwitchButton");
		m_pButtonSwitch->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05,0),CEGUI::UDim(0.2,0)));
		m_pButtonSwitch->setSize(CEGUI::USize (CEGUI::UDim(0,200), CEGUI::UDim(0,75)));
		m_pButtonSwitch->setText("Switch");
		
		m_pButtonSwitch->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameMenu::SwitchEvent, this));
		
		m_pButtonDebug = CEGUI::WindowManager::getSingleton().createWindow( "Test/Button", "mainMenu/DebugButton");
		m_pButtonDebug->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05,0),CEGUI::UDim(0.35,0)));
		m_pButtonDebug->setSize(CEGUI::USize (CEGUI::UDim(0,200), CEGUI::UDim(0,75)));
		m_pButtonDebug->setText("Debug");
		
		m_pButtonDebug->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameMenu::DebugEvent, this));

		
		// set buttons to root window
		GetRootWindow()->addChild(m_pButtonExit);
		GetRootWindow()->addChild(m_pButtonSwitch);
		GetRootWindow()->addChild(m_pButtonDebug);
		
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
	
	// register input callback to input system
	if(FWG_FAILED(result = m_pInputSystem->RegisterCallback(this)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Register mouse callback failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	// load 
	return FWG_NO_ERROR;
}

GameMenu::~GameMenu()
{
	if(m_pInputSystem)
	{
		m_pInputSystem->UnregisterCallback(this);
	}
	
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

	//if(m_pButtonExit->isVisible())
	//{
	//	m_pButtonExit->setVisible(false);
	//} else {
	//	m_pButtonExit->setVisible(true);
	//}
	
	if(m_pCallback)
	{
		m_pCallback->OnSwitchEvent();
	}
	
	return true;
}

bool GameMenu::DebugEvent(const CEGUI::EventArgs&)
{
	if(m_pCallback)
	{
		m_pCallback->OnDebugEvent();
	}
	
	return true;
}



void GameMenu::OnMouseMoved(const OIS::MouseState& arg)
{
	if((arg.buttons & (1 << OIS::MB_Right)) == 0)
	{
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(static_cast<float>(arg.X.rel), static_cast<float>(arg.Y.rel));
	}
}

void GameMenu::OnMousePressed(const OIS::MouseState& arg, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(ConvertMouseButtonId(id));
}

void GameMenu::OnMouseReleased(const OIS::MouseState& arg, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(ConvertMouseButtonId(id));
}

void GameMenu::ShowMenuButtons(bool show)
{
	if(show)
	{
		m_pButtonExit->show();
		m_pButtonSwitch->show();
		m_pButtonDebug->show();
	} else {
		m_pButtonExit->hide();
		m_pButtonSwitch->hide();
		m_pButtonDebug->hide();
	}
}
