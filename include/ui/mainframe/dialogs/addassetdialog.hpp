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
class Asset;

class AddAssetDialog: public wxDialog{
    public:
        AddAssetDialog(wxWindow *parentWindow);
        void SetupLayout();
        wxString GetAssetName();
        wxString GetAssetSponser();
        wxDateTime GetExitDate();
    private:
        wxStaticText *inputAssetNameText;//
        wxTextCtrl *inputAssetNameCtrl;//
        wxStaticText *inputAssetSponserText;//
        wxTextCtrl *inputAssetSponserCtrl;//
        wxDatePickerCtrl * inputAssetExitDateCtrl;
        wxStaticText *inputAssetExitDateText;

        wxButton *confirmAssetButton;
        wxButton *cancelAssetButton;
};


#endif