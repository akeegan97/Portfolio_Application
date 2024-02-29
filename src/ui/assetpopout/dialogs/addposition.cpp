#include "ui/assetpopout/dialogs/addposition.hpp"


AddPositionDialog::AddPositionDialog(wxWindow* parentWindow, Portfolio &portfolio):
    wxDialog(parentWindow, wxID_ANY, "Add Position",wxDefaultPosition, wxDefaultSize,wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
    m_portfolio(portfolio){
        SetupLayout();
    }

void AddPositionDialog::SetupLayout(){
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    wxArrayString typeChoices;
    typeChoices.Add("Standalone");
    typeChoices.Add("Component");
    typeChoices.Add("Hybrid");

    typeOfNewPositionChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, typeChoices);

    wxArrayString investorChoices;


    for(auto inv: m_portfolio.GetInvestors()){
        std::string name = inv->GetName();
        investorChoices.Add(name);
    }

    associatedInvestor = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, investorChoices);

    topSizer->Add(typeOfNewPositionChoice, 1, wxALL|wxEXPAND, 5);
    topSizer->Add(associatedInvestor, 1, wxALL|wxEXPAND, 5);

    mainSizer->Add(topSizer, 1, wxALL|wxEXPAND,5);

    lockInChoiceButton = new wxButton(this, wxID_ANY, "Confirm Position Type");
    lockInChoiceButton->Bind(wxEVT_BUTTON, &AddPositionDialog::OnConfirmPositionType,this);

    lockInInvestorButton = new wxButton(this, wxID_ANY, "Confirm Investor");
    lockInInvestorButton->Bind(wxEVT_BUTTON, &AddPositionDialog::OnConfirmAssociatedInvestor, this);

    addNewInvestorButton = new wxButton(this, wxID_ANY, "Add New Investor");
    addNewInvestorButton->Bind(wxEVT_BUTTON, &AddPositionDialog::AddInvestor, this);

    buttonSizer->Add(lockInChoiceButton,1, wxALL|wxEXPAND, 5);
    buttonSizer->Add(lockInInvestorButton, 1, wxALL|wxEXPAND,5);
    buttonSizer->Add(addNewInvestorButton, 1, wxALL|wxEXPAND, 5);

    mainSizer->Add(buttonSizer, 1, wxALL|wxEXPAND,5);

}

void AddPositionDialog::OnConfirmAssociatedInvestor(wxCommandEvent &e){
    m_associatedInvestorName = associatedInvestor->GetSelection();
    lockInChoiceButton->SetBackgroundColour(wxColor(0, 252, 118));
    this->Refresh();
    this->Update();
}

void AddPositionDialog::OnConfirmPositionType(wxCommandEvent &e){
    m_typeOfPosition = typeOfNewPositionChoice->GetSelection();
}
