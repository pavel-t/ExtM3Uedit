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
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
//*)

class ExtM3UeditFrame: public wxFrame
{
public:

    ExtM3UeditFrame(wxWindow* parent,wxWindowID id = -1);
    virtual ~ExtM3UeditFrame();

private:

    //(*Handlers(ExtM3UeditFrame)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    //*)

    //(*Identifiers(ExtM3UeditFrame)
    static const long idMenuQuit;
    static const long idMenuAbout;
    static const long ID_STATUSBAR;
    //*)

    //(*Declarations(ExtM3UeditFrame)
    wxStatusBar* StatusBar;
    //*)

    DECLARE_EVENT_TABLE()
};

#endif // EXTM3UEDITMAIN_H
