#include "gterrainmgr.h"

#include <OGRE/Terrain/OgreTerrain.h>
#include <OGRE/Terrain/OgreTerrainGroup.h>

#include <bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <wx/xml/xml.h>

#include <GameXmlDefinitions/gxmldefs.h>
#include <GameXmlDefinitions/gxmlutils.h>
#include "RenderComp/grendercmgr.h"
#include "PhysicsComp/gphyssystem.h"

// chunk size must be 2^n
static const wxInt32 CHUNK_SHIFT = 6; // n
static const wxInt32 CHUNK_SIZE = 1<<CHUNK_SHIFT;
static const wxInt32 CHUNK_VERTICES = CHUNK_SIZE + 1;

struct TerrainPage : public RefObjectImpl<IRefObject> {
	wxString m_filename;
	wxInt32 m_pageX;
	wxInt32 m_pageY;
};

struct TerrainDef : public RefObjectImpl<IRefObject> {
	wxDword m_mapSize;
	float m_worldSize;
	wxVector< RefObjSmPtr<TerrainPage> > m_terrainPages;
};

GameTerrainManager::GameTerrainManager(GameLogger* pLogger) : m_spLogger(pLogger)
															, m_pRenderMgr(nullptr)
															, m_pPhysicsMgr(nullptr)
															, m_globalTerrainOptions(nullptr)
															, m_pTerrainGroup(nullptr)
{}

GameTerrainManager::~GameTerrainManager()
{
	Uninitialize();
}

GameErrorCode GameTerrainManager::Initialize(RenderSystem* pRenderMgr, PhysicsSystem* pPhysMgr)
{
	m_pRenderMgr = pRenderMgr;
	m_pPhysicsMgr = pPhysMgr;
	
	m_gridOrigin[0] = 0;
	m_gridOrigin[1] = 0;
	
	if(m_pRenderMgr)
	{
		
		FWG_RETURN_FAIL(PrepareTerrainGlobalOptions());
	}
	
	
	
	return FWG_NO_ERROR;
	
}

void GameTerrainManager::Uninitialize()
{
	if(m_pTerrainGroup)
	{
		m_pTerrainGroup->removeAllTerrains();
		OGRE_DELETE m_pTerrainGroup;
		m_pTerrainGroup = nullptr;
	}
	
	if(m_globalTerrainOptions)
	{
		OGRE_DELETE m_globalTerrainOptions;
		m_globalTerrainOptions = nullptr;
	}
	
	if(!m_physTerrainGrid.empty()&&(m_pPhysicsMgr))
	{
		TPhysTerrainGrid::iterator iter;
		for( iter = m_physTerrainGrid.begin(); iter != m_physTerrainGrid.end(); ++iter)
		{
			if(iter->m_pRigidBody != nullptr)
			{
				m_pPhysicsMgr->GetDynamicsWorld()->removeRigidBody(iter->m_pRigidBody);
				delete iter->m_pRigidBody;
				iter->m_pRigidBody = nullptr;
			}
			
			if(iter->m_pTerrainData != nullptr)
			{
				delete[] iter->m_pTerrainData;
				iter->m_pTerrainData = nullptr;
			}
		}
		
		m_physTerrainGrid.clear();
	}
}

