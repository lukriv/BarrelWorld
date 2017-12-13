#include "Rotator.h"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Node.h>

using namespace Urho3D;

namespace BW
{

	void Rotator::RegisterObject(Context* pContext)
	{
		pContext->RegisterFactory<Rotator>();
	}
		
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


