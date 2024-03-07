#include "ui/investorpopout/investorpopout.hpp"
#include <memory>
#include <unordered_set>

// void InvestorPopout::SetUpLayout(){
//     auto mainSizer = new wxBoxSizer(wxHORIZONTAL);
//     auto rightSizer = new wxBoxSizer(wxVERTICAL);
//     auto leftSizer = new wxBoxSizer(wxVERTICAL);
//     auto bottomSizer = new wxBoxSizer(wxHORIZONTAL);

//     investor->displaysForInvestorPopout.clear();
//     std::unordered_set<std::shared_ptr<Asset>> processedAssets;
//     for(auto &pos : investor->positions){
//         pos->CalculateHistoricalManagementFees(investor->managementFeePercentage);
//         pos->UpdateFinancesPostDistributionChanges(pos->assetPtr->distributions, investor->promoteFeePercentage,investor->managementFeePercentage);
//     }

//     for (const auto& position : investor->positions) {
//         std::shared_ptr<Asset> asset = position->assetPtr;
//         std::cout<<"Called before asset popout: Length of Distributions: "<<asset->distributions.size()<<std::endl;
//         if (processedAssets.find(asset) == processedAssets.end()) {
//             processedAssets.insert(asset);
//             auto investorAssetDisplay = std::make_shared<InvestorAssetDisplay>(asset, investor);
//             investor->displaysForInvestorPopout.push_back(investorAssetDisplay);
//         }
//     }
//     investorAssetDisplayVirtualListControl = new VListControl<std::shared_ptr<InvestorAssetDisplay>>(this, wxID_ANY, FromDIP(wxDefaultPosition), FromDIP(wxDefaultSize));
//     investorAssetDisplayVirtualListControl->setItems(investor->displaysForInvestorPopout);
//     investorAssetDisplayVirtualListControl->SetBackgroundColour(wxColor(0,0,0));

//     leftSizer->Add(investorAssetDisplayVirtualListControl, 1, wxALL, 10);
//     //next add notebook for distributions per asset vlistcontrols 
//     distributionsByAssetNoteBook = new wxNotebook(this, wxID_ANY);
//     distributionsByAssetNoteBook->SetForegroundColour(wxColor(250,0,0));
//     distributionsByAssetNoteBook->SetBackgroundColour(wxColor(0,0,0));
//     processedAssets.clear();

//     for(const auto&position:investor->positions){
//         std::shared_ptr<Asset>asset = position->assetPtr;
//         if(processedAssets.find(asset) == processedAssets.end()){
//             processedAssets.insert(asset);
//             wxPanel *panel = new wxPanel(distributionsByAssetNoteBook, wxID_ANY);
//             auto panelSizer = new wxBoxSizer(wxVERTICAL);
//             panel->SetBackgroundColour(wxColor(0,0,0));
//             VListControl<Distribution> *netIncomeVLC = new VListControl<Distribution>(panel, wxID_ANY, FromDIP(wxDefaultPosition), FromDIP(wxDefaultSize));
//             distributionsByAssetNoteBook->AddPage(panel, asset->assetName);
//             VListControl<ManagementFee> *mgmtFeeVLC = new VListControl<ManagementFee>(panel, wxID_ANY, FromDIP(wxDefaultPosition), FromDIP(wxDefaultSize));
//             VListControl<PromoteFee> *promoteFeeVLC = new VListControl<PromoteFee>(panel, wxID_ANY, FromDIP(wxDefaultPosition), FromDIP(wxDefaultSize));
//             mgmtFeeVLC->SetBackgroundColour(wxColor(0,0,0));
//             netIncomeVLC->SetBackgroundColour(wxColor(0,0,0));
//             promoteFeeVLC->SetBackgroundColour(wxColor(0,0,0));
//             std::vector<Distribution> mergedDistributions;
//             for(const auto&position:investor->positions){
//                 if(position->assetPtr == asset){
//                     mergedDistributions.insert(mergedDistributions.end(),position->netIncome.begin(),position->netIncome.end());
//                 }
//             }
//             std::vector<ManagementFee> mergedMgmtFees;
//             for(const auto&position:investor->positions){
//                 if(position->assetPtr == asset){
//                     mergedMgmtFees.insert(mergedMgmtFees.begin(),position->managementFees.begin(), position->managementFees.end());
//                 }
//             }
//             std::vector<PromoteFee> mergedPromoteFees;
//             for(const auto& position:investor->positions){
//                 if(position->assetPtr == asset){
//                     mergedPromoteFees.insert(mergedPromoteFees.begin(),position->promoteFees.begin(), position->promoteFees.end());
//                 }
//             }
//             mgmtFeeVLC->setItems(mergedMgmtFees);
//             netIncomeVLC->setItems(mergedDistributions);
//             promoteFeeVLC->setItems(mergedPromoteFees);
//             panelSizer->Add(netIncomeVLC, 1, wxALL|wxEXPAND, 10);
//             panelSizer->Add(mgmtFeeVLC, 1, wxALL|wxEXPAND, 10);
//             panelSizer->Add(promoteFeeVLC, 1, wxALL|wxEXPAND, 10);
//             panel->SetSizer(panelSizer);
//             panel->Layout();
//         }
//     }
//     rightSizer->Add(distributionsByAssetNoteBook, 1, wxALL, 10);
//     mainSizer->Add(leftSizer,7,wxRIGHT, 5);
//     mainSizer->Add(rightSizer, 3, wxALL|wxEXPAND,5);
//     //add staticTexts
//     //add notes Notebook with multiline text edits
//     //potentially add chart specific for this investor's positions "Mini portfolio chart" like that on mainframe
//     for(const auto &pos:investor->positions){
//         pos->assetPtr->PopulatePositionsHistoricalValuation();
//     }
//     for(const auto&pos:investor->positions){
//         std::cout<<"Asset: "<<pos->assetPtr->assetName<<"Investor: "<<investor->clientName<<std::endl;
//         for(const auto& val: pos->historicalValuation){
//             std::cout<<" Date: "<<val.valuationDate.FormatISODate()<<" Amount: "<<val.valuation<<std::endl;
//         }
//     }
//     this->SetSizer(mainSizer);
//     this->Layout();
// }

