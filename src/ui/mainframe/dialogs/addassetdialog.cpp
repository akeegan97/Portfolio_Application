#include "ui/mainframe/dialogs/addassetdialog.hpp"


AddAssetDialog::AddAssetDialog(wxWindow*parentWindow,Portfolio &portfolio):
    wxDialog(parentWindow, wxID_ANY, "Instantiate New Asset",wxDefaultPosition, wxDefaultSize,wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
    m_portfolio(portfolio){
        SetupLayout();
    }

void AddAssetDialog::SetupLayout(){
    wxBoxSizer *mainSizer  = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topLeftSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topRightSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *bottomSizer = new wxBoxSizer(wxHORIZONTAL);

    wxString allowableChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ";
    wxString allowableNumbers = "1234567890.";
    wxTextValidator characterValidator(wxFILTER_INCLUDE_CHAR_LIST);
    wxTextValidator numberValidator(wxFILTER_INCLUDE_CHAR_LIST);
    characterValidator.SetIncludes(wxArrayString(1, &allowableChars));
    numberValidator.SetIncludes(wxArrayString(1, &allowableNumbers));

    assetNameCtrlText = new wxStaticText(this, wxID_ANY,"Enter Asset Name");
    assetNameCtrl = new wxTextCtrl(this, wxID_ANY);
    assetNameCtrl->SetValidator(characterValidator);

    assetSponserCtrlText = new wxStaticText(this, wxID_ANY,"Enter Sponser Name");
    assetSponserCtrl = new wxTextCtrl(this, wxID_ANY);
    assetSponserCtrl->SetValidator(characterValidator);

    assetExitDateText = new wxStaticText(this, wxID_ANY,"Enter Asset Exit");
    assetExitDateCtrl = new wxDatePickerCtrl(this, wxID_ANY);
    wxDateTime setDate = wxDateTime::Today();
    assetExitDateCtrl->SetValue(setDate);

    wxArrayString investorChoices;
    for(auto inv: m_portfolio.GetInvestors()){
        std::string name = inv->GetName();
        investorChoices.Add(name);
    }

    investorChoiceCtrl = new wxChoice(this, wxID_ANY,wxDefaultPosition, wxDefaultSize,investorChoices);
    investorChoiceText = new wxStaticText(this, wxID_ANY,"Select Investor Initial");

    paidAmountText = new wxStaticText(this, wxID_ANY,"Enter Paid Amount");
    paidAmountTextCtrl = new wxTextCtrl(this, wxID_ANY);
    paidAmountTextCtrl->SetValidator(numberValidator);

    deployedAmountText = new wxStaticText(this, wxID_ANY, "Enter Deployed Amount");
    deployedAmountTextCtrl = new wxTextCtrl(this, wxID_ANY);
    deployedAmountTextCtrl->SetValidator(numberValidator);

    reserveAmountText = new wxStaticText(this, wxID_ANY, "Enter Reserve Amount");
    reserveAmountTextCtrl = new wxTextCtrl(this, wxID_ANY);
    reserveAmountTextCtrl->SetValidator(numberValidator);

    effectiveStartDateCtrlText = new wxStaticText(this, wxID_ANY,"Enter Deploy Date");
    effectiveStartDateCtrl = new wxDatePickerCtrl(this, wxID_ANY);
    effectiveStartDateCtrl->SetValue(setDate);

    confirmButton = new wxButton(this, wxID_OK, "Confirm Asset");
    cancelButton = new wxButton(this, wxID_CANCEL,"Cancel Asset");
    createNewInvestorButton = new wxButton(this, wxID_ANY,"Create New Investor");
    
    topLeftSizer->Add(assetNameCtrlText,1,wxLEFT,5);
    topLeftSizer->Add(assetNameCtrl,1,wxEXPAND,5);
    topLeftSizer->Add(assetSponserCtrlText, 1, wxLEFT,5);
    topLeftSizer->Add(assetSponserCtrl,1, wxEXPAND,5);
    topLeftSizer->Add(assetExitDateText,1,wxLEFT,5);
    topLeftSizer->Add(assetExitDateCtrl,1,wxEXPAND,5);
    topSizer->Add(topLeftSizer,1,wxEXPAND,5);
    topRightSizer->Add(investorChoiceText,1,wxLEFT,5);
    topRightSizer->Add(investorChoiceCtrl,1,wxEXPAND,5);
    topRightSizer->Add(paidAmountText,1,wxLEFT,5);
    topRightSizer->Add(paidAmountTextCtrl,1,wxEXPAND,5);
    topRightSizer->Add(deployedAmountText,1,wxLEFT,5);
    topRightSizer->Add(deployedAmountTextCtrl,1,wxEXPAND,5);
    topRightSizer->Add(reserveAmountText,1,wxLEFT,5);
    topRightSizer->Add(reserveAmountTextCtrl,1,wxEXPAND,5);
    topRightSizer->Add(effectiveStartDateCtrlText,1,wxLEFT,5);
    topRightSizer->Add(effectiveStartDateCtrl,1,wxEXPAND,5);
    topSizer->Add(topRightSizer,1,wxEXPAND,5);

    buttonSizer->Add(createNewInvestorButton,1,wxEXPAND,5);
    buttonSizer->Add(cancelButton, 1, wxEXPAND,5);
    buttonSizer->Add(confirmButton,1,wxEXPAND,5);

    bottomSizer->Add(buttonSizer,1,wxEXPAND,5);

    mainSizer->Add(topSizer,2,wxEXPAND,5);
    mainSizer->Add(bottomSizer,1,wxEXPAND,5);

    this->SetSizer(mainSizer);
    this->Layout();

}


wxString AddAssetDialog::GetAssetName(){
    return assetNameCtrl->GetValue();
}
wxString AddAssetDialog::GetAssetSponser(){
    return assetSponserCtrl->GetValue();
}

wxDateTime AddAssetDialog::GetExitDate(){
    return assetExitDateCtrl->GetValue();
}