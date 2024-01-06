#ifndef INVESTORPOSITIONEDIT_HPP
#define INVESTORPOSITIONEDIT_HPP
#include "wx/dialog.h"
#include "wx/datetime.h"
#include "wx/datectrl.h"
#include "wx/textctrl.h"
#include "wx/sizer.h"
#include "wx/stattext.h"
#include "wx/valtext.h"
#include "wx/button.h"



class InvestorPositionEditWindow : public wxDialog{
    public:
        InvestorPositionEditWindow(wxWindow* parentWindow);
        wxString GetClientName();
        wxString GetClientType();
        wxDateTime GetInvestmentDate();
        double GetSubscribed();
        double GetPaid();
        double GetDeployed();
        double GetReserve();
        double GetReturnOfCapital();
        void OnDeletePositionButton(wxCommandEvent& e);//debating on allowing position deletes at this stage
    private:
        wxDatePickerCtrl * datePicker;
        wxStaticText* datePickerText;
        wxTextCtrl* clientNameCtrl;
        wxStaticText* clientNameText;
        wxTextCtrl* clientTypeCtrl;
        wxStaticText* clientTypeText;
        wxTextCtrl* clientSubscribedCtrl;
        wxStaticText*clientSubscribedText;
        wxTextCtrl* clientPaidCtrl;
        wxStaticText*clientPaidText;
        wxTextCtrl*clientDeployedCtrl;
        wxStaticText*clientDeployedText;
        wxTextCtrl*clientReserveCtrl;
        wxStaticText*clientReserveText;
        wxTextCtrl*clientReturnOfCapitalCtrl;
        wxStaticText*clientReturnOfCapitalText;
};

#endif