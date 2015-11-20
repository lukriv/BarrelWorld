#include "glogicfreecam.h"
#include "glogicsystem.h"
#include <GameComp/gentity.h>
#include <GameComp/inputComp/ginputfreecam.h>
#include <GameXmlDefinitions/gxmldefs.h>
#include <wx/xml/xml.h>
#include <wx/scopedptr.h>

const btScalar STEP_SIZE = 0.02f;
const btScalar MOVE_STEP_SIZE = 1.0f;

static const btScalar _2_PI = SIMD_2_PI;
static const btScalar _HALF_PI = SIMD_HALF_PI;

GameErrorCode LogicFreeCamera::CreateComponent(TransformComponent* pTransform, FreeCameraInput* pFreeCamInput)
{
	m_spTransform = pTransform;
	m_spInput = pFreeCamInput;
		
	if(m_spTransform.IsEmpty() || m_spInput.IsEmpty())
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	return FWG_NO_ERROR;
}

LogicFreeCamera::LogicFreeCamera() : LogicBase(GAME_COMP_LOGIC_FREE_CAMERA), m_angleX(0.0f), m_angleY(0.0f)
{
}

GameErrorCode LogicFreeCamera::ProcessInput()
{
	GameErrorCode result = FWG_NO_ERROR;
	ControlStruct actualControls;
	m_spInput->ExportControlStruct(actualControls);
	
	if(!m_spTransform.IsEmpty())
	{
		
		if(actualControls.IsMousePressed(ControlStruct::MOUSE_BUTTON_RIGHT))
		{
			// turn camera around
			m_angleX += STEP_SIZE * ((btScalar)actualControls.GetRelX());
			m_angleY += STEP_SIZE * ((btScalar)actualControls.GetRelY());
			
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
			
			m_spTransform->GetData()->m_rotation.setEuler(-m_angleX, m_angleY, 0.0f);

		}
		
		if(actualControls.IsPressed(FreeCameraInput::INPUT_ACTION_FORWARD|FreeCameraInput::INPUT_ACTION_BACKWARD)
				&& !(actualControls.IsPressed(FreeCameraInput::INPUT_ACTION_FORWARD) && actualControls.IsPressed(FreeCameraInput::INPUT_ACTION_BACKWARD)))
		{
			btVector3 dirVec(0,0,actualControls.IsPressed(FreeCameraInput::INPUT_ACTION_FORWARD)? MOVE_STEP_SIZE : -MOVE_STEP_SIZE );
			m_spTransform->GetData()->m_translate += dirVec.rotate(m_spTransform->GetData()->m_rotation.getAxis(), m_spTransform->GetData()->m_rotation.getAngle());;
		}
		
		if(actualControls.IsPressed(FreeCameraInput::INPUT_ACTION_LEFT|FreeCameraInput::INPUT_ACTION_RIGHT)
				&& !(actualControls.IsPressed(FreeCameraInput::INPUT_ACTION_LEFT) && actualControls.IsPressed(FreeCameraInput::INPUT_ACTION_RIGHT)))
		{
			btVector3 dirVec( actualControls.IsPressed(FreeCameraInput::INPUT_ACTION_LEFT) ? MOVE_STEP_SIZE : -MOVE_STEP_SIZE, 0, 0);
			m_spTransform->GetData()->m_translate += dirVec.rotate(m_spTransform->GetData()->m_rotation.getAxis(), m_spTransform->GetData()->m_rotation.getAngle());;
		}
		

		
	}
	
	// update render component
	TaskMessage task(GAME_TASK_TRANSFORM_UPDATE);
	if(FWG_FAILED(result = m_pParent->ReceiveMessage(task)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Update component failed: 0x%08x"), m_pLogicSystem->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode LogicFreeCamera::ReceiveMessage(TaskMessage& msg)
{
	return FWG_NO_ERROR;
}


GameErrorCode LogicFreeCamera::Update()
{
	if(m_pParent == nullptr)
	{
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	if(!m_spInput.IsEmpty())
	{
		FWG_RETURN_FAIL(ProcessInput());
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode LogicFreeCamera::Load(wxXmlNode* pNode)
{
	if( pNode->GetName() != GAME_TAG_COMP_LOGIC_FREE_CAMERA )
	{
		return FWG_E_XML_INVALID_TAG_ERROR;
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode LogicFreeCamera::Store(wxXmlNode* pParentNode)
{
	wxXmlNode *pNewNode = nullptr;
	wxString content;
	FWG_RETURN_FAIL(GameNewChecked(pNewNode, wxXML_ELEMENT_NODE, GAME_TAG_COMP_LOGIC_FREE_CAMERA));
	wxScopedPtr<wxXmlNode> apNewNode(pNewNode);
	
	pParentNode->AddChild(apNewNode.release());
	
	return FWG_NO_ERROR;
}

LogicFreeCamera::~LogicFreeCamera()
{
}
