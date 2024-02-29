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
        void SetupLayout();
        void SetupLayoutStandalone();
        void SetupLayoutComponent();
        void SetupLayoutHybrid();

        void OnConfirmPositionType(wxCommandEvent &e);
        void OnConfirmAssociatedInvestor(wxCommandEvent &e);
        void AddInvestor(wxCommandEvent &e);
    private:
        wxDatePickerCtrl* datePicker;
        wxStaticText* datePickerText;
        wxTextCtrl* newCapitalCtrl;
        wxStaticText* newCapitalTextCtrl;
        wxTextCtrl* donorCapitalText;
        wxButton* lockInChoiceButton;
        wxButton* lockInInvestorButton;
        wxButton* addNewInvestorButton;
        wxChoice *typeOfNewPositionChoice;
        wxChoice *associatedInvestor;
        std::string m_associatedInvestorName;
        std::string m_typeOfPosition;
        Portfolio& m_portfolio;
};




#endif