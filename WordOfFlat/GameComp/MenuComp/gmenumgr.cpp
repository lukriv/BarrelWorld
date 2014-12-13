#include "gmenumgr.h"

#include <MyGUI/MyGUI_PointerManager.h>
#include <MyGUI/MyGUI_Button.h>

GameMenuSystem::GameMenuSystem(GameLogger *pLogger) : m_pLogger(pLogger)
						, m_pGui(nullptr)
						, m_pGuiPlatform(nullptr)
						, m_actualViewportIndex(0) {}

GameMenuSystem::~GameMenuSystem()
{
	Uninitialize();
}

GameErrorCode GameMenuSystem::Initialize( Ogre::RenderWindow* pWindow, Ogre::SceneManager* pSceneManager)
{
	
	// initialize MyGUI
	FWG_RETURN_FAIL(GameNewChecked(m_pGuiPlatform));
	m_pGuiPlatform->initialise(pWindow, pSceneManager);
	
	FWG_RETURN_FAIL(GameNewChecked(m_pGui));
	m_pGui->initialise();
	
	return FWG_NO_ERROR;
}

void GameMenuSystem::SetActualViewportIndex(wxInt32 actViewportIndex)
{
	wxCriticalSectionLocker lock(m_systemLock);
	m_actualViewportIndex = actViewportIndex;
}

wxInt32 GameMenuSystem::GetActualViewportIndex()
{ 
	wxCriticalSectionLocker lock(m_systemLock);
	return m_actualViewportIndex; 
}

void GameMenuSystem::AddMenu(GameMenuBase* pMenu)
{	
	TMenuList::iterator iter;
	wxCriticalSectionLocker lock(m_systemLock);
	
	iter = m_menuList.Find(pMenu);
	if(iter == m_menuList.end())
	{
		m_menuList.insert(pMenu);
	}
}

void GameMenuSystem::RemoveMenu(GameMenuBase* pMenu)
{
	TMenuList::iterator iter;
	wxCriticalSectionLocker lock(m_systemLock);
	
	iter = m_menuList.Find(pMenu);
	if(iter != m_menuList.end())
	{
		m_menuList.erase(iter);
	}
}

GameErrorCode GameMenuSystem::SwitchMenu(GameMenuBase* pMenu)
{
	TMenuList::iterator iter;
	wxCriticalSectionLocker lock(m_systemLock);
	
	for(iter = m_menuList.begin(); iter != m_menuList.end(); iter++)
	{
		if(*iter != pMenu) 
		{
			(*iter)->Destroy();
		}
	}
	
	m_pGuiPlatform->getRenderManagerPtr()->setActiveViewport(m_actualViewportIndex);
	
	return FWG_NO_ERROR;
}

void GameMenuSystem::Uninitialize()
{
	if(m_pGui)
	{
		m_pGui->shutdown();
		delete m_pGui;
		m_pGui = nullptr;
	}
	
	if(m_pGuiPlatform)
	{
		m_pGuiPlatform->shutdown();
		delete m_pGuiPlatform;
		m_pGuiPlatform = nullptr;
	}
}

