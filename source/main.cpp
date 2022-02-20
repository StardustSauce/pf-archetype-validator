#include "main.h"
#include "main-window.h"

IMPLEMENT_APP(ArchetypeValidator)

bool ArchetypeValidator::OnInit() {
  // Create and show the application's main window.
  MainWindow *mainWindow = new MainWindow(wxT("Sauce's PF1e Archetype Validator"), 600, 240);
  mainWindow->Show(true);

  return true;
}