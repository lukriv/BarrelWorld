#include "ActualMission.h"


#include <random>
#include <sstream>
//#include <functional>
//#include <algorithm>
//#include <queue>
#include <chrono>

#include <Urho3D/Scene/Scene.h>


#include "EntityCreator.h"

using namespace Urho3D;

BW::ActualMission::ActualMission() : m_enemyCount(0)
{
}

BW::ActualMission::~ActualMission()
{
}


void BW::ActualMission::CreateEnemy(Urho3D::Scene *pScene, Urho3D::Application *pApp)
{
	static std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
	
	
	std::uniform_int_distribution<int32_t> enemyDist(-100,100);
	Urho3D::WeakPtr<Urho3D::Node> spEnemy;
	
	std::stringstream sstr;
	
	sstr << "enemy_" << m_enemyCount;
		
	spEnemy = BW::EntityCreator::CreateAvatar(sstr.str().c_str(), pApp, pScene);
	
	spEnemy->SetPosition(Vector3(enemyDist(generator),35,enemyDist(generator)));
	
	m_spEnemies.Push(spEnemy);
	
	++m_enemyCount;

}
