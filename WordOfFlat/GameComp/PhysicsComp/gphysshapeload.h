#ifndef __GAME_PHYSICS_SHAPE_LOADER_H__
#define __GAME_PHYSICS_SHAPE_LOADER_H__


#include <GameSystem/glog.h>

class wxXmlNode;
class btCollisionShape;
class btBoxShape;

class PhysicsShapeLoader
{
public:
	static GameErrorCode LoadShape(wxXmlNode* pNode, btCollisionShape *&pCollisionShape, GameLogger *pLogger = nullptr);
	static GameErrorCode StoreShape(wxXmlNode* pParentNode, btCollisionShape *pCollisionShape, GameLogger *pLogger = nullptr);
	
private:
	static GameErrorCode LoadOneShape(wxXmlNode* pNode, btCollisionShape *&pCollisionShape, GameLogger *pLogger = nullptr);
	static GameErrorCode LoadBox(wxXmlNode* pNode, btCollisionShape *&pCollisionShape, GameLogger *pLogger = nullptr);
	static GameErrorCode StoreBox(wxXmlNode* pParentNode, btBoxShape *pCollisionShape, GameLogger *pLogger = nullptr);

};

#endif // __GAME_PHYSICS_SHAPE_LOADER_H__
