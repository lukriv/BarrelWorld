#ifndef __GAME_PROPERTY_COMPONENT_H__
#define __GAME_PROPERTY_COMPONENT_H__

#include <GameSystem/glog.h>
#include <GameSystem/gdefs.h>
#include <GameSystem/gerror.h>
#include <GameSystem/refobjectsmptr.h>
#include <GameSystem/gpropery.h>
#include <GameComp/gcompbase.h>
#include <GameComp/gmanagerbase.h>

class wxXmlNode; // forward declaration

class PropertyComponent : public ComponentBase, public GamePropertyContainer
{
public:
	
	PropertyComponent(GameLogger* pLogger);
	virtual ~PropertyComponent();

	GameErrorCode Create(GamePropertyContainer &propCont);
	
	virtual	GameErrorCode ReceiveMessage(TaskMessage& msg);

	virtual	GameErrorCode Load(wxXmlNode* XMLNode);
	virtual	GameErrorCode Store(wxXmlNode* ParentNode);

};

#endif // __GAME_PROPERTY_COMPONENT_H__