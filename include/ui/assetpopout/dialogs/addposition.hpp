#ifndef ADDPOSITION_HPP
#define ADDPOSITION_HPP
#define MY_POSITION_DELETE_CODE 1005
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
class AddPositionDialog : public wxDialog{
    public:
        AddPositionDialog(wxWindow *parentWindow, Portfolio &portfolio);
        wxDateTime GetInvestedDate();
        double GetSubscribedAmount();
        double GetPaidAmount();
        double GetDeployedAmount();
        double GetReserveAmount();
        double GetReturnOfCapital();
        void SetupLayout();
        void SetupLayoutStandalone();
        void SetupLayoutComponent();
        void SetupLayoutHybrid();
    private:
        wxDatePickerCtrl* datePicker;
        wxStaticText* datePickerText;
        wxTextCtrl* newCapitalCtrl;
        wxStaticText* newCapitalTextCtrl;
        wxTextCtrl* donorCapitalText;


        Portfolio& m_portfolio;
};


#endif