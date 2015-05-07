#include "grenderTerrDec.h"


#include <OGRE/OgreManualObject.h>
#include "grendercmgr.h"

RenderTerrainDecal::RenderTerrainDecal(RenderCompManager *pRenderMgr) : 
							RenderComponent(GAME_COMP_RENDER_TERRAIN_DECAL, pRenderMgr)
							, m_pManObject(nullptr)
							, m_pRayQuery(nullptr)
{
}

RenderTerrainDecal::~RenderTerrainDecal()
{
}

GameErrorCode RenderTerrainDecal::Initialize()
{
	Ogre::Ray verticalRay( Ogre::Vector3(0, 5000, 0), Ogre::Vector3::NEGATIVE_UNIT_Y );
	m_pRayQuery = m_pOwnerManager->GetOgreSceneManager()->createRayQuery(verticalRay);
	return FWG_NO_ERROR;
}

Ogre::Real RenderTerrainDecal::GetTerrainHeight(Ogre::Real x, Ogre::Real z)
{
	Ogre::Ray verticalRay( Ogre::Vector3(x, 5000, z), Ogre::Vector3::NEGATIVE_UNIT_Y );
	m_pRayQuery->setRay( verticalRay );
	
	// Execute query
	Ogre::RaySceneQueryResult &result = m_pRayQuery->execute();
	Ogre::RaySceneQueryResult::iterator itr = result.begin( );
	
	if ( itr != result.end() && itr->worldFragment )
	{
		Ogre::Vector3 intersection = itr->worldFragment->singleIntersection;
		return intersection.y;
	} else {
		return 0;
	}  
}

void RenderTerrainDecal::SetNewPosition(Ogre::Real x, Ogre::Real z)
{
	Ogre::Real rad = 1.0f;
	Ogre::Real x1 = x - rad;
	Ogre::Real z1 = z - rad;
	
	int x_size = 4;  // number of polygons
	int z_size = 4;
	
	Ogre::Real x_step = rad * 2/ x_size;
	Ogre::Real z_step = rad * 2/ z_size;
	
	m_pManObject->beginUpdate(0);
	// redefine vertices
	for (int i=0; i<=x_size; i++)
	{
	for (int j=0; j<=z_size; j++)
	{
		m_pManObject->position(Ogre::Vector3(x1, GetTerrainHeight(x1, z1) + 1, z1));
		m_pManObject->textureCoord((float)i / (float)x_size, (float)j / (float)z_size);
		z1 += z_step;
	}
	x1 += x_step;
	z1 = z - rad;
	}
	// redefine quads
	for (int i=0; i<x_size; i++)
	{
	for (int j=0; j<z_size; j++)
	{
		m_pManObject->quad( i * (x_size+1) + j,
					i * (x_size+1) + j + 1,
					(i + 1) * (x_size+1) + j + 1,
					(i + 1) * (x_size+1) + j);
	}
	}
	m_pManObject->end();
}


GameErrorCode RenderTerrainDecal::Create(const wxString& materialName)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxCriticalSectionLocker renderLock(m_renderLock);
	//TODO: create terrain decal manual object
	
	if(m_pManObject != nullptr)
	{
		FWGLOG_WARNING(wxT("Object is already created"), m_pOwnerManager->GetLogger());
		return FWG_E_OBJECT_ALREADY_EXISTS_ERROR;
	}
	
	m_pManObject = m_pOwnerManager->GetOgreSceneManager()->createManualObject();
	
	
	
	wxInt32 x_size = 4;  // number of polygons
	wxInt32 z_size = 4;
	
	m_pManObject->begin(materialName.ToStdString(), Ogre::RenderOperation::OT_TRIANGLE_LIST);
	for (wxInt32 i=0; i <= x_size; ++i)
	{
		for (wxInt32 j=0; j <= z_size; ++j)
		{
			m_pManObject->position(Ogre::Vector3(i, 0, j));
			m_pManObject->textureCoord((float)i / (float)x_size, (float)j / (float)z_size);
		}
	}
	
	for (wxInt32 i=0; i<x_size; ++i)
	{
		for (wxInt32 j=0; j<z_size; ++j)
		{
			m_pManObject->quad( i * (x_size+1) + j,
						i * (x_size+1) + j + 1,
						(i + 1) * (x_size+1) + j + 1,
						(i + 1) * (x_size+1) + j);
		}
	}
	m_pManObject->end();
	
	m_pOwnerManager->GetOgreSceneManager()->getRootSceneNode()->attachObject(m_pManObject);
	
	return result;
}

GameErrorCode RenderTerrainDecal::Load(wxXmlNode* XMLNode)
{
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}

GameErrorCode RenderTerrainDecal::Store(wxXmlNode* ParentNode)
{
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}

