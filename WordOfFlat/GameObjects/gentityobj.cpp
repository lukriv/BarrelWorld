#include "gentityobj.h"

void GameEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::RenderStates renStates(sf::BlendNone, getTransform(), m_pTexture, NULL);
	m_pGeometry->draw(target, renStates);
}

void GameEntity::UpdatePosition()
{
	SetPosition(m_pBody->GetPosition().x, m_pBody->GetPosition().y);
	SetRotation(m_pBody->GetAngle());
}

void GameEntity::TraceLogInfo(GameLogger* pLogger)
{
	FWGLOG_TRACE_FORMAT(wxT("GameEntity::TraceLogInfo() : Entity position [%0.3f,%0.3f], angle [%0.3f]"),
		pLogger, m_pBody->GetPosition().x, m_pBody->GetPosition().y, m_pBody->GetAngle(), FWGLOG_ENDVAL);
}
