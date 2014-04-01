#include "gentitycreator.h"

void GameEntityCreator::addRef()
{
	wxAtomicInc(m_refCount);
}

wxInt32 GameEntityCreator::release()
{
	wxInt32 refCount = wxAtomicDec(m_refCount);
	if (refCount == 0)
	{
		delete this;
	}
	return refCount;
}

