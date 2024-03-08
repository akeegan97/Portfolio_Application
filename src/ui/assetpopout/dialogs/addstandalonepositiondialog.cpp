#include "ui/assetpopout/dialogs/addstandalonepositiondialog.hpp"

AddStandalonePositionDialog::AddStandalonePositionDialog(wxWindow* parentWindow, Portfolio &portfolio):
    wxDialog(parentWindow, wxID_ANY, "Add Position",wxDefaultPosition, wxDefaultSize,wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
    m_portfolio(portfolio){
        SetupLayout();
    }

void AddStandalonePositionDialog::SetupLayout(){
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *leftTopSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *rightTopSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    wxString allowableNumbers = "0123456789.";
    wxTextValidator numberValidator(wxFILTER_INCLUDE_CHAR_LIST);
    numberValidator.SetIncludes(wxArrayString(1, &allowableNumbers));

    amountIncreaseCapitalText = new wxStaticText(this, wxID_ANY, "Enter Position Paid");
    amountIncreaseCapitalTextCtrl = new wxTextCtrl(this, wxID_ANY);
    amountIncreaseCapitalTextCtrl->SetValidator(numberValidator);

    amountDeployText = new wxStaticText(this, wxID_ANY, "Enter Amount To Deploy");
    amountDeployTextCtrl = new wxTextCtrl(this, wxID_ANY);
    amountDeployTextCtrl->SetValidator(numberValidator);

    amountReserveText = new wxStaticText(this, wxID_ANY, "Enter Amount To Reserve");
    amountReserveTextCtrl = new wxTextCtrl(this, wxID_ANY);
    amountReserveTextCtrl->SetValidator(numberValidator);

    wxDateTime setDate = wxDateTime::Today();
    dateNewPositionCtrl = new wxDatePickerCtrl(this, wxID_ANY);
    dateNewPositionCtrl->SetValue(setDate);
    dateNewPositionText = new wxStaticText(this, wxID_ANY, "Investment Date");

    leftTopSizer->Add(amountIncreaseCapitalText, 1, wxLEFT,5);
    leftTopSizer->Add(amountIncreaseCapitalTextCtrl,1,wxALL|wxEXPAND,5);
    leftTopSizer->Add(amountDeployText,1,wxLEFT,5);
    leftTopSizer->Add(amountDeployTextCtrl,1,wxEXPAND,5);
    leftTopSizer->Add(amountReserveText,1,wxLEFT,5);
    leftTopSizer->Add(amountReserveTextCtrl,1,wxEXPAND,5);

    rightTopSizer->Add(dateNewPositionText, 1, wxLEFT,5);
    rightTopSizer->Add(dateNewPositionCtrl,1,wxALL|wxEXPAND,5);
    topSizer->Add(leftTopSizer, 1, wxALL,5);
    topSizer->Add(rightTopSizer, 1, wxALL, 5);

    mainSizer->Add(topSizer, 1, wxALL | wxEXPAND,5);
    confirmButton = new wxButton(this, wxID_OK,"Confirm Position");
    buttonSizer->Add(confirmButton, 1, wxALL|wxEXPAND, 5);
    mainSizer->Add(buttonSizer, 1, wxALL|wxEXPAND,5);

    this->SetSizer(mainSizer);
    this->Layout();

}

wxDateTime AddStandalonePositionDialog::GetDateValue(){
    return dateNewPositionCtrl->GetValue();
}
double AddStandalonePositionDialog::GetPaidAmount(){
    return  wxAtof(amountIncreaseCapitalTextCtrl->GetValue());
}

double AddStandalonePositionDialog::GetDeployedAmount(){
    return wxAtof(amountDeployTextCtrl->GetValue());
}
double AddStandalonePositionDialog::GetReserveAmount(){
    return wxAtof(amountReserveTextCtrl->GetValue());
}