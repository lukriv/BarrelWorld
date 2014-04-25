#ifndef __REFERENCED_OBJECT_IMPLEMENTATION_H__
#define __REFERENCED_OBJECT_IMPLEMENTATION_H__


#include <wx/atomic.h>
#include "new.h"


/*! \brief Implementation of IRefObject interface
 */ 
template <typename T>
class RefObjectImpl: public T {
private:
	wxAtomicInt32 m_refCount;
	

public:
	RefObjectImpl() : m_refCount(1) {}

	virtual void addRef()
	{
		wxAtomicInc(m_refCount);
	}
	
	virtual wxInt32 release()
	{
		wxInt32 refCount = wxAtomicDec(m_refCount);
		if(refCount == 0)
		{
			GameDelete(this);
			return 0;
		} else {
			return refCount;
		}
	}
};

#endif //__REFERENCED_OBJECT_IMPLEMENTATION_H__