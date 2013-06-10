#ifndef __BVH_TREE_OBJECT_H__
#define __BVH_TREE_OBJECT_H__

#include "../GameObjects/gobject.h"
#include <Box2D/Collision/b2Collision.h>


/*! \brief Implementation of BVH tree node
 */
struct GameBVHTreeNode {
	b2AABB m_AABB;
	wxDword m_rightChild; //!< Index to left child (if node is internal) node or first triangle pointer (if node is terminal)
	wxDword m_size; //!< Number of triangles in node (zero indicates internal node)
	wxUint32 m_pad[2]; //!< alignment to 32B
}


/*! \brief Implementation of BVH tree
 */
class GameBVHTreeObject : public GameSceneObject {
	wxVector<GameBVHTreeNode*> m_treeNodes; // list of nodes
	wxVector<GameSceneObject*> m_objList;//!< list of Drawable objects

public:
	GameBVHTreeObject() {}
	GameErrorCode CreateTree();
	GameErrorCode UpdateTree(const wxVector<GameSceneObject*> &updList);
	
public:
	virtual void draw( sf::RenderTarget& target, sf::RenderStates states) const;
}

#endif //__BVH_TREE_OBJECT_H__