GameErrorCode GameTerrainManager::CreateTerrainGroup(TerrainDef& terrainDef)
{
	if(m_pTerrainGroup != nullptr)
	{
		// more then one terrain group is not allowed
		return FWG_E_OBJECT_ALREADY_EXISTS_ERROR;
	}

	// grid size	
	wxInt32 gridMin[2] = {wxINT32_MAX, wxINT32_MAX};
	wxInt32 gridMax[2] = {wxINT32_MIN, wxINT32_MIN};
	wxInt32 gridSize[2] = { 0, 0};

	wxInt32 sideSize = 0;
	wxInt32 gridArraySize = 0;
	
	
	if(m_pRenderMgr)
	{
		
    
		
		// create new terrain group
		m_pTerrainGroup = OGRE_NEW Ogre::TerrainGroup(m_pRenderMgr->GetOgreSceneManager()
													, Ogre::Terrain::ALIGN_X_Z
													, terrainDef.m_mapSize
													, terrainDef.m_worldSize);
		m_pTerrainGroup->setFilenameConvention("testTerrain", "dat");										
		m_pTerrainGroup->setOrigin(Ogre::Vector3::ZERO);
		
		//FWG_RETURN_FAIL(PrepareTerrainGlobalOptions());
		
		// configure terrain defaults
		Ogre::Terrain::ImportData& defaultimp = m_pTerrainGroup->getDefaultImportSettings();
		defaultimp.terrainSize = terrainDef.m_mapSize;
		defaultimp.worldSize = terrainDef.m_worldSize;
		defaultimp.inputScale = 1; // due terrain.png is 8 bpp
		defaultimp.minBatchSize = 33;
		defaultimp.maxBatchSize = 65;
		
		defaultimp.layerList.resize(1);
		defaultimp.layerList[0].worldSize = 100;
		defaultimp.layerList[0].textureNames.push_back("grass.png");
		defaultimp.layerList[0].textureNames.push_back("grassnormal.png");
		//defaultimp.layerList.resize(1);
		//defaultimp.layerList[0].worldSize = 100;
		//defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
		//defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
		//defaultimp.layerList[1].worldSize = 30;
		//defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
		//defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
		//defaultimp.layerList[2].worldSize = 200;
		//defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
		//defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
		
	}
	
	wxVector< RefObjSmPtr<TerrainPage> >::iterator iter;
	//precount terrain grid size
	for (iter = terrainDef.m_terrainPages.begin(); iter != terrainDef.m_terrainPages.end(); ++iter)
	{
		if( (*iter)->m_pageX > gridMax[0] )
		{
			gridMax[0] = (*iter)->m_pageX;
		}
		
		if( (*iter)->m_pageX < gridMin[0] )
		{
			gridMin[0] = (*iter)->m_pageX;
		}
		
		if( (*iter)->m_pageY > gridMax[1] )
		{
			gridMax[1] = (*iter)->m_pageY;
		}
		
		if( (*iter)->m_pageY < gridMin[1] )
		{
			gridMin[1] = (*iter)->m_pageY;
		}
	}
	
	gridSize[0] = gridMax[0] - gridMin[0] + 1;
	gridSize[1] = gridMax[1] - gridMin[1] + 1;
	
	// compute gridArraySize
	sideSize = (terrainDef.m_mapSize - 1) >> CHUNK_SHIFT;
	gridArraySize = gridSize[0] * gridSize[1] * sideSize * sideSize;
	btScalar terrainScale = static_cast<btScalar>(terrainDef.m_worldSize) / static_cast<btScalar>(terrainDef.m_mapSize - 1);
	btScalar chunkWorldSize = static_cast<btScalar>(CHUNK_SIZE) * terrainScale;
	//btScalar halfChunkWorldSize = chunkWorldSize / 2.0f;
	
	
	FWGLOG_INFO_FORMAT(wxT("Ogre terrain grid min[ %d, %d], max[ %d, %d], size[ %d, %d]"), m_spLogger
						, gridMin[0], gridMin[1] 
						, gridMax[0], gridMax[1]
						, gridSize[0], gridSize[1]
						, FWGLOG_ENDVAL	);
						
	FWGLOG_INFO_FORMAT(wxT("Physics terrain grid min[ %d, %d], max[ %d, %d], size[ %d, %d]"), m_spLogger
						, gridMin[0], gridMin[1] 
						, gridMax[0], gridMax[1]
						, gridSize[0], gridSize[1]
						, FWGLOG_ENDVAL	);						
						
	FWGLOG_INFO_FORMAT(wxT("Terrain grid size ( %d ), terrain scale ( %.4f ), chunk world size ( %.4f), side number of physics terr. to ogre terr. ( %d )"), m_spLogger
						, gridArraySize
						, terrainScale
						, chunkWorldSize
						, sideSize
						, FWGLOG_ENDVAL	);
	
	for (iter = terrainDef.m_terrainPages.begin(); iter != terrainDef.m_terrainPages.end(); ++iter)
	{
		TerrainPage *pTerrPage = (*iter).In();
		
		wxScopedArray<float> apTerrain;
		float minH = 0.0f;
		float maxH = 0.0f;
				
		FWG_RETURN_FAIL( LoadTerrainData(terrainDef.m_terrainPages[0]->m_filename
						, terrainDef.m_mapSize
						, apTerrain
						, minH
						, maxH) );
		
		if(m_pRenderMgr)
		{
		   // import settings
		   	//Ogre::Image img;
			//img.load(terrainDef.m_terrainPages[0]->m_filename.ToStdString(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

			//m_pTerrainGroup->defineTerrain(pTerrPage->m_pageX, pTerrPage->m_pageY, &img);
			
			//Ogre::Terrain::ImportData impData;
			//
			//
			//
			//impData.terrainSize = terrainDef.m_mapSize;
			//impData.worldSize = terrainDef.m_worldSize;
			//impData.inputScale = 1.0; // due terrain.png
			//impData.inputFloat = pTerrainData;
			
			
			m_pTerrainGroup->defineTerrain(pTerrPage->m_pageX, pTerrPage->m_pageY, apTerrain.get());
			
			
			//m_pTerrainGroup->getTerrain(pTerrPage->m_pageX, pTerrPage->m_pageY)->prepare(impData);
		}
		
		if(m_pPhysicsMgr)		
		{
			
			// prepare physics grid - grid will be separated to the chunks
			m_physTerrainGrid.resize(gridArraySize);
			
			for (wxInt32 gridY = 0; gridY < sideSize; ++gridY )
			{
				for (wxInt32 gridX = 0; gridX < sideSize; ++gridX )
				{
					float minHeight = 1000000.0f;
					float maxHeight = -1000000.0f;
					float *pTerrainData = new (std::nothrow) float[CHUNK_VERTICES*CHUNK_VERTICES];
					wxInt32 gridIndex = gridSize[0]*sideSize*(gridY + sideSize*(pTerrPage->m_pageY - gridMin[1])) + gridX + sideSize*(pTerrPage->m_pageX - gridMin[0]);
					
					//FWGLOG_DEBUG_FORMAT(wxT("Grid slot[ %d, %d]: index = %d, origin[ %.4f, %.4f]"), m_spLogger
					//	, gridX
					//	, gridY
					//	, gridIndex
					//	, (static_cast<btScalar>(gridX - sideSize/2)+0.5f)*chunkWorldSize + static_cast<btScalar>(pTerrPage->m_pageX)*terrainDef.m_worldSize
					//	, (static_cast<btScalar>(gridY - sideSize/2)+0.5f)*chunkWorldSize + static_cast<btScalar>(pTerrPage->m_pageY)*terrainDef.m_worldSize
					//	, FWGLOG_ENDVAL);
					
					for (wxInt32 i = 0; i < CHUNK_VERTICES; ++i)
					{
						float *pTerrBegin = apTerrain.get() + terrainDef.m_mapSize*(terrainDef.m_mapSize-i-1-gridY*CHUNK_SIZE) + gridX*CHUNK_SIZE;
						memcpy( pTerrainData + CHUNK_VERTICES*i
								, pTerrBegin
								, CHUNK_VERTICES * sizeof(float));
						

						for (wxInt32 j = 0; j < CHUNK_VERTICES; ++j)
						{
							if (minHeight > pTerrBegin[j])
							{
								minHeight = pTerrBegin[j];
							}
							
							if (maxHeight < pTerrBegin[j])
							{
								maxHeight = pTerrBegin[j];
							}
						}
						
						
								
						
					}
					
					btHeightfieldTerrainShape *pTerrainShape = new btHeightfieldTerrainShape(CHUNK_VERTICES,
																						CHUNK_VERTICES,
																						pTerrainData,
																						1,
																						minHeight,
																						maxHeight,
																						1,
																						PHY_FLOAT,
																						true);
					btDefaultMotionState *state = new btDefaultMotionState();
					
					btScalar mass = 0;
					btVector3 localInertia(0, 0, 0);
					
					pTerrainShape->calculateLocalInertia(mass, localInertia);
					
					pTerrainShape->setLocalScaling( btVector3(terrainScale, 1, terrainScale) );
					
					pTerrainShape->setUseZigzagSubdivision(true);
					//pTerrainShape->setUseDiamondSubdivision(true);
					
					btRigidBody::btRigidBodyConstructionInfo info(mass, state, pTerrainShape, localInertia);
					
					btRigidBody *pBody = new btRigidBody(info);
					
					pBody->getWorldTransform().setOrigin(btVector3( (static_cast<btScalar>(gridX - sideSize/2)+0.5f)*chunkWorldSize + static_cast<btScalar>(pTerrPage->m_pageX)*terrainDef.m_worldSize
													, (maxHeight + minHeight)/2.0f 
													, (static_cast<btScalar>(gridY - sideSize/2)+0.5f)*chunkWorldSize + static_cast<btScalar>(-pTerrPage->m_pageY)*terrainDef.m_worldSize));
					
					PhysicsTerrainData terrData;
					terrData.m_pRigidBody = pBody;
					terrData.m_pTerrainData = pTerrainData;
					
					m_physTerrainGrid[ gridIndex ] = terrData;
					
					
					m_pPhysicsMgr->GetDynamicsWorld()->addRigidBody(pBody);
				}
			}
			
			
		}
		
		
	}
	
	if(m_pTerrainGroup)
	{
		m_pTerrainGroup->loadAllTerrains(true);
		m_pTerrainGroup->freeTemporaryResources();
	}
	
	//{
	//	Ogre::Terrain *pTerrain = m_pTerrainGroup->getTerrain( 0, 0);
	//	wxDword maxSize = pTerrain->getSize() * pTerrain->getSize();
	//	float * pTerrData = pTerrain->getHeightData();
	//	
	//	// initialize min and max
	//	float minHeight = 100000; 
	//	float maxHeight = -100000;
	//	
	//	FWGLOG_DEBUG_FORMAT(wxT("Terrain size = %u"), m_spLogger, pTerrain->getSize(), FWGLOG_ENDVAL);
	//	
	//	for (wxDword i = 0; i < maxSize ; ++i)
	//	{
	//		if (minHeight > pTerrData[i])
	//		{
	//			minHeight = pTerrData[i];
	//		}
	//		
	//		if (maxHeight < pTerrData[i])
	//		{
	//			maxHeight = pTerrData[i];
	//		}
	//		FWGLOG_DEBUG_FORMAT(wxT("TerrainData[%u] = %.4f"), m_spLogger, i, pTerrData[i], FWGLOG_ENDVAL);
	//	}
	//	
	//	FWGLOG_DEBUG_FORMAT(wxT("TerrainData min = %.4f, max = %.4f"), m_spLogger, minHeight, maxHeight, FWGLOG_ENDVAL);
	//}
	
	
	
	return FWG_NO_ERROR;
}


