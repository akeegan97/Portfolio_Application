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
    portfolio.LoadFromFile("../storage/data4.json");
    portfolio.PopulateEvents();
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
    portfolio.SavePortfolioToFile(portfolio,"../storage/data3.json");
    return 0;
}

wxIMPLEMENT_APP(RugenBerg);    