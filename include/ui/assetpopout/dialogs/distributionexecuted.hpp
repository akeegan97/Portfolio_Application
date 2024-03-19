#ifndef DISTRIBUTIONEXECUTED_HPP
#define DISTRIBUTIONEXECUTED_HPP
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
    private:
        std::shared_ptr<Asset> m_asset;
        wxChoice *yearChoice;
        wxChoice *qChoice;
        wxStaticText *yearText;
        wxStaticText *qText;
        wxButton *getDistribution;
        wxStaticText *distributionAmountText;
        wxTextCtrl *distributionAmountTextCtrl;
        wxStaticText *reserveAmountText;
        wxTextCtrl *reserveAmountCtrl;
        wxButton *confirmButton;
        wxButton *cancelButton;
};




#endif