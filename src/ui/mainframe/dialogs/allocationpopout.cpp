#include "ui/mainframe/dialogs/allocationpopout.hpp"

AllocationPopout::AllocationPopout(wxWindow *parentWindow, const wxString &title, const wxPoint &pos,const wxSize &size,Portfolio &portfolio, wxDateTime &qEndDate):
    wxFrame(parentWindow, wxID_ANY,title, pos, size),m_Portfolio(portfolio),m_qEndDate(qEndDate){
        SetAllocations();
        SetupLayout();
        wxFont font = wxFont(14, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
        wxColour color = wxColor(255,255,255);
        wxColour foregroundcolor = wxColor(0,0,0);
        #ifdef __WXMSW__
        utilities::SetBackgroundColorForWindowAndChildren(this, color, foregroundcolor);
        utilities::SetFontForWindowAndChildren(this, font);
        #elif defined(__WXMAC__)
        utilities::SetFontForWindowAndChildren(this, font);
        #endif
    }

void AllocationPopout::SetupLayout(){
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText *headerText = new wxStaticText(this, wxID_ANY, wxString("ALLOCATION SHEET: "+m_qEndDate.FormatISODate()));
    mainSizer->Add(headerText, 1, wxALIGN_CENTER | wxALL, 5);
    m_AllocationVLC = new VListControl<Allocation>(this, wxID_ANY,wxDefaultPosition, wxDefaultSize);
    if(!m_Allocations.empty()){
        m_AllocationVLC->setItems(m_Allocations);
    }
    mainSizer->Add(m_AllocationVLC,1, wxEXPAND|wxALL,5);

    writeToCsv = new wxButton(this, wxID_ANY, "Write To CSV ../Documents/");
    mainSizer->Add(writeToCsv,1,wxALL|wxEXPAND,5);
    this->SetSizer(mainSizer);
    this->Layout();
    
}

void AllocationPopout::SetAllocations(){
    for(const auto& investor : m_Portfolio.GetInvestors()){
        Allocation newAllocation(investor,m_qEndDate);
        m_Allocations.push_back(newAllocation);
    }
}