#include "ui/transactionpopout/transactionpopout.hpp"


TransactionPopout::TransactionPopout(wxWindow* parentWindow,const wxString &title,const wxPoint &pos, const wxSize &size,std::shared_ptr<Asset> &asset)
            :wxFrame(NULL,wxID_ANY,title,pos,size),
            m_asset(asset){
                SetupLayout();
                wxFont font = wxFont(12, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
                wxColour color = wxColor(255,255,255);
                wxColour foregroundcolor = wxColor(0,0,0);
                #ifdef __WXMSW__
                utilities::SetBackgroundColorForWindowAndChildren(this, color, foregroundcolor);
                utilities::SetFontForWindowAndChildren(this, font);
                #endif
            };


void TransactionPopout::SetupLayout(){
    auto mainSizer = new wxBoxSizer(wxVERTICAL);
    auto topSizer = new wxBoxSizer(wxHORIZONTAL);
    auto lSideSizer = new wxBoxSizer(wxVERTICAL);
    allTransactionVListControl = new VListControl<Transaction>(this,wxID_ANY,wxDefaultPosition,wxDefaultSize);
    if(!m_asset->GetTransactions().empty()){
        allTransactionVListControl->setItems(m_asset->GetTransactions());
    }
    writeToCsv = new wxButton(this,wxID_ANY,"Write To CSV");
    writeToCsv->Bind(wxEVT_BUTTON,&TransactionPopout::ExecuteFileWrite,this);
    lSideSizer->Add(allTransactionVListControl,1,wxEXPAND|wxALL,5);
    lSideSizer->Add(writeToCsv,1,wxALL|wxEXPAND,5);
    topSizer->Add(lSideSizer,1,wxEXPAND|wxALL,5);
    mainSizer->Add(topSizer,1,wxALL|wxEXPAND,5);
    #ifdef __WXMAC__
    wxFont font = wxFont(14, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
    wxColour bgColor = wxColor(255,255,255);
    wxColour fgColor = wxColor(0,0,0);  
    this->SetBackgroundColour(bgColor);
    allTransactionVListControl->SetFont(font);
    allTransactionVListControl->SetBackgroundColour(bgColor);
    allTransactionVListControl->SetForegroundColour(fgColor);
    writeToCsv->SetForegroundColour(fgColor);
    writeToCsv->SetBackgroundColour(bgColor);
    writeToCsv->SetFont(font);
    #endif
    this->SetSizer(mainSizer);
    this->Layout();
}

void TransactionPopout::ExecuteFileWrite(wxCommandEvent &e){
    m_asset->WriteCSV();
}