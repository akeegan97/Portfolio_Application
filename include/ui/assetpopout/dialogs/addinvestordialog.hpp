#ifndef ADDINVESTORDIALOG_HPP
#define ADDINVESTORDIALOG_HPP
#include <wx/dialog.h>
#include <wx/datetime.h>
#include <wx/datectrl.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/valtext.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/arrstr.h>
#include "models/entities/portfolio.hpp"
class Portfolio;

class AddInvestorDialog: public wxDialog{
    public:
        AddInvestorDialog(wxWindow *parentWindow);
        void SetupLayout();
        wxString GetInvestorName();
        wxString GetInvestorType();
        double GetInvestorMgmtFee();
        double GetInvestorPromoteFee();
    private:
        wxStaticText* inputNameText;
        wxTextCtrl* inputNameTextCtrl;
        wxStaticText* inputTypeText;
        wxTextCtrl* inputTypeTextCtrl;
        wxStaticText* inputMgmtFeeText;
        wxTextCtrl* inputMgmtFeeTextCtrl;
        wxStaticText* inputPromoteFeeText;
        wxTextCtrl* inputPromoteFeeTextCtrl;

        wxButton* confirmNewInvestorButton;
};


#endif