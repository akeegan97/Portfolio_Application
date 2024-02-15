#include "ui/assetpopout/dialogs/moveDeploy.hpp"

MoveDeploy::MoveDeploy(wxWindow *parentWindow):
    wxDialog(parentWindow, wxID_ANY, "Move Deploy", wxDefaultPosition, wxSize(300,300), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER){
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

        //Date of Move
        datePicker = new wxDatePickerCtrl(this, wxID_ANY);
        wxDateTime setDate = wxDateTime::Today();
        datePicker->SetValue(setDate);
        datePicker->SetForegroundColour(wxColor(51,245,12));
        datePickerText = new wxStaticText(this, wxID_ANY, "Enter Date of Move");
        datePickerText->SetForegroundColour(wxColor(51,245,12));
        mainSizer->Add(datePickerText, 0, wxALL | wxLEFT,5);
        mainSizer->Add(datePicker, 0, wxALL | wxEXPAND,5);

        //Amount Moved:
        amountCtrl = new wxTextCtrl(this, wxID_ANY);
        amountCtrl->SetValidator(validator);
        amountCtrl->SetForegroundColour(wxColor(51,245,12));
        amountCtrl->SetBackgroundColour(wxColor(0,0,0));

        amountText = new wxStaticText(this, wxID_ANY, "Enter Amount Moved");
        amountText->SetForegroundColour(wxColor(51,245,12));
        mainSizer->Add(amountText, 0, wxALL|wxLEFT,5);
        mainSizer->Add(amountCtrl, 0, wxALL|wxEXPAND, 5);
        //Direction of Move:
        wxArrayString choices;
        choices.Add("Reserve to Deploy");
        choices.Add("Deploy to Reserve");
        choices.Add("Reserve to Return of Capital");
        choices.Add("Deploy to Return of Capital");
        directionOfMovement = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,choices);
        directionOfMovement->SetForegroundColour(wxColor(51,245,12));
        directionOfMovement->SetBackgroundColour(wxColor(0,0,0));
        directionOfMovementText = new wxStaticText(this, wxID_ANY, "Select Movement Type");
        directionOfMovementText->SetForegroundColour(wxColor(51,245,12));
        mainSizer->Add(directionOfMovementText, 0, wxALL | wxLEFT, 5);
        mainSizer->Add(directionOfMovement, 0, wxALL|wxEXPAND, 5);
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