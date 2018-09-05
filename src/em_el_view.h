/***************************************************************
 * Name:      em_el_view.h
 * Purpose:   ExtM3U entry list view control
 * Author:    Pavel Tyunin (pavel51tunin@gmail.com)
 * Created:   2018-09-04
 * Copyright: Pavel Tyunin
 * License:
 **************************************************************/

#ifndef EM_EL_VIEW_H
#define EM_EL_VIEW_H

#include <wx/listctrl.h>
#include "em_editor.h"

class EMEntryListView : public wxListView, public EMObserver
{
public:
    using wxListView::wxListView;
    void initEditor(EMEditor* e);

    void notify(EMUpdateMode t, Index begin, Index end) override;

    void ColumnsAutoResize();

protected:
    wxString OnGetItemText(long item, long column) const override;

private:
    void doReset();
    void doInsert(Index begin, Index end);
    void doUpdate(Index begin, Index end);
    void doDelete(Index begin, Index end);

    EMEditor* m_editor;
};

#endif // EM_EL_VIEW_H
