/***************************************************************
 * Name:      ExtM3UeditMain.h
 * Purpose:   Defines Application Frame
 * Author:    Pavel Tyunin (pavel51tunin@gmail.com)
 * Created:   2018-08-07
 * Copyright: Pavel Tyunin
 * License:
 **************************************************************/

#ifndef EXTM3UEDITMAIN_H
#define EXTM3UEDITMAIN_H

//(*Headers(ExtM3UeditFrame)
#include <wx/bmpbuttn.h>
#include <wx/frame.h>
#include <wx/listctrl.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/statusbr.h>
//*)
#include "em_el_view.h"

class ExtM3UeditFrame: public wxFrame, public EMObserver
{
public:

    ExtM3UeditFrame(wxWindow* parent,wxWindowID id = -1);
    virtual ~ExtM3UeditFrame();

    void notify(EMUpdateMode t, Index begin, Index end) override;
private:

    //(*Handlers(ExtM3UeditFrame)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnNewMenuItemSelected(wxCommandEvent& event);
    void OnOpenMenuItemSelected(wxCommandEvent& event);
    void OnSaveMenuItemSelected(wxCommandEvent& event);
    void OnSaveAsMenuItemSelected(wxCommandEvent& event);
    void OnAddEntryButtonClick(wxCommandEvent& event);
    void OnEntryUpButtonClick(wxCommandEvent& event);
    void OnEntryDownButtonClick(wxCommandEvent& event);
    void OnRemoveEntryButtonClick(wxCommandEvent& event);
    void OnEntryListViewResize(wxSizeEvent& event);
    //*)

    //(*Identifiers(ExtM3UeditFrame)
    static const long ID_ADDENTRY_BITMAPBUTTON;
    static const long ID_ENTRYUP_BITMAPBUTTON;
    static const long ID_ENTRYDOWN_BITMAPBUTTON;
    static const long ID_REMOVEENTRY_BITMAPBUTTON;
    static const long ID_ENTRY_LISTVIEW;
    static const long ID_MAIN_PANEL;
    static const long idMenuNew;
    static const long idMenuOpen;
    static const long idMenuSave;
    static const long idMenuSaveAs;
    static const long idMenuQuit;
    static const long idMenuAbout;
    static const long ID_STATUSBAR;
    //*)

    //(*Declarations(ExtM3UeditFrame)
    EMEntryListView* EntryListView;
    wxBitmapButton* AddEntryButton;
    wxBitmapButton* EntryDownButton;
    wxBitmapButton* EntryUpButton;
    wxBitmapButton* RemoveEntryButton;
    wxPanel* MainPanel;
    wxStatusBar* StatusBar;
    //*)
    EMEditor m_editor;

    DECLARE_EVENT_TABLE()
};

#endif // EXTM3UEDITMAIN_H
