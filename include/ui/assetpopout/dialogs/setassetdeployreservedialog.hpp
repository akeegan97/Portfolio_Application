#ifndef SETASSETDEPLOYRESERVEDIALOG_HPP
#define SETASSETDEPLOYRESERVEDIALOG_HPP

#include "wx/dialog.h"
#include "wx/textctrl.h"
#include "wx/valtext.h"
#include "wx/datetime.h"
#include "wx/datectrl.h"
#include "wx/sizer.h"
#include "wx/stattext.h"
#include "wx/button.h"
#include "wx/choice.h"
#include "wx/arrstr.h"


class SetAssetDeployReserveDialog : public wxDialog{
    public:
        SetAssetDeployReserveDialog(wxWindow *parentWindow);
        wxDateTime GetDate();
        double GetReserve();
        double GetDeploy();
    private:
        wxDatePickerCtrl* datePicker;
        wxStaticText* datePickerText;
        wxTextCtrl* deployAmountCtrl;
        wxStaticText* deployAmountCtrlText;
        wxTextCtrl* reserveAmountCtrl;
        wxStaticText* reserveAmountCtrlText;
};




#endif