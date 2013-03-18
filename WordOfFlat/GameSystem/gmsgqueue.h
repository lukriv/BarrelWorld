#ifndef __GAME_MESSAGE_QUEUE_H__
#define __GAME_MESSAGE_QUEUE_H__

#include <wx/msgqueue.h>
#include "gerror.h"


inline GameErrorCode GameConvertWxMsgQueueErr2GameErr(wxMessageQueueError msgQueueErr)
{
	switch (msgQueueErr)
	{
		case wxMSGQUEUE_NO_ERROR:
			return FWG_NO_ERROR;
		case wxMSGQUEUE_TIMEOUT:
			return FWG_E_TIMEOUT_WARNING;
		case wxMSGQUEUE_MISC_ERROR:
		default:
			return FWG_E_MISC_ERROR;
	}
}


template <typename T>
class GameMessageQueue : public wxMessageQueue<T>
{
public:
	// Add a message to this queue and signal the threads waiting for messages.
    //
    // This method is safe to call from multiple threads in parallel.
    inline GameErrorCode Post(const T& msg)
    {
		return GameConvertWxMsgQueueErr2GameErr(wxMessageQueue<T>::Post(msg));
    }

    // Remove all messages from the queue.
    //
    // This method is meant to be called from the same thread(s) that call
    // Post() to discard any still pending requests if they became unnecessary.
    inline GameErrorCode Clear()
    {
		return GameConvertWxMsgQueueErr2GameErr(wxMessageQueue<T>::Clear());
    }

    // Wait no more than timeout milliseconds until a message becomes available.
    //
    // Setting timeout to 0 is equivalent to an infinite timeout. See Receive().
    inline GameErrorCode ReceiveTimeout(long timeout, T& msg)
    {
		return GameConvertWxMsgQueueErr2GameErr(wxMessageQueue<T>::ReceiveTimeout(timeout, msg));
    }

    // Same as ReceiveTimeout() but waits for as long as it takes for a message
    // to become available (so it can't return wxMSGQUEUE_TIMEOUT)
    inline GameErrorCode Receive(T& msg)
    {
		return GameConvertWxMsgQueueErr2GameErr(wxMessageQueue<T>::Receive(msg));
    }

};




#endif //__GAME_MESSAGE_QUEUE_H__