#ifndef EXECUTEDISTRIBUTION_HPP
#define EXECUTEDISTRIBUTION_HPP
#include <optional>
#include <wx/dialog.h>
#include <wx/datetime.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/arrstr.h>
#include <wx/stattext.h>
#include <wx/valtext.h>
#include "models/entities/asset.hpp"

class Asset;
class ExecuteDistribution : public wxDialog{
    public:
        ExecuteDistribution(wxWindow *parentWindow, std::shared_ptr<Asset> &asset);
        void SetupLayout();
        double GetAmountOfDistribution(wxDateTime &date);
        wxDateTime GetDate();
        void UpdateAmountText(double amount);
        void OnGetAmount(wxCommandEvent &e);
        void EnableGetAmountButton();
        void EnableConfirmButton();
        double GetDistributionAmount();
        double GetReserveAmount();
        void OnSelectedDate(wxDateEvent &e);
        void OnAmountsChanged(wxCommandEvent &e);
        std::string GetNote();
    private:
        std::shared_ptr<Asset> m_asset;
        wxStaticText *getAmountText;
        wxButton *getAmount;
        wxStaticText *amountOfDistributionText;
        wxDatePickerCtrl *datePicker;
        wxStaticText *datePickerText;
        wxStaticText *amountToDistributeText;
        wxTextCtrl *amountToDistributeTextCtrl;
        wxStaticText *amountToReserveText;
        wxTextCtrl *amountToReserveTextCtrl;
        wxStaticText *noteText;
        wxTextCtrl *noteTextCrtl;
        wxButton *confirmButton;
        wxButton *cancelButton;
};

#endif