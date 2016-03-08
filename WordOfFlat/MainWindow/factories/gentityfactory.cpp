#include "gentityfactory.h"

#include <wx/scopedptr.h>
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreMeshManager.h>

#include <GameSystem/refobjectsmptr.h>
#include <GameComp/RenderComp/grendercompbase.h>
#include <GameComp/InputComp/ginputhandler.h>
#include <GameComp/RenderComp/grenderrigidbody.h>
#include <GameComp/PhysicsComp/gphysbase.h>
#include <GameComp/RenderComp/grendercamera.h>
#include <GameComp/gpropery.h>
#include <GameSystem/new.h>

#include "../controllers/glogicgamecam.h"
#include "../controllers/charControl.h"




GameErrorCode GameEntityFactory::CreateFloor(GameCompManager& compMgr)
{
	GameErrorCode result = FWG_NO_ERROR;
	if(!compMgr.GetRenderSystem().GetOgreRoot()->getMeshManager()->resourceExists("ground"))
	{
		Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
		Ogre::MeshPtr meshptr = compMgr.GetRenderSystem().GetOgreRoot()->getMeshManager()->createPlane(
			"ground",
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			plane, 
			1500, 1500,
			20, 20, 
			true,
			1, 5, 5, 
			Ogre::Vector3::UNIT_Z);
			
	}
	
	wxDword entityId = compMgr.GetEntityManager().GetNewId();
	
	RefObjSmPtr<TransformComponent>	spTransform;
	RefObjSmPtr<RenderPosition> spRenderPosition;
	RefObjSmPtr<RenderRigidBody> spRenderObject;
	RefObjSmPtr<PhysicsBase> spPhysicsComp;
	
	if(FWG_FAILED(result = compMgr.GetEntityManager().GetTransformManager().CreateComponent(entityId, spTransform.OutRef())))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create transform component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	// render position
	if(FWG_FAILED(result = compMgr.GetEntityManager().GetRenderPosManager().CreateComponent(entityId, spTransform, spRenderPosition.OutRef())))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create render position component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	FWG_RETURN_FAIL( spRenderPosition->Create() );
	
	
	
	if(FWG_FAILED(result = compMgr.GetEntityManager().GetRenderObjManager().CreateComponent<RenderRigidBody>(entityId, spRenderPosition, spRenderObject.OutRef())))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create render object component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	
	if(FWG_FAILED(result = spRenderObject->Create(wxT("ground"), wxT("Test/Floor"))))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create floor failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	
	if(FWG_FAILED(result = compMgr.GetEntityManager().GetPhysicsManager().CreateComponent(entityId, spTransform, spPhysicsComp.OutRef())))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create physics component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	btStaticPlaneShape *pPlane = new btStaticPlaneShape( btVector3(0, 1.0f, 0), 0);
	
	if(FWG_FAILED(result = spPhysicsComp->Create(0, pPlane)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create floor physics failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode GameEntityFactory::CreateMainCamera(GameCompManager& compMgr)
{
	GameErrorCode result = FWG_NO_ERROR;
	GamePropertyContainer propCont(m_spLogger);
	wxDword entityId = compMgr.GetEntityManager().GetNewId();
	
	RefObjSmPtr<TransformComponent>	spTransform = nullptr;
	RefObjSmPtr<RenderPosition> spRenderPosition;
	RefObjSmPtr<RenderCamera> spRenderObject;
	RefObjSmPtr<FreeCameraInput> spInput;
	RefObjSmPtr<LogicGameCamera> spController;
	
	if(FWG_FAILED(result = compMgr.GetEntityManager().GetTransformManager().CreateComponent(entityId, spTransform.OutRef())))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create transform component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	spTransform->GetData()->m_translate = btVector3(0,40,10);
		
	// render position
	if(FWG_FAILED(result = compMgr.GetEntityManager().GetRenderPosManager().CreateComponent(entityId, spTransform, spRenderPosition.OutRef())))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create render position component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	FWG_RETURN_FAIL( spRenderPosition->Create() );
	
	
	if(FWG_FAILED(result = compMgr.GetEntityManager().GetRenderObjManager().CreateComponent<RenderCamera>(entityId, spRenderPosition, spRenderObject.OutRef())))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create render camera failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}

	propCont.SetProperty(wxT("name"), wxT("MainCamera"));
	propCont.SetProperty(wxT("fov"), 45.0f);
	propCont.SetProperty(wxT("near"), 1.0f);
	propCont.SetProperty(wxT("far"), 50000.0f);
	
	if(FWG_FAILED(result = spRenderObject->Create(propCont)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create camera failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}

	FWG_RETURN_FAIL(GameNewChecked(spInput.OutRef()));
	FWG_RETURN_FAIL(spInput->Initialize(&compMgr.GetInputSystem()))
	
	//<input type="freeCameraInput">
	//			<key action="forward" value="key_i" />
	//			<key action="backward" value="key_k" />
	//			<key action="left" value="key_j" />
	//			<key action="right" value="key_l" />
	//		</input>
	
	InputDef inputDefinition;
	FWG_RETURN_FAIL(inputDefinition.m_inputMap.Insert(wxT("up"), OIS::KC_I));
	FWG_RETURN_FAIL(inputDefinition.m_inputMap.Insert(wxT("down"), OIS::KC_K));
	FWG_RETURN_FAIL(inputDefinition.m_inputMap.Insert(wxT("left"), OIS::KC_J));
	FWG_RETURN_FAIL(inputDefinition.m_inputMap.Insert(wxT("right"), OIS::KC_L));
	
	FWG_RETURN_FAIL(spInput->Create(inputDefinition));
	
	FWG_RETURN_FAIL(GameNewChecked(spController.OutRef()));
	FWG_RETURN_FAIL(spController->Initialize(spTransform, spInput, &compMgr));
	
	if(FWG_FAILED(result = compMgr.GetEntityManager().GetLogicControllerManager().AddLogicController(entityId, spController)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Add logic controller failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	return FWG_NO_ERROR;
		
}

GameErrorCode GameEntityFactory::CreateBox(GameCompManager& compMgr, const btVector3& place)
{
	GameErrorCode result = FWG_NO_ERROR;
	
	wxDword entityId = compMgr.GetEntityManager().GetNewId();
	
	RefObjSmPtr<TransformComponent>	spTransform;
	RefObjSmPtr<RenderPosition> spRenderPosition;
	RefObjSmPtr<RenderRigidBody> spRenderObject;
	RefObjSmPtr<PhysicsBase> spPhysicsComp;
	
	if(FWG_FAILED(result = compMgr.GetEntityManager().GetTransformManager().CreateComponent(entityId, spTransform.OutRef())))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create transform component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	spTransform->GetData()->m_translate = place;
	
	// render position
	if(FWG_FAILED(result = compMgr.GetEntityManager().GetRenderPosManager().CreateComponent(entityId, spTransform, spRenderPosition.OutRef())))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create render position component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	FWG_RETURN_FAIL( spRenderPosition->Create() );
	
	
	
	if(FWG_FAILED(result = compMgr.GetEntityManager().GetRenderObjManager().CreateComponent<RenderRigidBody>(entityId, spRenderPosition, spRenderObject.OutRef())))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create render object component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	
	if(FWG_FAILED(result = spRenderObject->Create(wxT("TestingCube"), wxT("Test/ColourTest"))))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create floor failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	
	if(FWG_FAILED(result = compMgr.GetEntityManager().GetPhysicsManager().CreateComponent(entityId, spTransform, spPhysicsComp.OutRef())))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create physics component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	btCollisionShape *pColShape = new btBoxShape(btVector3(1.0f,1.0f,1.0f));	  
	
	if(FWG_FAILED(result = spPhysicsComp->Create(1.0f, pColShape)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create floor physics failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode GameEntityFactory::CreateAvatar(GameCompManager& compMgr, const btVector3& place)
{
	GameErrorCode result = FWG_NO_ERROR;
	
	wxDword entityId = compMgr.GetEntityManager().GetNewId();
	
	RefObjSmPtr<TransformComponent>	spTransform;
	RefObjSmPtr<RenderPosition> spRenderPosition;
	RefObjSmPtr<RenderRigidBody> spRenderObject;
	RefObjSmPtr<PhysicsBase> spPhysicsComp;
	RefObjSmPtr<CharacterController> spCharControl;
	RefObjSmPtr<CharacterInput> spCharInput;
	
	if(FWG_FAILED(result = compMgr.GetEntityManager().GetTransformManager().CreateComponent(entityId, spTransform.OutRef())))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create transform component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	spTransform->GetData()->m_translate = place;
	
	// render position
	if(FWG_FAILED(result = compMgr.GetEntityManager().GetRenderPosManager().CreateComponent(entityId, spTransform, spRenderPosition.OutRef())))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create render position component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	FWG_RETURN_FAIL( spRenderPosition->Create() );
	
	
	
	if(FWG_FAILED(result = compMgr.GetEntityManager().GetRenderObjManager().CreateComponent<RenderRigidBody>(entityId, spRenderPosition, spRenderObject.OutRef())))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create render object component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	
	if(FWG_FAILED(result = spRenderObject->Create(wxT("Cube.mesh"), wxT("Material"))))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create floor failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	
	if(FWG_FAILED(result = compMgr.GetEntityManager().GetPhysicsManager().CreateComponent(entityId, spTransform, spPhysicsComp.OutRef())))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create physics component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	btCollisionShape *pColShape = new btBoxShape(btVector3(0.5f,1.0f,0.5f));	  
	
	if(FWG_FAILED(result = spPhysicsComp->Create(0, pColShape)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create floor physics failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	FWG_RETURN_FAIL(GameNewChecked(spCharInput.OutRef()));
	FWG_RETURN_FAIL(spCharInput->Initialize(&compMgr.GetInputSystem()));
	
	InputDef inputDefinition;
	FWG_RETURN_FAIL(inputDefinition.m_inputMap.Insert(wxT("forward"), OIS::KC_W));
	FWG_RETURN_FAIL(inputDefinition.m_inputMap.Insert(wxT("backward"), OIS::KC_S));
	FWG_RETURN_FAIL(inputDefinition.m_inputMap.Insert(wxT("left"), OIS::KC_A));
	FWG_RETURN_FAIL(inputDefinition.m_inputMap.Insert(wxT("right"), OIS::KC_D));
	
	FWG_RETURN_FAIL(spCharInput->Create(inputDefinition));
	
	FWG_RETURN_FAIL(GameNewChecked(spCharControl.OutRef(), &compMgr, spCharInput));
	
	if(FWG_FAILED(result = spCharControl->Initialize(spTransform, spPhysicsComp)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Initialize physics controller failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = compMgr.GetEntityManager().GetPhysicsControllerManager().AddPhysicsController(entityId, spCharControl)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create physics component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	return FWG_NO_ERROR;
}
