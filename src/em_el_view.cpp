/***************************************************************
 * Name:      em_el_view.cpp
 * Purpose:   ExtM3U entry list view control implementation
 * Author:    Pavel Tyunin (pavel51tunin@gmail.com)
 * Created:   2018-09-04
 * Copyright: Pavel Tyunin
 * License:
 **************************************************************/

#include "em_el_view.h"

void EMEntryListView::initEditor(EMEditor* e)
{
    m_editor = e;
    AppendColumn("Name", wxLIST_FORMAT_LEFT, 128);
    AppendColumn("URL");
    ColumnsAutoResize();
    doReset();
    m_editor->addObserver(this);
}

void EMEntryListView::notify(EMUpdateMode t, Index begin, Index end)
{
    switch(t)
    {
    case EMUpdateMode::Reset:
        doReset();
        break;
    case EMUpdateMode::Info:
        break;
    case EMUpdateMode::Insert:
        doInsert(begin, end);
        break;
    case EMUpdateMode::Update:
        doUpdate(begin, end);
        break;
    case EMUpdateMode::Delete:
        doDelete(begin, end);
    }
}

void EMEntryListView::ColumnsAutoResize()
{
    auto s = GetClientSize().x - GetColumnWidth(0);
    SetColumnWidth(1, std::max(s, 256));
}

wxString EMEntryListView::OnGetItemText(long item, long column) const
{
    if(column == 0)
        return m_editor->getName(item);
    return m_editor->getURL(item);
}

void EMEntryListView::doReset()
{
    DeleteAllItems();
    SetItemCount(m_editor->getEntryNumber());
    Refresh();
}

void EMEntryListView::doInsert(Index begin, Index end)
{
    auto en = m_editor->getEntryNumber();
    SetItemCount(en);
    RefreshItems(begin, en-1);
    auto fi = GetFirstSelected();
    if(fi != -1 && Index(fi) >= begin)
        Select(fi + end - begin);
}

void EMEntryListView::doUpdate(Index begin, Index end)
{
    RefreshItems(begin, end);
}

void EMEntryListView::doDelete(Index begin, Index end)
{
    while(end > begin)
        DeleteItem(begin++);
}

