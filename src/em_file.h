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
#include <memory>

enum class Encoding : unsigned char { Unknown, UTF8, ANSI };

class EMFile
{
public:
    wxArrayString open(wxString name);
    void save(wxArrayString data);
    void saveAs(wxArrayString data, wxString name);
    void close() { m_file.Close(); }
    bool isOpened() const { return m_file.IsOpened(); }
    wxString getName() const { return m_file.GetName(); }
    Encoding getEncoding() const noexcept { return m_encoding; }
    void setEncoding(Encoding e) noexcept;
    bool getBOM() const noexcept { return m_bom; }
    void setBOM(bool bom) noexcept { m_bom = bom; }

private:
    wxTextFile m_file;
    Encoding m_encoding = Encoding::UTF8;
    bool m_bom = false;
    std::unique_ptr<wxMBConv> m_conv;
    void createConv();
};

#endif // EM_FILE_H
