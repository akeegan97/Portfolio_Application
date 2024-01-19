#include <iostream>
#include <wx/wx.h>
#include "baseclasses/portfolio.hpp"
#include "mainframe.hpp"

class RugenBerg : public wxApp{
    public:
        bool OnInit()override;
        int OnExit()override;
        Portfolio portfolio;
};

bool RugenBerg::OnInit(){
    portfolio.LoadFromFile("../storage/data.json");
    std::cout<<"Assets: "<<portfolio.assetPtrs.size()<<std::endl;
    for(const auto& asset: portfolio.assetPtrs){
        for(const auto&inv: asset->investors){
             std::cout<<" Before Populate Investors: Asset: "<<asset->assetName.ToStdString()<<" Investor: "<< inv->clientName.ToStdString()<<" Positions.Size(): "<<inv->positions.size()<<std::endl;
        }
    }//at this moment we have the correct ordering
    portfolio.PopulateEvents();
    portfolio.PopulateInvestors();
    for(const auto& asset: portfolio.assetPtrs){
        for(const auto&inv: asset->investors){
             std::cout<<" After Populate Investors: Asset: "<<asset->assetName.ToStdString()<<" Investor: "<< inv->clientName.ToStdString()<<" Positions.Size(): "<<inv->positions.size()<<std::endl;
        }
    }
    std::cout<<" TOTAL INVESTORS IN PORTFOLIO>INVESTORPTRS: "<<portfolio.allInvestorPtrs.size()<<std::endl;
    
    if(!portfolio.allInvestorPtrs.empty()){
        portfolio.PopulateValuationMaps();    
    }
    MainFrame *frame = new MainFrame("rugenberg", wxDefaultPosition, wxSize(1200,800), portfolio);
    frame->SetBackgroundColour(wxColor(0,0,0));
    frame->Show(true);
    return true;
}

int RugenBerg::OnExit(){
    portfolio.SavePortfolioToFile(portfolio,"../storage/data2.json");
    return 0;
}

wxIMPLEMENT_APP(RugenBerg);    