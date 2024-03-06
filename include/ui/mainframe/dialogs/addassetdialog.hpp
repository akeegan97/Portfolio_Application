#ifndef ADDASSETDIALOG_HPP
#define ADDASSETDIALOG_HPP
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
#include "models/entities/asset.hpp"

class Portfolio;

class AddAssetDialog: public wxDialog{
    public:
        AddAssetDialog(wxWindow *parentWindow, Portfolio &portfolio);
        void SetupLayout();
        wxString GetAssetName();
        wxString GetAssetSponser();
        wxDateTime GetExitDate();
        wxDateTime GetEffectiveDate();
        double GetPaidAmount();
        double GetDeployedAmount();
        double GetReserveAmount();
        std::string GetInvestorChoiceName();
        void OnAddInvestor(wxCommandEvent &e);
        void UpdateInvestorChoice();
        
    private:
        Portfolio &m_portfolio;
        wxStaticText *assetNameCtrlText;
        wxTextCtrl *assetNameCtrl;
        wxStaticText *assetSponserCtrlText;
        wxTextCtrl *assetSponserCtrl;
        wxDatePickerCtrl * assetExitDateCtrl;
        wxStaticText *assetExitDateText;
        wxStaticText *investorChoiceText;
        wxChoice *investorChoiceCtrl;
        wxStaticText* paidAmountText;
        wxTextCtrl *paidAmountTextCtrl;
        wxTextCtrl *deployedAmountTextCtrl;
        wxStaticText *deployedAmountText;
        wxStaticText *reserveAmountText;
        wxTextCtrl *reserveAmountTextCtrl;
        wxStaticText *effectiveStartDateCtrlText;
        wxDatePickerCtrl *effectiveStartDateCtrl;

        wxButton *confirmButton;
        wxButton *cancelButton;
        wxButton *createNewInvestorButton;
};


#endif