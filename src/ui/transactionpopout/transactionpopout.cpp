#include "ui/transactionpopout/transactionpopout.hpp"


TransactionPopout::TransactionPopout(wxWindow* parentWindow,const wxString &title,const wxPoint &pos, const wxSize &size,std::shared_ptr<Asset> &asset)
            :wxFrame(NULL,wxID_ANY,title,pos,size),
            m_asset(asset){
                SetupLayout();
            };


void TransactionPopout::SetupLayout(){
    auto mainSizer = new wxBoxSizer(wxVERTICAL);
    auto topSizer = new wxBoxSizer(wxHORIZONTAL);
    auto lSideSizer = new wxBoxSizer(wxVERTICAL);
    allTransactionVListControl = new VListControl<Transaction>(this,wxID_ANY,wxDefaultPosition,wxDefaultSize);
    if(!m_asset->GetTransactions().empty()){
        allTransactionVListControl->setItems(m_asset->GetTransactions());
    }
    lSideSizer->Add(allTransactionVListControl,2,wxEXPAND|wxALL,5);
    topSizer->Add(lSideSizer,1,wxEXPAND|wxALL,5);
    mainSizer->Add(topSizer,1,wxALL|wxEXPAND,5);
    this->SetSizer(mainSizer);
    this->Layout();
}
