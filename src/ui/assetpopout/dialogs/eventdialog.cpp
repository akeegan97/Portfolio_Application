#include "assetpopout/eventdialog.hpp"

EventDialog::EventDialog(wxWindow *parentWindow, bool isEditMode):
    wxDialog(parentWindow, wxID_ANY, "Add Event", wxDefaultPosition, wxSize(300,500), wxDEFAULT_DIALOG_STYLE| wxRESIZE_BORDER){
        if(isEditMode){
            SetUpEditMode();
        }else{
            SetUpAddMode();
        }
    }

wxDateTime EventDialog::GetDate(){
    return datePicker->GetValue();
}
wxString EventDialog::GetDescription(){
    return discriptionCtrl->GetValue();
}
wxString EventDialog::GetHasHappenedTextFromChoiceCtrl(){
    return hasHappenedChoiceCtrl->GetStringSelection();
}

bool EventDialog::GetHasHappened(){
    return GetHasHappenedTextFromChoiceCtrl() == "Yes";
}

void EventDialog::SetUpAddMode(){
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    //date of event
    datePicker = new wxDatePickerCtrl(this, wxID_ANY);
    wxDateTime setDate = wxDateTime::Today();
    datePicker->SetValue(setDate);
    datePicker->SetForegroundColour(wxColor(51,245,12));
    datePickerText = new wxStaticText(this, wxID_ANY, "Enter Date of Event");
    datePickerText->SetForegroundColour(wxColor(51,245,12));
    mainSizer->Add(datePickerText, 0, wxALL | wxLEFT,5);
    mainSizer->Add(datePicker, 0, wxALL | wxEXPAND,5);
    //MultilineText Edit 

    discriptionCtrl = new wxTextCtrl(this, wxID_ANY,wxEmptyString, wxDefaultPosition, wxSize(250,150), wxTE_MULTILINE);
    discriptionCtrl->SetMaxLength(150);
    discriptionCtrl->SetForegroundColour(wxColor(51,245,12));
    discriptionCtrl->SetBackgroundColour(wxColor(0,0,0));

    discriptionText = new wxStaticText(this, wxID_ANY, "Enter Event Details");
    discriptionText->SetForegroundColour(wxColor(51,215,12));
    mainSizer->Add(discriptionText, 0, wxALL|wxLEFT, 5);
    mainSizer->Add(discriptionCtrl, 0, wxALL|wxLEFT,5);
    //Choices
    wxArrayString choices;
    choices.Add("Yes");
    choices.Add("No");
    hasHappenedChoiceCtrl = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices);
    hasHappenedChoiceCtrl->SetForegroundColour(wxColor(51,245,12));
    hasHappenedChoiceCtrl->SetBackgroundColour(wxColor(0,0,0));

    hasHappenedChoiceText = new wxStaticText(this, wxID_ANY, "Has Happened?");
    hasHappenedChoiceText->SetForegroundColour(wxColor(51,245,12));
    mainSizer->Add(hasHappenedChoiceText, 0, wxALL|wxLEFT,5);
    mainSizer->Add(hasHappenedChoiceCtrl, 0, wxALL|wxEXPAND,5);

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

void EventDialog::SetUpEditMode(){
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    //date of event
    wxStaticText* warningText = new wxStaticText(this, wxID_ANY,"Must Enter All Fields");
    datePicker = new wxDatePickerCtrl(this, wxID_ANY);
    wxDateTime setDate = wxDateTime::Today();
    datePicker->SetValue(setDate);
    datePicker->SetForegroundColour(wxColor(51,245,12));
    datePickerText = new wxStaticText(this, wxID_ANY, "Enter Date of Event");
    datePickerText->SetForegroundColour(wxColor(51,245,12));
    mainSizer->Add(warningText, 0, wxALL|wxLEFT,5);
    mainSizer->Add(datePickerText, 0, wxALL | wxLEFT,5);
    mainSizer->Add(datePicker, 0, wxALL | wxEXPAND,5);
    //MultilineText Edit 

    discriptionCtrl = new wxTextCtrl(this, wxID_ANY,wxEmptyString, wxDefaultPosition, wxSize(250,150), wxTE_MULTILINE);
    discriptionCtrl->SetMaxLength(150);
    discriptionCtrl->SetForegroundColour(wxColor(51,245,12));
    discriptionCtrl->SetBackgroundColour(wxColor(0,0,0));

    discriptionText = new wxStaticText(this, wxID_ANY, "Enter Event Details");
    discriptionText->SetForegroundColour(wxColor(51,215,12));
    mainSizer->Add(discriptionText, 0, wxALL|wxLEFT, 5);
    mainSizer->Add(discriptionCtrl, 0, wxALL|wxLEFT,5);
    //Choices
    wxArrayString choices;
    choices.Add("Yes");
    choices.Add("No");
    hasHappenedChoiceCtrl = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices);
    hasHappenedChoiceCtrl->SetForegroundColour(wxColor(51,245,12));
    hasHappenedChoiceCtrl->SetBackgroundColour(wxColor(0,0,0));

    hasHappenedChoiceText = new wxStaticText(this, wxID_ANY, "Has Happened?");
    hasHappenedChoiceText->SetForegroundColour(wxColor(51,245,12));
    mainSizer->Add(hasHappenedChoiceText, 0, wxALL|wxLEFT,5);
    mainSizer->Add(hasHappenedChoiceCtrl, 0, wxALL|wxEXPAND,5);

    //buttons
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* okayButton = new wxButton(this, wxID_OK, "Okay");
    wxButton* cancelButton = new wxButton(this, wxID_CANCEL, "Cancel");
    wxButton *deleteButton = new wxButton(this, wxID_DELETE, "Delete");
    deleteButton->Bind(wxEVT_BUTTON, &EventDialog::OnDeleteEventButtonPress, this);
    okayButton->SetForegroundColour(wxColor(51,245,12));
    okayButton->SetBackgroundColour(wxColor(0,0,0));
    cancelButton->SetForegroundColour(wxColor(252,0,0));
    cancelButton->SetBackgroundColour(wxColor(0,0,0));
    deleteButton->SetForegroundColour(wxColor(0,0,0));
    deleteButton->SetBackgroundColour(wxColor(250,0,0));

    buttonSizer->Add(okayButton, 0, wxALL, 5);
    buttonSizer->Add(cancelButton, 0, wxALL, 5);
    buttonSizer->Add(deleteButton, 0, wxALL, 5);

    mainSizer->Add(buttonSizer,0,wxCENTER);

    this->SetSizer(mainSizer);      
}

void EventDialog::OnDeleteEventButtonPress(wxCommandEvent &e){
    EndModal(DELETE_CODE_EVENT);
}