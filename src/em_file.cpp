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

wxArrayString EMFile::open(wxString name)
{
    if(m_file.IsOpened())
        m_file.Close(); // workaround

    if(!m_file.Open(name))
        throw std::runtime_error("Could not open file: " + name.ToStdString());

    wxArrayString data;
    auto n = m_file.GetLineCount();
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
    AssignToFile(m_file, std::move(data));
    if(!m_file.Write())
        throw std::runtime_error("Could not save file: " + m_file.GetName().ToStdString());
}

void EMFile::saveAs(wxArrayString data, wxString name)
{
    if(!m_file.Create(name) && !m_file.Open(std::move(name)))
        throw std::runtime_error("Could not save file as: " + m_file.GetName().ToStdString());
    save(std::move(data));
    open(name);
}

