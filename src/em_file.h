/***************************************************************
 * Name:      em_file.h
 * Purpose:   Playlist files access interface
 * Author:    Pavel Tyunin (pavel51tunin@gmail.com)
 * Created:   2018-08-22
 * Copyright: Pavel Tyunin
 * License:   GNU General Public License v3.0
 **************************************************************/

#ifndef EM_FILE_H
#define EM_FILE_H

#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/textfile.h>

class EMFile
{
public:
    wxArrayString open(wxString name);
    void save(wxArrayString data);
    void saveAs(wxArrayString data, wxString name);
    void close() { m_file.Close(); }
    bool isOpened() const { return m_file.IsOpened(); }
    wxString getName() const { return m_file.GetName(); }

private:
    wxTextFile m_file;
};

#endif // EM_FILE_H
