#ifndef ADDPOSITIONDIALOG_HPP
#define ADDPOSITIONDIALOG_HPP
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
#include "ui/assetpopout/dialogs/addstandalonepositiondialog.hpp"
#include "ui/assetpopout/dialogs/addcomponentpositiondialog.hpp"
class Portfolio;
class AddPositionDialog : public wxDialog{
    public:
        AddPositionDialog(wxWindow *parentWindow, Portfolio &portfolio, std::shared_ptr<Asset> asset);
        void SetupLayout();

        std::string& GetAssociatedInvestor();
        std::string& GetTypeOfNewPosition();
        
        void AddInvestor(wxCommandEvent &e);
        void UpdateInvestorChoice();
        void OnConfirmPosition(wxCommandEvent &e);
        void SetTypeOfPosition();
        void SetInvestorName();

    private:
        Portfolio& m_portfolio;
        std::shared_ptr<Asset> &m_asset;
        std::string m_associatedInvestor;
        std::string m_typeOfNewPosition;

        wxChoice* typeOfNewPositionChoice;
        wxChoice* associatedInvestorChoice;
        wxButton* addNewInvestorButton;
        wxButton* confirmSelectionsLaunchButton;
};




#endif