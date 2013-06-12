#ifndef __KD_TREE_OBJECT_H__
#define __KD_TREE_OBJECT_H__

#include "../GameObjects/gobject.h"
#include <Box2D/Collision/b2Collision.h>


/*! \brief Implementation of BVH tree node
 */
struct GameKDTreeNode {
	wxDword m_rightChild; //!< Index to left child (if node is internal) node or first triangle pointer (if node is terminal)
	wxDword m_size; //!< Number of triangles in node (zero indicates internal node)
	wxDword m_parent; //!< index to parent
	wxDword m_pad; //!< alignment to 32B
};

/*! \brief Implementation of BVH tree
 */
class GameKDTreeObject : public GameSceneObject {
	wxVector<GameKDTreeNode*> m_treeNodes; // list of nodes
	wxVector<GameSceneObject*> m_objList;//!< list of Drawable objects

public:
	GameKDTreeObject() : m_objList(objList) {}
	GameErrorCode CreateTree(const wxVector<GameSceneObject*> &objList);
	GameErrorCode UpdateTree(const wxVector<GameSceneObject*> &updList);
	
	void RemoveObj(GameSceneObject* obj);
	void AddObj(GameSceneObject* obj);
	
public:
	virtual void draw( sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif //__BVH_TREE_OBJECT_H__