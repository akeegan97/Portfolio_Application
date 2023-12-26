#include "investorPositionEdit.hpp"

InvestorPositionEditWindow::InvestorPositionEditWindow(wxWindow *parentWindow):
    wxDialog(parentWindow,wxID_ANY, "Edit InvestorPosition", wxDefaultPosition, wxDefaultSize,wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER){
        wxTextValidator validator(wxFILTER_DIGITS);
        wxBoxSizer * mainSizer = new wxBoxSizer(wxVERTICAL);
//Invested
        datePicker = new wxDatePickerCtrl(this, wxID_ANY);
        wxDateTime setDate(01,wxDateTime::Jan, 2001);
        datePicker->SetValue(setDate);
        datePicker->SetForegroundColour(wxColor(51,245,12));
        datePicker->SetBackgroundColour(wxColor(0,0,0));
        datePickerText = new wxStaticText(this, wxID_ANY, "Enter Date of Investment");
        datePickerText->SetForegroundColour(wxColor(51,245,12));
        mainSizer->Add(datePickerText, 0, wxALL | wxLEFT,10);
        mainSizer->Add(datePicker, 0, wxALL| wxEXPAND, 10);
//Name
        clientNameCtrl = new wxTextCtrl(this, wxID_ANY);
        clientNameCtrl->SetMaxLength(20);
        clientNameCtrl->SetForegroundColour(wxColor(51,245,12));
        clientNameCtrl->SetBackgroundColour(wxColor(0,0,0));
        clientNameText = new wxStaticText(this, wxID_ANY, "Enter Investor Name");
        clientNameText->SetForegroundColour(wxColor(51,245,12));
        mainSizer->Add(clientNameText, 0, wxALL | wxLEFT,10);
        mainSizer->Add(clientNameCtrl, 0, wxALL | wxEXPAND, 10);
//Type
        clientTypeCtrl = new wxTextCtrl(this, wxID_ANY);
        clientTypeCtrl->SetMaxLength(15);
        clientTypeCtrl->SetForegroundColour(wxColor(51,245,12));
        clientTypeCtrl->SetBackgroundColour(wxColor(0,0,0));
        clientTypeText = new wxStaticText(this, wxID_ANY, "Enter Investor Type");
        clientTypeText->SetForegroundColour(wxColor(51,245,12));
        mainSizer->Add(clientTypeText, 0, wxALL | wxLEFT,10);
        mainSizer->Add(clientTypeCtrl, 0, wxALL | wxEXPAND, 10);

//Subscribed
        clientSubscribedCtrl = new wxTextCtrl(this, wxID_ANY);
        clientSubscribedCtrl->SetValidator(validator);
        clientSubscribedCtrl->SetMaxLength(10);
        clientSubscribedCtrl->SetForegroundColour(wxColor(51,245,12));
        clientSubscribedCtrl->SetBackgroundColour(wxColor(0,0,0));
        clientSubscribedText = new wxStaticText(this, wxID_ANY, "Enter Subscribed Amount");
        clientSubscribedText->SetForegroundColour(wxColor(51,245,12));
        mainSizer->Add(clientSubscribedText,0,wxALL|wxLEFT,10);
        mainSizer->Add(clientSubscribedCtrl, 0, wxALL|wxEXPAND,10);
//Paid
        clientPaidCtrl = new wxTextCtrl(this, wxID_ANY);
        clientPaidCtrl->SetValidator(validator);
        clientPaidCtrl->SetMaxLength(10);
        clientPaidCtrl->SetForegroundColour(wxColor(51,245,12));
        clientPaidCtrl->SetBackgroundColour(wxColor(0,0,0));
        clientPaidText = new wxStaticText(this, wxID_ANY, "Enter Paid Amount");
        clientPaidText->SetForegroundColour(wxColor(51,245,12));
        mainSizer->Add(clientPaidText,0,wxALL|wxLEFT,10);
        mainSizer->Add(clientPaidCtrl, 0, wxALL|wxEXPAND,10);
//Deployed
        clientDeployedCtrl = new wxTextCtrl(this, wxID_ANY);
        clientDeployedCtrl->SetValidator(validator);
        clientDeployedCtrl->SetMaxLength(10);
        clientDeployedCtrl->SetForegroundColour(wxColor(51,245,12));
        clientDeployedCtrl->SetBackgroundColour(wxColor(0,0,0));
        clientDeployedText = new wxStaticText(this, wxID_ANY, "Enter Deployed Amount");
        clientDeployedText->SetForegroundColour(wxColor(51,245,12));
        mainSizer->Add(clientDeployedText,0,wxALL|wxLEFT,10);
        mainSizer->Add(clientDeployedCtrl, 0, wxALL|wxEXPAND,10);

//buttons
        wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        wxButton * okayButton = new wxButton(this, wxID_OK, "Okay");
        wxButton* cancelButton = new wxButton(this, wxID_CANCEL, "Cancel");

        buttonSizer->Add(okayButton, 0, wxALL, 5);
        buttonSizer->Add(cancelButton, 0, wxALL, 5);

        mainSizer->Add(buttonSizer,0,wxCENTER);

        this->SetSizer(mainSizer);
}

wxString InvestorPositionEditWindow::GetClientName(){
        return clientNameCtrl->GetValue();
}

wxString InvestorPositionEditWindow::GetClientType(){
        return clientTypeCtrl->GetValue();
}

wxDateTime InvestorPositionEditWindow::GetInvestmentDate(){
        return datePicker->GetValue();
}

double InvestorPositionEditWindow::GetSubscribed(){
        return wxAtof(clientSubscribedCtrl->GetValue());
}

double InvestorPositionEditWindow::GetPaid(){
        return wxAtof(clientPaidCtrl->GetValue());
}
double InvestorPositionEditWindow::GetDeployed(){
        return wxAtof(clientDeployedCtrl->GetValue());
}