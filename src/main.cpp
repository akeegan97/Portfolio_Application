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
    portfolio.LoadFromFile("../storage/data2.json");
    std::cout<<" TOTAL INVESTORS IN PORTFOLIO>INVESTORPTRS: "<<portfolio.allInvestorPtrs.size()<<std::endl;
    
    MainFrame *frame = new MainFrame("rugenberg", wxDefaultPosition, wxSize(1500,800), portfolio);
    frame->Show(true);
    return true;
}

int RugenBerg::OnExit(){
    portfolio.SavePortfolioToFile(portfolio,"../storage/data2.json");
    return 0;
}

wxIMPLEMENT_APP(RugenBerg);    