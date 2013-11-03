///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "gui.h"

///////////////////////////////////////////////////////////////////////////

MainFrameBase::MainFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 500,300 ), wxDefaultSize );
	
	m_menuBar = new wxMenuBar( 0 );
	m_menuFile = new wxMenu();
	wxMenuItem* newGame;
	newGame = new wxMenuItem( m_menuFile, ID_NEW_GAME, wxString( _("New Game") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( newGame );
	
	wxMenuItem* loadGame;
	loadGame = new wxMenuItem( m_menuFile, ID_LOAD_GAME, wxString( _("Load Game") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( loadGame );
	
	wxMenuItem* saveGame;
	saveGame = new wxMenuItem( m_menuFile, ID_SAVE_GAME, wxString( _("Save Game") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( saveGame );
	
	m_menuFile->AppendSeparator();
	
	wxMenuItem* menuFileExit;
	menuFileExit = new wxMenuItem( m_menuFile, wxID_EXIT, wxString( _("E&xit") ) + wxT('\t') + wxT("Alt+X"), wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( menuFileExit );
	
	m_menuBar->Append( m_menuFile, _("&File") ); 
	
	this->SetMenuBar( m_menuBar );
	
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_htmlWin1 = new wxHtmlWindow( this, wxID_ANY, wxDefaultPosition, wxSize( 385,300 ), wxHW_SCROLLBAR_AUTO );
	bSizer2->Add( m_htmlWin1, 0, wxALL, 5 );
	
	wxArrayString m_choice1Choices;
	m_choice1 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 385,-1 ), m_choice1Choices, 0 );
	m_choice1->SetSelection( 0 );
	bSizer2->Add( m_choice1, 0, wxALL, 5 );
	
	
	mainSizer->Add( bSizer2, 1, wxEXPAND|wxRIGHT, 0 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	
	mainSizer->Add( bSizer3, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( mainSizer );
	this->Layout();
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	this->Connect( menuFileExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnExitClick ) );
}

MainFrameBase::~MainFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	this->Disconnect( wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnExitClick ) );
	
}
