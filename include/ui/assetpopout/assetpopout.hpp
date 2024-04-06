#ifndef ASSETPOPOUT_HPP
#define ASSETPOPOUT_HPP
#include <wx/wx.h>
#include "helpers/vlistcontrol.hpp"
#include "models/entities/portfolio.hpp"
#include "models/entities/asset.hpp"
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
#include "helpers/utilities.hpp"
#include "ui/assetpopout/dialogs/addpositiondialog.hpp"


class AssetPopout : public wxFrame{
    public:
        AssetPopout(wxWindow *parentWindow, const wxString &title, const wxPoint &pos, const wxSize &size, Portfolio &port, std::shared_ptr<Asset> asset);
        
        Portfolio &portfolio;
        std::shared_ptr<Asset> asset;
    private:
        VListControl<std::shared_ptr<InvestorPositionDisplay>>* investorPositionDisplayVirtualListControl;
        VListControl<Valuation>* valuationListControl;
        VListControl<Distribution>* distributionListControl;
        wxStaticText *totalCommittedText;//
        wxStaticText *totalPaidText;//
        wxStaticText *totalDeployedCapitalText;//
        wxStaticText *numInvestorsText;//
        wxStaticText *totalReserveCapitalText;//
        wxStaticText *totalReturnedCapitalText;//
        wxStaticText *assetIRR;
        //New static text lines?
        wxStaticText *totalMgmtFeesGeneratedText;
        wxStaticText *totalPromoteFeesGeneratedText;
        wxStaticText *totalMgmtFeesDueText;
        wxButton * addDistributionButton;
        wxButton *assetLevelMovementOfCapitalButton;
        wxButton *addValuationButton;
        wxButton *executeDistributionButton;
        wxButton *addPositionButton;
        wxButton *viewTransactionsButton;
        wxPanel *chartPanelHolderPanel;
        wxPanel *distributionChartPanelHolder;
        void SetupLayout();
        void UpdateDisplayTextValues();
        void OnAddDistributionClicked(wxCommandEvent &e);
        void OnCapitalMovement(wxCommandEvent &e);
        void OnAddValuation(wxCommandEvent &e);
        void OnClose(wxCloseEvent &e);
        void OnDistributionEdit(wxListEvent &e);
        void OnValuationEdit(wxListEvent &e);
        void OnAddPosition(wxCommandEvent &e);
        void UpdateExecuteDistributionButton();
        void AllowBindingOnValuationLC();
        //for valuation+deployed capital chart
        Chart* PopulateDrawChartValuationDeploy();
        void UpdateChartValuationDeploy();
        //for distributions chart
        Chart* PopulateDrawChartDistribution();
        void UpdateChartDistribution();
        void OnExecuteDistribution(wxCommandEvent &e);
        void OnTransactionClick(wxCommandEvent &e);
};

#endif
