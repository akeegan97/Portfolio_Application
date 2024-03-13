#ifndef ADDCOMPONENTPOSITIONDIALOG_HPP
#define ADDCOMPONENTPOSITIONDIALOG_HPP
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
#include <utility>
#include "models/entities/portfolio.hpp"

class AddComponentPositionDialog : public wxDialog{
    public:
        AddComponentPositionDialog(wxWindow *parentWindow, std::shared_ptr<Asset> asset, Portfolio &portfolio);
        void SetupLayout();
        wxDateTime GetDateValue();
        std::map<size_t, wxTextCtrl*> GetAllocations();
        void OnXClick(wxCloseEvent &e);
    private:
        Portfolio &m_portfolio;
        std::shared_ptr<Asset> m_asset;
        std::map<size_t, wxTextCtrl*> allocationInputs;
        wxDatePickerCtrl *dateInvested;
        wxStaticText* dateInvestedText;
        wxButton *confirmButton;
        wxButton *cancelButton;

};



#endif