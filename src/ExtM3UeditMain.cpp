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
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(ExtM3UeditFrame)
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
    wxMenuItem* QuitMenuItem;

    Create(parent, id, _("ExtM3Uedit"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    SetClientSize(wxSize(800,600));
    MenuBar = new wxMenuBar();
    FileMenu = new wxMenu();
    QuitMenuItem = new wxMenuItem(FileMenu, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    FileMenu->Append(QuitMenuItem);
    MenuBar->Append(FileMenu, _("&File"));
    HelpMenu = new wxMenu();
    AboutMenuItem = new wxMenuItem(HelpMenu, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
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
