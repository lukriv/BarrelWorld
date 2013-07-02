#include "gresloader.h"
#include <Box2D/Box2D.h>

GameErrorCode GameTestResourceLoader::Initialize(GameLogger* pLogger)
{
	if (m_isInitialized)
	{
		return FWG_NO_ERROR;
	}
	
	m_spLogger.Attach(pLogger);
	
	m_isInitialized = true;
	return FWG_NO_ERROR;
}

GameErrorCode GameTestResourceLoader::LoadGeometryList(TGameGeometryMap& geomList)
{
	TGameGeometryMapItem item;
	item.first.assign(wxT("wood_box"));
	geomList.insert(item);
	
	item.first.assign(wxT("ground"));
	geomList.insert(item);
	return FWG_NO_ERROR;
}

GameErrorCode GameTestResourceLoader::LoadGeometry(GameShapeId geomID, IGameGeometry*& pShape)
{	
	wxScopedPtr<sf::VertexArray> apVertexArray;
	wxScopedPtr<GameSFMLGeometry> apGeometry;
	pShape = NULL;
	
	if (geomID == 1)
	{
		apVertexArray.reset(new (std::nothrow) sf::VertexArray(sf::Quads, 4));
		(*apVertexArray)[0].position = sf::Vector2f(-0.5f, -0.5f);
		(*apVertexArray)[1].position = sf::Vector2f(0.5f, -0.5f);
		(*apVertexArray)[2].position = sf::Vector2f(0.5f, 0.5f);
		(*apVertexArray)[3].position = sf::Vector2f(-0.5f, 0.5f);
		
		(*apVertexArray)[0].texCoords = sf::Vector2f(0, 0);
		(*apVertexArray)[1].texCoords = sf::Vector2f(256, 0);
		(*apVertexArray)[2].texCoords = sf::Vector2f(256, 256);
		(*apVertexArray)[3].texCoords = sf::Vector2f(0, 256);
		
		apGeometry = new (std::nothrow) GameSFMLGeometry(apVertexArray.release());
		if (apGeometry.get() == NULL) return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	if (geomID == 2)
	{
		apVertexArray.reset(new (std::nothrow) sf::VertexArray(sf::Quads, 4));
		(*apVertexArray)[0].position = sf::Vector2f(-10, -0.5f);
		(*apVertexArray)[1].position = sf::Vector2f(10, -0.5f);
		(*apVertexArray)[2].position = sf::Vector2f(10, 0.5f);
		(*apVertexArray)[3].position = sf::Vector2f(-10, 0.5f);
		
		(*apVertexArray)[0].texCoords = sf::Vector2f(0, 0);
		(*apVertexArray)[1].texCoords = sf::Vector2f(256, 0);
		(*apVertexArray)[2].texCoords = sf::Vector2f(256, 256);
		(*apVertexArray)[3].texCoords = sf::Vector2f(0, 256);
		
		apGeometry = new (std::nothrow) GameSFMLGeometry(apVertexArray.release());
		if (apGeometry.get() == NULL) return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	pShape = apGeometry.release();
	
	return FWG_NO_ERROR;
}

GameErrorCode GameTestResourceLoader::LoadTexture(GameTextureId texID, sf::Texture*& pTex)
{
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}

GameErrorCode GameTestResourceLoader::LoadTextureList(TGameTextureMap& texList)
{
	TGameTextureMapItem item;
	item.first = 1;
	item.second.m_texFileName.assign(wxT("res/img/ground.png"));
	texList.insert(item);
	
	item.first = 2;
	item.second.m_texFileName.assign(wxT("res/img/woodbox.png"));
	texList.insert(item);
	
	return FWG_NO_ERROR;
	
}

GameErrorCode GameTestResourceLoader::LoadTextureFromFile(const wxChar* texFileName, sf::Texture*& pTex)
{
	 // Load a sprite to display
	 wxScopedPtr<sf::Texture> apTexture;
	 apTexture.reset(new (std::nothrow) sf::Texture());
	 std::string fileNameStr();
	 fileNameStr.assign(texFileName);
	 if (!apTexture->loadFromFile(fileNameStr))
	 {
		 FWGLOG_ERROR_FORMAT(wxT("GameTestResourceLoader::LoadTextureFromFile() : Load texture from \"%s\" failed: 0x%08x"),
					spLogger, texFileName, FWG_E_MISC_ERROR, FWGLOG_ENDVAL);
		 return FWG_E_MISC_ERROR;
	 }
	 
	 return FWG_NO_ERROR;
}

GameErrorCode GameTestResourceLoader::LoadPhysJointList(TGamePhysJointMap& physJointList)
{
	FWG_UNREFERENCED_PARAMETER(physJointList);
	return FWG_NO_ERROR;
}

GameErrorCode GameTestResourceLoader::LoadPhysBodyList(TGamePhysBodyMap& physBodyList)
{
	TGamePhysBodyMapItem item;
	item.first = 1;
	item.second.m_fixtureRefList.push_back(1);
	physBodyList.insert(item);
	
	item.second.m_fixtureRefList.clear();
	
	item.first = 2;
	item.second.m_fixtureRefList.push_back(2);
	physBodyList.insert(item);
	
	return FWG_NO_ERROR;
}

GameErrorCode GameTestResourceLoader::LoadPhysFixList(TGamePhysFixMap& physFixList)
{
	TGamePhysFixMapItem item;
	item.first = 1;
	item.second.m_shapeRef = 1;
	physFixList.insert(item);
	
	item.first = 2;
	item.second.m_shapeRef = 2;
	physFixList.insert(item);
	
	return FWG_NO_ERROR;
}

GameErrorCode GameTestResourceLoader::LoadPhysJoint(GamePhysObjId jointID, b2JointDef*& pJointDef)
{
	FWG_UNREFERENCED_PARAMETER(jointID);
	pJointDef = NULL;
	return FWG_NO_ERROR;
}

GameErrorCode GameTestResourceLoader::LoadPhysBody(GamePhysObjId bodyID, b2BodyDef*& pBodyDef)
{
	wxScopedPtr<b2BodyDef> apBodyDef;
	pBodyDef = NULL;
	
	apBodyDef.reset(new (std::nothrow) b2BodyDef());
	if (apBodyDef.get() == NULL)
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	switch (bodyID)
	{
	case 1:
		apBodyDef->type = b2_dynamicBody;
		break;
	case 2: 
		apBodyDef->type = b2_staticBody;
		break;
	default:
		return FWG_E_OBJECT_NOT_FOUND_ERROR;
	}
	
	pBodyDef = apBodyDef.release();
	
	return FWG_NO_ERROR;

}

GameErrorCode GameTestResourceLoader::LoadPhysFixture(GamePhysObjId fixID, b2FixtureDef*& pFixDef)
{
	wxScopedPtr<b2FixtureDef> apFixtureDef;
	pFixDef = NULL;
	
	apFixtureDef.reset(new (std::nothrow) b2FixtureDef());
	if (apFixtureDef.get() == NULL)
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	switch (fixID)
	{
	case 1:
		apFixtureDef->friction = 0.5;
		apFixtureDef->restitution = 0.2;
		apFixtureDef->density = 100;
	case 2:
		apFixtureDef->friction = 0.7;
		apFixtureDef->restitution = 0.2;
		apFixtureDef->density = 0;
	default:
		return FWG_E_OBJECT_NOT_FOUND_ERROR;
	}
	
	pFixDef = apFixtureDef.release();
	
	return FWG_NO_ERROR;
}




