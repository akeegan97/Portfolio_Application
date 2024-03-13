#include <iostream>
#include <wx/wx.h>
#include "models/entities/portfolio.hpp"
#include "ui/mainframe/mainframe.hpp"


class RugenBerg : public wxApp{
    public:
        bool OnInit()override;
        int OnExit()override;
        Portfolio portfolio;
};

bool RugenBerg::OnInit(){
    portfolio.LoadFromFile("../storage/fundtesting.json");
    std::cout<<" TOTAL INVESTORS IN PORTFOLIO>INVESTORPTRS: "<<portfolio.allInvestorPtrs.size()<<std::endl;
    
    MainFrame *frame = new MainFrame("rugenberg", wxDefaultPosition, wxSize(1300,800), portfolio);
    frame->Show(true);
    return true;
}

int RugenBerg::OnExit(){
    portfolio.SavePortfolioToFile(portfolio,"../storage/fundtesting.json");
    return 0;
}

wxIMPLEMENT_APP(RugenBerg);    