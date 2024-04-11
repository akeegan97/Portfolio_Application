#include "ui/mainframe/dialogs/addassetdialog.hpp"
#include "ui/assetpopout/dialogs/addinvestordialog.hpp"
#include "helpers/utilities.hpp"


AddAssetDialog::AddAssetDialog(wxWindow*parentWindow,Portfolio &portfolio):
    wxDialog(parentWindow, wxID_ANY, "Instantiate New Asset",wxDefaultPosition, wxSize(700,550),wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
    m_portfolio(portfolio){
        SetupLayout();
        wxFont font = wxFont(14, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
        wxColour color = wxColor(255,255,255);
        wxColour foregroundcolor = wxColor(0,0,0);
        #ifdef __WXMSW__
        utilities::SetBackgroundColorForWindowAndChildren(this, color, foregroundcolor);
        utilities::SetFontForWindowAndChildren(this, font);
        #elif defined(__WXMAC__)
        utilities::SetFontForWindowAndChildren(this, font);
        #endif
    }

void AddAssetDialog::SetupLayout(){
    wxBoxSizer *mainSizer  = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topLeftSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topRightSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *bottomSizer = new wxBoxSizer(wxHORIZONTAL);

    wxString allowableChars = "0123456789.,abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ";
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

    noteForTransactionText = new wxStaticText(this, wxID_ANY,"Enter Note For Transaction");
    noteForTransactionTextCtrl = new wxTextCtrl(this,wxID_ANY);
    noteForTransactionTextCtrl->SetValidator(characterValidator);
    noteForTransactionTextCtrl->SetMaxLength(50);

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
    paidAmountText->SetForegroundColour(wxColor(255,255,255));

    deployedAmountText = new wxStaticText(this, wxID_ANY, "Enter Deployed Amount");
    deployedAmountTextCtrl = new wxTextCtrl(this, wxID_ANY);
    deployedAmountTextCtrl->SetValidator(numberValidator);
    deployedAmountTextCtrl->Bind(wxEVT_TEXT,&AddAssetDialog::OnAmountsChanged,this);
    deployedAmountText->SetForegroundColour(wxColor(255,255,255));

    reserveAmountText = new wxStaticText(this, wxID_ANY, "Enter Reserve Amount");
    reserveAmountTextCtrl = new wxTextCtrl(this, wxID_ANY);
    reserveAmountTextCtrl->Bind(wxEVT_TEXT,&AddAssetDialog::OnAmountsChanged,this);
    reserveAmountTextCtrl->SetValidator(numberValidator);
    reserveAmountText->SetForegroundColour(wxColor(255,255,255));

    effectiveStartDateCtrlText = new wxStaticText(this, wxID_ANY,"Enter Deploy Date");
    effectiveStartDateCtrl = new wxDatePickerCtrl(this, wxID_ANY);
    effectiveStartDateCtrl->SetValue(setDate);
    effectiveStartDateCtrlText->SetForegroundColour(wxColor(255,255,255));

    confirmButton = new wxButton(this, wxID_OK, "Confirm Asset");
    confirmButton->Enable(false);
    confirmButton->SetForegroundColour(wxColor(255,255,255));

    cancelButton = new wxButton(this, wxID_CANCEL,"Cancel Asset");
    cancelButton->SetForegroundColour(wxColor(255,255,255));
    createNewInvestorButton = new wxButton(this, wxID_ANY,"Create New Investor");
    createNewInvestorButton->Bind(wxEVT_BUTTON, &AddAssetDialog::OnAddInvestor, this);

    topLeftSizer->Add(assetNameCtrlText,1,wxLEFT,5);
    topLeftSizer->Add(assetNameCtrl,1,wxEXPAND|wxALL,5);
    topLeftSizer->Add(assetSponserCtrlText, 1, wxLEFT,5);
    topLeftSizer->Add(assetSponserCtrl,1, wxEXPAND|wxALL,5);
    topLeftSizer->Add(assetExitDateText,1,wxLEFT,5);
    topLeftSizer->Add(assetExitDateCtrl,1,wxEXPAND|wxALL,5);
    topLeftSizer->Add(noteForTransactionText,1,wxLEFT,5);
    topLeftSizer->Add(noteForTransactionTextCtrl,1,wxEXPAND|wxALL,5);
    topSizer->Add(topLeftSizer,1,wxALL,20);
    topRightSizer->Add(investorChoiceText,1,wxLEFT,5);
    topRightSizer->Add(investorChoiceCtrl,1,wxEXPAND|wxALL,5);
    topRightSizer->Add(paidAmountText,1,wxLEFT,5);
    topRightSizer->Add(paidAmountTextCtrl,1,wxEXPAND|wxALL,5);
    topRightSizer->Add(deployedAmountText,1,wxLEFT,5);
    topRightSizer->Add(deployedAmountTextCtrl,1,wxEXPAND|wxALL,5);
    topRightSizer->Add(reserveAmountText,1,wxLEFT,5);
    topRightSizer->Add(reserveAmountTextCtrl,1,wxEXPAND|wxALL,5);
    topRightSizer->Add(effectiveStartDateCtrlText,1,wxLEFT,5);
    topRightSizer->Add(effectiveStartDateCtrl,1,wxEXPAND|wxALL,5);

    topSizer->Add(topRightSizer,1,wxALL,20);

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

wxDateTime AddAssetDialog::GetEffectiveDate(){
    return effectiveStartDateCtrl->GetValue();
}
double AddAssetDialog::GetPaidAmount(){
    return wxAtof(paidAmountTextCtrl->GetValue());
}
double AddAssetDialog::GetReserveAmount(){
    return wxAtof(reserveAmountTextCtrl->GetValue());
}
double AddAssetDialog::GetDeployedAmount(){
    return wxAtof(deployedAmountTextCtrl->GetValue());
}
std::string AddAssetDialog::GetInvestorChoiceName(){
    return investorChoiceCtrl->GetStringSelection().ToStdString();
}

std::string AddAssetDialog::GetNote(){
    return noteForTransactionTextCtrl->GetValue().ToStdString();
}

void AddAssetDialog::OnAddInvestor(wxCommandEvent &e){
    AddInvestorDialog dialog(this);
    int retValue = dialog.ShowModal();
    if(retValue == wxID_OK){
        wxString name = dialog.GetInvestorName();
        wxString type = dialog.GetInvestorType();
        double promotefee = dialog.GetInvestorPromoteFee();
        double managementFee = dialog.GetInvestorMgmtFee();
        if(name == ""||type ==""){
            return;
        }
        Investor newInvestor(name, type, managementFee, promotefee);
        std::shared_ptr<Investor> newInvestorPtr = std::make_shared<Investor>(newInvestor);
        m_portfolio.AddInvestor(newInvestorPtr);
        UpdateInvestorChoice();
        this->Layout();
    }
}

void AddAssetDialog::UpdateInvestorChoice(){
    wxArrayString investorChoices;
    for(auto &inv: m_portfolio.GetInvestors()){
        std::string name = inv->GetName();
        investorChoices.Add(name);
    }
    investorChoiceCtrl->Clear();
    investorChoiceCtrl->Append(investorChoices);
    investorChoiceCtrl->Refresh();
}

void AddAssetDialog::UpdateConfirmButton(){
    double paid = GetPaidAmount();
    double deployedAmount = GetDeployedAmount();
    double reserveAmount = GetReserveAmount();
    if(paid!=0 && paid == (deployedAmount+reserveAmount)){
        confirmButton->Enable(true);
    }
}

void AddAssetDialog::OnAmountsChanged(wxCommandEvent &e){
    UpdateConfirmButton();
}