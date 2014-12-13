#include "gmenubase.h"
#include "gmenumgr.h"

/////////////////////
//  GameMenuBase   //
/////////////////////


GameMenuBase::~GameMenuBase()
{
	if(m_pMenuRes)
	{
		m_pMenuRes->RemoveMenu(this);
		m_pMenuRes = nullptr;
	}
}

GameMenuBase::GameMenuBase() : m_pMenuRes(nullptr)
							, m_enabled(false) {}


GameErrorCode GameMenuBase::Enable()
{
	wxCriticalSectionLocker lock(m_menuLock);
	
	if((!m_enabled)&&(m_pMenuRes))
	{
		FWG_RETURN_FAIL(m_pMenuRes->SwitchMenu(this));
		FWG_RETURN_FAIL(Create());
		m_enabled = true;
	}
	
	return FWG_NO_ERROR;
}


GameErrorCode GameMenuBase::Disable()
{
	wxCriticalSectionLocker lock(m_menuLock);
	
	if(m_enabled)
	{
		FWG_RETURN_FAIL(Destroy());
		m_enabled = false;
	}
	
	return FWG_NO_ERROR;
}

void GameMenuBase::SetMenuSystem(GameMenuSystem* pMenuRes)
{
	wxCriticalSectionLocker lock(m_menuLock);
	m_pMenuRes = pMenuRes;
}
