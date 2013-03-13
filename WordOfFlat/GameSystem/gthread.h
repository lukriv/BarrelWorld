#ifndef __GAME_THREAD_H__
#define __GAME_THREAD_H__

#include <wx/thread.h>
#include "gerror.h"

inline GameErrorCode GameConvertWxThreadErr2GameErr(wxThreadError threadErr)
{
	switch (threadErr)
	{
	case wxTHREAD_NO_ERROR:
		return FWG_NO_ERROR;
	case wxTHREAD_NO_RESOURCE:
		return FWG_E_NO_RESOURCES_ERROR;
	case wxTHREAD_RUNNING:
		return FWG_E_THREAD_ALREADY_RUNNING_ERROR;
	case wxTHREAD_NOT_RUNNING:
		return FWG_E_THREAD_NOT_RUNNING_ERROR;
	case wxTHREAD_KILLED:
		return FWG_E_THREAD_ALREADY_KILLED_ERROR;
	case wxTHREAD_MISC_ERROR:
	default:
		return FWG_E_MISC_ERROR;
	}
}

/*!
 * \class GameThread
 * \author Lukas
 * \date 8.3.2013
 * \file gthread.h
 * \brief Wraper over wxThread
 * 
 * Wraps error codes.
 */
class GameThread : public wxThread {
public:
	inline GameErrorCode Create (unsigned int stackSize=0){
		return GameConvertWxThreadErr2GameErr(wxThread::Create(stackSize));
	}
	
	inline GameErrorCode Delete (ExitCode *rc=NULL, wxThreadWait waitMode=wxTHREAD_WAIT_BLOCK){
		return GameConvertWxThreadErr2GameErr(wxThread::Delete(rc, waitMode));
	}
	
	inline GameErrorCode Kill (){
		return GameConvertWxThreadErr2GameErr(wxThread::Kill());
	}
  
	inline GameErrorCode Pause (){
		return GameConvertWxThreadErr2GameErr(wxThread::Pause());
	}
 
	inline GameErrorCode Resume (){
		return GameConvertWxThreadErr2GameErr(wxThread::Resume());
	}
  
	inline GameErrorCode Run (){
		return GameConvertWxThreadErr2GameErr(wxThread::Run());
	}
	
}

#endif //__GAME_THREAD_H__