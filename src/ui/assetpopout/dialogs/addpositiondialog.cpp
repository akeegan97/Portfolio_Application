#include "ui/assetpopout/dialogs/addpositiondialog.hpp"
#include "helpers/utilities.hpp"

AddPositionDialog::AddPositionDialog(wxWindow* parentWindow, Portfolio &portfolio, std::shared_ptr<Asset> asset):
    wxDialog(parentWindow, wxID_ANY, "Add Position",wxDefaultPosition, wxDefaultSize,wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
    m_portfolio(portfolio),m_asset(asset){
        SetupLayout();
        #ifdef __WXMSW__
        wxFont font = wxFont(12, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
        wxColour color = wxColor(255,255,255);
        wxColour foregroundcolor = wxColor(0,0,0);
        utilities::SetBackgroundColorForWindowAndChildren(this, color, foregroundcolor);
        utilities::SetFontForWindowAndChildren(this, font);
        #endif
    }

void AddPositionDialog::SetupLayout(){
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *topLeftSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topRightSizer = new wxBoxSizer(wxVERTICAL);
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

    typeOfNewPositionText = new wxStaticText(this, wxID_ANY, "Select Type");
    associatedInvestorText = new wxStaticText(this, wxID_ANY,"Select Investor");

    topLeftSizer->Add(typeOfNewPositionText, 1, wxLEFT, 5);
    topLeftSizer->Add(typeOfNewPositionChoice, 1, wxALL|wxEXPAND,5);

    topRightSizer->Add(associatedInvestorText,1,wxLEFT,5);
    topRightSizer->Add(associatedInvestorChoice,1,wxALL|wxEXPAND,5);

    topSizer->Add(topLeftSizer, 1, wxALL|wxEXPAND, 5);
    topSizer->Add(topRightSizer, 1, wxALL|wxEXPAND, 5);

    mainSizer->Add(topSizer, 1, wxALL|wxEXPAND,5);

    addNewInvestorButton = new wxButton(this, wxID_ANY, "Add New Investor");
    addNewInvestorButton->Bind(wxEVT_BUTTON, &AddPositionDialog::AddInvestor, this);
    confirmSelectionsLaunchButton = new wxButton(this, wxID_ANY, "Confirm");
    confirmSelectionsLaunchButton->Bind(wxEVT_BUTTON, &AddPositionDialog::OnConfirmPosition, this);
    wxButton *okayButton = new wxButton(this, wxID_OK, "Done");
    buttonSizer->Add(addNewInvestorButton, 1, wxALL|wxEXPAND);
    buttonSizer->Add(confirmSelectionsLaunchButton, 1, wxALL|wxEXPAND);
    buttonSizer->Add(okayButton, 1, wxALL|wxEXPAND,5);


    mainSizer->Add(buttonSizer, 1, wxALL|wxEXPAND,5);

    this->SetSizer(mainSizer);
    this->Layout();
    #ifdef __WXMAC__
        wxFont font = wxFont(14, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
        wxColour color = wxColor(255,255,255);
        wxColour foregroundcolor = wxColor(0,0,0);
        typeOfNewPositionChoice->SetFont(font);
        typeOfNewPositionText->SetFont(font);
        associatedInvestorText->SetFont(font);
        associatedInvestorChoice->SetFont(font);
        addNewInvestorButton->SetFont(font);
        confirmSelectionsLaunchButton->SetFont(font);
        okayButton->SetFont(font);

    #endif
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
    m_asset->SetPositionID();
    if(positionType == "Standalone"){
        AddStandalonePositionDialog dialog(this->GetParent(), m_portfolio);
        int retValue = dialog.ShowModal();
        if(retValue == wxID_OK){
            std::shared_ptr<Position> newPositionPtr = std::make_shared<Position>();
            newPositionPtr->SetInvestorPtr(associatedInvestorPointer);
            newPositionPtr->SetAssetPtr(m_asset);
            wxDateTime dateInvested = dialog.GetDateValue();
            double amountPaid = dialog.GetPaidAmount();
            if(amountPaid == 0){
                return;
            }
            double deployedAmount = dialog.GetDeployedAmount();
            double reserveAmount = dialog.GetReserveAmount();
            newPositionPtr->SetDateInvested(dateInvested);
            newPositionPtr->SetPaid(amountPaid);
            std::pair<wxDateTime, double> movement = std::make_pair(dateInvested, deployedAmount);
            m_asset->AddNewDeployed(deployedAmount);
            m_asset->AddNewReserve(reserveAmount);
            m_asset->AddMovement(movement);
            m_asset->SetNewCommittedOnNewPosition(amountPaid);
            for(auto &pos : m_asset->GetPositionsForIDP()){
                double oldDeploy = pos->GetDeployed();
                double newOwnership = pos->GetCommitted() / m_asset->GetTotalCommitted();
                double newDeployedValue = m_asset->GetTotalAssetDeployed() * newOwnership;
                std::pair<wxDateTime, double> movement = std::make_pair(dateInvested, (newDeployedValue - oldDeploy));
                pos->AddMovementDeploy(movement);
            }
            m_asset->AddPosition(newPositionPtr);
            m_asset->TriggerUpdateDerivedValues();
            m_asset->SetPositionValues();
            associatedInvestorPointer->AddPosition(newPositionPtr);
            for(auto&pos:m_asset->GetPositionsForIDP()){
                if(pos->GetInvestorPtr()==associatedInvestorPointer){
                    std::pair<wxDateTime, double> movement = std::make_pair(dateInvested, pos->GetDeployed());
                    pos->AddMovementDeploy(movement);
                }
            }
            //here need to update all positions management fees from this new position entrance -> current Q
            //also need to make the whole management fees vector for new position
            newPositionPtr->TriggerUpdateOfManagementFeeVector();
            m_asset->TriggerUpdateOfDistributionsForPositions();
        }
    }else if(positionType == "Component"){
        AddComponentPositionDialog dialog(this->GetParent(), m_asset, m_portfolio);
        int retValue = dialog.ShowModal();
        if(retValue == wxID_OK){
            std::shared_ptr<Position> newPositionPtr = std::make_shared<Position>();
            newPositionPtr->SetInvestorPtr(associatedInvestorPointer);
            newPositionPtr->SetAssetPtr(m_asset);
            wxDateTime dateInvested = dialog.GetDateValue();
            newPositionPtr->SetDateInvested(dateInvested);
            double totalDonatedCapital = 0;
            for(auto input: dialog.GetAllocations()){
                auto positionId = input.first;
                auto amountReturned = input.second;
                double allocatedAmount = wxAtof(amountReturned->GetValue());
                std::shared_ptr<Position> thisPosition = m_asset->GetPositionByID(positionId);
                std::pair<wxDateTime, double> movement = std::make_pair(dateInvested, allocatedAmount);
                thisPosition->AddRocMovement(movement);
                thisPosition->UpdateROC();
                thisPosition->SetCommitted();
                totalDonatedCapital+=allocatedAmount;
            }
            newPositionPtr->SetPaid(totalDonatedCapital);
            m_asset->AddPosition(newPositionPtr);
            m_asset->SetPositionValues();
            associatedInvestorPointer->AddPosition(newPositionPtr);
            //need to update donor positions' mgmt fees vector 
            //need to also create the new positions mgmt fees vector 
            newPositionPtr->TriggerUpdateOfManagementFeeVector();
            m_asset->TriggerUpdateOfDistributionsForPositions();
            m_asset->TriggerUpdateDerivedValues();
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