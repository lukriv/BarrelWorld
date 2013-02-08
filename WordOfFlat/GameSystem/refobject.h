#ifndef __REFERENCED_OBJECT_H__
#define __REFERENCED_OBJECT_H__


#include <wx/defs.h>


class IRefObject {
protected:
	virtual ~IRefObject(){};

public:
	virtual void addRef() = 0;
	virtual wxInt32 release() = 0;
};

#endif //__REFERENCED_OBJECT_H__