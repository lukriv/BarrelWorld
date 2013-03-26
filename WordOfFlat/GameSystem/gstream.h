#ifndef __GAME_STREAM_H__
#define __GAME_STREAM_H__


#include <wx/stream.h>
#include "gerror.h"

inline GameErrorCode GameConvertWxStreamErr2GameErr(wxStreamError streamErr)
{
	switch (streamErr)
	{
	case wxSTREAM_NO_ERROR:
		return FWG_NO_ERROR;
	case wxSTREAM_EOF:
		return FWG_E_STREAM_EOF_ERROR;
	case wxSTREAM_WRITE_ERROR:
		return FWG_E_STREAM_WRITE_ERROR;
	case wxSTREAM_READ_ERROR:
		return FWG_E_STREAM_READ_ERROR;
	default:
		return FWG_E_MISC_ERROR;
	}
}


#endif //__GAME_STREAM_H__