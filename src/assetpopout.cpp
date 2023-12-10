#include "assetpopout.hpp"


void AssetPopout::setupLayout(){
    auto mainSizer = new wxBoxSizer(wxHORIZONTAL);
    //left side sizer
    auto lSideSizer = new wxBoxSizer(wxVERTICAL);
    //populate the investor.investorsPositionDisplay vectors here:

    for(auto& investor : asset->investors){
        for(auto& position : investor.positions){
            if(position.assetPtr == asset){
                position.calculateOwnership(portfolio);
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
    lSideSizer->Add(investorPositionDisplayVirtualListControl, 5, wxEXPAND | wxALL, 10);
    mainSizer->Add(lSideSizer);
    this->SetSizer(mainSizer);
    investorPositionDisplayVirtualListControl->Refresh();
    this->Layout();
}