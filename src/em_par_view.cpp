/***************************************************************
 * Name:      em_par_view.cpp
 * Purpose:   ExtM3U parameters view control implementation
 * Author:    Pavel Tyunin (pavel51tunin@gmail.com)
 * Created:   2018-09-05
 * Copyright: Pavel Tyunin
 * License:
 **************************************************************/

#include "em_par_view.h"

void EMParametersView::initEditor(EMEditor* e)
{
    m_editor = e;
    doDeselectEntry();
    m_editor->addObserver(this);
    Bind(wxEVT_PG_CHANGED, onChanged, this);
}

void EMParametersView::selectEntry(Index ei)
{
    if(ei == -1)
        doDeselectEntry();
    else if(m_entry != ei)
        doSelectEntry(ei);
}

void EMParametersView::notify(EMUpdateMode t, Index begin, Index end)
{
    switch(t)
    {
    case EMUpdateMode::Reset:
        doDeselectEntry();
        break;
    case EMUpdateMode::Info:
        break;
    case EMUpdateMode::Insert:
        if(m_entry >= begin)
            m_entry += (end - begin);
        break;
    case EMUpdateMode::Update:
        if(m_entry >= begin && m_entry < end)
            doUpdate();
        break;
    case EMUpdateMode::Delete:
        if(m_entry >= end)
            m_entry -= (end - begin);
        else if(m_entry >= begin)
            doDeselectEntry();
    }
}

void EMParametersView::onChanged(wxPropertyGridEvent& event)
{
    auto p = event.GetProperty();
    if(p == m_len)
        m_editor->setLength(m_entry, event.GetPropertyValue().GetInteger());
    else if(p == m_name)
        m_editor->setName(m_entry, event.GetPropertyValue().GetString());
    else if(p == m_url)
        m_editor->setURL(m_entry, event.GetPropertyValue().GetString());
    else
        m_editor->setAttrVal(m_entry, m_attr_cat->Index(p), event.GetPropertyValue().GetString());
}

void EMParametersView::doDeselectEntry()
{
    m_entry = -1;
    Clear();
    m_len = nullptr;
    m_attr_cat = nullptr;
    m_name = nullptr;
    m_url = nullptr;
}

void EMParametersView::doSelectEntry(Index ei)
{
    if(m_entry == -1)
    {
        Append(m_len = new wxIntProperty("Length"));
        Append(m_name = new wxStringProperty("Name"));
        Append(m_url = new wxLongStringProperty("URL"));
    }
    m_entry = ei;
    doUpdate();
}

void EMParametersView::doUpdate()
{
    m_len->SetValueFromString(m_editor->getLength(m_entry));
    m_name->SetValueFromString(m_editor->getName(m_entry));
    m_url->SetValueFromString(m_editor->getURL(m_entry));

    auto an = m_editor->getAttrNumber(m_entry);
    if(!an && m_attr_cat)
    {
        DeleteProperty(m_attr_cat);
        m_attr_cat = nullptr;
    }
    else if(an && !m_attr_cat)
        Insert(m_name, m_attr_cat = new wxPropertyCategory("Custom attributes"));

    if(an)
    {
        // Check if attribute sets differ
        bool attrs_match = (m_attr_cat->GetChildCount() == (unsigned)an);
        if(attrs_match)
            for(Index i=0; i<an; ++i)
                if(m_editor->getAttrKey(m_entry, i) != m_attr_cat->Item(i)->GetName())
                {
                    attrs_match = false;
                    break;
                }

        if(!attrs_match)
        {
            m_attr_cat->DeleteChildren();
            for(Index i=0; i<an; ++i)
            {
                auto attr_key = m_editor->getAttrKey(m_entry, i);
                AppendIn(m_attr_cat, new wxStringProperty("  "+attr_key, attr_key));
            }
        }

        for(Index i=0; i<an; ++i)
            m_attr_cat->Item(i)->SetValueFromString(m_editor->getAttrVal(m_entry, i));
    }
}