#include "addDistribution.hpp"

AddDistributionDialog::AddDistributionDialog(wxWindow* parentWindow):
    wxDialog(parentWindow, wxID_ANY, "Add Distribution", wxDefaultPosition,wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER){
        wxTextValidator validator(wxFILTER_DIGITS);
        wxBoxSizer * mainSizer = new wxBoxSizer(wxVERTICAL);

        //Date of Distribution

        datePicker = new wxDatePickerCtrl(this, wxID_ANY);
        wxDateTime today = wxDateTime::Today();
        datePicker->SetValue(today);
        datePicker->SetBackgroundColour(wxColor(0,0,0));
        datePicker->SetForegroundColour(wxColor(51,245,12));

        datePickerText = new wxStaticText(this, wxID_ANY, "Enter Date of Distribution");
        datePickerText->SetForegroundColour(wxColor(51,245,12));
        datePickerText->SetBackgroundColour(wxColor(0,0,0));

        mainSizer->Add(datePickerText, 0, wxALL|wxLEFT, 10);
        mainSizer->Add(datePicker, 0, wxALL|wxLEFT, 10);

        //Distribution Amount

        amountCtrl = new wxTextCtrl(this, wxID_ANY);
        amountCtrl->SetForegroundColour(wxColor(51,245,12));
        amountCtrl->SetBackgroundColour(wxColor(0,0,0));
        amountCtrl->SetMaxLength(12);
        amountCtrl->SetValidator(validator);
        amountText = new wxStaticText(this, wxID_ANY, "Enter Distribution Amount");
        amountText->SetForegroundColour(wxColor(51,245,12));
        amountText->SetBackgroundColour(wxColor(0,0,0));

        mainSizer->Add(amountText, 0, wxALL|wxLEFT, 10);
        mainSizer->Add(amountCtrl, 0, wxALL|wxLEFT, 10);

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

    wxDateTime AddDistributionDialog::GetDistributionDate(){
        return datePicker->GetValue();
    }

    double AddDistributionDialog::GetDistributionAmount(){
        return wxAtof(amountCtrl->GetValue());
    }