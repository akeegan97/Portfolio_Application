#ifndef DISTRIBUTIONDIALOG_HPP
#define DISTRIBUTIONDIALOG_HPP
#define MY_CUSTOM_DELETE_CODE 1001
#include "wx/dialog.h"
#include "wx/datetime.h"
#include "wx/datectrl.h"
#include "wx/textctrl.h"
#include "wx/sizer.h"
#include "wx/stattext.h"
#include "wx/valtext.h"
#include "wx/button.h"


class DistributionDialog : public wxDialog{
    public:
        DistributionDialog(wxWindow *parentWindow, bool isEditMode);
        wxDateTime GetDistributionDate();
        double GetDistributionAmount();
        void SetUpOnAddDistribution();
        void SetUpOnEditDistribution();
        void OnDeleteButtonPress(wxCommandEvent &e);
    private:
        wxDatePickerCtrl* datePicker;
        wxStaticText* datePickerText;
        wxTextCtrl* amountCtrl;
        wxStaticText* amountText;
};


#endif