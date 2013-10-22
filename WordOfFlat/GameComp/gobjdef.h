#ifndef __GAME_OBJECT_DEFINITIONS_H__
#define __GAME_OBJECT_DEFINITIONS_H__

#include "../GameSystem/gdefs.h"

typedef wxDword GameObjectID; //!< Object id type


static const float Pixelize = 50.0f; //!< Conversion from meters to pixels (one meter is 50 pixels)

static const GameObjectID GAME_OBJECT_ID_INVALID = UINT_MAX;


#endif //__GAME_OBJECT_DEFINITIONS_H__