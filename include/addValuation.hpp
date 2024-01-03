#ifndef ADDVALUATION_HPP
#define ADDVALUATION_HPP
#include "wx/dialog.h"
#include "wx/textctrl.h"
#include "wx/valtext.h"
#include "wx/datetime.h"
#include "wx/datectrl.h"
#include "wx/sizer.h"
#include "wx/stattext.h"
#include "wx/button.h"
#include "wx/arrstr.h"

class AddValuation : public wxDialog{
    public:
        AddValuation(wxWindow *parentWindow);
        wxDateTime GetDate();
        double GetValuation();
    private:
        wxDatePickerCtrl *datePicker;
        wxStaticText *datePickerText;
        wxTextCtrl *valuationCtrl;
        wxStaticText *valuationText;
};




#endif
