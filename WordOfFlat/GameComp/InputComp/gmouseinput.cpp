#include "terrdecalinput.h"


GameErrorCode GenericMouseInput::CreateObject(const InputDef& inputDef, GameInputSystem& inputSystem, InputComponent*& pNewInputComp)
{
		
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<GenericMouseInput> spInputComp;
	FWG_RETURN_FAIL(GameNewChecked(spInputComp.OutRef()));
	
	if(FWG_FAILED(result = inputSystem.RegisterCallback(spInputComp)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Register mouse callback failed: 0x%08x"), inputSystem.GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	
	pNewInputComp = spInputComp.Detach();
	
	
	return FWG_NO_ERROR;
}
