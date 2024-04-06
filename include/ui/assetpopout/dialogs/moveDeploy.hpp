#ifndef MOVEDEPLOY_HPP
#define MOVEDEPLOY_HPP
#include "wx/dialog.h"
#include "wx/textctrl.h"
#include "wx/valtext.h"
#include "wx/datetime.h"
#include "wx/datectrl.h"
#include "wx/sizer.h"
#include "wx/stattext.h"
#include "wx/button.h"
#include "wx/choice.h"
#include "wx/arrstr.h"
#include "models/entities/asset.hpp"
class Asset;


class MoveDeploy : public wxDialog{
    public:
        MoveDeploy(wxWindow *parentWindow,std::shared_ptr<Asset> &asset);
        wxDateTime GetDate();
        double GetAmountMoved();
        wxString GetSelectedMovementDirection();
        void OnAmountsChanged(wxCommandEvent&e);
        void UpdateConfirmButton();
        void SetupLayout();
        std::string GetNote();

    private:
        wxDatePickerCtrl* datePicker;
        wxStaticText* datePickerText;
        wxTextCtrl* amountCtrl;
        wxStaticText* amountText;
        wxChoice *directionOfMovement;
        wxStaticText *directionOfMovementText;
        std::shared_ptr<Asset> m_asset;
        wxButton* okayButton;
        wxTextCtrl* noteForTransactionTextCtrl;
        wxStaticText* noteForTransactionText;
};

#endif
