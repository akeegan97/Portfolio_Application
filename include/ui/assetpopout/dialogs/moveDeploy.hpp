#ifndef MOVEDEPLOY_HPP
#define MOVEDEPLOY_HPP
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


class MoveDeploy : public wxDialog{
    public:
        MoveDeploy(wxWindow *parentWindow);
        wxDateTime GetDate();
        double GetAmountMoved();
        wxString GetSelectedMovementDirection();

    private:
        wxDatePickerCtrl* datePicker;
        wxStaticText* datePickerText;
        wxTextCtrl* amountCtrl;
        wxStaticText* amountText;
        wxChoice *directionOfMovement;
        wxStaticText *directionOfMovementText;
};

#endif
