#include "ui/assetpopout/dialogs/addinvestordialog.hpp"


AddInvestorDialog::AddInvestorDialog(wxWindow *parentWindow):
    wxDialog(parentWindow, wxID_ANY, "Fill Out Position", wxDefaultPosition, wxDefaultSize,wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
    {
        SetupLayout();
    }

void AddInvestorDialog::SetupLayout(){
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *leftTopSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *rightTopSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    wxString allowableChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ";
    wxString allowableNumbers = "012345679.";
    wxTextValidator characterValidator(wxFILTER_INCLUDE_CHAR_LIST);
    wxTextValidator numberValidator(wxFILTER_INCLUDE_CHAR_LIST);
    characterValidator.SetIncludes(wxArrayString(1, &allowableChars));
    numberValidator.SetIncludes(wxArrayString(1,&allowableNumbers));

    inputNameText = new wxStaticText(this, wxID_ANY, "Enter Investor Name");
    inputNameTextCtrl = new wxTextCtrl(this, wxID_ANY);
    inputNameTextCtrl->SetValidator(characterValidator);

    inputTypeText = new wxStaticText(this, wxID_ANY, "Enter Type Of Investor");
    inputTypeTextCtrl = new wxTextCtrl(this, wxID_ANY);
    inputTypeTextCtrl->SetValidator(characterValidator);

    inputMgmtFeeText = new wxStaticText(this, wxID_ANY, "Enter MGMT FEE %");
    inputMgmtFeeTextCtrl = new wxTextCtrl(this, wxID_ANY);
    inputMgmtFeeTextCtrl->SetValidator(numberValidator);

    inputPromoteFeeText = new wxStaticText(this, wxID_ANY, "Enter Promote FEE %");
    inputPromoteFeeTextCtrl = new wxTextCtrl(this, wxID_ANY);
    inputPromoteFeeTextCtrl->SetValidator(numberValidator); 

    confirmNewInvestorButton = new wxButton(this, wxID_OK,"Confirm Investor");

    leftTopSizer->Add(inputNameText, 1, wxLEFT,5);
    leftTopSizer->Add(inputNameTextCtrl,1,wxALL|wxEXPAND,5);
    leftTopSizer->Add(inputMgmtFeeText,1,wxLEFT,5);
    leftTopSizer->Add(inputMgmtFeeTextCtrl,1,wxALL|wxEXPAND,5);

    rightTopSizer->Add(inputTypeText,1,wxLEFT,5);
    rightTopSizer->Add(inputTypeTextCtrl,1,wxALL|wxEXPAND,5);
    rightTopSizer->Add(inputPromoteFeeText,1,wxLEFT,5);
    rightTopSizer->Add(inputPromoteFeeTextCtrl,1,wxALL|wxEXPAND,5);

    topSizer->Add(leftTopSizer, 1, wxALL|wxEXPAND,5);
    topSizer->Add(rightTopSizer,1,wxALL|wxEXPAND,5);

    mainSizer->Add(topSizer,1,wxALL|wxEXPAND,5);

    buttonSizer->Add(confirmNewInvestorButton, 1, wxALL|wxEXPAND,5);

    mainSizer->Add(buttonSizer, 1, wxALL|wxEXPAND);

    this->SetSizer(mainSizer);
    this->Layout();
}


wxString AddInvestorDialog::GetInvestorName(){
    return inputNameTextCtrl->GetValue();
}

wxString AddInvestorDialog::GetInvestorType(){
    return inputTypeTextCtrl->GetValue();
}

double AddInvestorDialog::GetInvestorMgmtFee(){
    return wxAtof(inputMgmtFeeTextCtrl->GetValue());
}

double AddInvestorDialog::GetInvestorPromoteFee(){
    return wxAtof(inputPromoteFeeTextCtrl->GetValue());
}
