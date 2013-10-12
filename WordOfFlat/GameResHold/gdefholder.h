#ifndef __GAME_DEFINITION_HOLDER_H__
#define __GAME_DEFINITION_HOLDER_H__


class GameDefinitionHolder : public IRefObject {
public:
	GameErrorCode GetEntityDef(wxDword entityID, RenderCompDef &basEntityDef); 
	GameErrorCode GetAnimDef(wxDword animID, AnimationDef &animDef);
		
};






#endif //__GAME_DEFINITION_HOLDER_H__