#include "assetpopout.hpp"
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include "investorPositionEdit.hpp"
#include "addDistribution.hpp"
#include "moveDeploy.hpp"
#include "addValuation.hpp"
#include "customevents.hpp"


void AssetPopout::setupLayout(){
    auto mainSizer = new wxBoxSizer(wxVERTICAL);
    auto topSizer = new wxBoxSizer(wxHORIZONTAL);//only the IPD
    auto middleSizer = new wxBoxSizer(wxHORIZONTAL);//Valuations/Events/Distributions
    auto bottomSizer = new wxBoxSizer(wxHORIZONTAL);//Text values + future buttons
    asset->investorsPositionsDisplays.clear();
    for(auto& investor : asset->investors){
        for(auto& position : investor->positions){
            if(position->assetPtr == asset){
                position->calculateOwnership(portfolio);//calculate the ownership of each position in the asset only done when asset is popped out
                auto investorPositionDisplay = std::make_shared<InvestorPositionDisplay>(
                    investor, position
                );
                asset->investorsPositionsDisplays.push_back(investorPositionDisplay);
            }
        }
    }
    investorPositionDisplayVirtualListControl = new VListControl<std::shared_ptr<InvestorPositionDisplay>>(this, wxID_ANY, FromDIP(wxDefaultPosition), FromDIP(wxDefaultSize));
    investorPositionDisplayVirtualListControl->SetBackgroundColour(wxColor(0,0,0));
    investorPositionDisplayVirtualListControl->setItems(asset->investorsPositionsDisplays);
    investorPositionDisplayVirtualListControl->Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, &AssetPopout::OnInvestorPositionClick, this);

    topSizer->Add(investorPositionDisplayVirtualListControl,wxALL|wxEXPAND, 10);
    mainSizer->Add(topSizer, 3, wxALL|wxEXPAND,10);

    valuationListControl = new VListControl<Valuation>(this, wxID_ANY, FromDIP(wxDefaultPosition),FromDIP(wxDefaultSize));
    valuationListControl->setItems(asset->valuations);
    valuationListControl->SetBackgroundColour(wxColor(0,0,0));

    middleSizer->Add(valuationListControl, 3, wxALL, 10);

    eventsVirtualListControl = new VListControl<std::shared_ptr<AssetEvent>>(this, wxID_ANY, FromDIP(wxDefaultPosition), FromDIP(wxDefaultSize));
    eventsVirtualListControl->setItems(asset->events);
    eventsVirtualListControl->SetBackgroundColour(wxColor(0,0,0));

    middleSizer->Add(eventsVirtualListControl, 3, wxALL, 10);
    
    distributionListControl = new VListControl<Distribution>(this, wxID_ANY, FromDIP(wxDefaultPosition), FromDIP(wxDefaultSize));
    distributionListControl->setItems(asset->distributions);
    distributionListControl->SetBackgroundColour(wxColor(0,0,0));

    middleSizer->Add(distributionListControl, 3, wxALL, 10);

    mainSizer->Add(middleSizer, 3, wxALL|wxEXPAND, 10);


    auto textSizer = new wxBoxSizer(wxVERTICAL);

    totalSubscribedText = new wxStaticText(this, wxID_ANY, "");
    totalPaidText = new wxStaticText(this, wxID_ANY, "");
    totalDeployedCapitalText = new wxStaticText(this, wxID_ANY,"");
    numInvestorsText = new wxStaticText(this, wxID_ANY, "");
    totalReserveCapitalText = new wxStaticText(this, wxID_ANY, "");
    totalReturnedCapitalText = new wxStaticText(this, wxID_ANY, "");
    totalMgmtFeesGeneratedText = new wxStaticText(this, wxID_ANY, "");
    totalPromoteFeesGeneratedText = new wxStaticText(this, wxID_ANY, "");

    textSizer->Add(numInvestorsText,1,wxEXPAND|wxALL,10);
    textSizer->Add(totalSubscribedText,1,wxEXPAND|wxALL,10);
    textSizer->Add(totalPaidText,1,wxEXPAND|wxALL,10);
    textSizer->Add(totalDeployedCapitalText,1,wxEXPAND|wxALL,10);
    textSizer->Add(totalReserveCapitalText,1,wxEXPAND|wxALL,10);
    textSizer->Add(totalReturnedCapitalText,1,wxEXPAND|wxALL,10);
    textSizer->Add(totalMgmtFeesGeneratedText, 1, wxEXPAND| wxALL, 10);
    textSizer->Add(totalPromoteFeesGeneratedText, 1, wxEXPAND | wxALL, 10);

    bottomSizer->Add(textSizer);

    addDistributionButton = new wxButton(this, wxID_ANY, "Add Distribution");
    addDistributionButton->SetBackgroundColour(wxColor(0,0,0));
    addDistributionButton->SetForegroundColour(wxColor(51,245,12));
    addDistributionButton->Bind(wxEVT_BUTTON, &AssetPopout::OnAddDistributionClicked, this);

    assetLevelMovementOfCapitalButton = new wxButton(this, wxID_ANY, "Asset Level Movement of Capital");
    assetLevelMovementOfCapitalButton->SetBackgroundColour(wxColor(0,0,0));
    assetLevelMovementOfCapitalButton->SetForegroundColour(wxColor(51,245,12));
    assetLevelMovementOfCapitalButton->Bind(wxEVT_BUTTON, &AssetPopout::OnDeployMovement, this);

    addValuationButton = new wxButton(this, wxID_ANY, "Add Valuation");
    addValuationButton->SetBackgroundColour(wxColor(0,0,0));
    addValuationButton->SetForegroundColour(wxColor(51,245,12));
    addValuationButton->Bind(wxEVT_BUTTON, &AssetPopout::OnAddValuation, this);

    bottomSizer->Add(addDistributionButton);
    bottomSizer->Add(assetLevelMovementOfCapitalButton);
    bottomSizer->Add(addValuationButton);
    mainSizer->Add(bottomSizer, 3, wxALL|wxEXPAND, 10);
    this->SetSizer(mainSizer);

    for(auto& investor: asset->investors){
        for(auto &position: investor->positions){
            if(position->assetPtr==asset){
                ManagementFee mgmtFee;
                mgmtFee = position->CalculatePositionManagementFees(*position, investor->managementFeePercentage);
                position->PushFeeToVector(mgmtFee);
            }
        }
    }
    this->Layout();
}

