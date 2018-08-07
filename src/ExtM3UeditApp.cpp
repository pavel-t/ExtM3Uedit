/***************************************************************
 * Name:      ExtM3UeditApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Pavel Tyunin (pavel51tunin@gmail.com)
 * Created:   2018-08-07
 * Copyright: Pavel Tyunin
 * License:
 **************************************************************/

#include "ExtM3UeditApp.h"

//(*AppHeaders
#include "ExtM3UeditMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(ExtM3UeditApp)

bool ExtM3UeditApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
        ExtM3UeditFrame* Frame = new ExtM3UeditFrame(0);
        Frame->Show();
        SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
