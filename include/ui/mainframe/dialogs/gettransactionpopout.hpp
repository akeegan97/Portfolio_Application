#ifndef GETTRANSACTIONPOPOUT_HPP
#define GETTRANSACTIONPOPOUT_HPP
#include <wx/frame.h>
#include <wx/datetime.h>
#include <wx/datectrl.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include "helpers/vlistcontrol.hpp"
#include "models/supporting/transaction.hpp"
class Transaction;
class Portfolio;

class GetTransactionPopout : public wxFrame{
    public:
        GetTransactionPopout(wxWindow* parentWindow,const wxString &title,const wxPoint &pos, const wxSize &size, Portfolio &portfolio);
        wxDateTime GetStartDate();
        wxDateTime GetEndDate();
        void SetDisplayTransactions(wxDateTime&startDate, wxDateTime&endDate);
        void SetTransactions();
    private:
        VListControl<Transaction>* allAssetsTransactionsVLC;
        wxButton *writeToCsv;
        void ExecuteFileWrite(wxCommandEvent &e);
        void SetupLayout();
        wxDatePickerCtrl* startDate;
        wxDatePickerCtrl* endDate;
        wxButton* refreshVLCButton;
        void RefreshVLC(wxCommandEvent &e);
        Portfolio &m_portfolio;
        std::vector<Transaction> m_allTransactions;
        std::vector<Transaction> m_displayTransactions;

};

#endif
