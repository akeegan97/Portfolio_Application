#include "ui/assetpopout/dialogs/addposition.hpp"


AddPositionDialog::AddPositionDialog(wxWindow* parentWindow, Portfolio &portfolio):
    wxDialog(parentWindow, wxID_ANY, "Add Position",wxDefaultPosition, wxDefaultSize,wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
    m_portfolio(portfolio){

    }

void AddPositionDialog::SetupLayout(){
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topSizers = new wxBoxSizer(wxHORIZONTAL);

    wxArrayString typeChoices;
    typeChoices.Add("Standalone");
    typeChoices.Add("Component");
    typeChoices.Add("Hybrid");

    wxArrayString investorChoices;

    for(auto inv: m_portfolio.GetInvestors()){
        std::string name = inv->GetName();
        investorChoices.Add(name);
    }

    
    
}