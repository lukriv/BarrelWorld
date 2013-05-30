#ifndef __GAME_FLAT_WORLD_CLIENT_H__
#define __GAME_FLAT_WORLD_CLIENT_H__




enum GameTextureId {
	GAME_TEXTURE_ID_UNDEFINED = 0,
	GAME_TEXTURE_ID_WOOD_BOX = 1
};

/*! \brief Id for dynamic object shapes */
enum GameShapeId {
	GAME_SHAPE_ID_UNDEFINED = 0, //!< Undefined shape - initial value
	GAME_SHAPE_ID_BOX = 1 //!< Box shape - basic box
};



class IGameFlatWorldClient {
public:
	virtual bool DrawScreen() = 0;
	
}


#endif //__GAME_FLAT_WORLD_CLIENT_H__