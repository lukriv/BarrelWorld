#ifndef __PERSISTENT_OBJECT_H__
#define __PERSISTENT_OBJECT_H__


#include <wx/defs.h>


class IPersistentObject {
public:
	GameErrorCode Load(wxInputStream &stream) = 0;
	GameErrorCode Store(wxOutputStream &stream) = 0;
}

#endif //__PERSISTENT_OBJECT_H__