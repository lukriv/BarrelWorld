#include <wx/init.h>
#include <wx/wxcrtvararg.h>
#include <wx/string.h>
#include "../../WordOfFlat/GameSystem/gdefs.h"
#include "../../WordOfFlat/GameSystem/gerror.h"
#include "../../WordOfFlat/GameMsgCli/gamemsgcliimpl.h"



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
	
	GameMsgCli client;
	if(FWG_FAILED(result = client.Initialize(NULL))) {
		GameConvertErrCode2String(result, errStr);
		wxPrintf(wxT("Client initialize failed: %s\n"), errStr.GetData());
	}
	
	if(FWG_FAILED(result = client.Connect())) {
		GameConvertErrCode2String(result, errStr);
		wxPrintf(wxT("Client initialize failed: %s\n"), errStr.GetData());
	}
	
	if(!client.IsConnected()) {
		wxPrintf(wxT("Client is not connected\n"));
	}
	
	wxThread::Sleep(5000);
	
	if(client.GetCliAddress() != GAME_ADDR_SERVER + 1) {
		wxPrintf(wxT("Client address is wrong: %d\n"), client.GetCliAddress());
	}
	
	return 0;
}
