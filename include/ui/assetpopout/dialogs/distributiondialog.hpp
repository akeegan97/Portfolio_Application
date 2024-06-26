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
        DistributionDialog(wxWindow *parentWindow, bool isEditMode, wxDateTime &setDate, double &setAmount);
        wxDateTime GetDistributionDate();
        double GetDistributionAmount();
        void SetUpOnAddDistribution();
        void SetUpOnEditDistribution(wxDateTime &setDate, double &setAmount);
        void OnDeleteButtonPress(wxCommandEvent &e);
        std::string GetNote();
    private:
        wxDatePickerCtrl* datePicker;
        wxStaticText* datePickerText;
        wxTextCtrl* amountCtrl;
        wxStaticText* amountText;
        wxTextCtrl* noteForTransactionTextCtrl;
        wxStaticText* noteForTransactionText;
};


#endif