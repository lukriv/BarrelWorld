#ifndef __BARREL_WORLD_SUBSYSTEM_MANAGER__H__
#define __BARREL_WORLD_SUBSYSTEM_MANAGER__H__

namespace BW
{

class SubsystemManager
{
private:
	Urho3D::Application* m_app;
private:
	SubsystemManager();
	~SubsystemManager();

public:
	Urho3D::Application& GetApp();
	
private:
	static SubsystemManager* g_pResMgr;
public:
	static SubsystemManager& GetSubsystemMgr();
	
	static void Create(Urho3D::Application *pApp);
	static void Release();
	
};

}

#endif // __BARREL_WORLD_RESOURCE_MANAGER__H__
