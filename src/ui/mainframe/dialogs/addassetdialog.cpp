#include "ui/mainframe/dialogs/addassetdialog.hpp"


AddAssetDialog::AddAssetDialog(wxWindow*parentWindow):
    wxDialog(parentWindow, wxID_ANY, "Add Asset",wxDefaultPosition, wxDefaultSize,wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER){
        SetupLayout();
    }

void AddAssetDialog::SetupLayout(){
    wxBoxSizer *mainSizer  = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *topRightSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topLeftSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *bottomLeftSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *bottomRightSizer = new wxBoxSizer(wxVERTICAL);

    wxString allowableChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ";
    wxString allowableNumbers = "1234567890.";
    wxTextValidator characterValidator(wxFILTER_INCLUDE_CHAR_LIST);
    wxTextValidator numberValidator(wxFILTER_INCLUDE_CHAR_LIST);

    characterValidator.SetIncludes(wxArrayString(1, &allowableChars));
    numberValidator.SetIncludes(wxArrayString(1, &allowableNumbers));

    inputAssetNameText = new wxStaticText(this, wxID_ANY, "Enter Asset Name");
    inputAssetNameCtrl = new wxTextCtrl(this, wxID_ANY);
    inputAssetNameCtrl->SetValidator(characterValidator);

    topLeftSizer->Add(inputAssetNameText, 1, wxCENTER,5);
    topLeftSizer->Add(inputAssetNameCtrl, 1, wxCENTER,5);

    inputAssetSponserText = new wxStaticText(this, wxID_ANY, "Enter Asset Sponser");
    inputAssetSponserCtrl = new wxTextCtrl(this, wxID_ANY);
    inputAssetSponserCtrl->SetValidator(characterValidator);

    topLeftSizer->Add(inputAssetSponserText,1,wxCENTER,5);
    topLeftSizer->Add(inputAssetSponserCtrl, 1, wxCENTER,5);

    inputAssetExitDateText = new wxStaticText(this, wxID_ANY, "Enter Asset Exit Date");
    wxDateTime setDate = wxDateTime::Today();
    inputAssetExitDateCtrl = new wxDatePickerCtrl(this, wxID_ANY);
    inputAssetExitDateCtrl->SetValue(setDate);

    topRightSizer->Add(inputAssetExitDateText, 1, wxCENTER,5);
    topRightSizer->Add(inputAssetExitDateCtrl, 1, wxCENTER, 5);

    topSizer->Add(topLeftSizer, 1, wxALL|wxEXPAND,5);
    topSizer->Add(topRightSizer, 1, wxALL|wxEXPAND,5);

    confirmAssetButton = new wxButton(this, wxID_OK,"Add Asset");
    cancelAssetButton = new wxButton(this, wxID_CANCEL, "Cancel");

    bottomLeftSizer->Add(confirmAssetButton, 1, wxCENTER,5);
    bottomRightSizer->Add(cancelAssetButton, 1, wxCENTER,5);

    bottomSizer->Add(bottomLeftSizer, 1, wxEXPAND,5);
    bottomSizer->Add(bottomRightSizer, 1, wxEXPAND,5);

    mainSizer->Add(topSizer,1,wxEXPAND,5);
    mainSizer->Add(bottomSizer, 1, wxEXPAND,5);
    this->SetSizer(mainSizer);
    this->Layout();
}


wxString AddAssetDialog::GetAssetName(){
    return inputAssetNameCtrl->GetValue();
}
wxString AddAssetDialog::GetAssetSponser(){
    return inputAssetSponserCtrl->GetValue();
}

wxDateTime AddAssetDialog::GetExitDate(){
    return inputAssetExitDateCtrl->GetValue();
}