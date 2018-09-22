/***************************************************************
 * Name:      ExtM3UeditMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Pavel Tyunin (pavel51tunin@gmail.com)
 * Created:   2018-08-07
 * Copyright: Pavel Tyunin
 * License:
 **************************************************************/

#include "ExtM3UeditMain.h"
#include <wx/aboutdlg.h>
#include <wx/filedlg.h>
#include <wx/textdlg.h>
#include <wx/wupdlock.h>

//(*InternalHeaders(ExtM3UeditFrame)
#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(ExtM3UeditFrame)
const long ExtM3UeditFrame::ID_ADDENTRY_BITMAPBUTTON = wxNewId();
const long ExtM3UeditFrame::ID_ENTRYUP_BITMAPBUTTON = wxNewId();
const long ExtM3UeditFrame::ID_ENTRYDOWN_BITMAPBUTTON = wxNewId();
const long ExtM3UeditFrame::ID_REMOVEENTRY_BITMAPBUTTON = wxNewId();
const long ExtM3UeditFrame::ID_ENTRY_LISTVIEW = wxNewId();
const long ExtM3UeditFrame::ID_PARAM_VIEW = wxNewId();
const long ExtM3UeditFrame::ID_ADDATTR_BITMAPBUTTON = wxNewId();
const long ExtM3UeditFrame::ID_ATTRUP_BITMAPBUTTON = wxNewId();
const long ExtM3UeditFrame::ID_ATTRDOWN_BITMAPBUTTON = wxNewId();
const long ExtM3UeditFrame::ID_REMOVEATTR_BITMAPBUTTON = wxNewId();
const long ExtM3UeditFrame::ID_MAIN_PANEL = wxNewId();
const long ExtM3UeditFrame::idMenuNew = wxNewId();
const long ExtM3UeditFrame::idMenuOpen = wxNewId();
const long ExtM3UeditFrame::idMenuSave = wxNewId();
const long ExtM3UeditFrame::idMenuSaveAs = wxNewId();
const long ExtM3UeditFrame::idMenuQuit = wxNewId();
const long ExtM3UeditFrame::idMenuAbout = wxNewId();
const long ExtM3UeditFrame::ID_STATUSBAR = wxNewId();
//*)

BEGIN_EVENT_TABLE(ExtM3UeditFrame,wxFrame)
    //(*EventTable(ExtM3UeditFrame)
    //*)
END_EVENT_TABLE()

