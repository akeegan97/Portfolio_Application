#include "addValuation.hpp"

AddValuation::AddValuation(wxWindow *parentWindow):
    wxDialog(parentWindow, wxID_ANY, "Add Valuation", wxDefaultPosition, wxSize(300,200), wxDEFAULT_DIALOG_STYLE| wxRESIZE_BORDER){
        wxTextValidator validator(wxFILTER_DIGITS);
        wxBoxSizer * mainSizer = new wxBoxSizer(wxVERTICAL);

        //date of valuation

        datePicker = new wxDatePickerCtrl(this, wxID_ANY);
        wxDateTime setDate = wxDateTime::Today();
        datePicker->SetValue(setDate);
        datePicker->SetForegroundColour(wxColor(51,245,12));

        datePickerText = new wxStaticText(this, wxID_ANY, "Enter Date of Valuation");
        datePickerText->SetForegroundColour(wxColor(51,245,12));
        datePickerText->SetBackgroundColour(wxColor(0,0,0));

        mainSizer->Add(datePickerText, 0, wxALL|wxLEFT, 5);
        mainSizer->Add(datePicker, 0, wxALL|wxEXPAND, 5);

        //valuation amount

        valuationCtrl = new wxTextCtrl(this, wxID_ANY);
        valuationCtrl->SetForegroundColour(wxColor(51,245,12));
        valuationCtrl->SetBackgroundColour(wxColor(0,0,0));
        valuationCtrl->SetMaxLength(12);
        valuationCtrl->SetValidator(validator);

        valuationText = new wxStaticText(this, wxID_ANY, "Enter Valuation Amount");
        valuationText->SetForegroundColour(wxColor(51,245,12));
        valuationText->SetBackgroundColour(wxColor(0,0,0));

        mainSizer->Add(valuationText, 0, wxALL|wxLEFT,5);
        mainSizer->Add(valuationCtrl, 0, wxALL|wxEXPAND,5);

        //buttons

        wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        wxButton *okayButton = new wxButton(this, wxID_OK, "Okay");
        wxButton *cancelButton = new wxButton(this, wxID_CANCEL,"Cancel");

        okayButton->SetForegroundColour(wxColor(51,245,12));
        okayButton->SetBackgroundColour(wxColor(0,0,0));

        cancelButton->SetForegroundColour(wxColor(252,0,0));
        cancelButton->SetBackgroundColour(wxColor(0,0,0));

        buttonSizer->Add(okayButton, 0, wxALL, 5);
        buttonSizer->Add(cancelButton, 0, wxALL, 5);

        mainSizer->Add(buttonSizer, 0, wxCENTER);

        this->SetSizer(mainSizer);

    }

    wxDateTime AddValuation::GetDate(){
        return datePicker->GetValue();
    }

    double AddValuation::GetValuation(){
        return wxAtof(valuationCtrl->GetValue());
    }
    