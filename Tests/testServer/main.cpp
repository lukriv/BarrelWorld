/////////////////////////////////////////////////////////////////////////////
// Name:        samples/sockbase/client.cpp
// Purpose:     Sockets sample for wxBase
// Author:      Lukasz Michalski
// Modified by:
// Created:     27.06.2005
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Lukasz Michalski <lmichalski@user.sourceforge.net>
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/wx.h"
#include "wx/socket.h"
#include "wx/event.h"
#include "wx/list.h"
#include "wx/cmdline.h"
#include "wx/datetime.h"
#include "wx/timer.h"
#include "wx/thread.h"

const char *GetSocketErrorMsg(int pSockError)
{
    switch(pSockError)
    {
        case wxSOCKET_NOERROR:
            return "wxSOCKET_NOERROR";

        case wxSOCKET_INVOP:
            return "wxSOCKET_INVOP";

        case wxSOCKET_IOERR:
            return "wxSOCKET_IOERR";

        case wxSOCKET_INVADDR:
            return "wxSOCKET_INVADDR";

        case wxSOCKET_NOHOST:
            return "wxSOCKET_NOHOST";

        case wxSOCKET_INVPORT:
            return "wxSOCKET_INVPORT";

        case wxSOCKET_WOULDBLOCK:
            return "wxSOCKET_WOULDBLOCK";

        case wxSOCKET_TIMEDOUT:
            return "wxSOCKET_TIMEDOUT";

        case wxSOCKET_MEMERR:
            return "wxSOCKET_MEMERR";

        default:
            return "Unknown";
    }
}





//main server class contains listening socket
//and list of two type worker classes that serve clients
class Server : public wxApp
{
    DECLARE_EVENT_TABLE()
public:
    Server() : m_maxConnections(-1) {}
    ~Server() {}
private:

    virtual bool OnInit();
    virtual int OnExit();

    void OnInitCmdLine(wxCmdLineParser& pParser);
    bool OnCmdLineParsed(wxCmdLineParser& pParser);

    void OnSocketEvent(wxSocketEvent& pEvent);
    void OnTimerEvent(wxTimerEvent& pEvent);
    void DumpStatistics();

    wxSocketServer* m_listeningSocket;

    // statistics
    unsigned m_threadWorkersCreated;
    unsigned m_threadWorkersDone;
    unsigned m_threadWorkersFailed;
    unsigned m_maxThreadWorkers;

    unsigned m_eventWorkersCreated;
    unsigned m_eventWorkersDone;
    unsigned m_eventWorkersFailed;
    unsigned m_maxEventWorkers;

    long int m_maxConnections;

    unsigned short m_port;

    wxTimer mTimer;
};

DECLARE_APP(Server);



/******************* Implementation ******************/
IMPLEMENT_APP_CONSOLE(Server)


void
Server::OnInitCmdLine(wxCmdLineParser& pParser)
{
    wxApp::OnInitCmdLine(pParser);
}

void
Server::DumpStatistics()
{
    wxString mode;
    wxLogMessage("Server mode: %s",mode);
    wxLogMessage("\t\t\t\tThreads\tEvents\tTotal");
    if ((int)(m_threadWorkersDone+m_eventWorkersDone) == m_maxConnections)
    {
        wxLogMessage("%ld connection(s) served, exiting",m_maxConnections);
        ExitMainLoop();
    }
}


bool
Server::OnCmdLineParsed(wxCmdLineParser& pParser)
{
    return wxApp::OnCmdLineParsed(pParser);
}

bool Server::OnInit()
{
    wxLog* logger = new wxLogStderr();
    wxLog::SetActiveTarget(logger);

    m_port = 3000;

    //send interesting things to console
    if (!wxApp::OnInit())
        return false;

    //setup listening socket
    wxIPV4address la;
    la.Service(m_port);
    m_listeningSocket = new wxSocketServer(la,wxSOCKET_NOWAIT|wxSOCKET_REUSEADDR);
    m_listeningSocket->SetEventHandler(*this);
    m_listeningSocket->SetNotify(wxSOCKET_CONNECTION_FLAG);
    m_listeningSocket->Notify(true);
    if (!m_listeningSocket->IsOk())
    {
        wxLogError("Cannot bind listening socket");
        return false;
    }

    wxLogMessage("Server listening at port %u, waiting for connections", m_port);
    return true;
}


int Server::OnExit()
{
    m_listeningSocket->Destroy();
    return 0;
}

void Server::OnSocketEvent(wxSocketEvent& pEvent)
{
    switch(pEvent.GetSocketEvent())
    {
        case wxSOCKET_INPUT:
            wxLogError("Unexpected wxSOCKET_INPUT in wxSocketServer");
            break;
        case wxSOCKET_OUTPUT:
            wxLogError("Unexpected wxSOCKET_OUTPUT in wxSocketServer");
        break;
        case wxSOCKET_CONNECTION:
        {
            wxSocketBase* sock = m_listeningSocket->Accept();
            wxIPV4address addr;
            if (!sock->GetPeer(addr))
            {
                wxLogError("Server: cannot get peer info");
            } else {
                wxLogMessage("Got connection from %s:%d",addr.IPAddress().c_str(), addr.Service());
            }
 

        }
        break;
        case wxSOCKET_LOST:
            wxLogError("Unexpected wxSOCKET_LOST in wxSocketServer");
        break;
    }
}



BEGIN_EVENT_TABLE(Server,wxEvtHandler)
  EVT_SOCKET(wxID_ANY,Server::OnSocketEvent)
END_EVENT_TABLE()





