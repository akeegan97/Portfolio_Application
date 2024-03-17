#ifndef ADDSTANDALONEPOSITIONDIALOG_HPP
#define ADDSTANDALONEPOSITIONDIALOG_HPP
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
#include "ui/assetpopout/dialogs/addinvestordialog.hpp"
class Portfolio;

class AddStandalonePositionDialog : public wxDialog{
    public:
        AddStandalonePositionDialog(wxWindow *parentWindow, Portfolio&portfolio);
        void SetupLayout();
        wxDateTime GetDateValue();
        double GetPaidAmount();
        double GetDeployedAmount();
        double GetReserveAmount();
    private:
        Portfolio &m_portfolio;
        wxStaticText* amountIncreaseCapitalText;
        wxTextCtrl* amountIncreaseCapitalTextCtrl;
        wxStaticText *amountDeployText;
        wxTextCtrl *amountDeployTextCtrl;
        wxStaticText *amountReserveText;
        wxTextCtrl *amountReserveTextCtrl;
        wxStaticText* dateNewPositionText;
        wxDatePickerCtrl* dateNewPositionCtrl;
        wxButton* confirmButton;
};


#endif