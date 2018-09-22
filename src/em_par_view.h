/***************************************************************
 * Name:      em_par_view.h
 * Purpose:   ExtM3U parameters view control
 * Author:    Pavel Tyunin (pavel51tunin@gmail.com)
 * Created:   2018-09-05
 * Copyright: Pavel Tyunin
 * License:
 **************************************************************/

#ifndef EM_PAR_VIEW_H
#define EM_PAR_VIEW_H

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/props.h>
#include "em_editor.h"

class EMParametersView : public wxPropertyGrid, public EMObserver
{
public:
    using wxPropertyGrid::wxPropertyGrid;
    void initEditor(EMEditor* e);
    void selectEntry(Index ei);

    void notify(EMUpdateMode t, Index begin, Index end) override;

    void onChanged(wxPropertyGridEvent& event);

    void addAttr();
    void removeAttr();

private:
    void doDeselectEntry();
    void doSelectEntry(Index ei);
    void doUpdate();
    Index getSelectedAttr() const;

    EMEditor* m_editor;
    Index m_entry;
    wxIntProperty* m_len;
    wxPropertyCategory* m_attr_cat;
    wxStringProperty* m_name;
    wxLongStringProperty* m_url;
};

#endif // EM_PAR_VIEW_H
