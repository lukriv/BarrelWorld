#ifndef __GAME_PHYSICS_SHAPE_LOADER_H__
#define __GAME_PHYSICS_SHAPE_LOADER_H__


#include <GameSystem/glog.h>

class wxXmlNode;
class btCollisionShape;
class btBoxShape;

class PhysicsShapeLoader
{
private:
	GameLoggerPtr m_spLogger;
public:
	PhysicsShapeLoader(GameLogger *pLogger);
	GameErrorCode LoadShape(wxXmlNode* pNode, btCollisionShape *&pCollisionShape);
	GameErrorCode StoreShape(wxXmlNode* pParentNode, btCollisionShape *pCollisionShape);
	
private:
	GameErrorCode LoadOneShape(wxXmlNode* pNode, btCollisionShape *&pCollisionShape);
	GameErrorCode LoadBox(wxXmlNode* pNode, btCollisionShape *&pCollisionShape);
	GameErrorCode StoreBox(wxXmlNode* pParentNode, btBoxShape *pCollisionShape);

};

#endif // __GAME_PHYSICS_SHAPE_LOADER_H__
