/***************************************************************
 * Name:      em_data.cpp
 * Purpose:   Parsing and generating of Extended M3U files
 * Author:    Pavel Tyunin (pavel51tunin@gmail.com)
 * Created:   2018-08-10
 * Copyright: Pavel Tyunin
 * License:   GNU General Public License v3.0
 **************************************************************/

#include "em_data.h"
#include <algorithm>
#include <iterator>
#include <typeinfo>
#include <wx/log.h>
#include <wx/debug.h>

EMAttr::EMAttr(wxString s)
{
    auto de = std::find_if_not(s.begin(), s.end(), isDelimChar);
    m_delim.assign(s.begin(), de);

    auto ke = std::find(de, s.end(), '=');
    if(ke == de)
        wxLogError("Parsing error: key not found in attribute:\n%s", s);
    m_key.assign(de, ke);

    // Checking for value
    if(ke != s.end()) // True if there is '=' in the string
    {
        ++ke;
        auto ve = s.end();
        // Checking for quotes
        if(ke != s.end() && *ke == '"')
        {
            ++ke;
            --ve;
            if(ke == s.end() || *ve != '"')
            {
                wxLogError("Parsing error: closing quotes not found in attribute:\n%s", s);
                ++ve;
            }
        }
        else
            m_quoted = false;

        m_val.emplace(ke, ve);
    }
}

wxString EMAttr::toString() const
{
    wxString s(m_delim + m_key);
    if(hasValue())
    {
        s.append('=');
        if(m_quoted) s.append('"');
        s.append(m_val.value());
        if(m_quoted) s.append('"');
    }
    return s;
}

void EMAttr::setValue(wxString value)
{
    m_val = std::move(value);
    if(!m_quoted && hasDelimiterInValue())
    {
        wxLogWarning("New value contains delimiter, adding quotes:\n%s", toString());
        m_quoted = true;
    }
}

void EMAttr::setQuoted(bool quoted)
{
    wxASSERT_LEVEL_2(quoted || !hasDelimiterInValue());
    m_quoted = quoted;
}

bool EMAttr::hasDelimiterInValue() const
{
    return hasValue() &&
        std::any_of(m_val.value().begin(), m_val.value().end(), isDelimChar);
}

wxString EMLineDirectiveBase::toString() const
{
    return wxString('#') << getDirective() << attrString();
}

void EMLineDirectiveBase::normalize()
{
    for(auto& a : m_attr)
        a.normalize();
    m_after_attr.clear();
}

wxString EMLineDirectiveBase::attrString() const
{
    wxString s;
    for(auto& a : m_attr)
        s.append(a.toString());
    s.append(m_after_attr);
    return s;
}

void EMLineDirectiveBase::parseAttr(const wxString& attr)
{
    if(attr.IsEmpty())
        return;
    bool nd = false;
    bool quote = false;
    auto ab = attr.begin();
    auto ae = ab;
    while(ae != attr.end())
    {
        if(EMAttr::isDelimChar(*ae))
        {
            if(nd && !quote)
            {
                m_attr.emplace_back(wxString(ab, ae));
                ab = ae;
                nd = false;
            }
        }
        else
        {
            nd = true;
            if(*ae == '"')
                quote = !quote;
        }
        ++ae;
    }
    if(nd)
        m_attr.emplace_back(wxString(ab, ae));
    else
        m_after_attr.assign(ab, ae);

    if(quote)
        wxLogError("Parsing error: closing quotes expected:\n%s", attr);
}

EMLineDirective::EMLineDirective(const wxString& l)
{
    wxASSERT_LEVEL_2(l.StartsWith('#'));
    auto db = ++l.begin();
    auto de = std::find_if(db, l.end(), [](auto c){ return c == ':' || EMAttr::isDelimChar(c); });
    wxASSERT_LEVEL_2(de != db);
    if(de != l.end() && *de == ':')
        ++de;
    m_dir.assign(db, de);
    parseAttr(wxString(de, l.end()));
}

void EMLineDirective::setDirective(wxString dir)
{
    wxASSERT(!dir.IsEmpty());
    wxASSERT_LEVEL_2(checkDirective(dir));
    m_dir = std::move(dir);
}

bool EMLineDirective::checkDirective(const wxString& dir)
{
    if(dir.IsEmpty())
        return false;
    for(std::size_t i=0; i<dir.size(); ++i)
        if((dir[i] < 'A' || dir[i] > 'Z') &&
           (dir[i] < '0' || dir[i] > '9') &&
           !(i && i == dir.size()-1 && dir[i] == ':'))
            return false;
    return true;
}

