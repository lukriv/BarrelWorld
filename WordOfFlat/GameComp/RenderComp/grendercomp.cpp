#include "grendercomp.h"

void GameRenderComp::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::RenderStates renStates(m_blendmode, getTransform(), m_pTexture, NULL);
	m_pGeometry->draw(target, renStates);
}

void GameRenderComp::UpdateEntity(const GameEntityUpdateStruct& updStruct)
{
	if (m_pBody != NULL)
	{
		SetPosition(m_pBody->GetPosition().x, m_pBody->GetPosition().y);
		SetRotation(m_pBody->GetAngle());
	}
	
	if(m_pAnimation != NULL) 
	{
		m_pAnimation->UpdateTimeIncremental(updStruct.m_timeDiff);
		m_pTexture = m_pAnimation->GetActualFrame();
	}
}

void GameRenderComp::DebugInfo(GameLogger* pLogger)
{
	FWGLOG_DEBUG_FORMAT(wxT("GameEntity::EntityDebugInfo() : Entity info: Internal texture [%u, %u]; m_pGeometry (0x%x)")
		, pLogger
		, m_internalTexture.getSize().x
		, m_internalTexture.getSize().y
		, m_pGeometry
		, FWGLOG_ENDVAL);
}
