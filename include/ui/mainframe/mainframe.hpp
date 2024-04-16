#ifndef MAINFRAME_HPP
#define MAINFRAME_HPP
#include <random>

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include <wx/xy/xyplot.h>
#include <wx/xy/xydataset.h>
#include <wx/xy/timeseriesdataset.h>
#include <wx/chartpanel.h>
#include <wx/xy/xylinerenderer.h>
#include <wx/axis/dateaxis.h>
#include <wx/colorscheme.h>

#include "helpers/customevents.hpp"
#include "models/entities/position.hpp"
#include "models/entities/investor.hpp"
#include "models/entities/asset.hpp"
#include "models/supporting/valuation.hpp"
#include "models/entities/portfolio.hpp"
#include "models/components/investorassetdisplay.hpp"

#include "helpers/vlistcontrol.hpp"
#include "helpers/utilities.hpp"


class MainFrame : public wxFrame{
    public:
        MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size, Portfolio &port);
        Portfolio &portfolio;
    
    private:
        VListControl<std::shared_ptr<Asset>>* allAssetVListControl;
        VListControl<std::shared_ptr<Investor>>* allInvestorVListControl;
        wxStaticText * totalInvestedText;
        wxStaticText *totalReserveText;
        wxStaticText * totalInvestorCountText;
        wxStaticText * totalValuationText;
        wxStaticText * totalCapitalText;
        wxStaticText *totalPortfolioIrr;
        wxStaticText *totalMgmtFeesDue;
        wxStaticText *totalMgmtFeesEarned;
        wxStaticText *totalPromoteFeesEarned;
        wxStaticText *totalFundMoneyInUse;
        wxPanel* chartPanelHolderPanel;
        wxStaticText * quoteOfTheDate;
        wxPanel* quoteOftheDatePanel;
        std::string originalQuoteText;
        wxButton *addAssetButton;
        wxButton *addInvestorButton;
        wxButton *openTransactions;
        void setupLayout();
        void UpdatePortfolioDisplayValues();
        void ReadPickQuote(const std::string&filePath);
        void OnAssetVLCClick(wxListEvent&e);
        void OnAssetPopoutClose(wxCommandEvent &e);
        Chart* PopulateDrawChart(Portfolio &portfolio);
        void UpdateChart();
        void UpdateAssetListControl();
        void UpdateInvestorListControl();
        void OnInvestorVLCClick(wxListEvent &e);
        void OnFrameResizeForQuote(wxSizeEvent &e);
        void OnAddAsset(wxCommandEvent &e);
        void OnAddInvestor(wxCommandEvent &e);
        void OnClose(wxCloseEvent &e);
        void OnGetTransactionsClick(wxCommandEvent &e);
};


#endif