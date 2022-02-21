#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "pf-class.h"
#include "pf-data.h"
#include <vector>

class ArchetypeTable : public wxListView {
  public:
    enum DisplayStatus { SHOW, HIDE, DISABLE };
  private:
    class TableElement {
      public:
        DisplayStatus m_Display;
        const Archetype* m_Archetype;
        TableElement(DisplayStatus status, const Archetype* archetype);
    };
  // variable declarations
  public:
  private:
    bool m_ShowHidden;
    std::vector<TableElement> m_Archetypes;
    std::vector<int> m_ValidIndices;
  // function declarations
  public:
    ArchetypeTable(wxWindow* parent, bool showHeader);
    int Size() const;
    bool GetShowHidden() const;
    const Archetype* GetArchetype(int position) const;
    DisplayStatus GetDisplayStatus(int position) const;
    void SetShowHidden(bool showHidden);
    void SetDisplayStatus(int position, DisplayStatus status);
    void SetSelectedClass(const PfClass* list, bool showAll);
    void RecalculateSize();
    void RecalculateStatuses();
    wxString OnGetItemText(long item, long column) const override;
    wxListItemAttr* OnGetItemAttr (long item) const override;
    int RowToIndex(int row);
  private:
};
