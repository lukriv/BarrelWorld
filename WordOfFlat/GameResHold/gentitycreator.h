#ifndef __DYNAMIC_ENTITY_CREATOR_H__
#define __DYNAMIC_ENTITY_CREATOR_H__

#include <wx/atomic.h>
#include "../GameSystem/refobject.h"
#include "../GameSystem/refobjectsmptr.h"


class GameEntityCreator : public IRefObject {
private:
	wxAtomicInt m_refCount;
	
public:
	GameEntityCreator() : m_refCount(1) {}

public:
	virtual void addRef();
	virtual wxInt32 release();
};


#endif //__DYNAMIC_ENTITY_CREATOR_H__