ExtM3UeditFrame::ExtM3UeditFrame(wxWindow* parent,wxWindowID id)
{
    wxGCC_WARNING_SUPPRESS_CAST_FUNCTION_TYPE()
    //(*Initialize(ExtM3UeditFrame)
    wxBoxSizer* BoxSizer1;
    wxBoxSizer* BoxSizer2;
    wxBoxSizer* BoxSizer3;
    wxMenu* FileMenu;
    wxMenu* HelpMenu;
    wxMenuBar* MenuBar;
    wxMenuItem* AboutMenuItem;
    wxMenuItem* NewMenuItem;
    wxMenuItem* OpenMenuItem;
    wxMenuItem* QuitMenuItem;
    wxMenuItem* SaveAsMenuItem;
    wxMenuItem* SaveMenuItem;

    Create(parent, id, _("ExtM3Uedit"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    SetClientSize(wxSize(800,600));
    MainPanel = new wxPanel(this, ID_MAIN_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_MAIN_PANEL"));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer2 = new wxBoxSizer(wxVERTICAL);
    AddEntryButton = new wxBitmapButton(MainPanel, ID_ADDENTRY_BITMAPBUTTON, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_PLUS")),wxART_BUTTON), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_ADDENTRY_BITMAPBUTTON"));
    BoxSizer2->Add(AddEntryButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    EntryUpButton = new wxBitmapButton(MainPanel, ID_ENTRYUP_BITMAPBUTTON, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_GO_UP")),wxART_BUTTON), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_ENTRYUP_BITMAPBUTTON"));
    BoxSizer2->Add(EntryUpButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    EntryDownButton = new wxBitmapButton(MainPanel, ID_ENTRYDOWN_BITMAPBUTTON, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_GO_DOWN")),wxART_BUTTON), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_ENTRYDOWN_BITMAPBUTTON"));
    BoxSizer2->Add(EntryDownButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    RemoveEntryButton = new wxBitmapButton(MainPanel, ID_REMOVEENTRY_BITMAPBUTTON, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_DELETE")),wxART_BUTTON), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_REMOVEENTRY_BITMAPBUTTON"));
    BoxSizer2->Add(RemoveEntryButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1->Add(BoxSizer2, 0, wxTOP|wxBOTTOM|wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    EntryListView = new EMEntryListView(MainPanel, ID_ENTRY_LISTVIEW, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_VIRTUAL, wxDefaultValidator, _T("ID_ENTRY_LISTVIEW"));
    BoxSizer1->Add(EntryListView, 1, wxALL|wxEXPAND, 5);
    ParametersView = new EMParametersView(MainPanel,ID_PARAM_VIEW,wxDefaultPosition,wxSize(280,590),0,_T("ID_PARAM_VIEW"));
    BoxSizer1->Add(ParametersView, 0, wxALL|wxEXPAND|wxFIXED_MINSIZE, 5);
    BoxSizer3 = new wxBoxSizer(wxVERTICAL);
    AddAttrButton = new wxBitmapButton(MainPanel, ID_ADDATTR_BITMAPBUTTON, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_PLUS")),wxART_BUTTON), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_ADDATTR_BITMAPBUTTON"));
    BoxSizer3->Add(AddAttrButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    AttrUpButton = new wxBitmapButton(MainPanel, ID_ATTRUP_BITMAPBUTTON, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_GO_UP")),wxART_BUTTON), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_ATTRUP_BITMAPBUTTON"));
    BoxSizer3->Add(AttrUpButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    AttrDownButton = new wxBitmapButton(MainPanel, ID_ATTRDOWN_BITMAPBUTTON, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_GO_DOWN")),wxART_BUTTON), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_ATTRDOWN_BITMAPBUTTON"));
    BoxSizer3->Add(AttrDownButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    RemoveAttrButton = new wxBitmapButton(MainPanel, ID_REMOVEATTR_BITMAPBUTTON, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_DELETE")),wxART_BUTTON), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_REMOVEATTR_BITMAPBUTTON"));
    BoxSizer3->Add(RemoveAttrButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1->Add(BoxSizer3, 0, wxTOP|wxBOTTOM|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    MainPanel->SetSizer(BoxSizer1);
    BoxSizer1->Fit(MainPanel);
    BoxSizer1->SetSizeHints(MainPanel);
    MenuBar = new wxMenuBar();
    FileMenu = new wxMenu();
    NewMenuItem = new wxMenuItem(FileMenu, idMenuNew, _("New\tCtrl+N"), _("Create a new empty playlist"), wxITEM_NORMAL);
    NewMenuItem->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_NEW")),wxART_MENU));
    FileMenu->Append(NewMenuItem);
    OpenMenuItem = new wxMenuItem(FileMenu, idMenuOpen, _("Open...\tCtrl+O"), _("Open an existing playlist file"), wxITEM_NORMAL);
    OpenMenuItem->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_FILE_OPEN")),wxART_MENU));
    FileMenu->Append(OpenMenuItem);
    SaveMenuItem = new wxMenuItem(FileMenu, idMenuSave, _("Save\tCtrl+S"), _("Save the playlist"), wxITEM_NORMAL);
    SaveMenuItem->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_FILE_SAVE")),wxART_MENU));
    FileMenu->Append(SaveMenuItem);
    SaveAsMenuItem = new wxMenuItem(FileMenu, idMenuSaveAs, _("Save As...\tCtrl+Alt+S"), _("Save the playlist under a different name"), wxITEM_NORMAL);
    SaveAsMenuItem->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_FILE_SAVE_AS")),wxART_MENU));
    FileMenu->Append(SaveAsMenuItem);
    QuitMenuItem = new wxMenuItem(FileMenu, idMenuQuit, _("Quit\tAlt+F4"), _("Quit the application"), wxITEM_NORMAL);
    QuitMenuItem->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_QUIT")),wxART_MENU));
    FileMenu->Append(QuitMenuItem);
    MenuBar->Append(FileMenu, _("&File"));
    HelpMenu = new wxMenu();
    AboutMenuItem = new wxMenuItem(HelpMenu, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    AboutMenuItem->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_INFORMATION")),wxART_MENU));
    HelpMenu->Append(AboutMenuItem);
    MenuBar->Append(HelpMenu, _("Help"));
    SetMenuBar(MenuBar);
    StatusBar = new wxStatusBar(this, ID_STATUSBAR, 0, _T("ID_STATUSBAR"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar);

    Connect(ID_ADDENTRY_BITMAPBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ExtM3UeditFrame::OnAddEntryButtonClick);
    Connect(ID_ENTRYUP_BITMAPBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ExtM3UeditFrame::OnEntryUpButtonClick);
    Connect(ID_ENTRYDOWN_BITMAPBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ExtM3UeditFrame::OnEntryDownButtonClick);
    Connect(ID_REMOVEENTRY_BITMAPBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ExtM3UeditFrame::OnRemoveEntryButtonClick);
    Connect(ID_ENTRY_LISTVIEW,wxEVT_COMMAND_LIST_ITEM_SELECTED,(wxObjectEventFunction)&ExtM3UeditFrame::OnEntryListViewItemSelect);
    Connect(ID_ADDATTR_BITMAPBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ExtM3UeditFrame::OnAddAttrButtonClick);
    Connect(ID_ATTRUP_BITMAPBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ExtM3UeditFrame::OnAttrUpButtonClick);
    Connect(ID_ATTRDOWN_BITMAPBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ExtM3UeditFrame::OnAttrDownButtonClick);
    Connect(ID_REMOVEATTR_BITMAPBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ExtM3UeditFrame::OnRemoveAttrButtonClick);
    Connect(idMenuNew,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ExtM3UeditFrame::OnNewMenuItemSelected);
    Connect(idMenuOpen,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ExtM3UeditFrame::OnOpenMenuItemSelected);
    Connect(idMenuSave,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ExtM3UeditFrame::OnSaveMenuItemSelected);
    Connect(idMenuSaveAs,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ExtM3UeditFrame::OnSaveAsMenuItemSelected);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ExtM3UeditFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ExtM3UeditFrame::OnAbout);
    //*)
    EntryListView->Connect(wxEVT_SIZE,(wxObjectEventFunction)&ExtM3UeditFrame::OnEntryListViewResize,0,this);
    wxGCC_WARNING_RESTORE_CAST_FUNCTION_TYPE()

    SetIcon(wxIcon("aaaa", wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));

    m_editor.addObserver(this);
    EntryListView->initEditor(&m_editor);
    ParametersView->initEditor(&m_editor);
    notify(EMUpdateMode::Info, 0, 0);
}

ExtM3UeditFrame::~ExtM3UeditFrame()
{
    //(*Destroy(ExtM3UeditFrame)
    //*)
}

inline wxString getAppVersion() { return "v0.1-dev"; }

void ExtM3UeditFrame::notify(EMUpdateMode t, Index /*begin*/, Index /*end*/)
{
    if(t == EMUpdateMode::Reset || t == EMUpdateMode::Info)
        SetTitle(m_editor.getFileName() + " - " + wxTheApp->GetAppName() + ' ' + getAppVersion());
}

void ExtM3UeditFrame::OnQuit(wxCommandEvent& /*event*/)
{
    Close();
}

void ExtM3UeditFrame::OnAbout(wxCommandEvent& /*event*/)
{
    wxAboutDialogInfo a;
    a.SetVersion(getAppVersion());
    a.SetDescription("Extended M3U playlist editor");
    a.SetCopyright("(C) 2018 Pavel Tyunin <pavel51tunin@gmail.com>");
    a.SetIcon(wxIcon("aaaa", wxBITMAP_TYPE_ICO_RESOURCE, 64, 64));
    wxAboutBox(a, this);
}

void ExtM3UeditFrame::OnEntryListViewResize(wxSizeEvent& event)
{
    EntryListView->ColumnsAutoResize();
    event.Skip();
}

void ExtM3UeditFrame::OnNewMenuItemSelected(wxCommandEvent& /*event*/)
{
    m_editor.createFile();
}

void ExtM3UeditFrame::OnOpenMenuItemSelected(wxCommandEvent& /*event*/)
{
    wxFileDialog d(this, wxFileSelectorPromptStr, wxEmptyString, wxEmptyString,
                   "M3U files (*.m3u;*.m3u8)|*.m3u;*.m3u8|All files|*",
                   wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if(d.ShowModal() == wxID_OK)
        m_editor.openFile(d.GetPath());
}

void ExtM3UeditFrame::OnSaveMenuItemSelected(wxCommandEvent& event)
{
    if(m_editor.isSaveable())
        m_editor.saveFile();
    else
        OnSaveAsMenuItemSelected(event);
}

void ExtM3UeditFrame::OnSaveAsMenuItemSelected(wxCommandEvent& /*event*/)
{
    wxFileDialog d(this, wxFileSelectorPromptStr, wxEmptyString, wxEmptyString,
                   "M3U files (*.m3u;*.m3u8)|*.m3u;*.m3u8|All files|*",
                   wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if(d.ShowModal() == wxID_OK)
        m_editor.saveFileAs(d.GetPath());
}

void ExtM3UeditFrame::OnAddEntryButtonClick(wxCommandEvent& /*event*/)
{
    wxTextEntryDialog d(this, "Enter URL for the new entry");
    if(d.ShowModal() == wxID_OK)
    {
        auto s = EntryListView->GetFirstSelected();
        m_editor.addEntry((s != -1? s : m_editor.getEntryNumber()), d.GetValue());
    }
}

void ExtM3UeditFrame::OnEntryUpButtonClick(wxCommandEvent& /*event*/)
{
    auto s = EntryListView->GetFirstSelected();
    if(s != -1)
    {
        wxWindowUpdateLocker lock(EntryListView);
        m_editor.moveEntryUp(s);
    }
}

void ExtM3UeditFrame::OnEntryDownButtonClick(wxCommandEvent& /*event*/)
{
    auto s = EntryListView->GetFirstSelected();
    if(s != -1)
    {
        wxWindowUpdateLocker lock(EntryListView);
        m_editor.moveEntryDown(s);
    }
}

void ExtM3UeditFrame::OnRemoveEntryButtonClick(wxCommandEvent& /*event*/)
{
    auto s = EntryListView->GetFirstSelected();
    if(s != -1)
        m_editor.removeEntry(s);
}

void ExtM3UeditFrame::OnEntryListViewItemSelect(wxListEvent& /*event*/)
{
    ParametersView->selectEntry(EntryListView->GetFirstSelected());
}

void ExtM3UeditFrame::OnAddAttrButtonClick(wxCommandEvent& /*event*/)
{
    ParametersView->addAttr();
}

void ExtM3UeditFrame::OnAttrUpButtonClick(wxCommandEvent& /*event*/)
{
    ParametersView->attrUp();
}

void ExtM3UeditFrame::OnAttrDownButtonClick(wxCommandEvent& /*event*/)
{
    ParametersView->attrDown();
}

void ExtM3UeditFrame::OnRemoveAttrButtonClick(wxCommandEvent& /*event*/)
{
    ParametersView->removeAttr();
}
