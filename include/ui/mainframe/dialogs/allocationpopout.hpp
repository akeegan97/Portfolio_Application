#ifndef ALLOCATIONPOPOUT_HPP
#define ALLOCATIONPOPOUT_HPP
//Popout for allocation sheet
#include <wx/wx.h>
#include "helpers/utilities.hpp"
#include "helpers/vlistcontrol.hpp"
#include "models/entities/investor.hpp"
#include "models/entities/portfolio.hpp"
#include "models/components/allocation.hpp"

class Portfolio;
class Allocation;

class AllocationPopout: public wxFrame{
    public:
        AllocationPopout(wxWindow *parentWindow, const wxString &title, const wxPoint &pos,const wxSize &size, Portfolio &portfolio, wxDateTime &qEndDate);
        void SetAllocations();
    private:
        wxDateTime m_qEndDate;
        Portfolio &m_Portfolio;
        std::vector<Allocation> m_Allocations;
        VListControl<Allocation>* m_AllocationVLC;
        wxButton *writeToCsv;
        void ExecuteFileWrite(wxCommandEvent &e);
        void SetupLayout();
};

#endif