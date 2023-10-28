#include <iostream>
#include <wx/wx.h>
#include "portfolio.hpp"
#include "mainframe.hpp"


class RugenBerg : public wxApp{
    public:
        bool OnInit()override;
        int OnExit()override;
    private:
        Portfolio portfolio;
};

bool RugenBerg::OnInit(){
    //portfolio.LoadFromFile("..\\storage\\data.json");
    MainFrame *frame = new MainFrame("rugenberg", wxDefaultPosition, wxDefaultSize,portfolio);
    // frame->SetBackgroundColour(wxColor(35, 207, 61));
    frame->Show(true);
    return true;

}

int RugenBerg::OnExit(){
    //portfolio.SavePortfolioToFile(portfolio,"..\\storage\\data.json");
    return 0;
}

wxIMPLEMENT_APP(RugenBerg);


// int main(){

//     std::cout<<"Ran EXE"<<std::endl;
//     return 0;
// }