#ifndef __BARREL_WORLD_ROTATE_BOX_LOGIC__H__
#define __BARREL_WORLD_ROTATE_BOX_LOGIC__H__

#include <Urho3D/Scene/LogicComponent.h>

namespace BW
{

	class Rotator : public Urho3D::LogicComponent
	{
		URHO3D_OBJECT(Rotator, Urho3D::LogicComponent);
	public:

		Rotator(Urho3D::Context *pContext);
		~Rotator();

		virtual	void Update(float timeStep) override;



	};

}

#endif // __BARREL_WORLD_ROTATE_BOX_LOGIC__H__
