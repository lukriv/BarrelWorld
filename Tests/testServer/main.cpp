#include <wx/init.h>
#include <wx/wxcrtvararg.h>
#include <wx/string.h>
#include "../../WordOfFlat/GameSystem/gdefs.h"
#include "../../WordOfFlat/GameSystem/gerror.h"
#include "../../WordOfFlat/GameMsgSrv/gamemsgsrvimpl.h"

int main(int argc, char **argv)
{
	wxInitializer intialize (argc, argv);
	if (!intialize.IsOk())
	{
		wxPrintf(wxT("Initializer failed"));
		return -1;
	}
	
	GameErrorCode result = FWG_NO_ERROR;
	wxString errStr;
	
	GameMsgSrv server;
	if(FWG_FAILED(result = server.Initialize(NULL))) {
		GameConvertErrCode2String(result, errStr);
		wxPrintf(wxT("Server initialize failed: %s\n"), errStr.GetData());
	}
	
	if(FWG_FAILED(result = server.Connect())) {
		GameConvertErrCode2String(result, errStr);
		wxPrintf(wxT("Server initialize failed: %s\n"), errStr.GetData());
	}
	
	if(!server.IsConnected()) {
		wxPrintf(wxT("Server is not connected\n"));
	}
	
	wxThread::Sleep(120000);
	
	if(server.GetCliAddress() != GAME_ADDR_SERVER + 1) {
		wxPrintf(wxT("Server address is wrong: %d\n"), server.GetCliAddress());
	}
	
	return 0;
}
