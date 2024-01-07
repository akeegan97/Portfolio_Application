#ifndef ADD_EVENT_HPP
#define ADD_EVENT_HPP
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

class AddEvent : public wxDialog{
    public:
        AddEvent(wxWindow *parentWindow);
        wxDateTime GetDate();
        wxString GetDiscription();
        bool GetHasHappened();
        wxString GetHasHappenedTextFromChoiceCtrl();
    private:
        wxDatePickerCtrl* datePicker;
        wxStaticText* datePickerText;
        wxStaticText* discriptionText;
        wxTextCtrl* discriptionCtrl;
        wxStaticText* hasHappenedChoiceText;
        wxChoice* hasHappenedChoiceCtrl;
};

#endif
