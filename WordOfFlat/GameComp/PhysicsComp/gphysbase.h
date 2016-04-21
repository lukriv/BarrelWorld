#ifndef __GAME_PHYSHICS_BASE_H__
#define __GAME_PHYSHICS_BASE_H__

#include "gphyssystem.h"
#include "../transformComp/gtranscomp.h"
#include <bullet/BulletCollision/CollisionShapes/btCollisionShape.h>
#include <GameSystem/gpropery.h>

class PhysicsBase : public ComponentBase
{
protected:
	GamePhysicsSystem* m_pPhysSystem;
	RefObjSmPtr<TransformComponent> m_spTransform;
	btRigidBody* m_pRigidBody;
	wxString m_definitionFile; //!< Name of physics definition file - for load and store component (empty string = shape definition was used)
public:
	
	PhysicsBase(GamePhysicsSystem* pPhysSystem);
	virtual ~PhysicsBase();

	GameErrorCode Initialize(TransformComponent *pTransform);

	GameErrorCode Create(GamePropertyContainer &propMap);
	
	btRigidBody* GetRigidBody();
	
	virtual	GameErrorCode ReceiveMessage(TaskMessage& msg);

	virtual	GameErrorCode Load(wxXmlNode* XMLNode);
	virtual	GameErrorCode Store(wxXmlNode* ParentNode);

	



};

#endif // __GAME_PHYSHICS_BASE_H__
