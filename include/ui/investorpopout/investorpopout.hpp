#ifndef INVESTORPOPOUT_HPP
#define INVESTORPOPOUT_HPP
#include "wx/frame.h"
#include <wx/stattext.h>
#include <wx/notebook.h>
#include "helpers/vlistcontrol.hpp"
#include "models/entities/portfolio.hpp"
#include "models/entities/asset.hpp"
#include "models/components/investorassetdisplay.hpp"
#include <wx/chart.h>
#include <wx/xy/xyplot.h>
#include <wx/bars/barplot.h>
#include <wx/category/categorysimpledataset.h>
#include <wx/category/categoryrenderer.h>
#include <wx/axis/categoryaxis.h>
#include <wx/xy/timeseriesdataset.h>
#include <wx/chartpanel.h>
#include <wx/aui/tabart.h>
#include <wx/aui/auibook.h>
#include "helpers/utilities.hpp"
#include <wx/button.h>
#include <wx/datectrl.h>
#include "models/supporting/statement.hpp"

struct Details;
class Statement;
class InvestorPopout : public wxFrame{
    public:
        InvestorPopout(wxWindow *parentWindow, const wxString &title, const wxPoint &pos, const wxSize &size,
        Portfolio &port, std::shared_ptr<Investor> investor);
        Portfolio &portfolio;
        std::shared_ptr<Investor> investor;
    private:
        VListControl<std::shared_ptr<InvestorAssetDisplay>> *investorAssetDisplayVirtualListControl;
        wxAuiNotebook* distributionsByAssetNoteBook;
        wxStaticText *totalPaid;
        void SetupLayout();
        void WriteStatementDetails();
        Chart* PopulateInvestorsValuationDeployChart();
        Chart* PopulateInvestorsDistributionChart();
        void UpdateInvestorsValuationDeployChart();
        void UpdateInvestorsDistributionChart();
        void OnMakeStatementClick(wxCommandEvent &e);
        void OnWriteToCsvClick(wxCommandEvent &e);
        wxButton *DisplayStatement;
        wxButton *WriteToCsv;
        wxDatePickerCtrl* startDate;
        wxDatePickerCtrl* endDate;
        VListControl<Details> *itdDetailsVLC;

        wxStaticText *beginningBalance;
        wxStaticText *additionalCapital;
        wxStaticText *returnedCapital;
        wxStaticText *endingBalance;

        wxStaticText *distributionsThisperiod;
        wxStaticText *changeInValuationthisPeriod;
        wxStaticText *returnAmountThisPeriod;
        wxStaticText *returnPercentThisPeriod;

        wxStaticText *paidCapital;
        wxStaticText *returnedPrincipal;
        wxStaticText *endingBalanceITD;
        wxStaticText *totalDistributions;
        wxStaticText *totalGain;
        wxStaticText *irr;
        Statement m_clientStatement;
};

#endif

