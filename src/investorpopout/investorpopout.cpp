#include "investorpopout/investorpopout.hpp"
#include <memory>
#include <unordered_set>

void InvestorPopout::SetUpLayout(){
    auto mainSizer = new wxBoxSizer(wxHORIZONTAL);
    auto rightSizer = new wxBoxSizer(wxVERTICAL);
    auto leftSizer = new wxBoxSizer(wxVERTICAL);
    auto bottomSizer = new wxBoxSizer(wxHORIZONTAL);

    investor->displaysForInvestorPopout.clear();
    std::unordered_set<std::shared_ptr<Asset>> processedAssets;
    for(auto &pos : investor->positions){
        pos->CalculateHistoricalManagementFees(investor->managementFeePercentage);
        pos->UpdateFinancesPostDistributionChanges(pos->assetPtr->distributions, investor->promoteFeePercentage,investor->managementFeePercentage);
    }

    for (const auto& position : investor->positions) {
        std::shared_ptr<Asset> asset = position->assetPtr;
        std::cout<<"Called before asset popout: Length of Distributions: "<<asset->distributions.size()<<std::endl;
        if (processedAssets.find(asset) == processedAssets.end()) {
            processedAssets.insert(asset);
            auto investorAssetDisplay = std::make_shared<InvestorAssetDisplay>(asset, investor);
            investor->displaysForInvestorPopout.push_back(investorAssetDisplay);
        }
    }
    investorAssetDisplayVirtualListControl = new VListControl<std::shared_ptr<InvestorAssetDisplay>>(this, wxID_ANY, FromDIP(wxDefaultPosition), FromDIP(wxDefaultSize));
    investorAssetDisplayVirtualListControl->setItems(investor->displaysForInvestorPopout);
    investorAssetDisplayVirtualListControl->SetBackgroundColour(wxColor(0,0,0));

    leftSizer->Add(investorAssetDisplayVirtualListControl, 1, wxALL, 10);
    //next add notebook for distributions per asset vlistcontrols 
    distributionsByAssetNoteBook = new wxNotebook(this, wxID_ANY);
    distributionsByAssetNoteBook->SetForegroundColour(wxColor(250,0,0));
    distributionsByAssetNoteBook->SetBackgroundColour(wxColor(0,0,0));
    processedAssets.clear();

    for(const auto&position:investor->positions){
        std::shared_ptr<Asset>asset = position->assetPtr;
        if(processedAssets.find(asset) == processedAssets.end()){
            processedAssets.insert(asset);
            wxPanel *panel = new wxPanel(distributionsByAssetNoteBook, wxID_ANY);
            auto panelSizer = new wxBoxSizer(wxVERTICAL);
            panel->SetBackgroundColour(wxColor(0,0,0));
            VListControl<Distribution> *netIncomeVLC = new VListControl<Distribution>(panel, wxID_ANY, FromDIP(wxDefaultPosition), FromDIP(wxDefaultSize));
            distributionsByAssetNoteBook->AddPage(panel, asset->assetName);
            VListControl<ManagementFee> *mgmtFeeVLC = new VListControl<ManagementFee>(panel, wxID_ANY, FromDIP(wxDefaultPosition), FromDIP(wxDefaultSize));
            VListControl<PromoteFee> *promoteFeeVLC = new VListControl<PromoteFee>(panel, wxID_ANY, FromDIP(wxDefaultPosition), FromDIP(wxDefaultSize));
            mgmtFeeVLC->SetBackgroundColour(wxColor(0,0,0));
            netIncomeVLC->SetBackgroundColour(wxColor(0,0,0));
            promoteFeeVLC->SetBackgroundColour(wxColor(0,0,0));
            std::vector<Distribution> mergedDistributions;
            for(const auto&position:investor->positions){
                if(position->assetPtr == asset){
                    mergedDistributions.insert(mergedDistributions.end(),position->netIncome.begin(),position->netIncome.end());
                }
            }
            std::vector<ManagementFee> mergedMgmtFees;
            for(const auto&position:investor->positions){
                if(position->assetPtr == asset){
                    mergedMgmtFees.insert(mergedMgmtFees.begin(),position->managementFees.begin(), position->managementFees.end());
                }
            }
            std::vector<PromoteFee> mergedPromoteFees;
            for(const auto& position:investor->positions){
                if(position->assetPtr == asset){
                    mergedPromoteFees.insert(mergedPromoteFees.begin(),position->promoteFees.begin(), position->promoteFees.end());
                }
            }
            mgmtFeeVLC->setItems(mergedMgmtFees);
            netIncomeVLC->setItems(mergedDistributions);
            promoteFeeVLC->setItems(mergedPromoteFees);
            panelSizer->Add(netIncomeVLC, 1, wxALL|wxEXPAND, 10);
            panelSizer->Add(mgmtFeeVLC, 1, wxALL|wxEXPAND, 10);
            panelSizer->Add(promoteFeeVLC, 1, wxALL|wxEXPAND, 10);
            panel->SetSizer(panelSizer);
            panel->Layout();
        }
    }
    rightSizer->Add(distributionsByAssetNoteBook, 1, wxALL, 10);
    mainSizer->Add(leftSizer,7,wxRIGHT, 5);
    mainSizer->Add(rightSizer, 3, wxALL|wxEXPAND,5);
    //add staticTexts
    //add notes Notebook with multiline text edits
    //potentially add chart specific for this investor's positions "Mini portfolio chart" like that on mainframe

    this->SetSizer(mainSizer);
    this->Layout();
}

