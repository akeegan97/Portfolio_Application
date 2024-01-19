#include "investorpopout/investorpopout.hpp"
#include <memory>
#include <unordered_set>

void InvestorPopout::SetUpLayout(){
    auto mainSizer = new wxBoxSizer(wxVERTICAL);
    auto topSizer = new wxBoxSizer(wxHORIZONTAL);
    auto middleSizer = new wxBoxSizer(wxHORIZONTAL);
    auto bottomSizer = new wxBoxSizer(wxHORIZONTAL);

    investor->displaysForInvestorPopout.clear();
    std::unordered_set<std::shared_ptr<Asset>> processedAssets;

    for (const auto& position : investor->positions) {
        std::cout<<"Number of positions: "<<investor->positions.size()<<std::endl;
        std::shared_ptr<Asset> asset = position->assetPtr;

        if (processedAssets.find(asset) == processedAssets.end()) {
            processedAssets.insert(asset);
            auto investorAssetDisplay = std::make_shared<InvestorAssetDisplay>(asset, investor);
            investor->displaysForInvestorPopout.push_back(investorAssetDisplay);
            std::cout<<"Pushed IAD To Vector: "<<investorAssetDisplay->investorPtr->clientName.ToStdString()<<std::endl;
        }
    }
    investorAssetDisplayVirtualListControl = new VListControl<std::shared_ptr<InvestorAssetDisplay>>(this, wxID_ANY, FromDIP(wxDefaultPosition), FromDIP(wxDefaultSize));
    std::cout<<"Number of Elements in Investor->displaysForInvestorPopout: "<<investor->displaysForInvestorPopout.size()<<std::endl;
    investorAssetDisplayVirtualListControl->setItems(investor->displaysForInvestorPopout);
    investorAssetDisplayVirtualListControl->SetBackgroundColour(wxColor(0,0,0));

    topSizer->Add(investorAssetDisplayVirtualListControl, 3, wxALL, 10);

    mainSizer->Add(topSizer,3,wxALL|wxEXPAND, 10);
    this->SetSizer(mainSizer);
    this->Layout();
}