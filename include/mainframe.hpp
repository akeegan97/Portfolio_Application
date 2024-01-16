#ifndef MAINFRAME_HPP
#define MAINFRAME_HPP
#include <random>
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include "helpers/customevents.hpp"
#include "baseclasses/asset.hpp"
#include "baseclasses/investor.hpp"
#include "baseclasses/position.hpp"
#include "baseclasses/valuation.hpp"
#include "baseclasses/portfolio.hpp"
#include "baseclasses/investorpositiondisplay.hpp"
#include "helpers/vlistcontrol.hpp"
#include <wx/xy/xyplot.h>
#include <wx/xy/xydataset.h>
#include <wx/xy/timeseriesdataset.h>
#include <wx/chartpanel.h>
#include <wx/xy/xylinerenderer.h>
#include <wx/axis/dateaxis.h>
#include <wx/colorscheme.h>


class MainFrame : public wxFrame{
    public:
        MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size, Portfolio &port)
        : wxFrame(NULL, wxID_ANY, title, pos, size),
            portfolio(port),
            allAssetVListControl(nullptr),
            allAssetEventVListControl(nullptr),
            totalInvestedText(nullptr),
            totalInvestorCountText(nullptr),
            totalValuationText(nullptr),
            chartPanelHolderPanel(nullptr),
            quoteOfTheDate(nullptr)
            {
                setupLayout();
                UpdatePortfolioDisplayValues();
                ReadPickQuote("../storage/RugenBergQuotes.txt");
                Bind(ASSET_POPOUT_CLOSED, &MainFrame::OnAssetPopoutClose, this);
            };
        Portfolio &portfolio;
    
    private:
        VListControl<std::shared_ptr<Asset>>* allAssetVListControl;
        VListControl<std::shared_ptr<AssetEvent>> *allAssetEventVListControl;
        VListControl<std::shared_ptr<Investor>>* allInvestorVListControl;
        wxStaticText * totalInvestedText;
        wxStaticText * totalInvestorCountText;
        wxStaticText * totalValuationText;
        wxPanel* chartPanelHolderPanel;
        wxStaticText * quoteOfTheDate;
        wxPanel* quoteOftheDatePanel;
        void setupLayout();
        void UpdatePortfolioDisplayValues();
        void ReadPickQuote(const std::string&filePath);
        void OnAssetVLCClick(wxListEvent&e);
        void OnAssetPopoutClose(wxCommandEvent &e);
        Chart* PopulateDrawChart(Portfolio &portfolio);
        void UpdateChart();
        void UpdateAssetListControl();
        void UpdateEventListControl();
        void OnInvestorVLCClick(wxListEvent &e);
};


#endif