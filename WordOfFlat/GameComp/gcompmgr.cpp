#include "gcompmgr.h"

GameCompManager::GameCompManager(GameLogger *pLogger) : m_spLogger(pLogger)
														, m_tranformMgr(pLogger)
														, m_renderMgr(pLogger)
														, m_menuMgr(pLogger)
														, m_inputSystem(pLogger)
														, m_logicMgr(pLogger)
														, m_entityMgr(pLogger)
{}

GameCompManager::~GameCompManager() 
{
	Uninitialize();
}

GameErrorCode GameCompManager::Initialize(GameEngineSettings& settings)
{
	GameErrorCode result = FWG_NO_ERROR;
	
	if(FWG_FAILED(result = m_renderMgr.Initialize(settings)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Render manager initialize failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	} 
	
	FWGLOG_INFO(wxT("Render manager initialization was successful"), m_spLogger);
	
	if(FWG_FAILED(result = m_menuMgr.Initialize(m_renderMgr.GetOgreRenderWindow())))
	{
		FWGLOG_ERROR_FORMAT(wxT("Menu manager initialize failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	} 
	
	//FWGLOG_INFO(wxT("Menu system initialization was successful"), m_spLogger);
	
	if(FWG_FAILED(result = m_inputSystem.Initialize(m_renderMgr.GetOgreRenderWindow(), true)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Input system initialize failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	} 
	
	FWGLOG_INFO(wxT("Input system initialization was successful"), m_spLogger);
	
	//if(FWG_FAILED(result = m_logicMgr.Initialize(m_renderMgr.GetOgreRenderWindow(), m_renderMgr.GetOgreSceneManager())))
	//{
	//	FWGLOG_ERROR_FORMAT(wxT("Menu manager initialize failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
	//	return result;
	//} 
	
	//if(FWG_FAILED(result = m_entityMgr.Initialize(m_renderMgr.GetOgreRenderWindow(), m_renderMgr.GetOgreSceneManager())))
	//{
	//	FWGLOG_ERROR_FORMAT(wxT("Menu manager initialize failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
	//	return result;
	//} 

	return FWG_NO_ERROR;
}

void GameCompManager::Uninitialize()
{
	m_entityMgr.DestroyAllEntities();
	m_inputSystem.Uninitialize();
	m_menuMgr.Uninitialize();
	m_renderMgr.Uninitialize();
	
}