EMLineExtinf::EMLineExtinf(const wxString& l)
{
    wxASSERT_LEVEL_2(l.StartsWith("#EXTINF:"));
    auto ab = l.begin();
    std::advance(ab, 8);
    auto se = std::find_if_not(ab, l.end(), wxIsspace);
    auto ae = std::find_if(se, l.end(), EMAttr::isDelimChar);
    // Checking for length
    if(std::all_of(se, ae, [](auto c){ return c == '-' || wxIsdigit(c); }))
    {
        long len;
        if(wxString(se, ae).ToLong(&len))
        {
            // Valid length found
            m_pre_len.assign(ab, se);
            m_len = len;
            ab = ae;
        }
    }

    ae = std::find(l.rbegin(), l.rend(), ',').base();
    if(ae == l.begin())
        ae = l.end();
    // Name can not begin inside the quotes
    if(std::count(ab, ae, '"') % 2)
        ae = l.end();
    ae = std::find_if_not(ae, l.end(), EMAttr::isDelimChar);
    m_name.assign(ae, l.end());
    parseAttr(wxString(ab, ae));
}

void EMLineExtinf::normalize()
{
    m_pre_len.clear();
    EMLineDirectiveBase::normalize();
    m_after_attr = ',';
    m_name.Trim();
}

wxString EMLineExtinf::attrString() const
{
    wxString s;
    if(hasLen())
        s << m_pre_len << m_len.value();
    return s << EMLineDirectiveBase::attrString() << m_name;
}

void EMLineExtinf::setLen(int len)
{
    // Prevent merging length with first attribute by adding space.
    // This is not needed if there are no attributes, because the name
    // is always separated with a comma.
    if(!hasLen() && hasAttributes() && getAttributes()[0].getDelimiter().IsEmpty())
        getAttributes()[0].setDelimiter(' ');
    m_len = len;
}

void EMLineExtinf::setName(wxString name)
{
    // Name should be separated with a comma
    if(!hasName() && !name.IsEmpty() && !m_after_attr.Contains(','))
        m_after_attr.append(',');
    wxASSERT(name.IsEmpty() || !EMAttr::isDelimChar(*name.begin()));
    wxASSERT(!name.Contains(','));
    wxASSERT(!name.Contains('"'));
    m_name = std::move(name);
}


EMEntry::EMEntry(std::vector<std::unique_ptr<EMLine>>lines)
    : m_lines(std::move(lines))
{
    for(auto& l:m_lines)
    {
        if(typeid(*l) == typeid(EMLineExtinf))
        {
            wxASSERT(!m_extinf);
            wxASSERT(!m_url);
            m_extinf = static_cast<EMLineExtinf*>(l.get());
        }
        else if(typeid(*l) == typeid(EMLineURL))
        {
            wxASSERT(!m_url);
            m_url = static_cast<EMLineURL*>(l.get());
        }
    }
}

wxArrayString EMEntry::toText() const
{
    wxArrayString t;
    t.reserve(m_lines.size());
    for(auto& l : m_lines)
        t.push_back(l->toString());
    return t;
}

