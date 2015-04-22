#include "grenderTerrDec.h"

RenderTerrainDecal::RenderTerrainDecal()
{
}

RenderTerrainDecal::~RenderTerrainDecal()
{
}

GameErrorCode RenderTerrainDecal::Create(const wxString& materialName)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<RenderObject> spManualObject;
	wxCriticalSectionLocker renderLock(m_renderLock);
	if(manObjDef.m_manualObjectType.Cmp(MANUAL_OBJECT_TERRAIN_DECAL) == 0)
	{
		//TODO: create terrain decal manual object
		ManualObject* pMeshDecal = nullptr;
		
		if(!renderObjectDef.GetName().IsEmpty())
		{
			pMeshDecal = new Ogre::ManualObject(manObjDef.GetName().ToStdString());
		} else {
			FWGLOG_ERROR(wxT("Manual object name is empty"), m_spLogger);
			return FWG_E_NOT_IMPLEMENTED_ERROR;
		}
		
		m_pSceneManager->getRootSceneNode()->attachObject(mMeshDecal);
		
		wxInt32 x_size = 4;  // number of polygons
		wxInt32 z_size = 4;
		
		pMeshDecal->begin(manObjDef.m_material->m_name.ToStdString(), Ogre::RenderOperation::OT_TRIANGLE_LIST);
		for (wxInt32 i=0; i <= x_size; ++i)
		{
			for (wxInt32 j=0; j <= z_size; ++j)
			{
				pMeshDecal->position(Ogre::Vector3(i, 0, j));
				pMeshDecal->textureCoord((float)i / (float)x_size, (float)j / (float)z_size);
			}
		}
		
		for (wxInt32 i=0; i<x_size; ++i)
		{
			for (wxInt32 j=0; j<z_size; ++j)
			{
				pMeshDecal->quad( i * (x_size+1) + j,
							i * (x_size+1) + j + 1,
							(i + 1) * (x_size+1) + j + 1,
							(i + 1) * (x_size+1) + j);
			}
		}
		pMeshDecal->end();
		
		FWG_RETURN_FAIL(GameNewChecked(spManualObject.OutRef(), pMeshDecal));
	}
	
	pManObj = spManualObject.Detach();
	
	return result;
}

GameErrorCode RenderTerrainDecal::Load(wxXmlNode* XMLNode)
{
}

GameErrorCode RenderTerrainDecal::Store(wxXmlNode* ParentNode)
{
}
