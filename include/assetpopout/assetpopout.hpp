#ifndef ASSETPOPOUT_HPP
#define ASSETPOPOUT_HPP
#include <wx/wx.h>
#include "helpers/vlistcontrol.hpp"
#include "baseclasses/portfolio.hpp"
#include "baseclasses/asset.hpp"

class AssetPopout : public wxFrame{
    public:
        AssetPopout(wxWindow *parentWindow, const wxString &title, const wxPoint &pos, const wxSize &size, Portfolio &port, std::shared_ptr<Asset> asset)
        : wxFrame(parentWindow, wxID_ANY, title, pos, size),
            portfolio(port),
            asset(asset),
            investorPositionDisplayVirtualListControl(nullptr),
            valuationListControl(nullptr){
                setupLayout();
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
        void setupLayout();
        void UpdateDisplayTextValues();
        void OnInvestorPositionClick(wxListEvent &e);
        void OnAddDistributionClicked(wxCommandEvent &e);
        void OnDeployMovement(wxCommandEvent &e);
        void OnAddValuation(wxCommandEvent &e);
        void OnAddEvent(wxCommandEvent &e);
        void OnClose(wxCloseEvent &e);
        void OnDistributionEdit(wxListEvent &e);
        void OnValuationEdit(wxListEvent &e);
        void OnEventEdit(wxListEvent &e);
};


#endif
