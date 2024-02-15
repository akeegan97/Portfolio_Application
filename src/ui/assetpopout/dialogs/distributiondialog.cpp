#include "ui/assetpopout/dialogs/distributiondialog.hpp"
DistributionDialog::DistributionDialog(wxWindow* parentWindow,bool isEditMode,wxDateTime &setDate, double &setAmount):
    wxDialog(parentWindow, wxID_ANY, "Add Distribution", wxDefaultPosition,wxSize(300,250), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER){
        if(isEditMode){
            SetUpOnEditDistribution(setDate, setAmount);
        }else{
            SetUpOnAddDistribution();
        }
    }

    wxDateTime DistributionDialog::GetDistributionDate(){
        return datePicker->GetValue();
    }

    double DistributionDialog::GetDistributionAmount(){
        return wxAtof(amountCtrl->GetValue());
    }

    void DistributionDialog::SetUpOnAddDistribution(){
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
        wxBoxSizer * mainSizer = new wxBoxSizer(wxVERTICAL);

        //Date of Distribution

        datePicker = new wxDatePickerCtrl(this, wxID_ANY);
        wxDateTime today = wxDateTime::Today();
        datePicker->SetValue(today);
        datePicker->SetForegroundColour(wxColor(51,245,12));

        datePickerText = new wxStaticText(this, wxID_ANY, "Enter Date of Distribution");
        datePickerText->SetForegroundColour(wxColor(51,245,12));
        datePickerText->SetBackgroundColour(wxColor(0,0,0));

        mainSizer->Add(datePickerText, 0, wxALL|wxLEFT, 5);
        mainSizer->Add(datePicker, 0, wxALL|wxEXPAND, 5);

        //Distribution Amount

        amountCtrl = new wxTextCtrl(this, wxID_ANY);
        amountCtrl->SetForegroundColour(wxColor(51,245,12));
        amountCtrl->SetBackgroundColour(wxColor(0,0,0));
        amountCtrl->SetMaxLength(12);
        amountCtrl->SetValidator(validator);
        amountText = new wxStaticText(this, wxID_ANY, "Enter Distribution Amount");
        amountText->SetForegroundColour(wxColor(51,245,12));
        amountText->SetBackgroundColour(wxColor(0,0,0));

        mainSizer->Add(amountText, 0, wxALL|wxLEFT, 5);
        mainSizer->Add(amountCtrl, 0, wxALL|wxEXPAND, 5);

        //buttons
        wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        wxButton * okayButton = new wxButton(this, wxID_OK, "Okay");
        wxButton * cancelButton = new wxButton(this, wxID_CANCEL, "Cancel");

        okayButton->SetForegroundColour(wxColor(51,245,12));
        okayButton->SetBackgroundColour(wxColor(0,0,0));

        cancelButton->SetForegroundColour(wxColor(252,0,0));
        cancelButton->SetBackgroundColour(wxColor(0,0,0));

        buttonSizer->Add(okayButton, 0, wxALL, 5);
        buttonSizer->Add(cancelButton, 0, wxALL, 5);

        mainSizer->Add(buttonSizer, 0, wxCENTER);
        this->SetSizer(mainSizer);
    }

    void DistributionDialog::SetUpOnEditDistribution(wxDateTime &setDate, double &setAmount){
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
        wxBoxSizer * mainSizer = new wxBoxSizer(wxVERTICAL);

        //Date of Distribution
        wxStaticText* warningText = new wxStaticText(this, wxID_ANY,"Must Enter All Fields");
        warningText->SetBackgroundColour(wxColor(0,0,0));
        warningText->SetForegroundColour(wxColour(250,0,0));
        mainSizer->Add(warningText, 0, wxALL|wxLEFT,5);

        datePicker = new wxDatePickerCtrl(this, wxID_ANY);
        datePicker->SetValue(setDate);
        datePicker->SetForegroundColour(wxColor(51,245,12));

        datePickerText = new wxStaticText(this, wxID_ANY, "Enter Date of Distribution");
        datePickerText->SetForegroundColour(wxColor(51,245,12));
        datePickerText->SetBackgroundColour(wxColor(0,0,0));

        mainSizer->Add(datePickerText, 0, wxALL|wxLEFT, 5);
        mainSizer->Add(datePicker, 0, wxALL|wxEXPAND, 5);

        //Distribution Amount

        amountCtrl = new wxTextCtrl(this, wxID_ANY);
        amountCtrl->SetForegroundColour(wxColor(51,245,12));
        amountCtrl->SetBackgroundColour(wxColor(0,0,0));
        amountCtrl->SetMaxLength(12);
        amountCtrl->SetValidator(validator);
        wxString setString = wxString::Format(wxT("%f"),setAmount);
        amountCtrl->SetValue(setString);
        amountText = new wxStaticText(this, wxID_ANY, "Enter Distribution Amount");
        amountText->SetForegroundColour(wxColor(51,245,12));
        amountText->SetBackgroundColour(wxColor(0,0,0));

        mainSizer->Add(amountText, 0, wxALL|wxLEFT, 5);
        mainSizer->Add(amountCtrl, 0, wxALL|wxEXPAND, 5);

        //buttons
        wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        wxButton * okayButton = new wxButton(this, wxID_OK, "Okay");
        wxButton * cancelButton = new wxButton(this, wxID_CANCEL, "Cancel");
        wxButton *deleteButton = new wxButton(this, wxID_DELETE, "Delete");

        okayButton->SetForegroundColour(wxColor(51,245,12));
        okayButton->SetBackgroundColour(wxColor(0,0,0));

        cancelButton->SetForegroundColour(wxColor(252,0,0));
        cancelButton->SetBackgroundColour(wxColor(0,0,0));

        deleteButton->SetForegroundColour(wxColor(51,245,12));
        deleteButton->SetBackgroundColour(wxColor(250,0,0));
        deleteButton->Bind(wxEVT_BUTTON, &DistributionDialog::OnDeleteButtonPress, this);

        buttonSizer->Add(okayButton, 0, wxALL, 5);
        buttonSizer->Add(deleteButton, 0, wxALL, 5);
        buttonSizer->Add(cancelButton, 0, wxALL, 5);

        mainSizer->Add(buttonSizer, 0, wxCENTER);
        this->SetSizer(mainSizer);
    }
void DistributionDialog::OnDeleteButtonPress(wxCommandEvent &e){
    EndModal(MY_CUSTOM_DELETE_CODE);
}