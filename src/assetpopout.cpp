#include "assetpopout.hpp"
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include "investorPositionEdit.hpp"
#include "addDistribution.hpp"


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

    textSizer->Add(numInvestorsText,1,wxEXPAND|wxALL,10);
    textSizer->Add(totalSubscribedText,1,wxEXPAND|wxALL,10);
    textSizer->Add(totalPaidText,1,wxEXPAND|wxALL,10);
    textSizer->Add(totalDeployedCapitalText,1,wxEXPAND|wxALL,10);
    textSizer->Add(totalReserveCapitalText,1,wxEXPAND|wxALL,10);
    textSizer->Add(totalReturnedCapitalText,1,wxEXPAND|wxALL,10);

    bottomSizer->Add(textSizer);

    addDistributionButton = new wxButton(this, wxID_ANY, "Add Distribution");
    addDistributionButton->SetBackgroundColour(wxColor(0,0,0));
    addDistributionButton->SetForegroundColour(wxColor(51,245,12));
    addDistributionButton->Bind(wxEVT_BUTTON, &AssetPopout::OnAddDistributionClicked, this);

    bottomSizer->Add(addDistributionButton);

    mainSizer->Add(bottomSizer, 3, wxALL|wxEXPAND, 10);
    this->SetSizer(mainSizer);

    for(auto& investor: asset->investors){
        for(auto &position: investor->positions){
            if(position->assetPtr==asset){
                ManagementFee mgmtFee;
                std::cout<<"Position's MGMT FEE VECTOR LENGTH: "<<position->managementFees.size()<<std::endl;
                mgmtFee = position->CalculatePositionManagementFees(*position, investor->managementFeePercentage);
                position->PushFeeToVector(mgmtFee);
               // position->CalculatePositionNetIncome(asset->distributions.back(), investor->promoteFeePercentage);//ONLY FOR TESTING EVENTUALLY MOVE TO EVT THAT CORRESPONDS TO NEW DISTRIBUTION TO ASSET
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
        std::cout<<"Clicked Add Distribution Button:"<<std::endl;
        /*
            1. Pushing Distribution to Asset.distributions vector
            2. Calculate Position's "Net Income" -> IE Distribution - MGMT FEES DUE - PROMOTE FEE 
            3. Push "Net" Distribution from ^ to Position.netIncome vector
            4. Push the Promote Fees taken out of the gross distribution to Position.promoteFees vector
            5. reset/recalibrate the Position.mgmtFeesDue double value
        Data Structure: 
            Recieve Distribuition <Date, Amount> from User:->
                Asset.Distributions -> houses the <Date, Gross Amount>
                Position.NetIncome -> houses the <Date, Net of Fees Amount(both fees)>
                Position.PromoteFees -> houses the <Date, promoteFee for that distribution>
                Position.mgmtFeesDue -> keeps track of the rolling mgmt fees due and needs to be reduced by the distribution
                Position.ManagementFees ->Auto calculated each Q based off of the Deployed Capital and movements in and out of Deployed
                                        In the Q
        */
    }

}