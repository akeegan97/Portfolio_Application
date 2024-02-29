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

    associatedInvestorChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, investorChoices);

    topSizer->Add(typeOfNewPositionChoice, 1, wxALL|wxEXPAND, 5);
    topSizer->Add(associatedInvestorChoice, 1, wxALL|wxEXPAND, 5);

    mainSizer->Add(topSizer, 1, wxALL|wxEXPAND,5);

    addNewInvestorButton = new wxButton(this, wxID_ANY, "Add New Investor");
    addNewInvestorButton->Bind(wxEVT_BUTTON, &AddPositionDialog::AddInvestor, this);
    confirmSelectionsLaunchButton = new wxButton(this, wxID_ANY, "Confirm");
    buttonSizer->Add(addNewInvestorButton, 1, wxALL|wxEXPAND, 5);
    buttonSizer->Add(confirmSelectionsLaunchButton, 1, wxALL|wxEXPAND,5);


    mainSizer->Add(buttonSizer, 1, wxALL|wxEXPAND,5);

    this->SetSizer(mainSizer);
    this->Layout();
}

void AddPositionDialog::AddInvestor(wxCommandEvent &e){
    AddInvestorDialog addInvestorDialog(this);
    int retValue = addInvestorDialog.ShowModal();
    if(retValue == wxID_OK){
        wxString newInvestorName = addInvestorDialog.GetInvestorName();
        wxString newInvestorType = addInvestorDialog.GetInvestorType();
        double newInvestorMgmtFee = addInvestorDialog.GetInvestorMgmtFee();
        double newInvestorPromoteFee = addInvestorDialog.GetInvestorPromoteFee();

        Investor newInvestor(newInvestorName,newInvestorType, newInvestorMgmtFee, newInvestorPromoteFee);

        std::shared_ptr<Investor> newInvestorPtr = std::make_shared<Investor>(newInvestor);
        m_portfolio.AddInvestor(newInvestorPtr);
        UpdateInvestorChoice();
        this->Layout();
    }
}

void AddPositionDialog::UpdateInvestorChoice(){
    wxArrayString investorChoices;
    for(auto& inv:m_portfolio.GetInvestors()){
        std::string name = inv->GetName();
        investorChoices.Add(name);
    }
    associatedInvestorChoice->Clear();
    associatedInvestorChoice->Append(investorChoices);
    associatedInvestorChoice->Refresh();
}






std::string& AddPositionDialog::GetAssociatedInvestor(){
    return m_associatedInvestor;
}
std::string& AddPositionDialog::GetTypeOfNewPosition(){
    return m_typeOfNewPosition;
}
