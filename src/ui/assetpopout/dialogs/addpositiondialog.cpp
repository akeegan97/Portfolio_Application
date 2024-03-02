#include "ui/assetpopout/dialogs/addpositiondialog.hpp"


AddPositionDialog::AddPositionDialog(wxWindow* parentWindow, Portfolio &portfolio, std::shared_ptr<Asset> asset):
    wxDialog(parentWindow, wxID_ANY, "Add Position",wxDefaultPosition, wxDefaultSize,wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
    m_portfolio(portfolio),m_asset(asset){
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
    confirmSelectionsLaunchButton->Bind(wxEVT_BUTTON, &AddPositionDialog::OnConfirmPosition, this);
    wxButton *okayButton = new wxButton(this, wxID_OK, "Done");
    buttonSizer->Add(addNewInvestorButton, 1, wxALL|wxEXPAND, 5);
    buttonSizer->Add(confirmSelectionsLaunchButton, 1, wxALL|wxEXPAND,5);
    buttonSizer->Add(okayButton, 1, wxALL|wxEXPAND,5);


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

void AddPositionDialog::OnConfirmPosition(wxCommandEvent &e){
    SetTypeOfPosition();
    SetInvestorName();
    std::string positionType = GetTypeOfNewPosition();
    std::string investorName = GetAssociatedInvestor();
    auto associatedInvestorPointer = m_portfolio.GetInvestorByName(investorName);
    if(positionType == "Standalone"){
        AddStandalonePositionDialog dialog(this->GetParent(), m_portfolio);
        int retValue = dialog.ShowModal();
        if(retValue == wxID_OK){
            Position newPosition;
            std::shared_ptr<Position> newPositionPtr = std::make_shared<Position>();
            newPositionPtr->SetInvestorPtr(associatedInvestorPointer);
            newPositionPtr->SetAssetPtr(m_asset);
            wxDateTime dateInvested = dialog.GetDateValue();
            double amountPaid = dialog.GetPaidAmount();
            newPositionPtr->SetDateInvested(dateInvested);
            newPositionPtr->SetPaid(amountPaid);
            m_asset->AddPosition(newPositionPtr);
            m_asset->SetNewCommittedOnNewPosition(amountPaid);
            m_asset->SetPositionValues();
            associatedInvestorPointer->AddPosition(newPositionPtr);
            newPositionPtr->TriggerUpdateOfManagementFeeVector();
            m_asset->TriggerUpdateOfDistributionsForPositions();
        }
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

void AddPositionDialog::SetTypeOfPosition(){
    m_typeOfNewPosition = typeOfNewPositionChoice->GetStringSelection().ToStdString();
}

void AddPositionDialog::SetInvestorName(){
    m_associatedInvestor = associatedInvestorChoice->GetStringSelection().ToStdString();
}