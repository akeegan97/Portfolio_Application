#include "ui/assetpopout/dialogs/setassetdeployreservedialog.hpp"

SetAssetDeployReserveDialog::SetAssetDeployReserveDialog(wxWindow*parentWindow):
    wxDialog(parentWindow, wxID_ANY, "Set Deploy Reserve", wxDefaultPosition, wxDefaultSize,wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER){
        wxArrayString includeList;
        includeList.Add(wxT("0"));
        includeList.Add(wxT("1"));
        includeList.Add(wxT("2"));
        includeList.Add(wxT("3"));
        includeList.Add(wxT("4"));
        includeList.Add(wxT("5"));
        includeList.Add(wxT("6"));
        includeList.Add(wxT("7"));
        includeList.Add(wxT("8"));
        includeList.Add(wxT("9"));
        includeList.Add(wxT(".")); 
        wxTextValidator validator(wxFILTER_INCLUDE_CHAR_LIST);
        validator.SetIncludes(includeList);

        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
        //date of movements
        datePicker = new wxDatePickerCtrl(this, wxID_ANY);
        wxDateTime setDate = wxDateTime::Today();
        datePicker->SetValue(setDate);

        datePickerText = new wxStaticText(this, wxID_ANY, "Enter Date");

        deployAmountCtrl = new wxTextCtrl(this, wxID_ANY);
        deployAmountCtrl->SetValidator(validator);
        reserveAmountCtrl = new wxTextCtrl(this, wxID_ANY);
        reserveAmountCtrl->SetValidator(validator);

        deployAmountCtrlText = new wxStaticText(this, wxID_ANY, "Enter Deploy Amount");
        reserveAmountCtrlText = new wxStaticText(this, wxID_ANY, "Enter Reserve Amount");

        wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        wxButton *okayButton = new wxButton(this, wxID_OK, "Confirm");
        wxButton* cancelButton = new wxButton(this, wxID_CANCEL, "Cancel");
        buttonSizer->Add(okayButton, 1, wxALL,5);
        buttonSizer->Add(cancelButton, 1, wxALL,5);

        mainSizer->Add(datePickerText, 1, wxLEFT, 5);
        mainSizer->Add(datePicker, 1, wxALL,5);
        mainSizer->Add(deployAmountCtrlText, 1, wxLEFT,5);
        mainSizer->Add(deployAmountCtrl, 1, wxALL, 5);
        mainSizer->Add(reserveAmountCtrlText, 1, wxLEFT,5);
        mainSizer->Add(reserveAmountCtrl, 1, wxALL,5);
        mainSizer->Add(buttonSizer, 1, wxCENTER);

        this->SetSizer(mainSizer);
    }

wxDateTime SetAssetDeployReserveDialog::GetDate(){
    return datePicker->GetValue();
}
double SetAssetDeployReserveDialog::GetReserve(){
    return wxAtof(reserveAmountCtrl->GetValue());
}

double SetAssetDeployReserveDialog::GetDeploy(){
    return wxAtof(deployAmountCtrl->GetValue());
}