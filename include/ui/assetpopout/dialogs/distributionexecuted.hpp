#ifndef DISTRIBUTIONEXECUTED_HPP
#define DISTRIBUTIONEXECUTED_HPP
#include <optional>
#include <wx/dialog.h>
#include <wx/datetime.h>
#include <wx/datectrl.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/valtext.h>
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/arrstr.h>
#include <wx/stattext.h>
#include "models/entities/asset.hpp"


class DistributionExecution : public wxDialog{
    public:
        DistributionExecution(wxWindow *parentWindow, std::shared_ptr<Asset> &asset);
        void SetupLayout();
        void PopulateChoiceArrays();
        void PopulateQDistributions();
        std::optional<Distribution> GetSelectedDistribution(const int &selectedYear, const wxString &selectedQ);
        void OnGetAmount(wxCommandEvent &e);
        Distribution GetDistribution();
        double GetDistributeAmount();
        double GetReserveAmount();
        wxDateTime GetDateOfDistribution();
        void OnAmountsChanged(wxCommandEvent &e);
        void UpdateConfirmButton();
        void OnSelection(wxCommandEvent &e);
        void UpdateGetAmountButton();
    private:
        std::shared_ptr<Asset> m_asset;
        std::vector<Distribution> qDistributions;
        wxChoice *yearChoice;
        wxArrayString yearChoices;
        wxArrayString qChoices;
        wxChoice *qChoice;
        wxStaticText *yearText;
        wxStaticText *qText;
        wxButton *getDistribution;
        wxStaticText *selectedDistributionAmount;
        wxStaticText *distributionAmountText;
        wxTextCtrl *distributionAmountTextCtrl;
        wxStaticText *reserveAmountText;
        wxTextCtrl *reserveAmountCtrl;
        wxButton *confirmButton;
        wxButton *cancelButton;
        std::set<int> yearsInt;
        std::set<int> monthsInt;
        Distribution selectedDistribution;
};




#endif