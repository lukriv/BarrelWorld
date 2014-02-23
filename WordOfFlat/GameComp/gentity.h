#ifndef __GAME_ENTITY_H__
#define __GAME_ENTITY_H__

class GameEntity {
	wxDword m_tranform2dComp;
	wxDword m_render2dComp;
	wxDword m_animator2dComp;
	wxDword m_physComp;
public:
		GameEntity();
		~GameEntity();
		
		SetTransform2dComp (wxDword )

};

#endif // __GAME_ENTITY_H__
