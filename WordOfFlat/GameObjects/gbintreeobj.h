#ifndef __GAME_BINARY_TREE_OBJECT_H__
#define __GAME_BINARY_TREE_OBJECT_H__

#include "gobjbase.h"


class GameObjectBinaryTree : public GameObjectBase {

	
public:
//from IGameObject
	virtual GameObjectType GetType();
	virtual wxInt32 GetId();
	virtual void SetId(wxInt32 id);
	
	bool Initialize();
	
	/*! \brief Draw an object
	 * 
	 * If object is not drawable, this method do nothing.
	 * 
	 * \retval true Drawing object was successful.
	 * \retval false Drawing object failed.
	 */
	virtual bool Draw();
}


#endif //__GAME_BINARY_TREE_OBJECT_H__