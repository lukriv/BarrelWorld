#include "gutils.h"


void GameUtils::GetLookPoint(GameRenderSystem& renderSystem, GameMenuSystem& menuSystem, TransformComponent& transComp, btVector3 &output)
{
	Ogre::Vector2 vec2;
	menuSystem.GetPointerPosition(vec2);
	Ogre::Plane plane( Ogre::Vector3(0,1,0), transComp.GetData()->m_translate.getY());
	
	Ogre::Ray ray = renderSystem.GetMainCamera()->getCameraToViewportRay( vec2.x, vec2.y );
	
	std::pair< bool, Ogre::Real > rayResult = ray.intersects(plane);
	
	if(!rayResult.first)
	{
		return;
	}
	
	output = cvt(ray.getPoint(rayResult.second));
	
}

void GameUtils::GetCherryPickVector(GameRenderSystem& renderSystem, GameMenuSystem& menuSystem, btVector3& output)
{
	Ogre::Vector2 vec2;
	menuSystem.GetPointerPosition(vec2);
	Ogre::Ray ray = renderSystem.GetMainCamera()->getCameraToViewportRay( vec2.x, vec2.y );
	output = cvt(ray.getDirection());
}
