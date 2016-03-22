#include "glogicgamecam.h"
#include <OGRE/OgreRay.h>
#include <OGRE/OgreSceneQuery.h>
#include <GameComp/LogicComp/glogicsystem.h>
#include <GameComp/gentitymgr.h>
#include <GameComp/gcompmgr.h>
#include <GameXmlDefinitions/gxmldefs.h>
#include <GameXmlDefinitions/gxmlutils.h>
#include <wx/xml/xml.h>
#include <wx/scopedptr.h>
#include <MainWindow/factories/gentityfactory.h>

const btScalar STEP_SIZE = 1.0f;
const btScalar MOVE_STEP_SIZE = 5.0f;

static const btScalar _2_PI = SIMD_2_PI;
static const btScalar _HALF_PI = SIMD_HALF_PI;

LogicGameCamera::LogicGameCamera() : m_pCompMgr(nullptr)
	, m_angleX(0.0f)
	, m_angleY(-3*SIMD_PI/8)
	, m_diffSinceLastFrameX(0.0f)
	, m_diffSinceLastFrameY(0.0f)
	, m_lastVector(0,0,0)
	, m_diffMoveVertical(0.0f)
	, m_diffMoveHorizontal(0.0f)
	, m_diffWheel(0.0f)
{
}

GameErrorCode LogicGameCamera::Update(float timeDiff)
{
	ControlStruct actualControls;
	m_spInput->ExportControlStruct(actualControls);
	
	if(actualControls.WasMouseClicked(ControlStruct::MOUSE_BUTTON_LEFT))
	{
		FWG_RETURN_FAIL(CreateBox());
	}
	
	//if(actualControls.IsMousePressed(ControlStruct::MOUSE_BUTTON_RIGHT))
	//{
	//	m_diffSinceLastFrameX += (btScalar)actualControls.GetRelX();
	//	m_diffSinceLastFrameY += (btScalar)actualControls.GetRelY();
	//}
	
	
	if(m_spFollowedObject.IsEmpty())
	{
		if(actualControls.IsPressed(FreeCameraInput::INPUT_ACTION_UP))
		{
			m_diffMoveVertical -= MOVE_STEP_SIZE;
		}
		
		if(actualControls.IsPressed(FreeCameraInput::INPUT_ACTION_DOWN))
		{
			m_diffMoveVertical += MOVE_STEP_SIZE;
		}
		
		if(actualControls.IsPressed(FreeCameraInput::INPUT_ACTION_LEFT))
		{
			m_diffMoveHorizontal -= MOVE_STEP_SIZE;
		}
		
		if(actualControls.IsPressed(FreeCameraInput::INPUT_ACTION_RIGHT))
		{
			m_diffMoveHorizontal += MOVE_STEP_SIZE;
		}
	}
	
	m_diffWheel += (btScalar) actualControls.GetMouseWheelDelta();
	
	if(timeDiff == 0.0f)
	{
		return FWG_NO_ERROR;
	}
	
	// turn camera around
	m_angleX -= STEP_SIZE * (m_diffSinceLastFrameX) * timeDiff;
	m_angleY -= STEP_SIZE * (m_diffSinceLastFrameY) * timeDiff;
			
	UpdateCameraAngle();

	if(m_spFollowedObject.IsEmpty())
	{
		{
			btVector3 dirVec(m_diffMoveHorizontal*timeDiff,0,m_diffMoveVertical*timeDiff );
			m_spTransform->GetData()->m_translate += dirVec;
		}
			
	} else {
		btVector3 dirVec(  m_spFollowedObject->GetData()->m_translate - m_lastVector );
		m_lastVector = m_spFollowedObject->GetData()->m_translate;
		dirVec.setY(0);
		m_spTransform->GetData()->m_translate += dirVec;
	}
		
	{
		btVector3 dirVec( 0, m_diffWheel*MOVE_STEP_SIZE*timeDiff, 0);
		m_spTransform->GetData()->m_translate += dirVec;
	}

		
	// update render component
	FWG_RETURN_FAIL(SendUpdateMessage());
	
	m_diffSinceLastFrameX = 0.0f;
	m_diffSinceLastFrameY = 0.0f;
	
	m_diffMoveVertical = 0.0f;
	m_diffMoveHorizontal = 0.0f;
	m_diffWheel = 0.0f;
	
	return FWG_NO_ERROR;
}

