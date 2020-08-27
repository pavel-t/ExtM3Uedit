/***************************************************************
 * Name:      em_file.cpp
 * Purpose:   Playlist files access
 * Author:    Pavel Tyunin (pavel51tunin@gmail.com)
 * Created:   2018-08-22
 * Copyright: Pavel Tyunin
 * License:   GNU General Public License v3.0
 **************************************************************/

#include "em_file.h"
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

wxArrayString EMFile::open(wxString name)
{
    if(m_file.IsOpened())
        m_file.Close(); // workaround

    auto conv = std::make_unique<wxConvAuto>(wxFONTENCODING_SYSTEM);
    if(!m_file.Open(name, *conv))
        throw std::runtime_error("Could not open file: " + name.ToStdString());
    m_encoding = DetectEncoding(*conv);
    m_bom = conv->GetBOM() != wxBOM_None;
    //Set encoding manually for empty file
    auto n = m_file.GetLineCount();
    if(conv->GetBOM() == wxBOM_Unknown && (!n || (n == 1 && m_file[0].IsEmpty())))
    {
        setEncoding(Encoding::UTF8);
        setBOM(false);
    }
    m_conv = std::move(conv);

    wxArrayString data;
    data.reserve(n);
    for(std::size_t i=0; i<n; ++i)
        data.push_back(std::move(m_file[i]));
    if(n && m_file.GetLineType(n-1) != wxTextFileType_None)
        data.emplace_back();    // The empty last line
    return data;
}

static void AssignToFile(wxTextFile& f, wxArrayString&& d)
{
    auto fl = f.GetLineCount();
    auto dl = d.size();
    if(dl)
        --dl;   // The last line needs special handling
    while(fl > dl)
        f.RemoveLine(--fl);
    for(std::size_t i=0; i<fl; ++i)
        f[i] = std::move(d[i]);
    while(fl < dl)
        f.AddLine(std::move(d[fl++]));
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
    AssignToFile(m_file, std::move(data));
    if(!m_conv)
        createConv();
    if(!m_file.Write(wxTextFileType_None, *m_conv))
        throw std::runtime_error("Could not save file: " + m_file.GetName().ToStdString());
}

void EMFile::saveAs(wxArrayString data, wxString name)
{
    if(!m_file.Create(name) && !m_file.Open(std::move(name)))
        throw std::runtime_error("Could not save file as: " + m_file.GetName().ToStdString());
    save(std::move(data));
    open(name);
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
