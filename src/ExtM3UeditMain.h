/***************************************************************
 * Name:      ExtM3UeditMain.h
 * Purpose:   Defines Application Frame
 * Author:    Pavel Tyunin (pavel51tunin@gmail.com)
 * Created:   2018-08-07
 * Copyright: Pavel Tyunin
 * License:   GNU General Public License v3.0
 **************************************************************/

#ifndef EXTM3UEDITMAIN_H
#define EXTM3UEDITMAIN_H

//(*Headers(ExtM3UeditFrame)
#include "em_par_view.h"
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
    void OnEntryListViewItemSelect(wxListEvent& event);
    void OnAddAttrButtonClick(wxCommandEvent& event);
    void OnRemoveAttrButtonClick(wxCommandEvent& event);
    void OnAttrUpButtonClick(wxCommandEvent& event);
    void OnAttrDownButtonClick(wxCommandEvent& event);
    void OnNormalizeMenuItemSelected(wxCommandEvent& event);
    void OnEncodingUTF8MenuItemSelected(wxCommandEvent& event);
    void OnEncodingANSIMenuItemSelected(wxCommandEvent& event);
    void OnEncodingBOMMenuItemSelected(wxCommandEvent& event);
    void OnEncodingCRLFMenuItemSelected(wxCommandEvent& event);
    void OnEncodingLFMenuItemSelected(wxCommandEvent& event);
    void OnEncodingCRMenuItemSelected(wxCommandEvent& event);
    //*)

    //(*Identifiers(ExtM3UeditFrame)
    static const long ID_ADDENTRY_BITMAPBUTTON;
    static const long ID_ENTRYUP_BITMAPBUTTON;
    static const long ID_ENTRYDOWN_BITMAPBUTTON;
    static const long ID_REMOVEENTRY_BITMAPBUTTON;
    static const long ID_ENTRY_LISTVIEW;
    static const long ID_PARAM_VIEW;
    static const long ID_ADDATTR_BITMAPBUTTON;
    static const long ID_ATTRUP_BITMAPBUTTON;
    static const long ID_ATTRDOWN_BITMAPBUTTON;
    static const long ID_REMOVEATTR_BITMAPBUTTON;
    static const long ID_MAIN_PANEL;
    static const long idMenuNew;
    static const long idMenuOpen;
    static const long idMenuSave;
    static const long idMenuSaveAs;
    static const long idMenuEncodingUTF8;
    static const long idMenuEncodingANSI;
    static const long idMenuEncodingUnknown;
    static const long idMenuEncodingBOM;
    static const long idMenuEncodingCRLF;
    static const long idMenuEncodingLF;
    static const long idMenuEncodingCR;
    static const long idMenuEncoding;
    static const long idMenuQuit;
    static const long idMenuNormalize;
    static const long idMenuAbout;
    static const long ID_STATUSBAR;
    //*)

    //(*Declarations(ExtM3UeditFrame)
    EMEntryListView* EntryListView;
    EMParametersView* ParametersView;
    wxBitmapButton* AddAttrButton;
    wxBitmapButton* AddEntryButton;
    wxBitmapButton* AttrDownButton;
    wxBitmapButton* AttrUpButton;
    wxBitmapButton* EntryDownButton;
    wxBitmapButton* EntryUpButton;
    wxBitmapButton* RemoveAttrButton;
    wxBitmapButton* RemoveEntryButton;
    wxMenu* EncodingMenu;
    wxMenuItem* EncodingANSIMenuItem;
    wxMenuItem* EncodingBOMMenuItem;
    wxMenuItem* EncodingCRLFMenuItem;
    wxMenuItem* EncodingCRMenuItem;
    wxMenuItem* EncodingLFMenuItem;
    wxMenuItem* EncodingUTF8MenuItem;
    wxMenuItem* EncodingUnknownMenuItem;
    wxPanel* MainPanel;
    wxStatusBar* StatusBar;
    //*)
    EMEditor m_editor;

    DECLARE_EVENT_TABLE()
};

#endif // EXTM3UEDITMAIN_H
