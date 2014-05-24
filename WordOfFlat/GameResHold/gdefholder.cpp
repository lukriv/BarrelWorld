#include "gdefholder.h"

void GameDefinitionHolder::addRef()
{
	wxAtomicInc(m_refCount);
}

wxInt32 GameDefinitionHolder::release()
{
	wxInt32 refCount = wxAtomicDec(m_refCount);
	if(refCount == 0)
	{
		delete this;
	}
	
	return refCount;
}

