#ifndef __GAME_ENTITY_H__
#define __GAME_ENTITY_H__

class GameEntity {
	TransformComponent *m_pTransformComp; // transform component
	RenderComponent *m_pRenderComp;
	wxDword m_animator2dComp;
	wxDword m_physComp;
public:
		GameEntity();
		~GameEntity();
		
		inline void SetTransformComp ( TransformComponent* pTransform ) {m_pTransformComp = pTransform;}
		inline void SetRenderComp ( RenderComponent* pRenderComp ) {m_pRenderComp = pRenderComp;}
};

#endif // __GAME_ENTITY_H__