GameErrorCode GameTerrainManager::LoadTerrainData( const wxString& resource, wxDword terrDataSize, wxScopedArray<float> &retData, float &minHeight, float &maxHeight)
{
	Ogre::Image img;
	img.load(resource.ToStdString(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	
	if (( img.getHeight() != terrDataSize) || (img.getWidth() != terrDataSize))
	{
		FWGLOG_ERROR_FORMAT(wxT("Invalid image size: %u x %u (expected %u x %u)"), m_spLogger
				, img.getHeight()
				, img.getWidth()
				, terrDataSize
				, terrDataSize
				, FWGLOG_ENDVAL);
		return FWG_E_INVALID_DATA_ERROR;
	} else {
		FWGLOG_DEBUG_FORMAT(wxT("Image size: %u x %u"), m_spLogger
				, img.getHeight()
				, img.getWidth()
				, FWGLOG_ENDVAL);
	}
	
	Ogre::uchar *pImgData = img.getData();
	
	wxScopedArray<float> apFloat(new (std::nothrow) float[terrDataSize*terrDataSize]);
	
	FWGLOG_DEBUG_FORMAT(wxT("Image size = %u, dataSize = %u"), m_spLogger, img.getSize(), (terrDataSize*terrDataSize), FWGLOG_ENDVAL);
	
	// initialize min and max
	minHeight = 100000; 
	maxHeight = -100000;
	
	if(apFloat.get() == nullptr)
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	
	for (wxDword i = 0; i < img.getSize() ; ++i)
	{
		float temp = (static_cast<float>(pImgData[i])/255.0f) * 100.0f;
		apFloat[i] = temp;
		if (minHeight > temp)
		{
			minHeight = temp;
		}
		
		if (maxHeight < temp)
		{
			maxHeight = temp;
		}
		
		//FWGLOG_DEBUG_FORMAT(wxT("TerrainData[%u] = %.4f, imgData[%u] = %u"), m_spLogger, i, pFloat[i], i, pImgData[i], FWGLOG_ENDVAL);
	}
	
	retData.swap(apFloat);
	
	FWGLOG_DEBUG_FORMAT(wxT("TerrainData min = %.4f, max = %.4f"), m_spLogger, minHeight, maxHeight, FWGLOG_ENDVAL);
	
	return FWG_NO_ERROR;
}

GameErrorCode GameTerrainManager::PrepareTerrainGlobalOptions()
{
	
	m_globalTerrainOptions = OGRE_NEW Ogre::TerrainGlobalOptions();
	
	
	Ogre::Vector3 lightdir(0.55, -0.3, 0.75);
	lightdir.normalise();
    
    
	Ogre::Light* l = m_pRenderMgr->GetOgreSceneManager()->createLight("tstLight");
	l->setType(Ogre::Light::LT_DIRECTIONAL);
	l->setDirection(lightdir);
	l->setDiffuseColour(Ogre::ColourValue::White);
	l->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));
	
	
	m_globalTerrainOptions->setMaxPixelError(4);
	
	// lightmapped terrain distance
	m_globalTerrainOptions->setCompositeMapDistance(3000);
	
	// set ambient color
	m_globalTerrainOptions->setLightMapDirection(l->getDerivedDirection());
	m_globalTerrainOptions->setCompositeMapAmbient(m_pRenderMgr->GetOgreSceneManager()->getAmbientLight());
	m_globalTerrainOptions->setCompositeMapDiffuse(l->getDiffuseColour());
	
	return FWG_NO_ERROR;

}

