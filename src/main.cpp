#include <iostream>
#include <wx/wx.h>
#include "portfolio.hpp"
#include "mainframe.hpp"

class RugenBerg : public wxApp{
    public:
        bool OnInit()override;
        int OnExit()override;
        Portfolio portfolio;
};

bool RugenBerg::OnInit(){
    portfolio.LoadFromFile("../storage/data3.json");
    portfolio.PopulateEvents();
    portfolio.addValuation();
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