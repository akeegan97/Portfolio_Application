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
    portfolio.LoadFromFile("C:\\Desktop\\Code\\CPP\\Portfolio_Application\\storage\\data.json");
    portfolio.PopulateEvents();
    MainFrame *frame = new MainFrame("rugenberg", wxDefaultPosition, wxDefaultSize, portfolio);
    frame->SetBackgroundColour(wxColor(0,0,0));
    frame->Show(true);
    return true;
}


int RugenBerg::OnExit(){
    //portfolio.SavePortfolioToFile(portfolio,"..\\storage\\data.json");
    return 0;
}

wxIMPLEMENT_APP(RugenBerg);


