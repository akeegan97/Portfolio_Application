#ifndef MAINFRAME_HPP
#define MAINFRAME_HPP

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include "asset.hpp"
#include "investor.hpp"
#include "position.hpp"
#include "valuation.hpp"
#include "portfolio.hpp"
#include "investorpositiondisplay.hpp"
#include "vlistcontrol.hpp"

class MainFrame : public wxFrame{
    public:
        MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size, Portfolio &port)
        : wxFrame(NULL, wxID_ANY, title, pos, size),portfolio(port){
            setupLayout();
        };
    
    private:
        Portfolio &portfolio;
        VListControl<std::shared_ptr<Asset>>* allAssetVListControl;
        VListControl<InvestorPositionDisplay>* investorPositionListControl;
        VListControl<AssetEvent> *allAssetEventVListControl;
         
        void setupLayout();

};


#endif