#include "ui/assetpopout/dialogs/moveDeploy.hpp"
#include "helpers/utilities.hpp"

MoveDeploy::MoveDeploy(wxWindow *parentWindow,std::shared_ptr<Asset> &asset):
    wxDialog(parentWindow, wxID_ANY, "Move Deploy", wxDefaultPosition, wxSize(300,300), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
    m_asset(asset){
        SetupLayout();
    }

    void MoveDeploy::SetupLayout(){
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
        datePickerText = new wxStaticText(this, wxID_ANY, "Enter Date of Move");
        mainSizer->Add(datePickerText, 0, wxALL | wxLEFT,5);
        mainSizer->Add(datePicker, 0, wxALL | wxEXPAND,5);

        //Direction of Move:
        wxArrayString choices;
        choices.Add("Reserve to Deploy");
        choices.Add("Deploy to Reserve");
        choices.Add("Reserve to ROC");
        directionOfMovement = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,choices);
        directionOfMovement->SetBackgroundColour(wxColor(0,0,0));
        directionOfMovementText = new wxStaticText(this, wxID_ANY, "Select Movement Type");
        mainSizer->Add(directionOfMovementText, 0, wxALL | wxLEFT, 5);
        mainSizer->Add(directionOfMovement, 0, wxALL|wxEXPAND, 5);

        //Amount Moved:
        amountCtrl = new wxTextCtrl(this, wxID_ANY);
        amountCtrl->SetValidator(validator);
        amountCtrl->Bind(wxEVT_TEXT, &MoveDeploy::OnAmountsChanged,this);

        amountText = new wxStaticText(this, wxID_ANY, "Enter Amount Moved");
        mainSizer->Add(amountText, 0, wxALL|wxLEFT,5);
        mainSizer->Add(amountCtrl, 0, wxALL|wxEXPAND, 5);
        //buttons
        wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        okayButton = new wxButton(this, wxID_OK, "Okay");
        okayButton->Enable(false);
        wxButton* cancelButton = new wxButton(this, wxID_CANCEL, "Cancel");

        buttonSizer->Add(okayButton, 0, wxALL, 5);
        buttonSizer->Add(cancelButton, 0, wxALL, 5);

        mainSizer->Add(buttonSizer,0,wxCENTER);

        this->SetSizer(mainSizer);
        #ifdef __WXMSW__
            wxFont font = wxFont(14, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
            wxColour color = wxColor(255,255,255);
            wxColor fgColor = wxColor(0,0,0);
            utilities::SetBackgroundColorForWindowAndChildren(this, color,fgColor);
            utilities::SetFontForWindowAndChildren(this, font);
        #endif
        #ifdef __WXMAC__
            wxFont font = wxFont(14, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
            wxColour color = wxColor(255,255,255);
            wxColour foregroundcolor = wxColor(0,0,0);
            datePickerText->SetFont(font);
            datePicker->SetFont(font);
            amountCtrl->SetFont(font);
            amountText->SetFont(font);
            directionOfMovementText->SetFont(font);
            directionOfMovement->SetFont(font);
            cancelButton->SetFont(font);
            okayButton->SetFont(font);
        #endif
        this->Layout();
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

    void MoveDeploy::OnAmountsChanged(wxCommandEvent &e){
        UpdateConfirmButton();
    }
    void MoveDeploy::UpdateConfirmButton(){
        wxString directionOfMove = directionOfMovement->GetStringSelection();
        if(directionOfMove == "Reserve to Deploy"){
            double availableReserve = m_asset->GetTotalAssetReserve();
            if(availableReserve >= GetAmountMoved()){
                okayButton->Enable(true);
            }else{
                okayButton->Enable(false);
            }
        }else if(directionOfMove == "Deploy to Reserve"){
            double availableDeploy = m_asset->GetTotalAssetDeployed();
            if(availableDeploy >= GetAmountMoved()){
                okayButton->Enable(true);
            }else{
                okayButton->Enable(false);
            }
        }else if(directionOfMove == "Reserve to ROC"){
            double availableReserve = m_asset->GetTotalAssetReserve();
            if(availableReserve >= GetAmountMoved()){
                okayButton->Enable(true);
            }else{
                okayButton->Enable(false);
            }
        }
    }