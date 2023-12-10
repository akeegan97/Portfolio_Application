#include "assetpopout.hpp"
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>


void AssetPopout::setupLayout(){
    auto mainSizer = new wxBoxSizer(wxVERTICAL);
    auto topSizer = new wxBoxSizer(wxHORIZONTAL);
    auto lSideSizer = new wxBoxSizer(wxVERTICAL);
    for(auto& investor : asset->investors){
        for(auto& position : investor.positions){
            if(position.assetPtr == asset){
                position.calculateOwnership(portfolio);//calculate the ownership of each position in the asset only done when asset is popped out
                auto investorPositionDisplay = std::make_shared<InvestorPositionDisplay>(
                    std::make_shared<Investor>(investor), 
                    std::make_shared<Position>(position)
                );
                asset->investorsPositionsDisplays.push_back(investorPositionDisplay);
            }
        }
    }
    investorPositionDisplayVirtualListControl = new VListControl<std::shared_ptr<InvestorPositionDisplay>>(this, wxID_ANY, FromDIP(wxDefaultPosition), FromDIP(wxDefaultSize));
    investorPositionDisplayVirtualListControl->SetBackgroundColour(wxColor(0,0,0));
    investorPositionDisplayVirtualListControl->setItems(asset->investorsPositionsDisplays);
    lSideSizer->Add(investorPositionDisplayVirtualListControl, 1, wxEXPAND | wxALL, 10);
    topSizer->Add(lSideSizer,7);
    //adding in the valuations of the asset to the top right corner of window
    auto rSideSizer = new wxBoxSizer(wxVERTICAL);
    valuationListControl = new VListControl<Valuation>(this, wxID_ANY, FromDIP(wxDefaultPosition),FromDIP(wxDefaultSize));
    valuationListControl->setItems(asset->valuations);
    valuationListControl->SetBackgroundColour(wxColor(0,0,0));
    rSideSizer->Add(valuationListControl, 1, wxEXPAND | wxALL, 10);
    topSizer->Add(rSideSizer,3);

    mainSizer->Add(topSizer,8);

    auto bottomSizer = new wxBoxSizer(wxHORIZONTAL);

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

    eventsVirtualListControl = new VListControl<std::shared_ptr<AssetEvent>>(this, wxID_ANY, FromDIP(wxDefaultPosition), FromDIP(wxDefaultSize));
    eventsVirtualListControl->setItems(asset->events);
    eventsVirtualListControl->SetBackgroundColour(wxColor(0,0,0));
    bottomSizer->Add(eventsVirtualListControl,5,wxEXPAND|wxALL,10);
    
    mainSizer->Add(bottomSizer,2);

    this->SetSizer(mainSizer);
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