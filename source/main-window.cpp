#include "main-window.h"

MainWindow::MainWindow(const wxString& title, int width, int height)
  : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(width, height)) {
  SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
  SetIcon(d20_Normal_xpm);

  m_MainPanel = new wxPanel(this);
  m_TopPanel = new wxPanel(m_MainPanel);
  m_MiddlePanel = new wxPanel(m_MainPanel);

  m_ArchetypeLabel = new wxStaticText(m_TopPanel, wxID_ANY, "Archetype List");
  m_CustomClassLabel = new wxStaticText(m_MiddlePanel, wxID_ANY, "Selected Archetypes");
  m_ShowHidden = new wxCheckBox(m_TopPanel, wxID_ANY, "Show Hidden", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

  m_ClassDropdown = new wxComboBox(
    m_TopPanel,
    wxID_ANY,
    "Select a class...",
    wxDefaultPosition,
    wxDefaultSize,
    GetClassList(),
    wxCB_SORT | wxTE_PROCESS_ENTER
  );

  m_ArchetypeTable = new ArchetypeTable(m_MainPanel, true);
  m_SelectedArchetypes = new ArchetypeTable(m_MainPanel, false);
  m_TransferButton = new wxButton(m_MiddlePanel, wxID_ANY);
  
  // Setting m_ButtonStatus to a value which is Not Disabled, so it will be set properly.
  m_ButtonStatus = ADD;
  SetButtonStatus(DISABLED);

  // Apply Sizers to all objects
  wxBoxSizer* rootSizer = new wxBoxSizer(wxHORIZONTAL);
  rootSizer->Add(m_MainPanel, 1, wxEXPAND | wxALL, 10);
  rootSizer->SetSizeHints(this);
  this->SetSizerAndFit(rootSizer);

  wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
  mainSizer->Add(m_TopPanel, 0, wxEXPAND);
  mainSizer->AddSpacer(5);
  mainSizer->Add(m_ArchetypeTable, 1, wxEXPAND);
  mainSizer->AddSpacer(5);
  mainSizer->Add(m_MiddlePanel, 0, wxEXPAND);
  mainSizer->AddSpacer(5);
  mainSizer->Add(m_SelectedArchetypes, 0, wxEXPAND);
  mainSizer->SetSizeHints(this);
  m_MainPanel->SetSizerAndFit(mainSizer);


  wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
  topSizer->Add(m_ArchetypeLabel, 0, wxALIGN_BOTTOM);
  topSizer->AddStretchSpacer();
  topSizer->Add(m_ShowHidden, 0, wxALIGN_CENTER_VERTICAL);
  topSizer->AddSpacer(5);
  topSizer->Add(m_ClassDropdown, 0);
  topSizer->SetSizeHints(this);
  m_TopPanel->SetSizerAndFit(topSizer);

  wxBoxSizer* middleSizer = new wxBoxSizer(wxHORIZONTAL);
  middleSizer->Add(m_CustomClassLabel, 0, wxALIGN_BOTTOM);
  middleSizer->AddStretchSpacer();
  middleSizer->Add(m_TransferButton, 0, wxEXPAND);
  middleSizer->SetSizeHints(this);
  m_MiddlePanel->SetSizerAndFit(middleSizer);

  // Bind all events
  m_ShowHidden->Bind(wxEVT_CHECKBOX, &MainWindow::OnShowHiddenToggle, this);
  m_ClassDropdown->Bind(wxEVT_COMBOBOX, &MainWindow::OnClassDropdownClick, this);
  m_ArchetypeTable->Bind(wxEVT_LIST_ITEM_SELECTED, &MainWindow::OnSelectListedArchetype, this);
  m_ArchetypeTable->Bind(wxEVT_LIST_ITEM_DESELECTED, &MainWindow::OnDeselectArchetype, this);
  m_SelectedArchetypes->Bind(wxEVT_LIST_ITEM_SELECTED, &MainWindow::OnSelectAddedArchetype, this);
  m_SelectedArchetypes->Bind(wxEVT_LIST_ITEM_DESELECTED, &MainWindow::OnDeselectArchetype, this);

  // Set Show Hidden to true by default
  const bool defaultShowHidden = true;
  m_ShowHidden->SetValue(defaultShowHidden);
  m_ArchetypeTable->SetShowHidden(defaultShowHidden);

  // Center the window on the screen and Maximize.
  Centre();
  Maximize(true);
}

void MainWindow::OnShowHiddenToggle(wxCommandEvent& event) {
  m_ArchetypeTable->SetShowHidden((bool) event.GetInt());
}

void MainWindow::OnClassDropdownClick(wxCommandEvent& event) {
  m_TransferButton->Disable();
  std::string chosenClass = (std::string) event.GetString();
  if (chosenClass == "Vampire Hunter") {
    // TODO: Set this dialog box to force the user to apologize
    wxMessageDialog* rudeDialog = new wxMessageDialog(this, wxString("What's wrong with you?"), wxString("Stop It"), wxICON_QUESTION );
    rudeDialog->SetOKLabel("I'm Sorry");
    rudeDialog->ShowModal();
    rudeDialog->Destroy();
  } else if (chosenClass == "Unchained Barbarian" || chosenClass == "Unchained Rogue") {
    wxMessageDialog* errorDialog = new wxMessageDialog(this, wxString("This class does not have any archetypes."), wxString("No Archetypes"));
    errorDialog->ShowModal();
    errorDialog->Destroy();
  } else if (chosenClass == "Unchained Summoner") {
    wxMessageDialog* matchedWarning = new wxMessageDialog(this, wxString("Unchained Summoner's archetypes are listed in the regular Summoner's table. The Summoner's table will now be opened."), wxString("Redirect"), wxICON_QUESTION | wxOK);
    matchedWarning->ShowModal();
    matchedWarning->Destroy();
    const PfClass* list = &PfData::CLASSES.at("Summoner");
    m_ArchetypeTable->SetSelectedClass(list, true);
    m_SelectedArchetypes->SetSelectedClass(list, false);
    m_SelectedArchetypes->SetColumnWidth(0, m_ArchetypeTable->GetColumnWidth(0));
    m_SelectedArchetypes->RecalculateSize();
  } else {
    const PfClass* list = &PfData::CLASSES.at((std::string) event.GetString());
    m_ArchetypeTable->SetSelectedClass(list, true);
    m_SelectedArchetypes->SetSelectedClass(list, false);
    m_SelectedArchetypes->SetColumnWidth(0, m_ArchetypeTable->GetColumnWidth(0));
    m_SelectedArchetypes->RecalculateSize();
  }
  event.Skip();
}

void MainWindow::OnSelectListedArchetype(wxCommandEvent& event) {
  m_SelectedArchetypes->Select(m_SelectedArchetypes->GetFirstSelected(), false);
  long row = m_ArchetypeTable->GetFirstSelected();
  ArchetypeTable::DisplayStatus status = m_ArchetypeTable->GetDisplayStatus(m_ArchetypeTable->RowToIndex(row));
  if(status == ArchetypeTable::DisplayStatus::SHOW) {
    SetButtonStatus(ADD);
  }
  event.Skip();
}

void MainWindow::OnSelectAddedArchetype(wxCommandEvent& event) {
  m_ArchetypeTable->Select(m_ArchetypeTable->GetFirstSelected(), false);
  long row = m_SelectedArchetypes->GetFirstSelected();
  ArchetypeTable::DisplayStatus status = m_SelectedArchetypes->GetDisplayStatus(m_SelectedArchetypes->RowToIndex(row));
  if(status == ArchetypeTable::DisplayStatus::SHOW) {
    SetButtonStatus(REMOVE);
  }
  event.Skip();
}

void MainWindow::OnDeselectArchetype(wxCommandEvent& event) {
  SetButtonStatus(DISABLED);
  event.Skip();
}


void MainWindow::OnAddArchetype(wxCommandEvent& event) {
  long row = m_ArchetypeTable->GetFirstSelected();
  m_ArchetypeTable->SetDisplayStatus(m_ArchetypeTable->RowToIndex(row), ArchetypeTable::DisplayStatus::DISABLE);
  m_SelectedArchetypes->SetDisplayStatus(m_ArchetypeTable->RowToIndex(row), ArchetypeTable::DisplayStatus::SHOW);
  m_ArchetypeTable->RecalculateStatuses();
  m_ArchetypeTable->RecalculateSize();
  m_SelectedArchetypes->RecalculateSize();

  m_ArchetypeTable->Select(m_ArchetypeTable->GetFirstSelected(), false);
  m_ArchetypeTable->Refresh();
  m_SelectedArchetypes->Refresh();
  SetButtonStatus(DISABLED);
  event.Skip();
}

void MainWindow::OnRemoveArchetype(wxCommandEvent& event) {
  long row = m_SelectedArchetypes->GetFirstSelected();
  m_ArchetypeTable->SetDisplayStatus(m_SelectedArchetypes->RowToIndex(row), ArchetypeTable::DisplayStatus::SHOW);
  m_SelectedArchetypes->SetDisplayStatus(m_SelectedArchetypes->RowToIndex(row), ArchetypeTable::DisplayStatus::DISABLE);
  m_ArchetypeTable->RecalculateStatuses();
  m_ArchetypeTable->RecalculateSize();
  m_SelectedArchetypes->RecalculateSize();

  m_SelectedArchetypes->Select(m_SelectedArchetypes->GetFirstSelected(), false);
  m_ArchetypeTable->Refresh();
  m_SelectedArchetypes->Refresh();
  SetButtonStatus(DISABLED);
  event.Skip();
}

wxArrayString MainWindow::GetClassList() {
  const wxString extras[] = {
    "Unchained Barbarian",
    "Vampire Hunter",
    "Unchained Summoner",
    "Unchained Rogue"
  };
  wxArrayString classList = wxArrayString(4, extras);
  for (const auto& kv : PfData::CLASSES) {
    classList.Add(wxString(kv.first));
  }

  return classList;
}

void MainWindow::SetButtonStatus(ButtonStatus status) {
  if (status != m_ButtonStatus) {
    switch (status) {
      case ADD:
        m_TransferButton->Enable();
        m_TransferButton->SetLabelText("Add Archetype");
        m_TransferButton->Unbind(wxEVT_BUTTON, &MainWindow::OnRemoveArchetype, this);
        m_TransferButton->Bind(wxEVT_BUTTON, &MainWindow::OnAddArchetype, this);
        break;
      case REMOVE:
        m_TransferButton->Enable();
        m_TransferButton->SetLabelText("Delete Archetype");
        m_TransferButton->Unbind(wxEVT_BUTTON, &MainWindow::OnAddArchetype, this);
        m_TransferButton->Bind(wxEVT_BUTTON, &MainWindow::OnRemoveArchetype, this);
        break;
      case DISABLED:
        m_TransferButton->Disable();
        m_TransferButton->SetLabelText("Select an Archetype...");
        m_TransferButton->Unbind(wxEVT_BUTTON, &MainWindow::OnAddArchetype, this);
        m_TransferButton->Unbind(wxEVT_BUTTON, &MainWindow::OnRemoveArchetype, this);
        break;
      default:
        throw "Unhandled Button Status";
        break;
    }
    m_TransferButton->Refresh();
    m_ButtonStatus = status;
  }
}