std::unique_ptr<EMLine> EMLine::parse(const wxString& line)
{
    // Empty line
    if(line.IsEmpty() || std::all_of(line.begin(), line.end(), wxIsspace))
        return std::make_unique<EMLineText>(line);

    if(*line.begin() == '#')
    {
        if(line.StartsWith("#EXTINF:"))
            return std::make_unique<EMLineExtinf>(line);

        // Directive
        auto db = ++line.begin();
        auto de = std::find_if_not(db, line.end(), [](auto& c)
        {
            return (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
        });
        if(de != db && (de == line.end() || *de == ':' || wxIsspace(*de)))
            return std::make_unique<EMLineDirective>(line);

        // Comment
        return std::make_unique<EMLineText>(line);
    }

    return std::make_unique<EMLineURL>(line);
}

// Find bounds of the meaningful parts of entries
template<class Vline>
static auto FindEntryDelimiters(Vline& lines)
{
    auto li = lines.begin();
    // Skip #EXTM3U header
    if((*li)->isHeader())
        ++li;
    // Pairs of iterators represent areas without meaningful data
    std::vector<std::pair<decltype(li),decltype(li)>> delims;
    bool entry, extinf;

    auto unexpected_entry_end = [&]
    {
        // No URL found, ending entry at last non-empty line
        auto ls = std::find_if(std::make_reverse_iterator(li),
                               std::make_reverse_iterator(delims.back().second),
                               [](auto& l){ return !l->isEmpty(); });
        delims.emplace_back(ls.base(), li);
    };
    auto begin_entry = [&]
    {
        if(entry)
            unexpected_entry_end();
        entry = true;
        extinf = false;
    };
    auto begin_delim = [&]
    {
        extinf = entry = false;
        delims.emplace_back(li, li);
    };

    begin_delim();
    while(li != lines.end())
    {
        if(typeid(**li) == typeid(EMLineExtinf))
        {
            // Entry can contain only one #EXTINF
            if(!entry || extinf)
                begin_entry();
            extinf = true;
            ++li;
        }
        else if(entry)
        {
            // URL is the last meaningful line of the entry
            if(typeid(**(li++)) == typeid(EMLineURL))
                begin_delim();
        }
        else
        {
            if(typeid(**li) != typeid(EMLineText))
                begin_entry();
            else
                delims.back().second = ++li;
        }
    }
    if(entry)
        unexpected_entry_end();
    return delims;
}

// Find the longest sequence of empty lines in each inter-entry area
template<class Vdelim>
static auto FindDelimEmptyLineNums(const Vdelim& delims)
{
    std::vector<unsigned char> dl(delims.size());
    std::transform(delims.begin(), delims.end(), dl.begin(), [](auto& d)
    {
        if(d.first == d.second)
            return (unsigned char)0;
        std::vector<unsigned char> ne;
        ne.push_back(0);
        for(auto li = d.first; li != d.second; ++li)
        {
            if((*li)->isEmpty())
                ++ne.back();
            else if(ne.back())
                ne.push_back(0);
        }
        return *std::max_element(ne.begin(), ne.end());
    });
    return dl;
}

// Extend entry bounds, taking into account the empty lines and the common delimiter
template<class Vdelim, class Vsize>
static void ExtendEntries(Vdelim& delims, const Vsize& dl, unsigned char cl)
{
    for(std::size_t i=0; i<delims.size(); ++i)
    {
        // End the entry at the beginning of the longest empty area
        auto pos = std::search_n(delims[i].first, delims[i].second, dl[i],
            0, [](auto& l, int){ return l->isEmpty(); });
        delims[i].first = pos;
        // Begin the next entry after the common delimiter
        if(dl[i] >= cl) // This can be false for the last delimiter
            std::advance(pos, cl);
        delims[i].second = pos;
    }
}

EMFileData::EMFileData(const wxArrayString& text)
{
    if(text.empty())
        return;
    std::vector<std::unique_ptr<EMLine>> lines(text.size());
    std::transform(text.begin(), text.end(), lines.begin(), EMLine::parse);

    auto delims = FindEntryDelimiters(lines);
    auto dl = FindDelimEmptyLineNums(delims);
    if(delims.size() > 1)   // If there are no entries, use the default values
    {
        // Determining the common entry delimiter
        auto dle = dl.end();
        if(delims.size() >= 3)
            --dle;  // Ignore lines after the last entry if there are at least 2 entries
        m_delim_n = *std::min_element(dl.begin(), dle);
        // Check if common delimiter is present after the last entry
        m_delim_end = dl.back() >= m_delim_n;
    }
    // Extend entries to include all lines except the common delimiter
    ExtendEntries(delims, dl, m_delim_n);

    m_head.resize(std::distance(lines.begin(), delims.front().first));
    std::move(lines.begin(), delims.front().first, m_head.begin());

    for(std::size_t i=0; i<delims.size()-1; ++i)
    {
        decltype(lines) e(std::make_move_iterator(delims[i].second),
                          std::make_move_iterator(delims[i+1].first));
        m_entries.emplace_back(std::move(e));
    }

    m_end.resize(std::distance(delims.back().second, lines.end()));
    std::move(delims.back().second, lines.end(), m_end.begin());
}

wxArrayString EMFileData::toText() const
{
    wxArrayString t(headText());
    for(auto& e : m_entries)
    {
        t.resize(t.size() + m_delim_n);
        wxArrayString et(e.toText());
        t.insert(t.end(),
                 std::make_move_iterator(et.begin()),
                 std::make_move_iterator(et.end()));
    }
    if(m_delim_end)
        t.resize(t.size() + m_delim_n);
    wxArrayString e(endText());
    t.insert(t.end(),
             std::make_move_iterator(e.begin()),
             std::make_move_iterator(e.end()));
    return t;
}

wxArrayString EMFileData::headText() const
{
    wxArrayString t;
    t.reserve(m_head.size());
    for(auto& l : m_head)
        t.push_back(l->toString());
    return t;
}

wxArrayString EMFileData::endText() const
{
    wxArrayString t;
    t.reserve(m_end.size());
    for(auto& l : m_end)
        t.push_back(l->toString());
    return t;
}

void EMFileData::normalize()
{
    if(m_head.empty() || !m_head.front()->isHeader())
        m_head.insert(m_head.begin(), std::make_unique<EMLineDirective>("#EXTM3U"));
    for(auto& l : m_head)
        l->normalize();
    for(auto& e : m_entries)
        e.normalize();
    for(auto& l : m_end)
        l->normalize();
    // Last line should be empty
    if((!m_end.empty() && !m_end.back()->isEmpty()) ||
       (m_end.empty() && (!m_delim_end || !m_delim_n)))
        m_end.push_back(std::make_unique<EMLineText>(""));
}