template <typename T>
std::string formatDollarAmount(T value) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << std::abs(value);
    std::string formatted = stream.str();

    size_t dotPos = formatted.find('.');
    std::string intPart = formatted.substr(0, dotPos);
    std::string decimalPart = formatted.substr(dotPos + 1);

    std::string formattedWithCommas;
    int count = 0;
    for (auto it = intPart.rbegin(); it != intPart.rend(); ++it) {
        if (count == 3) {
            formattedWithCommas.push_back(',');
            count = 0;
        }
        count++;
        formattedWithCommas.push_back(*it);
    }

    std::reverse(formattedWithCommas.begin(), formattedWithCommas.end());
    
    std::string prefix = value < 0 ? "-$" : "$";
    return prefix + formattedWithCommas + '.' + decimalPart;
}

void AssetPopout::UpdateDisplayTextValues(){

    double numInvestors = asset->CalculateNumberOfInvestors();
    double totalPaid = asset->CalculatePaidCapital();
    std::string formattedTotalPaid = formatDollarAmount(totalPaid);
    double totalDeployed = asset->CalculateInvestedCapital();
    std::string formattedTotalDeployed = formatDollarAmount(totalDeployed);
    double totalSubscribed = asset->CalculateSubscribedCapital();
    std::string formattedSubscribed = formatDollarAmount(totalSubscribed);
    double totalReserveCapital = asset->CalculateReserveCapital();
    std::string formattedReserve = formatDollarAmount(totalReserveCapital);
    double totalReturnedCapital = asset->CalculateReturnedCapital();
    std::string formattedReturnedCapital = formatDollarAmount(totalReturnedCapital);
    double totalPromoteFees = asset->GetTotalPromoteFeesGenerated();
    std::string formatedPromoteFees = formatDollarAmount(totalPromoteFees);
    double totalMgmtFees = asset->GetTotalMgmtFeesGenerated();
    std::string formatedTotalMgmtFees = formatDollarAmount(totalMgmtFees);

    totalSubscribedText->SetLabel("Total Subscribed Amount: "+formattedSubscribed);
    totalSubscribedText->SetForegroundColour(wxColor(51,245,12));

    totalPaidText->SetLabel("Total Paid Amount: "+formattedTotalPaid);
    totalPaidText->SetForegroundColour(wxColor(51,245,12));

    totalDeployedCapitalText->SetLabel("Total Deployed Amount: "+formattedTotalDeployed);
    totalDeployedCapitalText->SetForegroundColour(wxColor(51,245,12));

    numInvestorsText->SetLabel(wxString::Format("Total Investors: %.2f", numInvestors));
    numInvestorsText->SetForegroundColour(wxColor(51,245,12));

    totalReserveCapitalText->SetLabel("Total Reserve Amount: "+formattedReserve);
    totalReserveCapitalText->SetForegroundColour(wxColor(51,245,12));

    totalReturnedCapitalText->SetLabel("Total Returned Amount: "+formattedReturnedCapital);
    totalReturnedCapitalText->SetForegroundColour(wxColor(51,245,12));

    totalMgmtFeesGeneratedText->SetLabel("Total Management Fees Earned: "+formatedTotalMgmtFees);
    totalMgmtFeesGeneratedText->SetForegroundColour(wxColor(51,245,12));

    totalPromoteFeesGeneratedText->SetLabel("Total Promote Fees Earned: "+formatedPromoteFees);
    totalPromoteFeesGeneratedText->SetForegroundColour(wxColor(51,245,12));
}


