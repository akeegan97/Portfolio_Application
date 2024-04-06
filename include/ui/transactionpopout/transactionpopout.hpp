#ifndef TRANSACTIONPOPOUT_HPP
#define TRANSACTIONPOPOUT_HPP
#include <wx/wx.h>
#include "helpers/utilities.hpp"
#include "helpers/vlistcontrol.hpp"
#include "models/entities/asset.hpp"
#include "models/entities/position.hpp"

class TransactionPopout : public wxFrame{
    public:
        TransactionPopout(wxWindow *parentWindow,const wxString &title,const wxPoint &pos, const wxSize &size,std::shared_ptr<Asset> &asset);
        std::shared_ptr<Asset> m_asset;
    private:
        VListControl<Transaction>* allTransactionVListControl;
        void SetupLayout();
};


#endif
