#ifndef __TREE_OBJECT_H__
#define __TREE_OBJECT_H__

#include "gameobj.h"


class GameTreeObject : public IGameObject {

public:
enum TreeType {
	TREE_SPRUCE
}
	
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


#endif //__TREE_OBJECT_H__