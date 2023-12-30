#ifndef ASSETPOPOUT_HPP
#define ASSETPOPOUT_HPP
#include <wx/wx.h>
#include "vlistcontrol.hpp"
#include "portfolio.hpp"
#include "asset.hpp"

class AssetPopout : public wxFrame{
    public:
        AssetPopout(const wxString &title, const wxPoint &pos, const wxSize &size, Portfolio &port, std::shared_ptr<Asset> asset)
        : wxFrame(NULL, wxID_ANY, title, pos, size),
            portfolio(port),
            asset(asset),
            investorPositionDisplayVirtualListControl(nullptr),
            valuationListControl(nullptr){
                setupLayout();
                UpdateDisplayTextValues();
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
        void setupLayout();
        void UpdateDisplayTextValues();
        void OnInvestorPositionClick(wxListEvent &e);
        void OnAddDistributionClicked(wxCommandEvent &e);
        
};


#endif
