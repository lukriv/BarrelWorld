#include "gresloader.h"

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

GameErrorCode GameTestResourceLoader::LoadGeometryList(wxVector<TGameGeometryMapItem>& geomList)
{
	TGameGeometryMapItem item;
	item.first.assign(wxT("wood_box"));
	geomList.push_back(item);
	
	item.first.assign(wxT("ground"));
	geomList.push_back(item);
	return FWG_NO_ERROR;
}

GameErrorCode GameTestResourceLoader::LoadShape(const wxChar* geomName, IGameGeometry*& pShape)
{	wxScopedPtr<sf::VertexArray> apVertexArray;
	wxScopedPtr<GameSFMLGeometry> apGeometry;
	wxString geomNameStr(geomName);
	pShape = NULL;
	
	if (geomNameStr.Cmp(wxT("wood_box")) == 0)
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
	
	if (geomNameStr.Cmp(wxT("ground")) == 0)
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

GameErrorCode GameTestResourceLoader::LoadTexture(const wxChar* texName, sf::Texture*& pTex)
{
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}

GameErrorCode GameTestResourceLoader::LoadTextureList(wxVector<TGameTextureMapItem>& texList)
{
	TGameTextureMapItem item;
	item.first.assign(wxT("ground"));
	item.second.m_texFileName.assign(wxT("res/img/ground.png"));
	texList.push_back(item);
	
	item.first.assign(wxT("wood_box"));
	item.second.m_texFileName.assign(wxT("res/img/woodbox.png"));
	texList.push_back(item);
	
	return FWG_NO_ERROR;
	
}

GameErrorCode GameTestResourceLoader::LoadTextureFromFile(const wxChar* texFileName, sf::Texture*& pTex)
{
	 // Load a sprite to display
	 wxScopedPtr<sf::Texture> apTexture = nullptr;
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
