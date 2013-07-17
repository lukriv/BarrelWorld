#include "gresholder.h"

#include <wx/scopedptr.h>
#include <sfml/system/Lock.hpp>
#include "../GameLoader/gtestloader.h"

GameResourceHolder* GameResourceHolder::g_pResourceHolder = NULL;

GameErrorCode GameResourceHolder::Initialize(GameLogger* pLogger, IGameResourceLoader* pResLoader)
{
	GameErrorCode result = FWG_NO_ERROR;
	if (pResLoader == NULL) {
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	m_pResLoader = pResLoader;
	
	m_spLogger = pLogger;

	if(FWG_FAILED(result = LoadResourceMaps()))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameResourceHolder::Initialize() : Load resource maps failed: 0x%08x"),
			pLogger, result, FWGLOG_ENDVAL);
		ClearResourceMaps();
		return result;
	}
		
	m_isInitialized = true;
	
	return result;
}

void GameResourceHolder::ClearResourceMaps()
{
	sf::Lock locker(m_resouceLocker);
	//texture map
	TGameTextureMap::iterator texIter;
	for (texIter = m_texMap.begin(); texIter != m_texMap.end(); texIter++)
	{
		if (texIter->second.m_pTexture != NULL) {
			delete texIter->second.m_pTexture;
			texIter->second.m_pTexture = NULL;
		}
		
		texIter->second.m_texFileName.Clear();
	}
	m_texMap.clear();
	
	//geometry map
	TGameGeometryMap::iterator geomIter;
	for (geomIter = m_geomMap.begin(); geomIter != m_geomMap.end(); geomIter++)
	{
		if (geomIter->second.m_pGeometry != NULL) {
			delete geomIter->second.m_pGeometry;
			geomIter->second.m_pGeometry = NULL;
		}
	}
	m_geomMap.clear();
	
	//joint map
	TGamePhysJointMap::iterator jointIter;
	for (jointIter = m_physJointMap.begin(); jointIter != m_physJointMap.end(); jointIter++)
	{
		if (jointIter->second.m_pJointDef != NULL) {
			delete jointIter->second.m_pJointDef;
			jointIter->second.m_pJointDef = NULL;
		}
	}
	m_physJointMap.clear();
	
	//body map
	TGamePhysBodyMap::iterator bodyIter;
	for (bodyIter = m_physBodyMap.begin(); bodyIter != m_physBodyMap.end(); bodyIter++)
	{
		if (bodyIter->second.m_pBodyDef != NULL) {
			delete bodyIter->second.m_pBodyDef;
			bodyIter->second.m_pBodyDef = NULL;
		}
		
		bodyIter->second.m_fixtureRefList.clear();
		
	}
	m_physBodyMap.clear();
	
	//fixture map
	TGamePhysFixMap::iterator fixIter;
	for (fixIter = m_physFixMap.begin(); fixIter != m_physFixMap.end(); fixIter++)
	{
		if (fixIter->second.m_pFixtureDef != NULL) {
			delete fixIter->second.m_pFixtureDef;
			fixIter->second.m_pFixtureDef = NULL;
		}
	}
	m_physFixMap.clear();
	
}

