#include "gtestloader.h"
#include <map>
#include <wx/scopedptr.h>
#include <sfml/Graphics.hpp>
#include "../GameObjects/gsfmlgeom.h"


GameErrorCode GameTestResourceLoader::Initialize(GameLogger* pLogger)
{
	if (m_isInitialized)
	{
		return FWG_NO_ERROR;
	}
	
	m_spLogger = pLogger;
	
	m_isInitialized = true;
	return FWG_NO_ERROR;
}

GameErrorCode GameTestResourceLoader::LoadGeometryList(TGameGeometryMap& geomList)
{
	TGameGeometryMapItem item;
	item.first = 1;
	geomList.insert(item);
	
	item.first = 2;
	geomList.insert(item);
	return FWG_NO_ERROR;
}

GameErrorCode GameTestResourceLoader::LoadGeometry(GameShapeId geomID, GameGeometryContainer*& pShape)
{	
	wxScopedPtr<GameGeometryContainer> apGeometry;
	pShape = NULL;
	
	apGeometry.reset(new (std::nothrow) GameGeometryContainer());
	if (apGeometry.get() == NULL) return FWG_E_MEMORY_ALLOCATION_ERROR;
	
	switch (geomID)
	{
	case 1:
		apGeometry->m_type = GAME_GEOM_QUADS;
		apGeometry->m_vertexes.resize(4);
		apGeometry->m_vertexes[0] = b2Vec2(-0.5f, -0.5f);
		apGeometry->m_vertexes[1] = b2Vec2(0.5f, -0.5f);
		apGeometry->m_vertexes[2] = b2Vec2(0.5f, 0.5f);
		apGeometry->m_vertexes[3] = b2Vec2(-0.5f, 0.5f);
		
		apGeometry->m_texCoords.resize(4);
		apGeometry->m_texCoords[0] = b2Vec2(0, 255);
		apGeometry->m_texCoords[1] = b2Vec2(255, 255);
		apGeometry->m_texCoords[2] = b2Vec2(255, 0);
		apGeometry->m_texCoords[3] = b2Vec2(0, 0);
		break;
	case 2:
		apGeometry->m_type = GAME_GEOM_QUADS;
		apGeometry->m_vertexes.resize(4);
		apGeometry->m_vertexes[0] = b2Vec2(-10, -0.5f);
		apGeometry->m_vertexes[1] = b2Vec2(10, -0.5f);
		apGeometry->m_vertexes[2] = b2Vec2(10, 0.5f);
		apGeometry->m_vertexes[3] = b2Vec2(-10, 0.5f);
		
		apGeometry->m_texCoords.resize(4);
		apGeometry->m_texCoords[0] = b2Vec2(0, 255);
		apGeometry->m_texCoords[1] = b2Vec2(5100, 255);
		apGeometry->m_texCoords[2] = b2Vec2(5100, 0);
		apGeometry->m_texCoords[3] = b2Vec2(0, 0);
		break;
	}
	
	pShape = apGeometry.release();
	
	return FWG_NO_ERROR;
}

GameErrorCode GameTestResourceLoader::LoadTexture(GameTextureId texID, sf::Texture*& pTexImage)
{
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}

GameErrorCode GameTestResourceLoader::LoadTextureList(TGameTextureMap& texList)
{
	TGameTextureMapItem item;
	item.first = 1;
	item.second.m_texFileName.assign(wxT("res/img/woodbox.png"));
	texList.insert(item);
	
	item.first = 2;
	item.second.m_texFileName.assign(wxT("res/img/ground.png"));
	texList.insert(item);
	
	item.first = 3;
	item.second.m_texFileName.assign(wxT("res/img/redball.png"));
	texList.insert(item);
	
	item.first = 4;
	item.second.m_texFileName.assign(wxT("res/img/redballempty.png"));
	texList.insert(item);
	
	item.first = 5;
	item.second.m_texFileName.assign(wxT("res/img/empty.png"));
	texList.insert(item);
	
	return FWG_NO_ERROR;
	
}

GameErrorCode GameTestResourceLoader::LoadTextureFromFile(const wxChar* texFileName, sf::Texture*& pTexImage)
{
	 // Load a sprite to display
	 wxScopedPtr<sf::Texture> apTexImage;
	 wxString fileName(texFileName);
	 apTexImage.reset(new (std::nothrow) sf::Texture());
	 std::string fileNameStr(fileName.c_str().AsChar());
	 if (!apTexImage->loadFromFile(fileNameStr))
	 {
		 FWGLOG_ERROR_FORMAT(wxT("GameTestResourceLoader::LoadTextureFromFile() : Load texture from \"%s\" failed: 0x%08x"),
					m_spLogger, texFileName, FWG_E_MISC_ERROR, FWGLOG_ENDVAL);
		 return FWG_E_MISC_ERROR;
	 }
	 
	 pTexImage = apTexImage.release();
	 
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
		break;
	case 2:
		apFixtureDef->friction = 0.7;
		apFixtureDef->restitution = 0.2;
		apFixtureDef->density = 0;
		break;
	default:
		return FWG_E_OBJECT_NOT_FOUND_ERROR;
	}
	
	pFixDef = apFixtureDef.release();
	
	return FWG_NO_ERROR;
}




