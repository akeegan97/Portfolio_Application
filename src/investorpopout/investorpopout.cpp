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
    //next add notebook for distributions per asset vlistcontrols 
    distributionsByAssetNoteBook = new wxNotebook(this, wxID_ANY);
    processedAssets.clear();

    for(const auto&position:investor->positions){
        std::shared_ptr<Asset>asset = position->assetPtr;
        if(processedAssets.find(asset) == processedAssets.end()){
            processedAssets.insert(asset);
            wxPanel *panel = new wxPanel(distributionsByAssetNoteBook, wxID_ANY);
            panel->SetBackgroundColour(wxColor(0,0,0));
            VListControl<Distribution> *netIncomeVLC = new VListControl<Distribution>(panel, wxID_ANY, FromDIP(wxDefaultPosition), FromDIP(wxDefaultSize));
            distributionsByAssetNoteBook->AddPage(panel, asset->assetName);
            netIncomeVLC->SetBackgroundColour(wxColor(0,0,0));
            std::vector<Distribution> mergeredDistributions;
            for(const auto&position:investor->positions){
                if(position->assetPtr == asset){
                    mergeredDistributions.insert(mergeredDistributions.end(),position->netIncome.begin(),position->netIncome.end());
                }
            }
            netIncomeVLC->setItems(mergeredDistributions);
        }
    }
    topSizer->Add(distributionsByAssetNoteBook, 3, wxALL, 10);
    

    mainSizer->Add(topSizer,3,wxALL|wxEXPAND, 10);
    this->SetSizer(mainSizer);
    this->Layout();
}

