#include "moveDeploy.hpp"

MoveDeploy::MoveDeploy(wxWindow *parentWindow):
    wxDialog(parentWindow, wxID_ANY, "Move Deploy", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER){
        wxTextValidator validator(wxFILTER_DIGITS);
        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

        //Date of Move
        datePicker = new wxDatePickerCtrl(this, wxID_ANY);
        wxDateTime setDate = wxDateTime::Today();
        datePicker->SetValue(setDate);
        datePickerText = new wxStaticText(this, wxID_ANY, "Enter Date of Move");
        datePickerText->SetForegroundColour(wxColor(51,245,12));
        mainSizer->Add(datePickerText, 0, wxALL | wxLEFT,10);
        mainSizer->Add(datePicker, 0, wxALL | wxEXPAND,10);

        //Amount Moved:
        amountCtrl = new wxTextCtrl(this, wxID_ANY);
        amountCtrl->SetValidator(validator);
        amountCtrl->SetForegroundColour(wxColor(51,245,12));
        amountCtrl->SetBackgroundColour(wxColor(0,0,0));

        amountText = new wxStaticText(this, wxID_ANY, "Enter Amount Moved");
        amountText->SetForegroundColour(wxColor(51,245,12));
        mainSizer->Add(amountText, 0, wxALL|wxLEFT,10);
        mainSizer->Add(amountCtrl, 0, wxALL|wxEXPAND, 10);
        //Direction of Move:
        wxArrayString choices;
        choices.Add("To Deploy");
        choices.Add("From Deploy");
        directionOfMovement = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,choices);
        directionOfMovement->SetForegroundColour(wxColor(51,245,12));
        directionOfMovement->SetBackgroundColour(wxColor(0,0,0));

        directionOfMovementText = new wxStaticText(this, wxID_ANY, "Select Movement Type");
        directionOfMovementText->SetForegroundColour(wxColor(51,245,12));
        mainSizer->Add(directionOfMovementText, 0, wxALL | wxLEFT, 10);
        mainSizer->Add(directionOfMovement, 0, wxALL|wxEXPAND, 10);

        //buttons
        wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        wxButton* okayButton = new wxButton(this, wxID_OK, "Okay");
        wxButton* cancelButton = new wxButton(this, wxID_CANCEL, "Cancel");
        okayButton->SetForegroundColour(wxColor(51,245,12));
        okayButton->SetBackgroundColour(wxColor(0,0,0));
        cancelButton->SetForegroundColour(wxColor(252,0,0));
        cancelButton->SetBackgroundColour(wxColor(0,0,0));

        buttonSizer->Add(okayButton, 0, wxALL, 5);
        buttonSizer->Add(cancelButton, 0, wxALL, 5);

        mainSizer->Add(buttonSizer,0,wxCENTER);

        this->SetSizer(mainSizer);
    }

    wxDateTime MoveDeploy::GetDate(){
        return datePicker->GetValue();
    }

    double MoveDeploy::GetAmountMoved(){
        return wxAtof(amountCtrl->GetValue());
    }
    
    wxString MoveDeploy::GetSelectedMovementDirection(){
        return directionOfMovement->GetStringSelection();
    }