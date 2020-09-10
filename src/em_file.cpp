/***************************************************************
 * Name:      em_file.cpp
 * Purpose:   Playlist files access
 * Author:    Pavel Tyunin (pavel51tunin@gmail.com)
 * Created:   2018-08-22
 * Copyright: Pavel Tyunin
 * License:   GNU General Public License v3.0
 **************************************************************/

#include "em_file.h"
#include <wx/textfile.h>
#include <utility>
#include <algorithm>
#include <stdexcept>

static Encoding DetectEncoding(const wxConvAuto& conv) noexcept
{
    switch(conv.GetBOM())
    {
    case wxBOM_UTF8:
        return Encoding::UTF8;
    case wxBOM_None:
        return (conv.IsFallbackUsed()? Encoding::ANSI : Encoding::UTF8);
    default:
        return Encoding::Unknown;
    }
}

static NewlineType DetectNewlineType(const wxTextFile& file) noexcept
{
    size_t n_le[3] {};
    const auto n = std::min(file.GetLineCount(), 100u);
    for(size_t i=0; i<n; ++i)
    {
        switch(file.GetLineType(i))
        {
        case wxTextFileType_Dos:
        case wxTextFileType_Os2:
            ++n_le[0];
            break;
        case wxTextFileType_Unix:
            ++n_le[1];
            break;
        case wxTextFileType_Mac:
            ++n_le[2];
            break;
        default:
            break;
        }
    }
    if(n_le[2] > n_le[1] && n_le[2] > n_le[0])
        return NewlineType::CR;
    else if(n_le[1] > n_le[0])
        return NewlineType::LF;
    else
        return NewlineType::CRLF;
}

wxArrayString EMFile::open(wxString name)
{
    wxTextFile file(name);
    auto conv = std::make_unique<wxConvAuto>(wxFONTENCODING_SYSTEM);
    if(!file.Open(*conv))
        throw std::runtime_error("Could not open file: " + name.ToStdString());
    m_name = std::move(name);
    m_encoding = DetectEncoding(*conv);
    m_bom = conv->GetBOM() != wxBOM_None;
    //Set encoding manually for empty file
    auto n = file.GetLineCount();
    if(conv->GetBOM() == wxBOM_Unknown && (!n || (n == 1 && file[0].IsEmpty())))
    {
        setEncoding(Encoding::UTF8);
        setBOM(false);
    }
    m_conv = std::move(conv);
    m_newline = DetectNewlineType(file);

    wxArrayString data;
    data.reserve(n);
    for(std::size_t i=0; i<n; ++i)
        data.push_back(std::move(file[i]));
    if(n && file.GetLineType(n-1) != wxTextFileType_None)
        data.emplace_back();    // The empty last line
    return data;
}

static void AssignToFile(wxTextFile& f, wxArrayString&& d, NewlineType newline)
{
    wxTextFileType ft = wxTextFileType_Dos;
    if(newline == NewlineType::LF)
        ft = wxTextFileType_Unix;
    else if(newline == NewlineType::CR)
        ft = wxTextFileType_Mac;

    size_t fl = 0;
    auto dl = d.size();
    if(dl)
        --dl;   // The last line needs special handling
    while(fl < dl)
        f.AddLine(std::move(d[fl++]), ft);
    if(!d.empty() && !d.back().empty())
        f.AddLine(d.back(), wxTextFileType_None);
}

void EMFile::save(wxArrayString data)
{
    if(m_bom)
    {
        wchar_t bom = L'\uFEFF';
        if(data.size())
            data[0].Prepend(bom);
        else
            data.Add(bom);
    }
    wxTextFile file(m_name);
    AssignToFile(file, std::move(data), m_newline);
    if(!m_conv)
        createConv();
    if(!file.Write(wxTextFileType_None, *m_conv))
        throw std::runtime_error("Could not save file: " + m_name.ToStdString());
}

void EMFile::saveAs(wxArrayString data, wxString name)
{
    m_name = std::move(name);
    save(std::move(data));
}

void EMFile::setEncoding(Encoding e) noexcept
{
    m_encoding = e;
    if(e == Encoding::ANSI)
        m_bom = false;
    m_conv.reset();
}

void EMFile::createConv()
{
    switch(m_encoding)
    {
    case Encoding::UTF8:
        m_conv = std::make_unique<wxMBConvUTF8>();
        break;
    case Encoding::ANSI:
        m_conv = std::make_unique<wxCSConv>(wxFONTENCODING_SYSTEM);
        break;
    default:
        throw std::logic_error("Unknown encoding");
    }
}
