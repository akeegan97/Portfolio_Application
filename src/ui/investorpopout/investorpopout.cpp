#include "ui/investorpopout/investorpopout.hpp"
#include <memory>
#include <unordered_set>

InvestorPopout::InvestorPopout(wxWindow *parentWindow, const wxString &title, const wxPoint &pos, const wxSize &size,
    Portfolio &port, std::shared_ptr<Investor> investor):
    wxFrame(parentWindow, wxID_ANY, title, pos, size),
    portfolio(port),
    investor(investor){
        SetupLayout();
        wxFont font = wxFont(12, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
        wxColour color = wxColor(255,255,255);
        wxColour foregroundcolor = wxColor(0,0,0);
        #ifdef __WXMSW__
        utilities::SetBackgroundColorForWindowAndChildren(this, color, foregroundcolor);
        utilities::SetFontForWindowAndChildren(this, font);
        #endif
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
    distributionsByAssetNoteBook = new wxAuiNotebook(this, wxID_ANY);
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
            #ifdef __WXMAC__
                wxFont font = wxFont(14, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
                wxColour bgColor = wxColor(255,255,255);
                wxColour fgColor = wxColor(0,0,0);  
                panel->SetBackgroundColour(bgColor);
                panel->SetForegroundColour(fgColor);
                netIncomeVLC->SetForegroundColour(fgColor);
                netIncomeVLC->SetBackgroundColour(bgColor);
                netIncomeVLC->SetFont(font);
                mgmtFeeVLC->SetForegroundColour(fgColor);
                mgmtFeeVLC->SetBackgroundColour(bgColor);
                mgmtFeeVLC->SetFont(font);
                promoteFeeVLC->SetFont(font);
                promoteFeeVLC->SetBackgroundColour(bgColor);
                promoteFeeVLC->SetForegroundColour(fgColor);

            #endif  
            panel->SetSizer(panelSizer);
            panel->Layout();
        }
    }
    rightSizer->Add(distributionsByAssetNoteBook, 1, wxALL, 10);
    mainSizer->Add(leftSizer, 7, wxRIGHT,5);
    mainSizer->Add(rightSizer,3,wxALL|wxEXPAND,5);
    
    wxFont font = wxFont(14, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
    wxColour bgColor = wxColor(255,255,255);
    wxColour fgColor = wxColor(0,0,0);  
    investorAssetDisplayVirtualListControl->SetForegroundColour(fgColor);
    investorAssetDisplayVirtualListControl->SetBackgroundColour(bgColor);
    investorAssetDisplayVirtualListControl->SetFont(font);
    this->SetBackgroundColour(bgColor);
    distributionsByAssetNoteBook->SetForegroundColour(fgColor);
    distributionsByAssetNoteBook->SetFont(font);
    auto tabArt = new wxAuiSimpleTabArt();
    tabArt->SetColour(wxColor(255,255,255));
    tabArt->SetActiveColour(wxColor(255,255,255));
    tabArt->SetSelectedFont(font);
    tabArt->SetNormalFont(font);
    distributionsByAssetNoteBook->SetArtProvider(tabArt);
    distributionsByAssetNoteBook->Bind(wxEVT_AUINOTEBOOK_PAGE_CLOSE, [&](wxAuiNotebookEvent& event) {
    event.Veto();//stop users from closing tabs using GUI button
    });
    this->SetSizer(mainSizer);
    this->Layout();  
}