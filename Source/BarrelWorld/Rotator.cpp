#include "Rotator.h"
#include <Urho3D/Scene/Node.h>



using namespace Urho3D;

namespace BW
{

	Rotator::Rotator(Context *pContext) : LogicComponent(pContext)
	{
		SetUpdateEventMask(USE_UPDATE);
	}

	Rotator::~Rotator()
	{
	}

	void Rotator::Update(float timeStep)
	{
		node_->Rotate(Quaternion(8*timeStep,16*timeStep,0));
	}

}