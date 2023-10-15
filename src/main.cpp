#include <iostream>
#include <wx/wx.h>
#include "portfolio.hpp"


class RugenBerg : public wxApp{
    public:
        bool OnInit()override;
        int OnExit()override;
    private:
        Portfolio portfolio;
};

bool RugenBerg::OnInit(){
    portfolio.LoadFromFile("..\\storage\\data.json");
    return true;

}

int RugenBerg::OnExit(){
    portfolio.SavePortfolioToFile(portfolio,"..\\storage\\data.json");
    return 0;
}

wxIMPLEMENT_APP(RugenBerg);