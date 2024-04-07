#include "ui/assetpopout/dialogs/executedistribution.hpp"

ExecuteDistribution::ExecuteDistribution(wxWindow *parentWindow, std::shared_ptr<Asset> &asset):
    wxDialog(parentWindow, wxID_ANY,"Distribute",wxDefaultPosition,wxSize(500,400),wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
    m_asset(asset){
        SetupLayout();
        #ifdef __WXMSW__
        wxFont font = wxFont(12, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
        wxColour color = wxColor(255,255,255);
        wxColour foregroundcolor = wxColor(0,0,0);
        utilities::SetBackgroundColorForWindowAndChildren(this, color, foregroundcolor);
        utilities::SetFontForWindowAndChildren(this, font);
        #endif
    }

void ExecuteDistribution::SetupLayout(){
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *topLeftSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topRightSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *bottomSizer = new wxBoxSizer(wxHORIZONTAL);

    wxString allowableNumbers = "0123456789.";
    wxString allowableChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ .";
    wxTextValidator numberValidator(wxFILTER_INCLUDE_CHAR_LIST);
    numberValidator.SetIncludes(wxArrayString(1,&allowableNumbers));
    wxTextValidator charValidator(wxFILTER_INCLUDE_CHAR_LIST);
    charValidator.SetIncludes(wxArrayString(1,&allowableChars));

    datePickerText = new wxStaticText(this, wxID_ANY,"Chose Date");
    datePicker = new wxDatePickerCtrl(this,wxID_ANY);
    wxDateTime today;
    today = wxDateTime::Today();
    datePicker->SetValue(today);
    datePicker->Bind(wxEVT_DATE_CHANGED,&ExecuteDistribution::OnSelectedDate,this);

    amountToDistributeText = new wxStaticText(this, wxID_ANY,"Amount To Distribute");
    amountToDistributeTextCtrl = new wxTextCtrl(this,wxID_ANY);
    amountToDistributeTextCtrl->SetValidator(numberValidator);
    amountToDistributeTextCtrl->Bind(wxEVT_TEXT,&ExecuteDistribution::OnAmountsChanged,this);

    amountToReserveText = new wxStaticText(this, wxID_ANY,"Amount To Reserve");
    amountToReserveTextCtrl = new wxTextCtrl(this, wxID_ANY);
    amountToReserveTextCtrl->SetValidator(numberValidator);
    amountToReserveTextCtrl->Bind(wxEVT_TEXT,&ExecuteDistribution::OnAmountsChanged,this);

    noteText = new wxStaticText(this, wxID_ANY,"Enter Note For Transaction");
    noteTextCrtl = new wxTextCtrl(this, wxID_ANY);
    noteTextCrtl->SetValidator(charValidator);

    getAmount = new wxButton(this, wxID_ANY,"Get Amount");
    getAmount->Enable(false);
    getAmount->Bind(wxEVT_BUTTON, &ExecuteDistribution::OnGetAmount,this);
    getAmountText = new wxStaticText(this, wxID_ANY,"");
    confirmButton = new wxButton(this, wxID_OK,"Confirm");
    confirmButton->Enable(false);
    cancelButton = new wxButton(this, wxID_CANCEL,"Cancel");

    topLeftSizer->Add(datePickerText,1,wxEXPAND|wxALL,5);
    topLeftSizer->Add(datePicker,1,wxEXPAND|wxALL,5);

    topRightSizer->Add(getAmount,1,wxEXPAND|wxALL,5);
    topRightSizer->Add(getAmountText,1,wxEXPAND|wxALL,5);

    topSizer->Add(topLeftSizer,1,wxALL,5);
    topSizer->Add(topRightSizer,1,wxALL,5);

    mainSizer->Add(topSizer);

    leftSizer->Add(amountToDistributeText,1,wxALL,5);
    leftSizer->Add(amountToDistributeTextCtrl,1,wxALL|wxEXPAND,5);

    leftSizer->Add(amountToReserveText,1,wxALL,5);
    leftSizer->Add(amountToReserveTextCtrl,1,wxALL|wxEXPAND,5);

    leftSizer->Add(noteText,1,wxALL,5);
    leftSizer->Add(noteTextCrtl,1,wxALL|wxEXPAND,5);

    mainSizer->Add(leftSizer,1,wxALL,5);
    mainSizer->Add(rightSizer,1,wxALL,5);

    bottomSizer->Add(confirmButton,1,wxALL|wxEXPAND,5);
    bottomSizer->Add(cancelButton,1,wxALL|wxEXPAND,5);
    mainSizer->Add(bottomSizer,1,wxALL|wxEXPAND,5);
    this->SetSizer(mainSizer);
    this->Layout();
    #ifdef __WXMAC__
        wxFont font = wxFont(14, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
        wxColour bgColor = wxColor(255,255,255);
        wxColour fgColor = wxColor(0,0,0);
        getAmountText->SetFont(font);
        getAmountText->SetForegroundColour(fgColor);
        amountOfDistributionText->SetFont(font);
        amountOfDistributionText->SetForegroundColour(fgColor);
        amountToReserveText->SetFont(font);
        amountToReserveText->SetForegroundColour(fgColor);
        amountToDistributeTextCtrl->SetFont(font);
        amountToDistributeTextCtrl->SetForegroundColour(fgColor);
        amountToReserveTextCtrl->SetFont(font);
        amountToReserveTextCtrl->SetForegroundColour(fgColor);
        noteText->SetFont(font);
        noteText->SetForegroundColour(fgColor);
        noteTextCrtl->SetFont(font);
        noteTextCrtl->SetForegroundColour(fgColor);
        confirmButton->SetFont(font);
        confirmButton->SetForegroundColour(fgColor);
        cancelButton->SetFont(font);
        cancelButton->SetForegroundColour(fgColor);
        getAmount->SetFont(font);
        getAmount->SetForegroundColour(fgColor);
        datePicker->SetFont(font);
        datePicker->SetForegroundColour(fgColor);
        datePickerText->SetFont(font);
        datePickerText->SetForegroundColour(fgColor);
        this->SetBackgroundColour(bgColor);
    #endif
}

double ExecuteDistribution::GetAmountOfDistribution(wxDateTime &date){
    double amount = 0;
    for(auto&distribution:m_asset->GetDistributions()){
        if(distribution.paid == false && distribution.distribution.first <= date){
            amount+=distribution.distribution.second;
        }
    }
    return amount;
}

void ExecuteDistribution::UpdateAmountText(double amount){
    getAmountText->SetLabel("Amount: "+utilities::formatDollarAmount(amount));
}

wxDateTime ExecuteDistribution::GetDate(){
    return datePicker->GetValue();
}

void ExecuteDistribution::OnGetAmount(wxCommandEvent &e){
    wxDateTime date = GetDate();
    double amount = GetAmountOfDistribution(date);
    UpdateAmountText(amount);
    this->Update();
    this->Layout();
}

void ExecuteDistribution::EnableGetAmountButton(){
    wxDateTime date = GetDate();
    double amount = GetAmountOfDistribution(date);
    if(amount >0){
        getAmount->Enable(true);
    }else{
        getAmount->Enable(false);
    }
}

void ExecuteDistribution::EnableConfirmButton(){
    wxDateTime date = GetDate();
    double amount = GetAmountOfDistribution(date);
    if(amount>0){
        double amountToDistribute = GetDistributionAmount();
        double amountToReserve = GetReserveAmount();
        if(amountToDistribute + amountToReserve == amount){
            confirmButton->Enable(true);
        }else{
            confirmButton->Enable(false);
        }
    }else{
        confirmButton->Enable(false);
    }
}

double ExecuteDistribution::GetDistributionAmount(){
    return wxAtof(amountToDistributeTextCtrl->GetValue());
}

double ExecuteDistribution::GetReserveAmount(){
    return wxAtof(amountToReserveTextCtrl->GetValue());
}

void ExecuteDistribution::OnSelectedDate(wxDateEvent &e){
    EnableGetAmountButton();
    UpdateAmountText(0);
}

void ExecuteDistribution::OnAmountsChanged(wxCommandEvent &e){
    EnableConfirmButton();
}

std::string ExecuteDistribution::GetNote(){
    return noteTextCrtl->GetValue().ToStdString();
}

