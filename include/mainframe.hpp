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
#include <wx/xy/xyplot.h>
#include <wx/xy/xydataset.h>
#include <wx/xy/timeseriesdataset.h>
#include <wx/chartpanel.h>


class MainFrame : public wxFrame{
    public:
        MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size, Portfolio &port)
        : wxFrame(NULL, wxID_ANY, title, pos, size),
            portfolio(port),
            allAssetVListControl(nullptr),
            allAssetEventVListControl(nullptr),
            investorPositionListControl(nullptr),
            totalInvestedText(nullptr),
            totalInvestorCountText(nullptr),
            totalValuationText(nullptr),
            chartPanel(nullptr)
            {
                setupLayout();
                UpdatePortfolioDisplayValues();
            };
        Portfolio &portfolio;
    
    private:
        VListControl<std::shared_ptr<Asset>>* allAssetVListControl;
        VListControl<InvestorPositionDisplay>* investorPositionListControl;
        VListControl<AssetEvent> *allAssetEventVListControl;
        wxStaticText * totalInvestedText;
        wxStaticText * totalInvestorCountText;
        wxStaticText * totalValuationText;
        wxChartPanel * chartPanel;
        void setupLayout();
        void UpdatePortfolioDisplayValues();

};


#endif