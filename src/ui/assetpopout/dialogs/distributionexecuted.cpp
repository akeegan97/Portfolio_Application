#include "ui/assetpopout/dialogs/distributionexecuted.hpp"
#include "helpers/utilities.hpp"

DistributionExecution::DistributionExecution(wxWindow *parentWindow, std::shared_ptr<Asset> &asset):
    wxDialog(parentWindow, wxID_ANY, "Distribute", wxDefaultPosition, wxDefaultSize,wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
    m_asset(asset){
        SetupLayout();
            wxFont font = wxFont(12, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
            wxColour color = wxColor(255,255,255);
            wxColour foregroundcolor = wxColor(0,0,0);
            #ifdef __WXMSW__
            utilities::SetBackgroundColorForWindowAndChildren(this, color, foregroundcolor);
            utilities::SetFontForWindowAndChildren(this, font);
            #endif
    }

void DistributionExecution::SetupLayout(){
    std::cout<<"Entering SetupLayout Function"<<std::endl;
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topLeftSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topMiddleSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topRightSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    wxString allowableNumbers = "0123457689. ";
    wxTextValidator numberValidator(wxFILTER_INCLUDE_CHAR_LIST);
    numberValidator.SetIncludes(wxArrayString(1, &allowableNumbers));
    PopulateQDistributions();
    PopulateChoiceArrays();
    for(const int &year : yearsInt){
        yearChoices.Add(wxString::Format(wxT("%d"),year));
    }
    for(const int&month:monthsInt){
        wxString qEndString;
        switch (month)
        {
        case 2:
            qEndString = wxT("Q1");
            break;
        case 5:
            qEndString = wxT("Q2");
            break;
        case 8:
            qEndString = wxT("Q3");
            break;
        case 11:
            qEndString = wxT("Q4");
            break;
        default:
            break;
        }
        if(!qEndString.IsEmpty() && qChoices.Index(qEndString)==wxNOT_FOUND){
            qChoices.Add(qEndString);
        }
    }
    yearChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, yearChoices);
    qChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, qChoices);
    yearText = new wxStaticText(this, wxID_ANY,"Year");
    qText = new wxStaticText(this, wxID_ANY,"Q");
    getDistribution = new wxButton(this, wxID_ANY,"Get Amount");
    getDistribution->Bind(wxEVT_BUTTON,&DistributionExecution::OnGetAmount,this);
    selectedDistributionAmount = new wxStaticText(this, wxID_ANY,"");
    distributionAmountText = new wxStaticText(this, wxID_ANY,"Amount To Distribute");
    distributionAmountTextCtrl = new wxTextCtrl(this, wxID_ANY);
    distributionAmountTextCtrl->SetValidator(numberValidator);
    reserveAmountText = new wxStaticText(this, wxID_ANY, "Amount To Reserve");
    reserveAmountCtrl = new wxTextCtrl(this, wxID_ANY);
    reserveAmountCtrl->SetValidator(numberValidator);

    confirmButton = new wxButton(this, wxID_OK,"Confirm");
    cancelButton = new wxButton(this, wxID_CANCEL, "Cancel");
    topLeftSizer->Add(yearText,1,wxALL|wxLEFT,5);
    topLeftSizer->Add(yearChoice,1,wxALL|wxEXPAND,5);
    topMiddleSizer->Add(qText,1,wxALL|wxLEFT,5);
    topMiddleSizer->Add(qChoice,1,wxALL|wxEXPAND,5);
    topRightSizer->Add(getDistribution, 1, wxALL|wxEXPAND,5);
    topRightSizer->Add(selectedDistributionAmount,1,wxALL|wxEXPAND,5);
    topSizer->Add(topLeftSizer, 1, wxALL,5);
    topSizer->Add(topMiddleSizer, 1, wxALL,5);
    topSizer->Add(topRightSizer, 1, wxALL, 5);
    mainSizer->Add(topSizer);
    leftSizer->Add(distributionAmountText,1,wxALL,5);
    leftSizer->Add(distributionAmountTextCtrl,1,wxALL|wxEXPAND,5);
    rightSizer->Add(reserveAmountText,1,wxALL,5);
    rightSizer->Add(reserveAmountCtrl,1,wxALL|wxEXPAND,5);
    mainSizer->Add(leftSizer,1,wxALL,5);
    mainSizer->Add(rightSizer,1,wxALL,5);
    bottomSizer->Add(confirmButton,1,wxALL|wxEXPAND,5);
    bottomSizer->Add(cancelButton,1,wxALL|wxEXPAND,5);
    mainSizer->Add(bottomSizer,1,wxALL|wxEXPAND);
    this->SetSizer(mainSizer);
    this->Layout();
    std::cout<<"Exiting SetupLayout Function"<<std::endl;
}