void AssetPopout::OnInvestorPositionClick(wxListEvent &e){
    InvestorPositionEditWindow editWindow(this);
    editWindow.SetBackgroundColour(wxColor(0,0,0));
    int returnValue = editWindow.ShowModal();

    if(returnValue==wxID_OK){
        long listIndex = e.GetIndex();
        auto& selectedInvestorPosition = investorPositionDisplayVirtualListControl->GetItemAtListIndex(listIndex);

        wxDateTime investedDate = editWindow.GetInvestmentDate();
        wxString clientName = editWindow.GetClientName();
        wxString clientType = editWindow.GetClientType();
        double clientSubscribed = editWindow.GetSubscribed();
        double clientPaid = editWindow.GetPaid();
        double clientDeployed = editWindow.GetDeployed();

        if(investedDate != wxDateTime(01,wxDateTime::Jan, 2001)){
            selectedInvestorPosition->positionPtr->dateInvested = investedDate;
        }
        if(clientName.ToStdString().size()!=0){
            selectedInvestorPosition->investorPtr->clientName = clientName;
        }
        if(clientType.ToStdString().size()!=0){
            selectedInvestorPosition->investorPtr->type = clientType;
        }
        if(clientSubscribed != 0){
            selectedInvestorPosition->positionPtr->subscribed = clientSubscribed;
        }
        if(clientPaid != 0){
            selectedInvestorPosition->positionPtr->paid = clientPaid;
        }
        if(clientDeployed != 0){
            selectedInvestorPosition->positionPtr->deployed = clientDeployed;
        }
        for(auto&ipd : asset->investorsPositionsDisplays){
            ipd->positionPtr->calculateOwnership(portfolio);
        }
        investorPositionDisplayVirtualListControl->Refresh();
        UpdateDisplayTextValues();
        portfolio.PopulateInvestors();//In case there are any changes to the Investors based on the user's input
        this->Refresh();
    }else if(returnValue == wxID_ANY){
        //exit 
    }
}

