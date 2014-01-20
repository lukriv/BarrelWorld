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
	this->SetSizeHints( wxSize( 800,600 ), wxDefaultSize );
	
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
	
	wxBoxSizer* leftSizer;
	leftSizer = new wxBoxSizer( wxVERTICAL );
	
	m_htmlDescription = new wxHtmlWindow( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxHW_SCROLLBAR_AUTO );
	m_htmlDescription->SetMinSize( wxSize( 385,300 ) );
	
	leftSizer->Add( m_htmlDescription, 0, wxALL, 5 );
	
	wxArrayString m_actionChoices;
	m_action = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), m_actionChoices, 0 );
	m_action->SetSelection( 0 );
	m_action->SetMinSize( wxSize( 385,-1 ) );
	
	leftSizer->Add( m_action, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* buttonsSizer;
	buttonsSizer = new wxBoxSizer( wxVERTICAL );
	
	buttonsSizer->SetMinSize( wxSize( 150,-1 ) ); 
	m_continueButton = new wxButton( this, wxID_ANY, _("Continue"), wxDefaultPosition, wxDefaultSize, 0 );
	m_continueButton->SetMinSize( wxSize( 120,-1 ) );
	
	buttonsSizer->Add( m_continueButton, 0, wxALL, 5 );
	
	m_dropWeaponButton = new wxButton( this, wxID_ANY, _("Drop Weapon"), wxDefaultPosition, wxDefaultSize, 0 );
	m_dropWeaponButton->SetMinSize( wxSize( 120,-1 ) );
	
	buttonsSizer->Add( m_dropWeaponButton, 0, wxALL, 5 );
	
	m_dropBagButton = new wxButton( this, wxID_ANY, _("Drop Bag Item"), wxDefaultPosition, wxDefaultSize, 0 );
	m_dropBagButton->SetMinSize( wxSize( 120,-1 ) );
	
	buttonsSizer->Add( m_dropBagButton, 0, wxALL, 5 );
	
	
	bSizer4->Add( buttonsSizer, 1, wxEXPAND, 5 );
	
	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel1->SetMinSize( wxSize( 300,-1 ) );
	
	bSizer4->Add( m_panel1, 1, wxEXPAND | wxALL, 5 );
	
	
	leftSizer->Add( bSizer4, 1, wxEXPAND, 5 );
	
	
	mainSizer->Add( leftSizer, 1, wxEXPAND|wxRIGHT, 0 );
	
	wxBoxSizer* rightSizer;
	rightSizer = new wxBoxSizer( wxVERTICAL );
	
	rightSizer->SetMinSize( wxSize( 375,120 ) ); 
	m_disciplineList = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxLC_REPORT|wxLC_SINGLE_SEL );
	m_disciplineList->SetMinSize( wxSize( 385,140 ) );
	
	rightSizer->Add( m_disciplineList, 0, wxALL, 5 );
	
	m_weaponList = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxLC_REPORT|wxLC_SINGLE_SEL );
	m_weaponList->SetMinSize( wxSize( 385,70 ) );
	
	rightSizer->Add( m_weaponList, 0, wxALL, 5 );
	
	wxStaticBoxSizer* bagSizer;
	bagSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("label") ), wxHORIZONTAL );
	
	bagSizer->SetMinSize( wxSize( 375,150 ) ); 
	m_itemList = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), 0, NULL, 0 ); 
	m_itemList->SetMinSize( wxSize( 185,180 ) );
	
	bagSizer->Add( m_itemList, 0, wxALL, 5 );
	
	m_specialItems = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), 0, NULL, 0 ); 
	m_specialItems->SetMinSize( wxSize( 185,180 ) );
	
	bagSizer->Add( m_specialItems, 0, wxALL, 5 );
	
	
	rightSizer->Add( bagSizer, 1, wxEXPAND, 5 );
	
	m_listCtrl6 = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_NO_HEADER|wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_VRULES );
	m_listCtrl6->SetMinSize( wxSize( 375,-1 ) );
	
	rightSizer->Add( m_listCtrl6, 0, wxALL, 5 );
	
	
	mainSizer->Add( rightSizer, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( mainSizer );
	this->Layout();
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( MainFrameBase::ResizeAll ) );
	this->Connect( newGame->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::CreateNewGame ) );
	this->Connect( loadGame->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::LoadGame ) );
	this->Connect( saveGame->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::SaveGame ) );
	this->Connect( menuFileExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnExitClick ) );
	m_continueButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::DoNextStep ), NULL, this );
	m_dropWeaponButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::DropWeapon ), NULL, this );
	m_dropBagButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::DropBagItem ), NULL, this );
}

MainFrameBase::~MainFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( MainFrameBase::ResizeAll ) );
	this->Disconnect( ID_NEW_GAME, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::CreateNewGame ) );
	this->Disconnect( ID_LOAD_GAME, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::LoadGame ) );
	this->Disconnect( ID_SAVE_GAME, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::SaveGame ) );
	this->Disconnect( wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnExitClick ) );
	m_continueButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::DoNextStep ), NULL, this );
	m_dropWeaponButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::DropWeapon ), NULL, this );
	m_dropBagButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::DropBagItem ), NULL, this );
	
}