void DistributionExecution::PopulateChoiceArrays(){
    for(auto &distribution : qDistributions){
        yearsInt.insert(distribution.distribution.first.GetYear());
        monthsInt.insert(distribution.distribution.first.GetMonth());
    }
}
void DistributionExecution::PopulateQDistributions(){
    qDistributions.clear();
    m_asset->SortDistributions2();
    wxDateTime oldestDistributionDate = m_asset->GetDistributions().begin()->distribution.first;
    wxDateTime qStartDate = utilities::GetQuarterStartDate(oldestDistributionDate);
    wxDateTime newestDistributionDate = m_asset->GetDistributions().rbegin()->distribution.first;
    wxDateTime qEndDate = utilities::GetQuarterEndDate(newestDistributionDate);
    while(qStartDate < qEndDate){
        double distributionAmount = 0.0;
        wxDateTime distributionDate = utilities::GetQuarterEndDate(qStartDate);
        for(auto &distribution : m_asset->GetDistributions()){
            if(distribution.distribution.first >= qStartDate && distribution.distribution.first <= distributionDate){
                distributionAmount += distribution.distribution.second;
            }
        }
        Distribution newDistribution;
        newDistribution.distribution.first = distributionDate;
        newDistribution.distribution.second = distributionAmount;
        // bool distributionExists = std::any_of(m_asset->GetQuarterDistributions().begin(), 
        //                                     m_asset->GetQuarterDistributions().end(),
        //                                     [newDistribution](const std::pair<Distribution, bool> &existingDistributionPair){
        //                                         std::cout<<"LAMBDA EXECUTED: "<<std::endl;
        //                                         return existingDistributionPair.first.distribution.first == newDistribution.distribution.first;
        //                                     }); PRODUCED SEGFAULT LOOK INTO WHY
        bool distributionExists = false;
        for(const auto& qd : m_asset->GetQuarterDistributions()){
            if(qd.first.distribution.first==newDistribution.distribution.first){
                distributionExists = true;
            }
        }
        if(newDistribution.distribution.second > 0 && !distributionExists){
            qDistributions.push_back(newDistribution);
        }
        qStartDate = utilities::GetNextQuarterStartDate(qStartDate);
    }
}
Distribution DistributionExecution::GetSelectedDistribution(const int &selectedYear, const wxString &selectedQ){
    int month;
    if(selectedQ == wxT("Q1"))month = 2;
    else if(selectedQ == wxT("Q2"))month = 5;
    else if(selectedQ == wxT("Q3"))month = 8;
    else if(selectedQ == wxT("Q4"))month = 11;
    for(const auto& distribution: qDistributions){
        if(distribution.distribution.first.GetYear()==selectedYear && distribution.distribution.first.GetMonth() == month){
            return distribution;
        }
    }
    //may need to adjust return type to option<Distribution> but should never not return a distribution as is.
}

void DistributionExecution::OnGetAmount(wxCommandEvent &e){
    wxString year = yearChoice->GetStringSelection();
    int yearInt = wxAtoi(year);
    wxString month = qChoice->GetStringSelection();
    selectedDistribution = GetSelectedDistribution(yearInt, month);
    selectedDistributionAmount->SetLabel("Amount: "+utilities::formatDollarAmount(selectedDistribution.distribution.second));
    selectedDistributionAmount->SetForegroundColour(wxColor(0,0,0));
    this->Update();
}

Distribution DistributionExecution::GetDistribution(){
    return selectedDistribution;
}
