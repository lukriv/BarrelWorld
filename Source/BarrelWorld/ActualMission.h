#ifndef __BARREL_WORLD_ACTUAL_MISSION__H__
#define __BARREL_WORLD_ACTUAL_MISSION__H__



#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Container/RefCounted.h>
#include <Urho3D/Container/Vector.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Engine/Application.h>
#include <cstdint>



namespace BW
{

class ActualMission : public Urho3D::RefCounted
{
	int32_t m_enemyCount;
	Urho3D::Vector< Urho3D::WeakPtr<Urho3D::Node> > m_spEnemies;
public:
	ActualMission();
	~ActualMission();
	
	void CreateEnemy(Urho3D::Scene *pScene, Urho3D::Application *pApp);

};

}

#endif // __BARREL_WORLD_ACTUAL_MISSION_MANAGER__H__
