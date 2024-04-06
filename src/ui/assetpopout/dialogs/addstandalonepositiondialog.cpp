#include "ui/assetpopout/dialogs/addstandalonepositiondialog.hpp"
#include "helpers/utilities.hpp"

AddStandalonePositionDialog::AddStandalonePositionDialog(wxWindow* parentWindow, Portfolio &portfolio):
    wxDialog(parentWindow, wxID_ANY, "Standalone Position",wxDefaultPosition, wxSize(650,475),wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
    m_portfolio(portfolio){
        SetupLayout();
            wxFont font = wxFont(14, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
            wxColour color = wxColor(255,255,255);
            wxColor fgColor = wxColor(0,0,0);
            utilities::SetBackgroundColorForWindowAndChildren(this, color,fgColor);
            utilities::SetFontForWindowAndChildren(this, font);
            Bind(wxEVT_CLOSE_WINDOW, &AddStandalonePositionDialog::OnClose,this);
    }

void AddStandalonePositionDialog::SetupLayout(){
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *leftTopSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *rightTopSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    wxString allowableNumbers = "0123456789.";
    wxTextValidator numberValidator(wxFILTER_INCLUDE_CHAR_LIST);
    numberValidator.SetIncludes(wxArrayString(1, &allowableNumbers));
    wxString allowableChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ .";
    wxTextValidator charValidator(wxFILTER_INCLUDE_CHAR_LIST);
    charValidator.SetIncludes(wxArrayString(1,&allowableChars));

    amountIncreaseCapitalText = new wxStaticText(this, wxID_ANY, "Enter Position Paid");
    amountIncreaseCapitalTextCtrl = new wxTextCtrl(this, wxID_ANY);
    amountIncreaseCapitalTextCtrl->SetValidator(numberValidator);
    amountIncreaseCapitalText->Bind(wxEVT_TEXT,&AddStandalonePositionDialog::OnAmountsChanged,this);

    amountDeployText = new wxStaticText(this, wxID_ANY, "Enter Amount To Deploy");
    amountDeployTextCtrl = new wxTextCtrl(this, wxID_ANY);
    amountDeployTextCtrl->SetValidator(numberValidator);
    amountDeployTextCtrl->Bind(wxEVT_TEXT,&AddStandalonePositionDialog::OnAmountsChanged,this);

    amountReserveText = new wxStaticText(this, wxID_ANY, "Enter Amount To Reserve");
    amountReserveTextCtrl = new wxTextCtrl(this, wxID_ANY);
    amountReserveTextCtrl->SetValidator(numberValidator);
    amountReserveTextCtrl->Bind(wxEVT_TEXT,&AddStandalonePositionDialog::OnAmountsChanged,this);

    wxDateTime setDate = wxDateTime::Today();
    dateNewPositionCtrl = new wxDatePickerCtrl(this, wxID_ANY);
    dateNewPositionCtrl->SetValue(setDate);
    dateNewPositionText = new wxStaticText(this, wxID_ANY, "Investment Date");

    noteForTransactionText = new wxStaticText(this, wxID_ANY,"Enter Note For Transaction");
    noteForTransactionTextCtrl = new wxTextCtrl(this, wxID_ANY);
    noteForTransactionTextCtrl->SetValidator(charValidator);
    noteForTransactionTextCtrl->SetMaxLength(55);


    leftTopSizer->Add(amountIncreaseCapitalText, 1, wxLEFT,5);
    leftTopSizer->Add(amountIncreaseCapitalTextCtrl,1,wxALL|wxEXPAND,5);
    leftTopSizer->Add(amountDeployText,1,wxLEFT,5);
    leftTopSizer->Add(amountDeployTextCtrl,1,wxEXPAND,5);
    leftTopSizer->Add(amountReserveText,1,wxLEFT,5);
    leftTopSizer->Add(amountReserveTextCtrl,1,wxEXPAND,5);

    rightTopSizer->Add(dateNewPositionText, 1, wxLEFT,5);
    rightTopSizer->Add(dateNewPositionCtrl,1,wxALL|wxEXPAND,5);
    rightTopSizer->Add(noteForTransactionText,1,wxLEFT,5);
    rightTopSizer->Add(noteForTransactionTextCtrl,1,wxALL|wxEXPAND,5);

    topSizer->Add(leftTopSizer, 1, wxALL,5);
    topSizer->Add(rightTopSizer, 1, wxALL, 5);

    mainSizer->Add(topSizer, 1, wxALL | wxEXPAND,5);
    confirmButton = new wxButton(this, wxID_OK,"Confirm Position");
    cancelButton = new wxButton(this, wxID_CANCEL,"Cancel Position");
    confirmButton->Enable(false);
    buttonSizer->Add(confirmButton, 1, wxALL|wxEXPAND, 5);
    buttonSizer->Add(cancelButton,1,wxALL|wxEXPAND,5);
    mainSizer->Add(buttonSizer, 1, wxALL|wxEXPAND,5);

    this->SetSizer(mainSizer);
    this->Layout();

}

wxDateTime AddStandalonePositionDialog::GetDateValue(){
    return dateNewPositionCtrl->GetValue();
}
double AddStandalonePositionDialog::GetPaidAmount(){
    return  wxAtof(amountIncreaseCapitalTextCtrl->GetValue());
}

double AddStandalonePositionDialog::GetDeployedAmount(){
    return wxAtof(amountDeployTextCtrl->GetValue());
}
double AddStandalonePositionDialog::GetReserveAmount(){
    return wxAtof(amountReserveTextCtrl->GetValue());
}

void AddStandalonePositionDialog::UpdateConfirmButton(){
    double paidAmount = GetPaidAmount();
    double amountDeployed = GetDeployedAmount();
    double amountReserve = GetReserveAmount();
    if(amountDeployed == 0 && amountReserve == 0 && paidAmount > 0){
        confirmButton->Enable(true);
    }else if(paidAmount > 0 && paidAmount == (amountReserve + amountDeployed)){
        confirmButton->Enable(true);
    }else{
        confirmButton->Enable(false);
    }
}

void AddStandalonePositionDialog::OnClose(wxCloseEvent &e){
        this->SetReturnCode(wxID_CANCEL);
        std::cout<<"Sending wxID_CANCEL!"<<std::endl;
    e.Skip();
}

void AddStandalonePositionDialog::OnAmountsChanged(wxCommandEvent &e){
    UpdateConfirmButton();
}

std::string AddStandalonePositionDialog::GetNote(){
    return noteForTransactionTextCtrl->GetValue().ToStdString();
}