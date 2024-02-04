#ifndef ASSETPOPOUT_HPP
#define ASSETPOPOUT_HPP
#include <wx/wx.h>
#include "helpers/vlistcontrol.hpp"
#include "baseclasses/portfolio.hpp"
#include "baseclasses/asset.hpp"
#include <wx/chart.h>
#include <wx/xy/xyplot.h>
#include <wx/bars/barplot.h>
#include <wx/bars/barrenderer.h>
#include <wx/category/categorysimpledataset.h>
#include <wx/category/categoryrenderer.h>
#include <wx/axis/categoryaxis.h>
#include <wx/xy/xydataset.h>
#include <wx/xy/timeseriesdataset.h>
#include <wx/chartpanel.h>
#include <wx/xy/xylinerenderer.h>
#include <wx/axis/dateaxis.h>
#include <wx/colorscheme.h>

class AssetPopout : public wxFrame{
    public:
        AssetPopout(wxWindow *parentWindow, const wxString &title, const wxPoint &pos, const wxSize &size, Portfolio &port, std::shared_ptr<Asset> asset)
        : wxFrame(parentWindow, wxID_ANY, title, pos, size),
            portfolio(port),
            asset(asset),
            investorPositionDisplayVirtualListControl(nullptr),
            valuationListControl(nullptr){
                SetupLayout();
                UpdateDisplayTextValues();
                Bind(wxEVT_CLOSE_WINDOW, &AssetPopout::OnClose, this);
            };
        
        Portfolio &portfolio;
        std::shared_ptr<Asset> asset;
    private:
        VListControl<std::shared_ptr<InvestorPositionDisplay>>* investorPositionDisplayVirtualListControl;
        VListControl<Valuation>* valuationListControl;
        VListControl<std::shared_ptr<AssetEvent>>*eventsVirtualListControl;
        VListControl<Distribution>* distributionListControl;
        wxStaticText *totalSubscribedText;//
        wxStaticText *totalPaidText;//
        wxStaticText *totalDeployedCapitalText;//
        wxStaticText *numInvestorsText;//
        wxStaticText *totalReserveCapitalText;//
        wxStaticText *totalReturnedCapitalText;//
        //New static text lines?
        wxStaticText *totalMgmtFeesGeneratedText;
        wxStaticText *totalPromoteFeesGeneratedText;
        wxButton * addDistributionButton;
        wxButton *assetLevelMovementOfCapitalButton;
        wxButton *addValuationButton;
        wxButton *addEventButton;
        wxPanel *chartPanelHolderPanel;
        wxPanel *distributionChartPanelHolder;
        void SetupLayout();
        void UpdateDisplayTextValues();
        void OnInvestorPositionClick(wxListEvent &e);
        void OnAddDistributionClicked(wxCommandEvent &e);
        void OnCapitalMovement(wxCommandEvent &e);
        void OnAddValuation(wxCommandEvent &e);
        void OnAddEvent(wxCommandEvent &e);
        void OnClose(wxCloseEvent &e);
        void OnDistributionEdit(wxListEvent &e);
        void OnValuationEdit(wxListEvent &e);
        void OnEventEdit(wxListEvent &e);
        //for valuation+deployed capital chart
        Chart* PopulateDrawChartValuationDeploy();
        void UpdateChartValuationDeploy();
        //for distributions chart
        Chart* PopulateDrawChartDistribution();
        void UpdateChartDistribution();
};


#endif