GameErrorCode GameTerrainManager::Load(wxXmlNode* pNode)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<TerrainDef> spTerrainDef;
	if(FWG_FAILED(result = CreateTerrainDef(pNode, spTerrainDef)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Reading terrain definitions failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = CreateTerrainGroup(*spTerrainDef)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create terrain group failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	return result;
	
}


GameErrorCode GameTerrainManager::CreateTerrainDef(wxXmlNode* pNode, RefObjSmPtr<TerrainDef>& spTerrain)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<TerrainDef> spTerrainTemp;
	
	wxString tempParam;
	
	FWG_RETURN_FAIL( GameNewChecked(spTerrainTemp.OutRef()));
	
	wxXmlNode *child = pNode->GetChildren();
	while (child)
	{
		if (child->GetName() == GAME_TAG_COMP_TERRAIN_PAGES) 
		{
			// create terrain pages
			wxXmlNode *pTerrainPages = child->GetChildren();
			
			while(pTerrainPages)
			{
				if(pTerrainPages->GetName() == GAME_TAG_COMP_TERRAIN_PAGE)
				{
					RefObjSmPtr<TerrainPage> spTerrainPage;
					FWG_RETURN_FAIL(GameNewChecked(spTerrainPage.OutRef()));
					if(FWG_FAILED(result = CreateTerrainPageDef(pTerrainPages, spTerrainPage)))
					{
						FWGLOG_ERROR_FORMAT(wxT("Create terrain page failed on line: %d with error: 0x%08x"),
									m_spLogger,
									pNode->GetLineNumber(),
									result,
									FWGLOG_ENDVAL);
						return result;
					}
					// add terrain page
					spTerrainTemp->m_terrainPages.push_back(spTerrainPage);
				} else if(pTerrainPages->GetType() != wxXML_COMMENT_NODE) {
					FWGLOG_ERROR_FORMAT(wxT("Unknown tag within tag ['%s'] on line: %d"),
									m_spLogger,
									pNode->GetName().GetData().AsInternal(),
									pNode->GetLineNumber(),
									FWGLOG_ENDVAL);
					return FWG_E_XML_INVALID_TAG_ERROR;
				}
				pTerrainPages = pTerrainPages->GetNext();
			}
		} else if(child->GetName() == GAME_PARAM_TERRAIN_MAPSIZE) {
			// mapsize
			wxInt32 tempInt = 0;
			FWG_RETURN_FAIL(GameXmlUtils::GetNodeContent(child, tempParam, m_spLogger));
			FWG_RETURN_FAIL(GameXmlUtils::ConvertToInt32(tempParam, tempInt));
			
			spTerrainTemp->m_mapSize = static_cast<wxDword>(tempInt);
			
		} else if(child->GetName() == GAME_PARAM_TERRAIN_WORLDSIZE) {
			// worldsize
			FWG_RETURN_FAIL(GameXmlUtils::GetNodeContent(child, tempParam, m_spLogger));
			FWG_RETURN_FAIL(GameXmlUtils::ConvertToFloat(tempParam, spTerrainTemp->m_worldSize));
		} else {
			GameXmlUtils::ProcessUnknownTag(child, m_spLogger);
		}
		
		child = child->GetNext();
		
	}
	
	// set return value
	spTerrain = spTerrainTemp;
	
	return FWG_NO_ERROR;
}

