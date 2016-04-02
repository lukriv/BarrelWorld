#include "gpropertycomp.h"

PropertyComponent::PropertyComponent(GameLogger* pLogger) : ComponentBase(GAME_COMP_PROPERTIES), GamePropertyContainer(pLogger)
{
}

PropertyComponent::~PropertyComponent()
{
}

GameErrorCode PropertyComponent::Create(GamePropertyContainer& propCont)
{
	GamePropertyContainer::operator =(propCont);
	return FWG_NO_ERROR;
}

GameErrorCode PropertyComponent::ReceiveMessage(TaskMessage& msg)
{
	return FWG_NO_ERROR;
}

GameErrorCode PropertyComponent::Load(wxXmlNode* XMLNode)
{
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}

GameErrorCode PropertyComponent::Store(wxXmlNode* ParentNode)
{
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}
