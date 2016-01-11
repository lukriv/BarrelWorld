#include "gmoveable.h"



GameErrorCode Moveable::Initialize(TransformComponent* pTransform, InputComponent* pInput)
{
	
	if((pTransform == nullptr)||(pInput == nullptr))
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	m_spTransform = pTransform;
	m_spInput = pInput;
		
	return FWG_NO_ERROR;
}
