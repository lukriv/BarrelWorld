#ifndef __GAME_SOCKET_H__
#define __GAME_SOCKET_H__


#include <wx/socket.h>
#include "gerror.h"

inline GameErrorCode GameConvertWxSocketErr2GameErr( wxSocketError streamErr)
{
	switch (streamErr)
	{
	case wxSOCKET_NOERROR:
		return FWG_NO_ERROR;
	case wxSOCKET_INVOP: 	
		return FWG_E_INVALID_OPERATION_ERROR;  // Invalid operation.
	case wxSOCKET_IOERR 	
		return FWG_E_IO_ERROR; // Input/Output error.
	case wxSOCKET_INVADDR 	
		return FWG_E_INVALID_ADDRESS_ERROR;	// Invalid address passed to wxSocket.
	case wxSOCKET_INVSOCK 	
		return FWG_E_INVALID_SOCKET_ERROR;  // Invalid socket (uninitialized).
	case wxSOCKET_NOHOST 	
		return FWG_E_NOHOST_ERROR; // No corresponding host.
	case wxSOCKET_INVPORT	
		return FWG_E_INVALID_PORT_ERROR; // Invalid port.
	case wxSOCKET_WOULDBLOCK 	
		return FWG_E_WOULD_BLOCK_WARNING; // The socket is non-blocking and the operation would block.
	case wxSOCKET_TIMEDOUT 	
		return FWG_E_TIMEOUT_WARNING; // The timeout for this operation expired.
	case wxSOCKET_MEMERR 
		return FWG_E_MEMORY_ERROR; // Memory exhausted.
	default:
		return FWG_E_MISC_ERROR;
	}
}



#endif // __GAME_SOCKET_H__