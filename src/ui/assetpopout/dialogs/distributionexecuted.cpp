#include "ui/assetpopout/dialogs/distributionexecuted.hpp"
#include "helpers/utilities.hpp"

DistributionExecution::DistributionExecution(wxWindow *parentWindow, std::shared_ptr<Asset> &asset):
    wxDialog(parentWindow, wxID_ANY, "Distribute", wxDefaultPosition, wxDefaultSize,wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
    m_asset(asset){
        SetupLayout();
    }

void DistributionExecution::SetupLayout(){
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topLeftSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topMiddleSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topRightSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    wxString allowableNumbers = "0123457689. ";
    wxTextValidator numberValidator(wxFILTER_INCLUDE_CHAR_LIST);
    numberValidator.SetIncludes(wxArrayString(1, &allowableNumbers));
    wxArrayString years;
    wxArrayString quarters;
    quarters.Add("3");
    quarters.Add("6");
    quarters.Add("9");
    quarters.Add("12");
    std::set<int> distributionYears;
    std::set<int> distributionQs;
    
    for(auto element: m_asset->GetQuarterDistributions()){
        distributionYears.insert(element.first.distribution.first.GetYear());
        wxDateTime qEnd = element.first.distribution.first;
        qEnd = utilities::GetQuarterEndDate(qEnd);
        distributionQs.insert(qEnd.GetMonth());
    }
    for(int year : distributionYears){
        years.Add(wxString::Format("%d", year));
    }
    yearChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, years);
    qChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, quarters);
    yearText = new wxStaticText(this, wxID_ANY,"Year");
    qText = new wxStaticText(this, wxID_ANY,"Q");
    getDistribution = new wxButton(this, wxID_ANY,"Get Amount");
    distributionAmountText = new wxStaticText(this, wxID_ANY,"Amount To Distribute");
    distributionAmountTextCtrl = new wxTextCtrl(this, wxID_ANY);
    distributionAmountTextCtrl->SetValidator(numberValidator);
    reserveAmountText = new wxStaticText(this, wxID_ANY, "Amount To Reserve");
    reserveAmountCtrl = new wxTextCtrl(this, wxID_ANY);
    reserveAmountCtrl->SetValidator(numberValidator);

    confirmButton = new wxButton(this, wxID_OK,"Confirm");
    cancelButton = new wxButton(this, wxID_CANCEL, "Cancel");
    topLeftSizer->Add(yearText,1,wxALL|wxLEFT,5);
    topLeftSizer->Add(yearChoice,1,wxALL|wxEXPAND,5);
    topMiddleSizer->Add(qText,1,wxALL|wxLEFT,5);
    topMiddleSizer->Add(qChoice,1,wxALL|wxEXPAND,5);
    topRightSizer->Add(getDistribution, 1, wxALL|wxEXPAND,5);
    topSizer->Add(topLeftSizer, 1, wxALL,5);
    topSizer->Add(topMiddleSizer, 1, wxALL,5);
    topSizer->Add(topRightSizer, 1, wxALL, 5);
    mainSizer->Add(topSizer);
    leftSizer->Add(distributionAmountText,1,wxALL,5);
    leftSizer->Add(distributionAmountTextCtrl,1,wxALL|wxEXPAND,5);
    rightSizer->Add(reserveAmountText,1,wxALL,5);
    rightSizer->Add(reserveAmountCtrl,1,wxALL|wxEXPAND,5);
    mainSizer->Add(leftSizer,1,wxALL,5);
    mainSizer->Add(rightSizer,1,wxALL,5);
    bottomSizer->Add(confirmButton,1,wxALL|wxEXPAND,5);
    bottomSizer->Add(cancelButton,1,wxALL|wxEXPAND,5);
    mainSizer->Add(bottomSizer,1,wxALL|wxEXPAND);
    this->SetSizer(mainSizer);
    this->Layout();
}