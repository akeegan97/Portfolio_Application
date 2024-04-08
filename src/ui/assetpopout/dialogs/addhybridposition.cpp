#include "ui/assetpopout/dialogs/addhybridposition.hpp"

AddHybridPositionDialog::AddHybridPositionDialog(wxWindow *parentWindow, std::shared_ptr<Asset> asset, Portfolio &portfolio):
    wxDialog(parentWindow, wxID_ANY,"HYBRID POSITION",wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER),
    m_portfolio(portfolio),
    m_asset(asset){
        SetupLayout();
        //#ifdef __WXMSW__
        wxFont font = wxFont(12, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
        wxColour color = wxColor(255,255,255);
        wxColour foregroundcolor = wxColor(0,0,0);
        utilities::SetBackgroundColorForWindowAndChildren(this, color, foregroundcolor);
        utilities::SetFontForWindowAndChildren(this, font);
       // #endif
    }


void AddHybridPositionDialog::SetupLayout(){
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxString allowableNumbers = "0123456789.";
    wxTextValidator numberValidator(wxFILTER_INCLUDE_CHAR_LIST);
    numberValidator.SetIncludes(wxArrayString(1,&allowableNumbers));
    for(const auto& position:m_asset->GetPositions()){
        wxString investorName = position->GetInvestorPtr()->GetName();
        size_t positionId = position->GetId();
        wxString committedAmountText = wxString::Format(wxT("%.2f"),position->GetCommitted());
        wxStaticText *nameLabel = new wxStaticText(this, wxID_ANY,investorName);
        wxStaticText *committedAmount = new wxStaticText(this, wxID_ANY,committedAmountText);
        wxTextCtrl* allocationInput = new wxTextCtrl(this, wxID_ANY);
        allocationInput->SetValidator(numberValidator);
        allocationInputs[positionId] = allocationInput;
        wxBoxSizer *rowSizer = new wxBoxSizer(wxHORIZONTAL);
        rowSizer->Add(nameLabel,1,wxALL,5);
        rowSizer->Add(committedAmount,1,wxALL,5);
        rowSizer->Add(allocationInput,1,wxALL|wxEXPAND,5);
        mainSizer->Add(rowSizer);
    }
    dateInvestedText = new wxStaticText(this, wxID_ANY,"Enter Invested Date");
    wxDateTime setDate = wxDateTime::Today();
    dateInvested = new wxDatePickerCtrl(this, wxID_ANY);
    dateInvested->SetValue(setDate);
    wxBoxSizer *dateSizer = new wxBoxSizer(wxHORIZONTAL);
    dateSizer->Add(dateInvestedText,1,wxALL,5);
    dateSizer->Add(dateInvested,1,wxALL|wxEXPAND,5);
    mainSizer->Add(dateSizer);
    additionalCapitalText = new wxStaticText(this, wxID_ANY,"Enter Additional Capital");
    additionalCapitalTextCtrl = new wxTextCtrl(this, wxID_ANY);
    additionalCapitalTextCtrl->SetValidator(numberValidator);
    wxBoxSizer *additionalCapitalSizer = new wxBoxSizer(wxVERTICAL);
    additionalCapitalSizer->Add(additionalCapitalText,1,wxALL,5);
    additionalCapitalSizer->Add(additionalCapitalTextCtrl,1,wxALL|wxEXPAND,5);
    mainSizer->Add(additionalCapitalSizer);
    noteForTransactionText = new wxStaticText(this, wxID_ANY,"Enter Note For Transaction");
    wxString allowableCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ .";
    wxTextValidator characterValidator(wxFILTER_INCLUDE_CHAR_LIST);
    characterValidator.SetIncludes(wxArrayString(1, &allowableCharacters));
    noteForTransactionTextCtrl = new wxTextCtrl(this, wxID_ANY);
    noteForTransactionTextCtrl->SetValidator(characterValidator);
    noteForTransactionTextCtrl->SetMaxLength(55);
    mainSizer->Add(noteForTransactionText,1,wxLEFT|wxALL,5);
    mainSizer->Add(noteForTransactionTextCtrl,1,wxALL|wxEXPAND,5);
    confirmButton = new wxButton(this, wxID_OK,"Confirm");
    cancelButton = new wxButton(this, wxID_CANCEL,"Cancel");
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(confirmButton,1,wxALL|wxEXPAND,5);
    buttonSizer->Add(cancelButton,1,wxALL|wxEXPAND,5);
    mainSizer->Add(buttonSizer);
    this->SetSizer(mainSizer);
    this->Layout();
}

wxDateTime AddHybridPositionDialog::GetDateValue(){
    return dateInvested->GetValue();
}

double AddHybridPositionDialog::GetAdditionalCapital(){
    return wxAtof(additionalCapitalTextCtrl->GetValue());
}

std::string AddHybridPositionDialog::GetNote(){
    return noteForTransactionTextCtrl->GetValue().ToStdString();
}

std::map<size_t,wxTextCtrl*> AddHybridPositionDialog::GetAllocations(){
    return allocationInputs;
}

