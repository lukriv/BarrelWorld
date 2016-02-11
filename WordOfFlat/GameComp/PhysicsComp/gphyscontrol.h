#ifndef __GAME_PHYSHICS_CONTROLER_H__
#define __GAME_PHYSHICS_CONTROLER_H__

#include "gphyssystem.h"
#include "../transformComp/gtranscomp.h"


class PhysicsControler : public ComponentBase, public btActionInterface
{
protected:
	GamePhysicsSystem* m_pPhysSystem;
	RefObjSmPtr<TransformComponent> m_spTransform;
	wxString m_definitionFile; //!< Name of physics definition file - for load and store component (empty string = shape definition was used)
public:
	PhysicsControler(GamePhysicsSystem* pPhysSystem);
	virtual ~PhysicsControler();
	
	GameErrorCode Initialize(TransformComponent *pTransform);
	
	GameErrorCode Create();

	virtual	GameErrorCode Load(wxXmlNode* pNode) override;
	virtual	GameErrorCode Store(wxXmlNode* pParentNode) override;
	
	virtual	GameErrorCode ReceiveMessage(TaskMessage& msg) override;


	// btActionInterface
	virtual	void debugDraw(btIDebugDraw* debugDrawer) override;
	
	virtual	void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep) override;
	
};

#endif // __GAME_PHYSHICS_CONTROLER_H__
