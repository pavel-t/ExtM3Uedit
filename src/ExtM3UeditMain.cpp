/***************************************************************
 * Name:      ExtM3UeditMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Pavel Tyunin (pavel51tunin@gmail.com)
 * Created:   2018-08-07
 * Copyright: Pavel Tyunin
 * License:
 **************************************************************/

#include "ExtM3UeditMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(ExtM3UeditFrame)
#include <wx/artprov.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(ExtM3UeditFrame)
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

    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ExtM3UeditFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ExtM3UeditFrame::OnAbout);
    //*)
    wxGCC_WARNING_RESTORE_CAST_FUNCTION_TYPE()
}

ExtM3UeditFrame::~ExtM3UeditFrame()
{
    //(*Destroy(ExtM3UeditFrame)
    //*)
}

void ExtM3UeditFrame::OnQuit(wxCommandEvent& /*event*/)
{
    Close();
}

void ExtM3UeditFrame::OnAbout(wxCommandEvent& /*event*/)
{
    wxMessageBox("ExtM3Uedit v0.1-dev", _("About"));
}
