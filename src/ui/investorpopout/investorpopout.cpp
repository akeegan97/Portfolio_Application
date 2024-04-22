#include "ui/investorpopout/investorpopout.hpp"
#include <memory>
#include <unordered_set>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/file.h>

InvestorPopout::InvestorPopout(wxWindow *parentWindow, const wxString &title, const wxPoint &pos, const wxSize &size,
    Portfolio &port, std::shared_ptr<Investor> investor):
    wxFrame(parentWindow, wxID_ANY, title, pos, size),
    portfolio(port),
    investor(investor){
        SetupLayout();
        wxFont font = wxFont(12, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
        wxColour color = wxColor(255,255,255);
        wxColour foregroundcolor = wxColor(0,0,0);
        #ifdef __WXMSW__
        utilities::SetBackgroundColorForWindowAndChildren(this, color, foregroundcolor);
        utilities::SetFontForWindowAndChildren(this, font);
        #endif
};

void InvestorPopout::SetupLayout(){

    wxBoxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *bottomSizer = new wxBoxSizer(wxHORIZONTAL);

    investor->ClearPositionDisplays();
    std::unordered_set<std::shared_ptr<Asset>> processedAssets;
    for(auto position: investor->GetPositions()){
        std::shared_ptr<Asset> asset = position->GetAssetPointer();
        if(processedAssets.find(asset)==processedAssets.end()){
            processedAssets.insert(asset);
            auto positionDisplay = std::make_shared<InvestorAssetDisplay>(asset, investor);
            investor->AddPositionDisplay(positionDisplay);
        }
    }

    investorAssetDisplayVirtualListControl = new VListControl<std::shared_ptr<InvestorAssetDisplay>>(this, wxID_ANY, FromDIP(wxDefaultPosition), FromDIP(wxDefaultSize));
    investorAssetDisplayVirtualListControl->setItems(investor->GetAssetDisplaysNonConst());

    leftSizer->Add(investorAssetDisplayVirtualListControl, 1, wxEXPAND|wxALL,10);
    distributionsByAssetNoteBook = new wxAuiNotebook(this, wxID_ANY);
    processedAssets.clear();
    for(auto position:investor->GetPositions()){
        std::shared_ptr<Asset> asset = position->GetAssetPointer();
        if(processedAssets.find(asset)==processedAssets.end()){
            processedAssets.insert(asset);
            wxPanel* panel = new wxPanel(distributionsByAssetNoteBook, wxID_ANY);
            auto panelSizer = new wxBoxSizer(wxVERTICAL);
            distributionsByAssetNoteBook->AddPage(panel, asset->GetAssetName());
            std::vector<Distribution> mergedDistributions;
            std::vector<ManagementFee> mergedMgmtFees;
            std::vector<PromoteFee> mergedPromoteFees;
            for(auto position:investor->GetPositions()){
                if(position->GetAssetPointer()==asset){
                    for (auto& distribution : position->GetNetIncome()) {
                        auto it = std::find_if(mergedDistributions.begin(), mergedDistributions.end(),
                                            [&distribution](const Distribution& d) { return d.distribution.first == distribution.distribution.first; });
                        if (it != mergedDistributions.end()) {
                            it->distribution.second += distribution.distribution.second;
                        } else {
                            mergedDistributions.push_back(distribution);
                        }
                    }
                    for (auto& mgmtFee : position->GetManagementFees()) {
                        auto it = std::find_if(mergedMgmtFees.begin(), mergedMgmtFees.end(),
                                            [&mgmtFee](const ManagementFee& mf) { return mf.quarter == mgmtFee.quarter; });
                        if (it != mergedMgmtFees.end()) {
                            it->amount += mgmtFee.amount;
                        } else {
                            mergedMgmtFees.push_back(mgmtFee);
                        }
                    }
                    for (auto& pf : position->GetPromoteFees()) {
                        auto it = std::find_if(mergedPromoteFees.begin(), mergedPromoteFees.end(),
                                            [&pf](const PromoteFee& p) { return p.promotefee.first == pf.promotefee.first; });
                        if (it != mergedPromoteFees.end()) {
                            it->promotefee.second += pf.promotefee.second;
                        } else {
                            mergedPromoteFees.push_back(pf);
                        }
                    }
                }
            }
            VListControl<Distribution> *netIncomeVLC = new VListControl<Distribution>(panel, wxID_ANY, FromDIP(wxDefaultPosition), FromDIP(wxDefaultSize));
            netIncomeVLC->setItems(mergedDistributions);
            VListControl<ManagementFee> *mgmtFeeVLC = new VListControl<ManagementFee>(panel, wxID_ANY, FromDIP(wxDefaultPosition), FromDIP(wxDefaultSize));
            mgmtFeeVLC->setItems(mergedMgmtFees);
            VListControl<PromoteFee> *promoteFeeVLC = new VListControl<PromoteFee>(panel, wxID_ANY, FromDIP(wxDefaultPosition), FromDIP(wxDefaultSize));
            promoteFeeVLC->setItems(mergedPromoteFees);
            panelSizer->Add(netIncomeVLC, 1, wxALL|wxEXPAND,10);
            panelSizer->Add(mgmtFeeVLC, 1, wxALL|wxEXPAND,10);
            panelSizer->Add(promoteFeeVLC, 1, wxALL|wxEXPAND,10);
            #ifdef __WXMAC__
                wxFont font = wxFont(14, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
                wxColour bgColor = wxColor(255,255,255);
                wxColour fgColor = wxColor(0,0,0);  
                panel->SetBackgroundColour(bgColor);
                panel->SetForegroundColour(fgColor);
                netIncomeVLC->SetForegroundColour(fgColor);
                netIncomeVLC->SetBackgroundColour(bgColor);
                netIncomeVLC->SetFont(font);
                mgmtFeeVLC->SetForegroundColour(fgColor);
                mgmtFeeVLC->SetBackgroundColour(bgColor);
                mgmtFeeVLC->SetFont(font);
                promoteFeeVLC->SetFont(font);
                promoteFeeVLC->SetBackgroundColour(bgColor);
                promoteFeeVLC->SetForegroundColour(fgColor);

            #endif  
            panel->SetSizer(panelSizer);
            panel->Layout();
        }
    }
    rightSizer->Add(distributionsByAssetNoteBook, 1, wxALL, 10);
    auto *leftmiddleSizer = new wxBoxSizer(wxHORIZONTAL);
    startDate = new wxDatePickerCtrl(this, wxID_ANY);
    endDate = new wxDatePickerCtrl(this, wxID_ANY);
    DisplayStatement = new wxButton(this, wxID_ANY,"Display Statement");
    WriteToCsv = new wxButton(this, wxID_ANY,"Write To CSV /Documents/..");
    WriteToCsv->Bind(wxEVT_BUTTON, &InvestorPopout::OnWriteToCsvClick,this);
    DisplayStatement->Bind(wxEVT_BUTTON, &InvestorPopout::OnMakeStatementClick, this);
    leftmiddleSizer->Add(startDate,1, wxALL,5);
    leftmiddleSizer->Add(endDate,1,wxALL,5);
    leftmiddleSizer->Add(DisplayStatement,1,wxALL,5);
    leftmiddleSizer->Add(WriteToCsv,1,wxALL,5);
    leftSizer->Add(leftmiddleSizer,1,wxALL,5);
    auto *leftTextSizer = new wxBoxSizer(wxHORIZONTAL);

    beginningBalance = new wxStaticText(this, wxID_ANY," ");
    additionalCapital = new wxStaticText(this, wxID_ANY," ");
    returnedCapital = new wxStaticText(this, wxID_ANY," ");
    endingBalance = new wxStaticText(this, wxID_ANY," ");

    distributionsThisperiod = new wxStaticText(this, wxID_ANY," ");
    changeInValuationthisPeriod = new wxStaticText(this, wxID_ANY,"");
    returnAmountThisPeriod = new wxStaticText(this, wxID_ANY," ");
    returnPercentThisPeriod = new wxStaticText(this, wxID_ANY," ");

    paidCapital = new wxStaticText(this, wxID_ANY," ");
    returnedPrincipal = new wxStaticText(this, wxID_ANY," ");
    endingBalanceITD = new wxStaticText(this, wxID_ANY," ");
    totalDistributions = new wxStaticText(this, wxID_ANY," ");
    totalGain = new wxStaticText(this, wxID_ANY," ");
    irr = new wxStaticText(this, wxID_ANY," ");

    itdDetailsVLC = new VListControl<Details>(this, wxID_ANY,wxDefaultPosition, wxDefaultSize);
    auto leftLeftSizer = new wxBoxSizer(wxVERTICAL);
    auto leftMiddleSizer = new wxBoxSizer(wxVERTICAL);
    auto leftRightSizer = new wxBoxSizer(wxVERTICAL);

    leftLeftSizer->Add(beginningBalance,1,wxALL,5);
    leftLeftSizer->Add(additionalCapital,1, wxALL,5);
    leftLeftSizer->Add(returnedCapital,1, wxALL,5);
    leftLeftSizer->Add(endingBalance,1, wxALL,5);

    leftMiddleSizer->Add(changeInValuationthisPeriod,1,wxALL,5);
    leftMiddleSizer->Add(distributionsThisperiod,1, wxALL,5);
    leftMiddleSizer->Add(returnAmountThisPeriod,1,wxALL,5);
    leftMiddleSizer->Add(returnPercentThisPeriod,1,wxALL,5);

    leftRightSizer->Add(paidCapital,1,wxALL,5);
    leftRightSizer->Add(returnedPrincipal,1, wxALL,5);
    leftRightSizer->Add(endingBalanceITD,1,wxALL,5);
    leftRightSizer->Add(totalDistributions,1, wxALL,5);
    leftRightSizer->Add(totalGain,1, wxALL,5);
    leftRightSizer->Add(irr,1, wxALL,5);

    leftTextSizer->Add(leftLeftSizer,1,wxALL,5);
    leftTextSizer->Add(leftMiddleSizer,1,wxALL,5);
    leftTextSizer->Add(leftRightSizer,1,wxALL,5);
    leftSizer->Add(leftTextSizer,2,wxALL,5);

    leftSizer->Add(itdDetailsVLC, 1, wxEXPAND|wxALL,10);
    mainSizer->Add(leftSizer, 7, wxRIGHT,5);
    mainSizer->Add(rightSizer,3,wxALL|wxEXPAND,5);
    wxFont font = wxFont(14, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
    wxColour bgColor = wxColor(255,255,255);
    wxColour fgColor = wxColor(0,0,0);  
    investorAssetDisplayVirtualListControl->SetForegroundColour(fgColor);
    investorAssetDisplayVirtualListControl->SetBackgroundColour(bgColor);
    investorAssetDisplayVirtualListControl->SetFont(font);
    this->SetBackgroundColour(bgColor);
    distributionsByAssetNoteBook->SetForegroundColour(fgColor);
    distributionsByAssetNoteBook->SetFont(font);
    auto tabArt = new wxAuiSimpleTabArt();
    tabArt->SetColour(wxColor(255,255,255));
    tabArt->SetActiveColour(wxColor(255,255,255));
    tabArt->SetSelectedFont(font);
    tabArt->SetNormalFont(font);
    distributionsByAssetNoteBook->SetArtProvider(tabArt);
    distributionsByAssetNoteBook->Bind(wxEVT_AUINOTEBOOK_PAGE_CLOSE, [&](wxAuiNotebookEvent& event) {
    event.Veto();//stop users from closing tabs using GUI button
    });
    #ifdef __WXMAC__
    startDate->SetForegroundColour(fgColor);
    startDate->SetBackgroundColour(bgColor);
    startDate->SetFont(font);

    endDate->SetForegroundColour(fgColor);
    endDate->SetBackgroundColour(bgColor);
    endDate->SetFont(font);
    
    DisplayStatement->SetForegroundColour(fgColor);
    DisplayStatement->SetBackgroundColour(bgColor);
    DisplayStatement->SetFont(font);

    WriteToCsv->SetForegroundColour(fgColor);
    WriteToCsv->SetBackgroundColour(bgColor);
    WriteToCsv->SetFont(font);

    beginningBalance->SetForegroundColour(fgColor);
    beginningBalance->SetBackgroundColour(bgColor);
    beginningBalance->SetFont(font);

    additionalCapital->SetForegroundColour(fgColor);
    additionalCapital->SetBackgroundColour(bgColor);
    additionalCapital->SetFont(font);

    returnedCapital->SetForegroundColour(fgColor);
    returnedCapital->SetBackgroundColour(bgColor);
    returnedCapital->SetFont(font);
    
        endingBalance->SetForegroundColour(fgColor);
    endingBalance->SetBackgroundColour(bgColor);
    endingBalance->SetFont(font);

        distributionsThisperiod->SetForegroundColour(fgColor);
    distributionsThisperiod->SetBackgroundColour(bgColor);
    distributionsThisperiod->SetFont(font);

        changeInValuationthisPeriod->SetForegroundColour(fgColor);
    changeInValuationthisPeriod->SetBackgroundColour(bgColor);
    changeInValuationthisPeriod->SetFont(font);

        returnAmountThisPeriod->SetForegroundColour(fgColor);
    returnAmountThisPeriod->SetBackgroundColour(bgColor);
    returnAmountThisPeriod->SetFont(font);

        returnPercentThisPeriod->SetForegroundColour(fgColor);
    returnPercentThisPeriod->SetBackgroundColour(bgColor);
    returnPercentThisPeriod->SetFont(font);

        paidCapital->SetForegroundColour(fgColor);
    paidCapital->SetBackgroundColour(bgColor);
    paidCapital->SetFont(font);

        returnedPrincipal->SetForegroundColour(fgColor);
    returnedPrincipal->SetBackgroundColour(bgColor);
    returnedPrincipal->SetFont(font);

        endingBalanceITD->SetForegroundColour(fgColor);
    endingBalanceITD->SetBackgroundColour(bgColor);
    endingBalanceITD->SetFont(font);

        totalDistributions->SetForegroundColour(fgColor);
    totalDistributions->SetBackgroundColour(bgColor);
    totalDistributions->SetFont(font);

        totalGain->SetForegroundColour(fgColor);
    totalGain->SetBackgroundColour(bgColor);
    totalGain->SetFont(font);

        irr->SetForegroundColour(fgColor);
    irr->SetBackgroundColour(bgColor);
    irr->SetFont(font);

        itdDetailsVLC->SetForegroundColour(fgColor);
    itdDetailsVLC->SetBackgroundColour(bgColor);
    itdDetailsVLC->SetFont(font);


    #endif
    this->SetSizer(mainSizer);
    this->Layout();  
}

void InvestorPopout::WriteStatementDetails(){
    wxDateTime statementBegin = startDate->GetValue();
    wxDateTime statementEnd = endDate->GetValue();
    Statement clientStatement(statementBegin, statementEnd,investor);
    m_clientStatement = clientStatement;

    beginningBalance->SetLabel("Beginning Balance: "+utilities::formatDollarAmount(m_clientStatement.GetBeginningBalance()));
    additionalCapital->SetLabel("Additional Capital: "+utilities::formatDollarAmount(m_clientStatement.GetAdditionalCapital()));
    returnedCapital->SetLabel("Returned Capital: "+utilities::formatDollarAmount(m_clientStatement.GetReturnedCapital()));
    endingBalance->SetLabel("Ending Balance: "+utilities::formatDollarAmount(m_clientStatement.GetEndingBalance()));

    changeInValuationthisPeriod->SetLabel("Change In Valuation This Period: "+utilities::formatDollarAmount(m_clientStatement.GetChangeInValuationThisPeriod()));
    distributionsThisperiod->SetLabel("Distributions This Period: "+utilities::formatDollarAmount(m_clientStatement.GetNetIncomeThisPeriod()));
    returnAmountThisPeriod->SetLabel("Return This Period: "+utilities::formatDollarAmount(m_clientStatement.GetReturnAmountThisPeriod()));
    returnPercentThisPeriod->SetLabel("Return % This Period: "+utilities::FormatPercentage(m_clientStatement.GetReturnPercentThisPeriod()));

    paidCapital->SetLabel("Paid Capital: "+utilities::formatDollarAmount(m_clientStatement.GetPaidCapital()));
    returnedPrincipal->SetLabel("Returned Principal: "+utilities::formatDollarAmount(m_clientStatement.GetReturnedPrincipal()));
    endingBalanceITD->SetLabel("Ending Balance: "+utilities::formatDollarAmount(m_clientStatement.GetEndingBalance()));
    totalDistributions->SetLabel("Total Distributions: "+utilities::formatDollarAmount(m_clientStatement.GetTotalNetIncomeToEndDate()));
    totalGain->SetLabel("Total Return: "+utilities::formatDollarAmount(m_clientStatement.GetTotalGain()));
    irr->SetLabel("IRR: "+utilities::FormatPercentage(m_clientStatement.GetCombinedIrr()));

    itdDetailsVLC->setItems(m_clientStatement.GetDetails());
}

void InvestorPopout::OnMakeStatementClick(wxCommandEvent &e){
    WriteStatementDetails();
    this->Update();
    this->Refresh();
    this->Layout();
}

void InvestorPopout::OnWriteToCsvClick(wxCommandEvent &e){
    wxString fromDate = startDate->GetValue().FormatISODate();
    wxString toDate = endDate->GetValue().FormatISODate();
    wxString clientName = investor->GetName();
    wxString fileName = clientName+" Statement From "+fromDate+" To "+toDate+".csv";
    wxFileName filePath(wxStandardPaths::Get().GetDocumentsDir(),fileName);
    wxFile file(filePath.GetFullPath(),wxFile::write);
    if(file.IsOpened()){
        file.Write(m_clientStatement.WriteCsv());
    }
    file.Close();
}