// Chart* InvestorPopout::PopulateInvestorsValuationDeployChart(){
//     return nullptr;
// }

// Chart* InvestorPopout::PopulateInvestorsDistributionChart(){
//     return nullptr;

// }

InvestorPopout::InvestorPopout(wxWindow *parentWindow, const wxString &title, const wxPoint &pos, const wxSize &size,
    Portfolio &port, std::shared_ptr<Investor> investor):
    wxFrame(parentWindow, wxID_ANY, title, pos, size),
    portfolio(port),
    investor(investor){
        SetupLayout();
        wxFont font = wxFont(12, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
        wxColour color = wxColor(255,255,255);
        utilities::SetBackgroundColorForWindowAndChildren(this, color);
        utilities::SetFontForWindowAndChildren(this, font);
};

void InvestorPopout::SetupLayout(){

    wxBoxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *bottomSizer = new wxBoxSizer(wxHORIZONTAL);

    investor->ClearPositionDisplays();
    std::unordered_set<std::shared_ptr<Asset>> processedAssets;
    for(auto position: investor->GetPositions()){
        std::shared_ptr<Asset> asset = position->GetAssetPointer();
        if(processedAssets.find(asset)==processedAssets.end()){
            processedAssets.insert(asset);
            auto positionDisplay = std::make_shared<InvestorAssetDisplay>(asset, investor);
            investor->AddPositionDisplay(positionDisplay);
        }
    }

    investorAssetDisplayVirtualListControl = new VListControl<std::shared_ptr<InvestorAssetDisplay>>(this, wxID_ANY, FromDIP(wxDefaultPosition), FromDIP(wxDefaultSize));
    investorAssetDisplayVirtualListControl->setItems(investor->GetAssetDisplaysNonConst());

    leftSizer->Add(investorAssetDisplayVirtualListControl, 1, wxALL,10);
    distributionsByAssetNoteBook = new wxNotebook(this, wxID_ANY);
    processedAssets.clear();
    for(auto position:investor->GetPositions()){
        std::shared_ptr<Asset> asset = position->GetAssetPointer();
        if(processedAssets.find(asset)==processedAssets.end()){
            processedAssets.insert(asset);
            wxPanel* panel = new wxPanel(distributionsByAssetNoteBook, wxID_ANY);
            auto panelSizer = new wxBoxSizer(wxVERTICAL);
            distributionsByAssetNoteBook->AddPage(panel, asset->GetAssetName());
            std::vector<Distribution> mergedDistributions;
            std::vector<ManagementFee> mergedMgmtFees;
            std::vector<PromoteFee> mergedPromoteFees;
            for(auto position:investor->GetPositions()){
                if(position->GetAssetPointer()==asset){
                    try{
                        for(auto&distribution:position->GetNetIncome()){
                            mergedDistributions.push_back(distribution);
                        }
                        for(auto&mgmtfee:position->GetManagementFees()){
                            mergedMgmtFees.push_back(mgmtfee);
                        }
                        for(auto&pf:position->GetPromoteFees()){
                            mergedPromoteFees.push_back(pf);
                        }
                    }catch(const std::exception &e){
                        wxLogError("Exception Caught: %s", e.what());
                    }catch(...){
                        wxLogError("UNKNOWN ERROR");
                    }
                }
            }
            VListControl<Distribution> *netIncomeVLC = new VListControl<Distribution>(panel, wxID_ANY, FromDIP(wxDefaultPosition), FromDIP(wxDefaultSize));
            netIncomeVLC->setItems(mergedDistributions);
            VListControl<ManagementFee> *mgmtFeeVLC = new VListControl<ManagementFee>(panel, wxID_ANY, FromDIP(wxDefaultPosition), FromDIP(wxDefaultSize));
            mgmtFeeVLC->setItems(mergedMgmtFees);
            VListControl<PromoteFee> *promoteFeeVLC = new VListControl<PromoteFee>(panel, wxID_ANY, FromDIP(wxDefaultPosition), FromDIP(wxDefaultSize));
            promoteFeeVLC->setItems(mergedPromoteFees);
            panelSizer->Add(netIncomeVLC, 1, wxALL|wxEXPAND,10);
            panelSizer->Add(mgmtFeeVLC, 1, wxALL|wxEXPAND,10);
            panelSizer->Add(promoteFeeVLC, 1, wxALL|wxEXPAND,10);
            panel->SetSizer(panelSizer);
            panel->Layout();
        }
    }
    rightSizer->Add(distributionsByAssetNoteBook, 1, wxALL, 10);
    mainSizer->Add(leftSizer, 7, wxRIGHT,5);
    mainSizer->Add(rightSizer,3,wxALL|wxEXPAND,5);
    this->SetSizer(mainSizer);
    this->Layout();  
}