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
#include "ui/assetpopout/dialogs/addinvestordialog.hpp"
class Portfolio;
class AddPositionDialog : public wxDialog{
    public:
        AddPositionDialog(wxWindow *parentWindow, Portfolio &portfolio);
        void SetupLayout();

        std::string& GetAssociatedInvestor();
        std::string& GetTypeOfNewPosition();
        
        void AddInvestor(wxCommandEvent &e);
        void UpdateInvestorChoice();
    private:
        Portfolio& m_portfolio;
        std::string m_associatedInvestor;
        std::string m_typeOfNewPosition;

        wxChoice* typeOfNewPositionChoice;
        wxChoice* associatedInvestorChoice;
        wxButton* addNewInvestorButton;
        wxButton* confirmSelectionsLaunchButton;
};




#endif