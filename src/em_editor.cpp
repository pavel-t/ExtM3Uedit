/***************************************************************
 * Name:      em_editor.cpp
 * Purpose:   ExtM3U editing interface implementation
 * Author:    Pavel Tyunin (pavel51tunin@gmail.com)
 * Created:   2018-08-29
 * Copyright: Pavel Tyunin
 * License:   GNU General Public License v3.0
 **************************************************************/

#include "em_editor.h"

void EMObservable::notify(EMUpdateMode t, Index begin, Index end) const
{
    if(end == -1 && begin != -1)
        end = begin + 1;
    for(auto& o : m_observers)
        o->notify(t, begin, end);
}

wxString EMEditor::getFileName() const
{
    if(m_file.isOpened())
        return m_file.getName();
    return "New playlist";
}

Index EMEditor::getEntryNumber() const
{
    return m_data->getEntries().size();
}

wxString EMEditor::getLength(Index i) const
{
    if(m_data->getEntries()[i].hasExtinf())
        return m_data->getEntries()[i].getExtinf().getLenStr();
    return wxString();
}

wxString EMEditor::getName(Index i) const
{
    if(m_data->getEntries()[i].hasExtinf())
        return m_data->getEntries()[i].getExtinf().getName();
    return wxString();
}

wxString EMEditor::getURL(Index i) const
{
    if(m_data->getEntries()[i].hasURL())
        return m_data->getEntries()[i].getURL().getText();
    return wxString();
}

Index EMEditor::getAttrNumber(Index i) const
{
    if(m_data->getEntries()[i].hasExtinf())
        return m_data->getEntries()[i].getExtinf().getAttributes().size();
    return 0;
}

wxString EMEditor::getAttrKey(Index i, Index ia) const
{
    return m_data->getEntries()[i].getExtinf().getAttributes()[ia].getKey();
}

wxString EMEditor::getAttrVal(Index i, Index ia) const
{
    return m_data->getEntries()[i].getExtinf().getAttributes()[ia].getValue();
}

void EMEditor::createFile()
{
    m_file.close();
    wxArrayString t;
    t.emplace_back("#EXTM3U");
    t.emplace_back();
    m_data = std::make_unique<EMFileData>(t);
    notify(EMUpdateMode::Reset);
}

void EMEditor::openFile(wxString name)
{
    m_data = std::make_unique<EMFileData>(m_file.open(std::move(name)));
    notify(EMUpdateMode::Reset);
}

void EMEditor::saveFile()
{
    m_file.save(m_data->toText());
}

void EMEditor::saveFileAs(wxString name)
{
    m_file.saveAs(m_data->toText(), std::move(name));
    notify(EMUpdateMode::Info);
}

void EMEditor::setFileEncoding(Encoding e)
{
    m_file.setEncoding(e);
    notify(EMUpdateMode::Info);
}

void EMEditor::setFileBOM(bool bom)
{
    m_file.setBOM(bom);
    notify(EMUpdateMode::Info);
}

void EMEditor::addEntry(Index i, wxString url)
{
    std::vector<std::unique_ptr<EMLine>> l;
    l.push_back(EMLine::parse(std::move(url)));

    m_data->getEntries().emplace(entryIter(i), std::move(l));
    notify(EMUpdateMode::Insert, i);
}

void EMEditor::removeEntry(Index i)
{
    m_data->getEntries().erase(entryIter(i));
    notify(EMUpdateMode::Delete, i);
}

void EMEditor::moveEntryUp(Index i)
{
    if(i == 0)
        return;

    std::iter_swap(entryIter(i), entryIter(i-1));
    notify(EMUpdateMode::Delete, i-1);
    notify(EMUpdateMode::Insert, i);
}

void EMEditor::moveEntryDown(Index i)
{
    if(i+1 >= getEntryNumber())
        return;

    std::iter_swap(entryIter(i), entryIter(i+1));
    notify(EMUpdateMode::Delete, i+1);
    notify(EMUpdateMode::Insert, i);
}

void EMEditor::addAttr(Index i, Index ia, wxString key)
{
    auto it = attrIter(i, ia);
    m_data->getEntries()[i].getExtinf().getAttributes().emplace(it, ' '+std::move(key));
    notify(EMUpdateMode::Update, i);
}

void EMEditor::removeAttr(Index i, Index ia)
{
    auto it = attrIter(i, ia);
    m_data->getEntries()[i].getExtinf().getAttributes().erase(it);
    notify(EMUpdateMode::Update, i);
}

void EMEditor::moveAttrUp(Index i, Index ia)
{
    if(ia == 0)
        return;

    std::iter_swap(attrIter(i, ia), attrIter(i, ia-1));
    notify(EMUpdateMode::Update, i);
}

void EMEditor::moveAttrDown(Index i, Index ia)
{
    if(ia+1 >= getAttrNumber(i))
        return;

    std::iter_swap(attrIter(i, ia), attrIter(i, ia+1));
    notify(EMUpdateMode::Update, i);
}

void EMEditor::setLength(Index i, int l)
{
    m_data->getEntries()[i].getExtinf().setLen(l);
    notify(EMUpdateMode::Update, i);
}

void EMEditor::setName(Index i, wxString n)
{
    m_data->getEntries()[i].getExtinf().setName(std::move(n));
    notify(EMUpdateMode::Update, i);
}

void EMEditor::setURL(Index i, wxString u)
{
    m_data->getEntries()[i].getURL().setText(std::move(u));
    notify(EMUpdateMode::Update, i);
}

void EMEditor::setAttrVal(Index i, Index ia, wxString v)
{
    m_data->getEntries()[i].getExtinf().getAttributes()[ia].setValue(std::move(v));
    notify(EMUpdateMode::Update, i);
}

void EMEditor::normalize()
{
    m_data->normalize();
    notify(EMUpdateMode::Update, 0, getEntryNumber());
}
