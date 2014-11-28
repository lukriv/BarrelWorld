#ifndef __GAME_TRANSFORM_COMPONENT_H__
#define __GAME_TRANSFORM_COMPONENT_H__

#include <bullet/vectormath/vmInclude.h>
#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>
#include "../gcompbase.h"

struct TransformData {
public:
	Vectormath::Aos::Vector3 	ATTRIBUTE_ALIGNED16(m_translate);
	Vectormath::Aos::Vector3	ATTRIBUTE_ALIGNED16(m_scale);
	Vectormath::Aos::Quat		ATTRIBUTE_ALIGNED16(m_rotation);
};




class TransformComponent : public ComponentBase {
	TransformData *m_pTransData;
	GameEntity* m_pParent;
	
public:
	TransformComponent();
	~TransformComponent();
	
	GameErrorCode Initialize();
	
	inline TransformData* GetData() { return m_pTransData; }
	
	
	virtual GameErrorCode ReceiveMessage(TaskMessage& msg);
	virtual GameErrorCode ReinitComponent(GameEntity* pNewParentEntity);
	virtual GameErrorCode Update();
	
};


#endif //__GAME_TRANSFORM_COMPONENT_MANAGER_H__