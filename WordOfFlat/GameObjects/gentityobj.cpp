#include "gentityobj.h"

void GameEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::RenderStates renStates(sf::BlendNone, getTransform(), m_pTexture, NULL);
	m_pGeometry->draw(target, states);
}

void GameEntity::UpdatePosition()
{
	setPosition(m_pBody->GetPosition().x, m_pBody->GetPosition().y);
	setRotation(m_pBody->GetAngle());
}
