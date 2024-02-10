#ifndef ADDPOSITION_HPP
#define ADDPOSITION_HPP
#define MY_POSITION_DELETE_CODE 1005
#include <wx/dialog.h>
#include <wx/datetime.h>
#include <wx/datectrl.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/valtext.h>
#include <wx/button.h>

class AddPositionDialog : public wxDialog{
    public:
        AddPositionDialog(wxWindow *parentWindow);
        wxDateTime GetInvestedDate();
        double GetSubscribedAmount();
        double GetPaidAmount();
        double GetDeployedAmount();
        double GetReserveAmount();
        double GetReturnOfCapital();
    private:
        wxDatePickerCtrl *datePicker;
        wxStaticText *datePickerText;
        wxTextCtrl *subscribedAmountCtrl;
        wxStaticText *subscribedAmountText;
        wxTextCtrl *paidAmountCtrl;
        wxStaticText *paidAmountText;
        wxTextCtrl *deployedAmountCtrl;
        wxStaticText *deployedAmountText;
        wxTextCtrl *reserveAmountCtrl;
        wxStaticText *reserveAmountText;
        wxTextCtrl *returnOfCapitalCtrl;
        wxStaticText *returnOfCapitalText;
};


#endif