void AssetPopout::OnAddDistributionClicked(wxCommandEvent &e){
    AddDistributionDialog addDistroWindow(this);
    addDistroWindow.SetBackgroundColour(wxColor(0,0,0));
    int retValue = addDistroWindow.ShowModal();
    if(retValue == wxID_OK){
        Distribution newDistribution;
        newDistribution.distribution.first = addDistroWindow.GetDistributionDate();
        newDistribution.distribution.second = addDistroWindow.GetDistributionAmount();
        //here specify if multiple distributions per Q are okay
        asset->distributions.push_back(newDistribution);
        for(auto&inv: asset->investors){
            for(auto&pos:inv->positions){
                if(pos->assetPtr == asset){
                    pos->CalculatePositionNetIncome(newDistribution, inv->promoteFeePercentage);
                    //Calculates and populates the positions netIncome and PromoteFees vector with an entry 
                }
            }
        }
        distributionListControl->setItems(asset->distributions);
        distributionListControl->Update();
        UpdateDisplayTextValues();
        this->Refresh();
    }
}

void AssetPopout::OnDeployMovement(wxCommandEvent &e){
    MoveDeploy DeployMovementWindow(this);
    DeployMovementWindow.SetBackgroundColour(wxColor(0,0,0));
    int retValue = DeployMovementWindow.ShowModal();
    if(retValue == wxID_OK){
        wxDateTime dateOfMovement = DeployMovementWindow.GetDate();
        double amountMoved = DeployMovementWindow.GetAmountMoved();
        wxString selectedMovementDirection = DeployMovementWindow.GetSelectedMovementDirection();
        if(selectedMovementDirection == "To Deploy"){
            for(auto & inv : asset->investors){
                for(auto &pos:inv->positions){
                    if(pos->assetPtr == asset){
                        pos->reserve -= amountMoved*pos->percentOwnership;
                        pos->deployed += amountMoved*pos->percentOwnership;
                        pos->movedToDeploy[dateOfMovement] = amountMoved * pos->percentOwnership;
                        pos->CalculatePositionManagementFees(*pos, inv->managementFeePercentage);
                    }
                }
            }
        }else if(selectedMovementDirection == "From Deploy"){
            for(auto & inv : asset->investors){
                for(auto &pos:inv->positions){
                    if(pos->assetPtr == asset){
                        pos->reserve += amountMoved*pos->percentOwnership;
                        pos->deployed -=amountMoved*pos->percentOwnership;
                        pos->movedOutOfDeployed[dateOfMovement] = amountMoved*pos->percentOwnership;
                        pos->CalculatePositionManagementFees(*pos, inv->managementFeePercentage);
                    }
                }
            }
        }
        investorPositionDisplayVirtualListControl->Refresh();
        UpdateDisplayTextValues();
        this->Refresh();
    }else if(retValue == wxID_CANCEL){
        //do nothing close
    }
}
void AssetPopout::OnAddValuation(wxCommandEvent &e){
    AddValuation addValuationDialog(this);
    addValuationDialog.SetBackgroundColour(wxColor(0,0,0));
    int retVal = addValuationDialog.ShowModal();
    if(retVal == wxID_OK){
        wxDateTime valuationDate = addValuationDialog.GetDate();
        double valuationAmount = addValuationDialog.GetValuation();
        Valuation newValuation;
        newValuation.valuationDate = valuationDate;
        newValuation.valuation = valuationAmount;
        asset->valuations.push_back(newValuation);
        valuationListControl->setItems(asset->valuations);
        portfolio.addValuation();

    }else if(retVal == wxID_CANCEL){
        //do nothing and exit
    }
}

void AssetPopout::OnClose(wxCloseEvent &e){
    wxCommandEvent evt(ASSET_POPOUT_CLOSED);
    wxPostEvent(GetParent(), evt);
    e.Skip();
}