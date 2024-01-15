#ifndef EVENTDIALOG_HPP
#define EVENTDIALOG_HPP
#define DELETE_CODE_EVENT 1002
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

class EventDialog : public wxDialog{
    public:
        EventDialog(wxWindow *parentWindow, bool isEditMode);
        wxDateTime GetDate();
        wxString GetDescription();
        bool GetHasHappened();
        wxString GetHasHappenedTextFromChoiceCtrl();
        void SetUpEditMode();
        void SetUpAddMode();
        void OnDeleteEventButtonPress(wxCommandEvent &e);
    private:
        wxDatePickerCtrl* datePicker;
        wxStaticText* datePickerText;
        wxStaticText* discriptionText;
        wxTextCtrl* discriptionCtrl;
        wxStaticText* hasHappenedChoiceText;
        wxChoice* hasHappenedChoiceCtrl;
};

#endif
