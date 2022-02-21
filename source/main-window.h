#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <string>
#include "pf-data.h"
#include "archetype-table.h"
#include "../resources/icons/d20-Normal.xpm"

class MainWindow : public wxFrame {
  private:
    enum ButtonStatus { DISABLED, ADD, REMOVE };
  // variable declarations
  private:
    wxPanel* m_MainPanel;
    wxPanel* m_TopPanel;
    wxPanel* m_MiddlePanel;
    wxStaticText* m_ArchetypeLabel;
    wxStaticText* m_CustomClassLabel;
    wxComboBox* m_ClassDropdown;
    ArchetypeTable* m_ArchetypeTable;
    ArchetypeTable* m_SelectedArchetypes;
    wxButton* m_TransferButton;
    wxCheckBox* m_ShowHidden;
    ButtonStatus m_ButtonStatus;
  // function declarations
  public:
    MainWindow(const wxString& title, int width, int height);
  private:
    void OnShowHiddenToggle(wxCommandEvent& event);
    void OnClassDropdownClick(wxCommandEvent& event);
    void OnSelectListedArchetype(wxCommandEvent& event);
    void OnDeselectArchetype(wxCommandEvent& event);
    void OnSelectAddedArchetype(wxCommandEvent& event);
    void OnAddArchetype(wxCommandEvent& event);
    void OnRemoveArchetype(wxCommandEvent& event);
    wxArrayString GetClassList();
    void SetButtonStatus(ButtonStatus status);
};