GameErrorCode GameResourceHolder::LoadResourceMaps()
{
	GameErrorCode result = FWG_NO_ERROR;
	if (FWG_FAILED(result = m_pResLoader->LoadTextureList(m_texMap))) {
		FWGLOG_ERROR_FORMAT(wxT("GameResourceHolder::LoadResourceMaps(): Load texture map failed: 0x%08x"),
				m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if (FWG_FAILED(result = m_pResLoader->LoadGeometryList(m_geomMap))) {
		FWGLOG_ERROR_FORMAT(wxT("GameResourceHolder::LoadResourceMaps(): Load geometry map failed: 0x%08x"),
				m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if (FWG_FAILED(result = m_pResLoader->LoadPhysJointList(m_physJointMap))) {
		FWGLOG_ERROR_FORMAT(wxT("GameResourceHolder::LoadResourceMaps(): Load joint map failed: 0x%08x"),
				m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if (FWG_FAILED(result = m_pResLoader->LoadPhysBodyList(m_physBodyMap))) {
		FWGLOG_ERROR_FORMAT(wxT("GameResourceHolder::LoadResourceMaps(): Load body map failed: 0x%08x"),
				m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if (FWG_FAILED(result = m_pResLoader->LoadPhysFixList(m_physFixMap))) {
		FWGLOG_ERROR_FORMAT(wxT("GameResourceHolder::LoadResourceMaps(): Load fixture map failed: 0x%08x"),
				m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	return result;
}

IGameGeometry* GameResourceHolder::GetGeometry(GameShapeId geomID)
{
	TGameGeometryMap::iterator iter;
	GameErrorCode result = FWG_NO_ERROR;
	
	sf::Lock locker(m_resouceLocker);
	
	//find geometry
	iter = m_geomMap.find(geomID);
	if (iter == m_geomMap.end()) return NULL;
	if (iter->second.m_pGeometry == NULL)
	{
		if(FWG_FAILED(result = m_pResLoader->LoadGeometry(geomID, iter->second.m_pGeometry))){
			FWGLOG_ERROR_FORMAT(wxT("GameResourceHolder::GetGeometry() : Load geometry failed: 0x%08x"),
				m_spLogger, result, FWGLOG_ENDVAL);
			return NULL;
		}

	}
	// increment reference counter
	iter->second.m_refCount++;
	return iter->second.m_pGeometry;
}



sf::Texture* GameResourceHolder::GetTexture(GameTextureId texID)
{
	TGameTextureMap::iterator iter;
	GameErrorCode result = FWG_NO_ERROR;
	
	sf::Lock locker(m_resouceLocker);
	
	iter = m_texMap.find(texID);
	if (iter == m_texMap.end()) return NULL;
	if (iter->second.m_pTexture == NULL)
	{
		if(FWG_FAILED(result = m_pResLoader->LoadTextureFromFile(iter->second.m_texFileName, iter->second.m_pTexture)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameResourceHolder::GetTexture() : Load texture failed: 0x%08x"),
				m_spLogger, result, FWGLOG_ENDVAL);
			return NULL;
		}
	}
	// increment reference counter
	iter->second.m_refCount++;	
	return iter->second.m_pTexture;
}



void GameResourceHolder::ReleaseGeometry(GameShapeId geomID)
{
	TGameGeometryMap::iterator iter;
	GameErrorCode result = FWG_NO_ERROR;
	
	sf::Lock locker(m_resouceLocker);
	
	//find geometry
	iter = m_geomMap.find(geomID);
	if (iter == m_geomMap.end()) return;
	if (iter->second.m_pGeometry != NULL)
	{
		iter->second.m_refCount--;
		if (iter->second.m_refCount == 0)
		{
			delete iter->second.m_pGeometry;
			iter->second.m_pGeometry = NULL;
		}
	}
}

void GameResourceHolder::ReleaseTexture(GameTextureId texID)
{
	TGameTextureMap::iterator iter;
	GameErrorCode result = FWG_NO_ERROR;
	
	sf::Lock locker(m_resouceLocker);
	
	iter = m_texMap.find(texID);
	if (iter == m_texMap.end()) return;
	if (iter->second.m_pTexture != NULL)
	{
		iter->second.m_refCount--;
		if (iter->second.m_refCount == 0)
		{
			delete iter->second.m_pTexture;
			iter->second.m_pTexture = NULL;
		}
	}
}

GameErrorCode GameResourceHolder::SetLoader(IGameResourceLoader* pLoader)
{
	if(!m_isInitialized) {
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	GameErrorCode result = FWG_NO_ERROR;
	
	m_pResLoader = pLoader;
	ClearResourceMaps();
	if (FWG_FAILED(result = LoadResourceMaps())) {
		FWGLOG_ERROR_FORMAT(wxT("GameResourceHolder::SetLoader() : Load maps failed: 0x%08x"),
			m_spLogger, result, FWGLOG_ENDVAL);
		ClearResourceMaps();
		return result;
	}
	return FWG_NO_ERROR;
}

void GameResourceHolder::addRef()
{
	wxAtomicInc(m_refCount);
}

wxInt32 GameResourceHolder::release()
{
	wxInt32 refCount = wxAtomicDec(m_refCount);
	if (refCount == 0)
	{
		delete g_pResourceHolder;
		g_pResourceHolder = NULL;
	}
	
	return refCount;
}

GameResourceHolder::~GameResourceHolder()
{
	ClearResourceMaps();
}

GameErrorCode GameResourceHolder::GetBodyDef(GamePhysObjId bodyID, b2BodyDef*& pBodyDef, wxVector<GamePhysObjId>& fixtureList)
{
	TGamePhysBodyMap::iterator iter;
	GameErrorCode result = FWG_NO_ERROR;
	
	sf::Lock locker(m_resouceLocker);
	
	//find geometry
	iter = m_physBodyMap.find(bodyID);
	if (iter == m_physBodyMap.end()) return FWG_E_OBJECT_NOT_FOUND_ERROR;
	if (iter->second.m_pBodyDef == NULL)
	{
		if(FWG_FAILED(result = m_pResLoader->LoadPhysBody(bodyID, iter->second.m_pBodyDef))){
			FWGLOG_ERROR_FORMAT(wxT("GameResourceHolder::GetBodyDef() : Load body def failed: 0x%08x"),
				m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
	}
	// increment reference counter
	iter->second.m_refCount++;
	fixtureList = iter->second.m_fixtureRefList;
	pBodyDef = iter->second.m_pBodyDef;
	return FWG_NO_ERROR;
}

GameErrorCode GameResourceHolder::GetFixtureDef(GamePhysObjId fixID, b2FixtureDef*& pFixtureDef, GameShapeId& shapeID)
{
	TGamePhysFixMap::iterator iter;
	GameErrorCode result = FWG_NO_ERROR;
	
	sf::Lock locker(m_resouceLocker);
	
	//find geometry
	iter = m_physFixMap.find(fixID);
	if (iter == m_physFixMap.end()) return FWG_E_OBJECT_NOT_FOUND_ERROR;
	if (iter->second.m_pFixtureDef == NULL)
	{
		if(FWG_FAILED(result = m_pResLoader->LoadPhysFixture(fixID, iter->second.m_pFixtureDef))){
			FWGLOG_ERROR_FORMAT(wxT("GameResourceHolder::GetFixtureDef() : Load body def failed: 0x%08x"),
				m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
	}
	// increment reference counter
	iter->second.m_refCount++;
	shapeID = iter->second.m_shapeRef;
	pFixtureDef = iter->second.m_pFixtureDef;
	
	return FWG_NO_ERROR;
}

GameErrorCode GameResourceHolder::GetJointDef(GamePhysObjId jointID, b2JointDef*& pJointDef, wxVector<GamePhysObjId>& bodyList)
{
	TGamePhysJointMap::iterator iter;
	GameErrorCode result = FWG_NO_ERROR;
	
	sf::Lock locker(m_resouceLocker);
	
	//find geometry
	iter = m_physJointMap.find(jointID);
	if (iter == m_physJointMap.end()) return FWG_E_OBJECT_NOT_FOUND_ERROR;
	if (iter->second.m_pJointDef == NULL)
	{
		if(FWG_FAILED(result = m_pResLoader->LoadPhysJoint(jointID, iter->second.m_pJointDef))){
			FWGLOG_ERROR_FORMAT(wxT("GameResourceHolder::GetJointDef() : Load joint def failed: 0x%08x"),
				m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
	}
	// increment reference counter
	iter->second.m_refCount++;
	bodyList = iter->second.m_bodyRefList;
	pJointDef = iter->second.m_pJointDef;
	return FWG_NO_ERROR;
}

void GameResourceHolder::ReleaseBody(GamePhysObjId bodyID)
{
	TGamePhysBodyMap::iterator iter;
	GameErrorCode result = FWG_NO_ERROR;
	
	sf::Lock locker(m_resouceLocker);
	
	iter = m_physBodyMap.find(bodyID);
	if (iter == m_physBodyMap.end()) return;
	if (iter->second.m_pBodyDef != NULL)
	{
		iter->second.m_refCount--;
		if (iter->second.m_refCount == 0)
		{
			delete iter->second.m_pBodyDef;
			iter->second.m_pBodyDef = NULL;
		}
	}
}

void GameResourceHolder::ReleaseFixture(GamePhysObjId fixID)
{
	TGamePhysFixMap::iterator iter;
	GameErrorCode result = FWG_NO_ERROR;
	
	sf::Lock locker(m_resouceLocker);
	
	iter = m_physFixMap.find(fixID);
	if (iter == m_physFixMap.end()) return;
	if (iter->second.m_pFixtureDef != NULL)
	{
		iter->second.m_refCount--;
		if (iter->second.m_refCount == 0)
		{
			delete iter->second.m_pFixtureDef;
			iter->second.m_pFixtureDef = NULL;
		}
	}
}

void GameResourceHolder::ReleaseJoint(GamePhysObjId jointID)
{
	TGamePhysJointMap::iterator iter;
	GameErrorCode result = FWG_NO_ERROR;
	
	sf::Lock locker(m_resouceLocker);
	
	iter = m_physJointMap.find(jointID);
	if (iter == m_physJointMap.end()) return;
	if (iter->second.m_pJointDef != NULL)
	{
		iter->second.m_refCount--;
		if (iter->second.m_refCount == 0)
		{
			delete iter->second.m_pJointDef;
			iter->second.m_pJointDef = NULL;
		}
	}
}

//----------- Static methods --------------//

GameResourceHolder* GameResourceHolder::GetResourceHolder()
{
	return g_pResourceHolder;
}

GameErrorCode GameResourceHolder::InitializeResourceHolder(GameLogger* pLogger)
{
	GameErrorCode result = FWG_NO_ERROR;
	if (g_pResourceHolder != NULL) {
		return FWG_NO_ERROR;
	}
	
	wxScopedPtr<GameTestResourceLoader> apLoader;
	apLoader.reset(new (std::nothrow) GameTestResourceLoader());
	
	GameResourceHolder* pResHolder = NULL;
	pResHolder = new (std::nothrow) GameResourceHolder();
	
	if (FWG_FAILED(result = pResHolder->Initialize(pLogger, apLoader.get())))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameResourceHolder::InitializeResourceHolder() : Resource holder initialization failed: 0x%08x"),
						pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	//release pointer
	apLoader.release();
	
	g_pResourceHolder = pResHolder;
	
	return result;
}


void GameResourceHolder::ReleaseResourceHolder()
{
	if(g_pResourceHolder != NULL) {
		g_pResourceHolder->release();
		g_pResourceHolder = NULL;
	}
}