LogicGameCamera::~LogicGameCamera()
{
}

void LogicGameCamera::UpdateCameraAngle()
{
	if(m_angleX < 0)
	{
		m_angleX += _2_PI;
	} else if(m_angleX > _2_PI) {
		m_angleX -= _2_PI;				
	}
		
	if(m_angleY > _HALF_PI)
	{
		m_angleY = _HALF_PI;
	} else if(m_angleY < -_HALF_PI) {
		m_angleY = -_HALF_PI;
	}
		
	m_spTransform->GetData()->m_rotation.setEuler(m_angleX, m_angleY, 0.0f);
}

GameErrorCode LogicGameCamera::SendUpdateMessage()
{
	GameErrorCode result = FWG_NO_ERROR;
	TaskMessage task(GAME_TASK_TRANSFORM_UPDATE);
	
	if(FWG_FAILED(result = GetEntityManager()->SendTaskMessage(GetComponentId(), task)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Update component failed: 0x%08x"), m_pLogicSystem->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	
	return result;
}

GameErrorCode LogicGameCamera::CreateBox()
{
	GameEntityFactory factory(GetEntityManager()->GetLogger());
	Ogre::Vector2 vec2;
	m_pCompMgr->GetMenuSystem().GetPointerPosition(vec2);
	
	Ogre::Ray ray = m_pCompMgr->GetRenderSystem().GetMainCamera()->getCameraToViewportRay( vec2.x, vec2.y );
	
	Ogre::RaySceneQuery *rayQuery = m_pCompMgr->GetRenderSystem().GetOgreSceneManager()->createRayQuery(ray);
	rayQuery->setQueryTypeMask( Ogre::SceneManager::ENTITY_TYPE_MASK);
	
	rayQuery->setSortByDistance(true);
	
	Ogre::RaySceneQueryResult &rayResult = rayQuery->execute();
	if(rayResult.size() == 0)
	{
		m_pCompMgr->GetRenderSystem().GetOgreSceneManager()->destroyQuery(rayQuery);
		return FWG_NO_ERROR;
	}
	
	Ogre::RaySceneQueryResultEntry& rayEntry = rayResult.back();
	
	btVector3 point(cvt(ray.getPoint(rayEntry.distance)));
	
	point.setY(point.getY() + 10.0f);
	
	m_pCompMgr->GetRenderSystem().GetOgreSceneManager()->destroyQuery(rayQuery);
	wxDword index;
	FWG_RETURN_FAIL(factory.CreateBox(*m_pCompMgr, index, point));
	return FWG_NO_ERROR;
}

GameErrorCode LogicGameCamera::Initialize(TransformComponent* pTransform, FreeCameraInput* pInput, GameCompManager *pCompMgr)
{
	if((pTransform == nullptr)||(pInput == nullptr))
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	m_spTransform = pTransform;
	m_spInput = pInput;
	m_pCompMgr = pCompMgr;
	
	return FWG_NO_ERROR;
}

void LogicGameCamera::SetFollowedObject(TransformComponent* pFollowedObject)
{
}

GameErrorCode LogicGameCamera::ReceiveMessage(TaskMessage& msg)
{
	switch (msg.GetTaskType())
	{
		case GAME_TASK_FOLLOW_OBJECT:
			{
				wxDword index;
				index = reinterpret_cast<wxDword>(msg.GetContext());
				if(index != 0)
				{
					m_spFollowedObject = GetEntityManager()->GetTransformManager().GetComponent(index);
				} else {
					m_spFollowedObject = nullptr;
				}
				FWGLOG_INFO_FORMAT(wxT("Set followed object to %u"), GetEntityManager()->GetLogger(), index, FWGLOG_ENDVAL);
				
			}
			break;
		default:
			break;
	}
	
	return FWG_NO_ERROR;
}




