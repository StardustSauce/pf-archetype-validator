#include "archetype-table.h"

ArchetypeTable::TableElement::TableElement(DisplayStatus status, const Archetype* archetype) 
  : m_Display(status), m_Archetype(archetype) { }

ArchetypeTable::ArchetypeTable(wxWindow* parent, bool showHeader) :
  wxListView(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_VIRTUAL | wxLC_VRULES | wxLC_SINGLE_SEL | (showHeader ? 0 : wxLC_NO_HEADER)),
  m_ShowHidden(false) {
  SetItemCount(0);
  EnableAlternateRowColours();
  SetAlternateRowColour(GetBackgroundColour());
  SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_INACTIVEBORDER));
  // Only available in wxWidgets 3.1.5
  ExtendRulesAndAlternateColour();
}

int ArchetypeTable::Size() const { return m_Archetypes.size(); }

bool ArchetypeTable::GetShowHidden() const { return m_ShowHidden; }

const Archetype* ArchetypeTable::GetArchetype(int position) const {
  return m_Archetypes.at(position).m_Archetype;
}

ArchetypeTable::DisplayStatus ArchetypeTable::GetDisplayStatus(int position) const {
  return m_Archetypes.at(position).m_Display;
}

void ArchetypeTable::SetShowHidden(bool showHidden) {
  m_ShowHidden = showHidden;
  RecalculateSize();
}

void ArchetypeTable::SetDisplayStatus(int position, DisplayStatus status) {
  m_Archetypes.at(position).m_Display = status;
}

void ArchetypeTable::SetSelectedClass(const PfClass* list, bool showAll) {
  ClearAll();
  m_Archetypes.clear();
  AppendColumn("");
  for (int i = 0; i < list->features.size(); i++) {
    AppendColumn(list->features[i]);
  }
  for (int i = 0; i < list->archetypes.size(); i++) {
    TableElement element = TableElement(showAll ? SHOW : DISABLE, &list->archetypes[i]);
    m_Archetypes.push_back(element);
  }
  RecalculateSize();
  for (int i = 0; i < list->features.size(); i++) {
    SetColumnWidth(i, i ? wxLIST_AUTOSIZE_USEHEADER : wxLIST_AUTOSIZE);
  }
}

int ArchetypeTable::RowToIndex(int row) {
  return m_ValidIndices.at(row);
}

void ArchetypeTable::RecalculateSize() {
  m_ValidIndices.clear();
  for (int i = 0; i < this->m_Archetypes.size(); i++) {
    bool isShowing = m_Archetypes[i].m_Display == SHOW;
    bool isHiddenButVisible = m_Archetypes[i].m_Display == HIDE && m_ShowHidden;
    if (isShowing || isHiddenButVisible) {
      m_ValidIndices.push_back(i);
    }
    if (isHiddenButVisible) {

    }
  }
  SetItemCount(m_ValidIndices.size());
}

void ArchetypeTable::RecalculateStatuses() {
  std::vector<bool> blockingStatuses(m_Archetypes.at(0).m_Archetype->size(), false);
  for (int pass = 0; pass < 2; pass++) {
    for (int archetypeIndex = 0; archetypeIndex < m_Archetypes.size(); archetypeIndex++) {
      TableElement* checkedRow = &m_Archetypes.at(archetypeIndex);
      // Checks if tag is DISABLE on the 0th pass, and if it isn't on the 1st
      if ((checkedRow->m_Display == DISABLE) != (pass != 0)) {
        // All rows are assumed to be Showing in the 0th pass until proven otherwise
        if (pass == 1) {
          checkedRow->m_Display = SHOW;
        }
        for (int replacementIndex = 0; replacementIndex < checkedRow->m_Archetype->size(); replacementIndex++) {
          ReplacementStatus status = checkedRow->m_Archetype->GetStatus(replacementIndex);
          bool isBlocking = status == REPLACED || status == CHANGED;
          if (pass == 0) {
            if (isBlocking) {
              blockingStatuses.at(replacementIndex) = true;
            }
          } else {
            bool isIncompatible = isBlocking && blockingStatuses.at(replacementIndex);
            if (isIncompatible) {
              checkedRow->m_Display = HIDE;
            }
          }
        }
      }
    }
  }
}

wxString ArchetypeTable::OnGetItemText(long item, long column) const {
  const TableElement* row = &m_Archetypes.at(m_ValidIndices.at(item));

  std::string val = "";
  if (column == 0) {
    val = row->m_Archetype->GetName();
  } else {
    val = Archetype::StatusToString(row->m_Archetype->GetStatus(column - 1));
  }
  return wxString(val);
}

wxListItemAttr* ArchetypeTable::OnGetItemAttr(long item) const {
  wxListItemAttr* attr = new wxListItemAttr();
  DisplayStatus status = m_Archetypes.at(m_ValidIndices.at(item)).m_Display;
  if (status == HIDE) {
    attr->SetTextColour(wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT));
  }
  if (item % 2) {
    attr->SetBackgroundColour(GetAlternateRowColour());
  }
  return attr;
}
