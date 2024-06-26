#ifndef VALUATIONDIALOG_HPP
#define VALUATIONDIALOG_HPP
#define MY_VALUATION_DELETE_CODE 1002
#include "wx/dialog.h"
#include "wx/textctrl.h"
#include "wx/valtext.h"
#include "wx/datetime.h"
#include "wx/datectrl.h"
#include "wx/sizer.h"
#include "wx/stattext.h"
#include "wx/button.h"
#include "wx/arrstr.h"

class ValuationDialog : public wxDialog{
    public:
        ValuationDialog(wxWindow *parentWindow, bool editMode, wxDateTime &setDate, double &setValue);
        wxDateTime GetDate();
        double GetValuation();
        void SetUpOnAdd();
        void SetUpOnEdit(wxDateTime &setDate, double &setValue);
        void OnDeleteButtonPress(wxCommandEvent &e);
        std::string GetNote();
    private:
        wxDatePickerCtrl *datePicker;
        wxStaticText *datePickerText;
        wxTextCtrl *valuationCtrl;
        wxStaticText *valuationText;
        wxStaticText *noteForTransactionText;
        wxTextCtrl *noteForTransactionTextCtrl;
};




#endif
