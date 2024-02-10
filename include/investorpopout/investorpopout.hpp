#ifndef INVESTORPOPOUT_HPP
#define INVESTORPOPOUT_HPP
#include "wx/frame.h"
#include <wx/stattext.h>
#include <wx/notebook.h>
#include "helpers/vlistcontrol.hpp"
#include "baseclasses/portfolio.hpp"
#include "baseclasses/investor.hpp"
#include "baseclasses/investorassetdisplay.hpp"
#include <wx/chart.h>
#include <wx/xy/xyplot.h>
#include <wx/bars/barplot.h>
#include <wx/category/categorysimpledataset.h>
#include <wx/category/categoryrenderer.h>
#include <wx/axis/categoryaxis.h>
#include <wx/xy/timeseriesdataset.h>
#include <wx/chartpanel.h>
#include "helpers/utilities.hpp"

class InvestorPopout : public wxFrame{
    public:
        InvestorPopout(wxWindow *parentWindow, const wxString &title, const wxPoint &pos, const wxSize &size,
        Portfolio &port, std::shared_ptr<Investor> investor):
        wxFrame(parentWindow, wxID_ANY, title, pos, size),
        portfolio(port),
        investor(investor){
            SetUpLayout();
        };
        Portfolio &portfolio;
        std::shared_ptr<Investor> investor;
    private:
        VListControl<std::shared_ptr<InvestorAssetDisplay>> *investorAssetDisplayVirtualListControl;
        wxNotebook* distributionsByAssetNoteBook;
        wxStaticText *totalPaid;
        void SetUpLayout();
        Chart* PopulateInvestorsValuationDeployChart();
        Chart* PopulateInvestorsDistributionChart();
        void UpdateInvestorsValuationDeployChart();
        void UpdateInvestorsDistributionChart();
        
};

#endif