GameErrorCode GameTerrainManager::CreateTerrainPageDef(wxXmlNode* pNode, RefObjSmPtr<TerrainPage>& spTerrainPage)
{
	//GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<TerrainPage> spTerrainPageTemp;
	
	wxString tempParam;
	
	FWG_RETURN_FAIL( GameNewChecked(spTerrainPageTemp.OutRef()));
	
	wxXmlNode *child = pNode->GetChildren();
	while (child)
	{
		if (child->GetName() == GAME_PARAM_FILENAME) 
		{
			// filename
			FWG_RETURN_FAIL(GameXmlUtils::GetNodeContent(child, spTerrainPageTemp->m_filename, m_spLogger));
		} else if(child->GetName() == GAME_PARAM_TERRAIN_PAGEX) {
			// page x
			FWG_RETURN_FAIL(GameXmlUtils::GetNodeContent(child, tempParam, m_spLogger));
			FWG_RETURN_FAIL(GameXmlUtils::ConvertToInt32(tempParam, spTerrainPageTemp->m_pageX));
			
			
		} else if(child->GetName() == GAME_PARAM_TERRAIN_PAGEY) {
			// page y
			FWG_RETURN_FAIL(GameXmlUtils::GetNodeContent(child, tempParam, m_spLogger));
			FWG_RETURN_FAIL(GameXmlUtils::ConvertToInt32(tempParam, spTerrainPageTemp->m_pageY));
		} else {
			GameXmlUtils::ProcessUnknownTag(child, m_spLogger);
		}
		
		child = child->GetNext();
		
	}
	
	// set return value
	spTerrainPage = spTerrainPageTemp;
	
	return FWG_NO_ERROR;
}


GameErrorCode GameTerrainManager::Store(wxXmlNode* pParentNode)
{
	FWGLOG_WARNING(wxT("Terrain storing is not implemented yet!"), m_spLogger);
	return FWG_NO_ERROR;
}
