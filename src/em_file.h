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
#include <memory>

enum class Encoding : unsigned char { Unknown, UTF8, ANSI };
enum class NewlineType : unsigned char { CRLF, LF, CR };

class EMFile
{
public:
    wxArrayString open(wxString name);
    void save(wxArrayString data);
    void saveAs(wxArrayString data, wxString name);
    void close() noexcept { m_name.clear(); }
    bool isOpened() const noexcept { return !m_name.IsEmpty(); }
    wxString getName() const noexcept { return m_name; }
    Encoding getEncoding() const noexcept { return m_encoding; }
    void setEncoding(Encoding e) noexcept;
    bool getBOM() const noexcept { return m_bom; }
    void setBOM(bool bom) noexcept { m_bom = bom; }
    NewlineType getNewlineType() const noexcept { return m_newline; }
    void setNewlineType(NewlineType newline) noexcept { m_newline = newline; }

private:
    wxString m_name;
    std::unique_ptr<wxMBConv> m_conv;
    Encoding m_encoding = Encoding::UTF8;
    NewlineType m_newline = NewlineType::CRLF;
    bool m_bom = false;
    void createConv();
};

#endif // EM_FILE_H
