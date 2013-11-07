///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GUI_H__
#define __GUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/html/htmlwin.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/listctrl.h>
#include <wx/listbox.h>
#include <wx/statbox.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define ID_NEW_GAME 1000
#define ID_LOAD_GAME 1001
#define ID_SAVE_GAME 1002

///////////////////////////////////////////////////////////////////////////////
/// Class MainFrameBase
///////////////////////////////////////////////////////////////////////////////
class MainFrameBase : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* m_menuBar;
		wxMenu* m_menuFile;
		wxHtmlWindow* m_htmlDescription;
		wxChoice* m_action;
		wxButton* m_continueButton;
		wxButton* m_dropWeaponButton;
		wxButton* m_dropBagButton;
		wxPanel* m_panel1;
		wxListCtrl* m_disciplineList;
		wxListCtrl* m_weaponList;
		wxListBox* m_itemList;
		wxListBox* m_specialItems;
		wxListCtrl* m_listCtrl6;
		wxStatusBar* m_statusBar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCloseFrame( wxCloseEvent& event ) { event.Skip(); }
		virtual void ResizeAll( wxSizeEvent& event ) { event.Skip(); }
		virtual void OnExitClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void DoNextStep( wxCommandEvent& event ) { event.Skip(); }
		virtual void DropWeapon( wxCommandEvent& event ) { event.Skip(); }
		virtual void DropBagItem( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("wxMiniApp"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxCLOSE_BOX|wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MainFrameBase();
	
};

#endif //__GUI_H__
