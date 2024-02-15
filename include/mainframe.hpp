#ifndef MAINFRAME_HPP
#define MAINFRAME_HPP
#include <random>
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include "helpers/customevents.hpp"
#include "baseclasses/asset2.hpp"
#include "baseclasses/investor2.hpp"
#include "baseclasses/position2.hpp"
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
#include "helpers/utilities.hpp"


class MainFrame : public wxFrame{
    public:
        MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size, Portfolio &port)
        : wxFrame(NULL, wxID_ANY, title, pos, size),
            portfolio(port),
            allAssetVListControl(nullptr),
            totalInvestedText(nullptr),
            totalInvestorCountText(nullptr),
            totalValuationText(nullptr),
            chartPanelHolderPanel(nullptr),
            quoteOfTheDate(nullptr)
            {
                setupLayout();
                ReadPickQuote("../storage/RugenBergQuotes.txt");
                UpdatePortfolioDisplayValues();
                Bind(ASSET_POPOUT_CLOSED, &MainFrame::OnAssetPopoutClose, this);
            };
        Portfolio &portfolio;
    
    private:
        VListControl<std::shared_ptr<Asset2>>* allAssetVListControl;
        VListControl<std::shared_ptr<Investor2>>* allInvestorVListControl;
        wxStaticText * totalInvestedText;
        wxStaticText * totalInvestorCountText;
        wxStaticText * totalValuationText;
        wxPanel* chartPanelHolderPanel;
        wxStaticText * quoteOfTheDate;
        wxPanel* quoteOftheDatePanel;
        std::string originalQuoteText;
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
        void OnFrameResizeForQuote(wxSizeEvent &e);
};


#endif