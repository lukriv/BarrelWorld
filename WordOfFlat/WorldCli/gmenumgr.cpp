#include "gmenumgr.h"

#include <MyGUI/MyGUI_PointerManager.h>
#include <MyGUI/MyGUI_Button.h>
#include "gclientlogic.h"

GameMenuResources::GameMenuResources() : m_pGui(NULL), m_pGuiPlatform(NULL)
{}

GameMenuResources::~GameMenuResources()
{
	if(m_pGui)
	{
		m_pGui->shutdown();
		delete m_pGui;
		m_pGui = NULL;
	}
	
	if(m_pGuiPlatform)
	{
		m_pGuiPlatform->shutdown();
		delete m_pGuiPlatform;
		m_pGuiPlatform = NULL;
	}	
}

GameErrorCode GameMenuResources::Initialize(GameLogger* pLogger, Ogre::RenderWindow* pWindow, Ogre::SceneManager* pSceneManager)
{
	
	m_pLogger = pLogger;
	
	// initialize MyGUI
	FWG_RETURN_FAIL(GameNewChecked(m_pGuiPlatform));
	m_pGuiPlatform->initialise(pWindow, pSceneManager);
	
	FWG_RETURN_FAIL(GameNewChecked(m_pGui));
	m_pGui->initialise();
	
	return FWG_NO_ERROR;
}

void GameMenuResources::AddMenu(GameMenuBase* pMenu)
{	
	TMenuList::iterator iter;
	iter = m_menuList.find(pMenu);
	if(iter == m_menuList.end())
	{
		m_menuList.insert(pMenu);
	}
}

void GameMenuResources::RemoveMenu(GameMenuBase* pMenu)
{
	TMenuList::iterator iter;
	iter = m_menuList.find(pMenu);
	if(iter != m_menuList.end())
	{
		m_menuList.erase(iter);
	}
}

GameErrorCode GameMenuResources::SetGuiVisible(GameMenuBase* pMenu, wxInt32 actualVieportIndex)
{
	TMenuList::iterator iter;
	for(iter = m_menuList.begin(); iter != m_menuList.end(); iter++)
	{
		if(*iter != pMenu) 
		{
			(*iter)->Destroy();
		}
	}
	
	m_pGuiPlatform->getRenderManagerPtr()->setActiveViewport(0);
	
	return FWG_NO_ERROR;
}




GameMenuBase::~GameMenuBase()
{
	m_spMenuRes.In()->RemoveMenu(this);
	m_spMenuRes.Release();
}

GameMenuBase::GameMenuBase(GameMenuResources* pMenuRes) : m_spMenuRes(pMenuRes)
{
	m_spMenuRes.In()->AddMenu(this);
}


GameErrorCode GameMenuBase::SetVisible(bool visibility, wxInt32 actualViewportIndex)
{
	if(visibility == true)
	{
		m_spMenuRes.In()->SetGuiVisible(this, actualViewportIndex);
		Create();
	} else {
		Destroy();
	}
	
	return FWG_NO_ERROR;
}

