#ifndef ADDDISTRIBUTION_HPP
#define ADDDISTRIBUTION_HPP
#include "wx/dialog.h"
#include "wx/datetime.h"
#include "wx/datectrl.h"
#include "wx/textctrl.h"
#include "wx/sizer.h"
#include "wx/stattext.h"
#include "wx/valtext.h"
#include "wx/button.h"


class AddDistributionDialog : public wxDialog{
    public:
        AddDistributionDialog(wxWindow *parentWindow);
        wxDateTime GetDistributionDate();
        double GetDistributionAmount();
    private:
        wxDatePickerCtrl* datePicker;
        wxStaticText* datePickerText;
        wxTextCtrl* amountCtrl;
        wxStaticText* amountText;
};



#endif