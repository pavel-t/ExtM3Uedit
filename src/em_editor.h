/***************************************************************
 * Name:      em_editor.h
 * Purpose:   ExtM3U editing interface
 * Author:    Pavel Tyunin (pavel51tunin@gmail.com)
 * Created:   2018-08-29
 * Copyright: Pavel Tyunin
 * License:   GNU General Public License v3.0
 **************************************************************/

#ifndef EM_EDITOR_H
#define EM_EDITOR_H

#include "em_data.h"
#include "em_file.h"

using Index = long;  // Type for entry indexes

enum class EMUpdateMode : unsigned char
{
    Reset,  // Full reload, e.g. opening file
    Info,   // File name, etc
    Insert,
    Update,
    Delete
};

class EMObserver
{
public:
    virtual void notify(EMUpdateMode t, Index begin, Index end) =0;
};

class EMObservable
{
public:
    void addObserver(EMObserver* o) { m_observers.push_back(o); }
    void removeObserver(EMObserver* o)
        { m_observers.erase(std::find(m_observers.begin(), m_observers.end(), o)); }
    void notify(EMUpdateMode t, Index begin = -1, Index end = -1) const;
private:
    std::vector<EMObserver*> m_observers;
};

class EMEditor : public EMObservable
{
public:
    EMEditor() { createFile(); }

    wxString getFileName() const;
    Index getEntryNumber() const;
    wxString getLength(Index i) const;
    wxString getName(Index i) const;
    wxString getURL(Index i) const;
    Index getAttrNumber(Index i) const;
    wxString getAttrKey(Index i, Index ia) const;
    wxString getAttrVal(Index i, Index ia) const;

    void createFile();
    void openFile(wxString name);
    void saveFile();
    void saveFileAs(wxString name);
    bool isSaveable() const { return m_file.isOpened(); }
    Encoding getFileEncoding() const noexcept { return m_file.getEncoding(); }
    void setFileEncoding(Encoding e);
    bool getFileBOM() const noexcept { return m_file.getBOM(); }
    void setFileBOM(bool bom);
    NewlineType getFileNewlineType() const noexcept { return m_file.getNewlineType(); }
    void setFileNewlineType(NewlineType newline);

    void addEntry(Index i, wxString url);
    void removeEntry(Index i);
    void moveEntryUp(Index i);
    void moveEntryDown(Index i);

    void addAttr(Index i, Index ia, wxString key);
    void removeAttr(Index i, Index ia);
    void moveAttrUp(Index i, Index ia);
    void moveAttrDown(Index i, Index ia);

    void setLength(Index i, int l);
    void setName(Index i, wxString n);
    void setURL(Index i, wxString u);
    void setAttrVal(Index i, Index ia, wxString v);

    void normalize();

private:
    auto entryIter(Index i) { return m_data->getEntries().begin() + i; }
    auto attrIter(Index i, Index ia)
        { return m_data->getEntries()[i].getExtinf().getAttributes().begin() + ia; }

    EMFile m_file;
    std::unique_ptr<EMFileData> m_data;
};

#endif // EM_EDITOR_H
