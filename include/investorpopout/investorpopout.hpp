#ifndef INVESTORPOPOUT_HPP
#define INVESTORPOPOUT_HPP
#include "wx/frame.h"
#include "helpers/vlistcontrol.hpp"
#include "baseclasses/portfolio.hpp"
#include "baseclasses/investor.hpp"
class InvestorPopout : public wxFrame{
    public:
        InvestorPopout(wxWindow *parentWindow, const wxString &title, const wxPoint &pos, const wxSize &size,
        Portfolio &port, std::shared_ptr<Investor> investor):
        wxFrame(parentWindow, wxID_ANY, title, pos, size){};
        Portfolio &portfolio;
        std::shared_ptr<Investor> investor;
    private:
        //VListControl<std::shared_ptr<>> 
}

#endif

