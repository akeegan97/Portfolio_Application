#include "ui/assetpopout/dialogs/valuationdialog.hpp"
#include "helpers/utilities.hpp"

ValuationDialog::ValuationDialog(wxWindow *parentWindow, bool editMode,wxDateTime &setDate, double &setValue):
    wxDialog(parentWindow, wxID_ANY, "Add Valuation", wxDefaultPosition, wxSize(300,250), wxDEFAULT_DIALOG_STYLE| wxRESIZE_BORDER){
        if(editMode){
            SetUpOnEdit(setDate, setValue);
            wxFont font = wxFont(14, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
            wxColour color = wxColor(255,255,255);
            wxColor fgColor = wxColor(0,0,0);
            utilities::SetBackgroundColorForWindowAndChildren(this, color,fgColor);
            utilities::SetFontForWindowAndChildren(this, font);
        }else{
            SetUpOnAdd();
            wxFont font = wxFont(14, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
            wxColour color = wxColor(255,255,255);
            wxColor fgColor = wxColor(0,0,0);
            utilities::SetBackgroundColorForWindowAndChildren(this, color,fgColor);
            utilities::SetFontForWindowAndChildren(this, font);
        }
    }
    
    void ValuationDialog::SetUpOnAdd(){
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

    void ValuationDialog::SetUpOnEdit(wxDateTime &setDate, double &setValue){
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


        datePicker = new wxDatePickerCtrl(this, wxID_ANY);
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
        wxString myString = wxString::Format(wxT("%.2f"), setValue);
        valuationCtrl->SetValue(myString);

        valuationText = new wxStaticText(this, wxID_ANY, "Enter Valuation Amount");
        valuationText->SetForegroundColour(wxColor(51,245,12));
        valuationText->SetBackgroundColour(wxColor(0,0,0));

        mainSizer->Add(valuationText, 0, wxALL|wxLEFT,5);
        mainSizer->Add(valuationCtrl, 0, wxALL|wxEXPAND,5);

        //buttons

        wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        wxButton *okayButton = new wxButton(this, wxID_OK, "Okay");
        wxButton *cancelButton = new wxButton(this, wxID_CANCEL,"Cancel");
        wxButton *deleteButton = new wxButton(this, MY_VALUATION_DELETE_CODE, "Delete");

        okayButton->SetForegroundColour(wxColor(51,245,12));
        okayButton->SetBackgroundColour(wxColor(0,0,0));

        cancelButton->SetForegroundColour(wxColor(252,0,0));
        cancelButton->SetBackgroundColour(wxColor(0,0,0));

        deleteButton->SetForegroundColour(wxColor(51,245,12));
        deleteButton->SetBackgroundColour(wxColor(250,0,0));
        deleteButton->Bind(wxEVT_BUTTON, &ValuationDialog::OnDeleteButtonPress, this);

        buttonSizer->Add(okayButton, 0, wxALL, 5);
        buttonSizer->Add(deleteButton, 0, wxALL, 5);
        buttonSizer->Add(cancelButton, 0, wxALL, 5);

        mainSizer->Add(buttonSizer, 0, wxCENTER);

        this->SetSizer(mainSizer);       
    }

    wxDateTime ValuationDialog::GetDate(){
        return datePicker->GetValue();
    }

    double ValuationDialog::GetValuation(){
        return wxAtof(valuationCtrl->GetValue());
    }
    
    void ValuationDialog::OnDeleteButtonPress(wxCommandEvent &e){
        EndModal(MY_VALUATION_DELETE_CODE);
    }