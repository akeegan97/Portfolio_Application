#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include "helpers/customevents.hpp"
#include "ui/assetpopout/assetpopout.hpp"
#include "ui/assetpopout/dialogs/investorPositionEdit.hpp"
#include "ui/assetpopout/dialogs/distributiondialog.hpp"
#include "ui/assetpopout/dialogs/moveDeploy.hpp"
#include "ui/assetpopout/dialogs/valuationdialog.hpp"
#include "ui/assetpopout/dialogs/setassetdeployreservedialog.hpp"
#include "ui/assetpopout/dialogs/executedistribution.hpp"
#include "ui/transactionpopout/transactionpopout.hpp"
AssetPopout::AssetPopout(wxWindow *parentWindow, const wxString &title, const wxPoint &pos, const wxSize &size, Portfolio &port, std::shared_ptr<Asset> asset)
    : wxFrame(parentWindow, wxID_ANY, title, pos, size),
        portfolio(port),
        asset(asset),
        investorPositionDisplayVirtualListControl(nullptr),
        valuationListControl(nullptr){
            SetupLayout();
            UpdateDisplayTextValues();
            wxFont font = wxFont(12, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
            wxColour color = wxColor(255,255,255);
            wxColour foregroundcolor = wxColor(0,0,0);
            #ifdef __WXMSW__
            utilities::SetBackgroundColorForWindowAndChildren(this, color, foregroundcolor);
            utilities::SetFontForWindowAndChildren(this, font);
            #endif
            Bind(wxEVT_CLOSE_WINDOW, &AssetPopout::OnClose, this);
            UpdateExecuteDistributionButton();
        };

void AssetPopout::SetupLayout(){
    //sizers
    wxSize vlcInitialSize = FromDIP(wxSize(400, 150));
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *middleChartSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *middleVLCSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    asset->ClearInvestorPositionDisplays();
    for(auto& position : asset->GetPositionsForIDP()){
        auto investorPositionDisplay = std::make_shared<InvestorPositionDisplay>(position);
        asset->AddInvestorPositionDisplay(investorPositionDisplay);
    }
    investorPositionDisplayVirtualListControl = new VListControl<std::shared_ptr<InvestorPositionDisplay>>(this,wxID_ANY,FromDIP(wxDefaultPosition),vlcInitialSize);
    if(!asset->GetIPDVector().empty()){
        investorPositionDisplayVirtualListControl->setItems(asset->GetIPDVector());
    }
    topSizer->Add(investorPositionDisplayVirtualListControl, wxALL| wxEXPAND,5);
    mainSizer->Add(topSizer,1,wxALL|wxEXPAND,5);

    chartPanelHolderPanel = new wxPanel(this, wxID_ANY);
    //change colors here
    wxBoxSizer * vdChartPanelSizer = new wxBoxSizer(wxVERTICAL);
    chartPanelHolderPanel->SetSizer(vdChartPanelSizer);
    wxChartPanel *valuationDeployChartPanel = new wxChartPanel(chartPanelHolderPanel, wxID_ANY);
    //change color here
    if(valuationDeployChartPanel->GetChart()!= nullptr){
        delete valuationDeployChartPanel->GetChart();
    }
    Chart* valuationDeployChart = PopulateDrawChartValuationDeploy();//change colors inside function
    if(valuationDeployChart!=nullptr){
        valuationDeployChartPanel->SetChart(valuationDeployChart);
        vdChartPanelSizer->Add(valuationDeployChartPanel,1,wxALL|wxEXPAND,5);
    }
    
    distributionChartPanelHolder = new wxPanel(this, wxID_ANY);
    wxBoxSizer *dChartPanelSizer = new wxBoxSizer(wxVERTICAL); 
    distributionChartPanelHolder->SetSizer(dChartPanelSizer);
    wxChartPanel *distributionChartPanel = new wxChartPanel(distributionChartPanelHolder, wxID_ANY);
    if(distributionChartPanel->GetChart()!=nullptr){
        delete distributionChartPanel->GetChart();
    }
    Chart* distributionChart = PopulateDrawChartDistribution();
    if(distributionChart!=nullptr){
        distributionChartPanel->SetChart(distributionChart);
        dChartPanelSizer->Add(distributionChartPanel,1,wxALL|wxEXPAND,5);
    }
    middleChartSizer->Add(chartPanelHolderPanel,5,wxALL|wxEXPAND,5);
    middleChartSizer->Add(distributionChartPanelHolder,5,wxALL|wxEXPAND,5);

    mainSizer->Add(middleChartSizer,6,wxALL|wxEXPAND,5);

    //add VLCs for Valuations/Distributions removal of Events
    valuationListControl = new VListControl<Valuation>(this, wxID_ANY,FromDIP(wxDefaultPosition),vlcInitialSize);
    if(!asset->GetValuations().empty()){
        valuationListControl->setItems(asset->GetValuationsNonConst());
        valuationListControl->Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, &AssetPopout::OnValuationEdit, this);
    }
    distributionListControl = new VListControl<Distribution>(this, wxID_ANY,FromDIP(wxDefaultPosition),vlcInitialSize);
    if(!asset->GetDistributions().empty()){
        distributionListControl->setItems(asset->GetDistributionsNonConst());
        distributionListControl->Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, &AssetPopout::OnDistributionEdit, this);
    }
    middleVLCSizer->Add(valuationListControl,5,wxALL|wxEXPAND,5);
    middleVLCSizer->Add(distributionListControl,5,wxALL|wxEXPAND,5);

    mainSizer->Add(middleVLCSizer, 1, wxALL|wxEXPAND, 5);

    wxBoxSizer *staticTextSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *rHalfTextSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *lHalfTextSizer = new wxBoxSizer(wxVERTICAL);

    //static texts
    numInvestorsText = new wxStaticText(this, wxID_ANY,"");//
    totalCommittedText = new wxStaticText(this, wxID_ANY,"");//
    totalPaidText = new wxStaticText(this, wxID_ANY, "");
    totalReturnedCapitalText = new wxStaticText(this, wxID_ANY, "");
    
    totalDeployedCapitalText = new wxStaticText(this, wxID_ANY, "");//
    totalReserveCapitalText = new wxStaticText(this, wxID_ANY, "");//
    totalMgmtFeesGeneratedText = new wxStaticText(this, wxID_ANY, "");//
    totalMgmtFeesDueText = new wxStaticText(this, wxID_ANY, "");///
    totalPromoteFeesGeneratedText = new wxStaticText(this, wxID_ANY,"");//
    assetIRR = new wxStaticText(this, wxID_ANY, "");//

    lHalfTextSizer->Add(assetIRR, 1, wxEXPAND,3);
    lHalfTextSizer->Add(numInvestorsText,1,wxEXPAND,3);
    lHalfTextSizer->Add(totalCommittedText, 1, wxEXPAND,3);
    lHalfTextSizer->Add(totalDeployedCapitalText, 1, wxEXPAND,3);
    lHalfTextSizer->Add(totalReserveCapitalText, 1, wxEXPAND,3);

    rHalfTextSizer->Add(totalPromoteFeesGeneratedText,1,wxEXPAND,3);
    rHalfTextSizer->Add(totalMgmtFeesGeneratedText,1,wxEXPAND,3);
    rHalfTextSizer->Add(totalMgmtFeesDueText,1,wxEXPAND,3);
    rHalfTextSizer->Add(totalPaidText, 1, wxEXPAND,3);
    rHalfTextSizer->Add(totalReturnedCapitalText,1,wxEXPAND,3);
    
    staticTextSizer->Add(lHalfTextSizer, 1, wxALL|wxEXPAND,3);
    staticTextSizer->Add(rHalfTextSizer, 1, wxALL|wxEXPAND,3);
    bottomSizer->Add(staticTextSizer, 5, wxALL|wxEXPAND,3);

    //buttons
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxVERTICAL);
    addDistributionButton = new wxButton(this, wxID_ANY, "Distribution");
    addDistributionButton->Bind(wxEVT_BUTTON, &AssetPopout::OnAddDistributionClicked,this);
    assetLevelMovementOfCapitalButton = new wxButton(this, wxID_ANY, "Movement Of Capital");
    assetLevelMovementOfCapitalButton->Bind(wxEVT_BUTTON, &AssetPopout::OnCapitalMovement, this);
    addValuationButton = new wxButton(this, wxID_ANY, "Valuation");
    addValuationButton->Bind(wxEVT_BUTTON, &AssetPopout::OnAddValuation, this);
    addPositionButton = new wxButton(this, wxID_ANY, "Add New Position");
    addPositionButton->Bind(wxEVT_BUTTON, &AssetPopout::OnAddPosition, this);
    executeDistributionButton = new wxButton(this, wxID_ANY,"Execute Distribution");
    executeDistributionButton->Bind(wxEVT_BUTTON, &AssetPopout::OnExecuteDistribution, this);
    viewTransactionsButton = new wxButton(this, wxID_ANY,"View Transactions");
    viewTransactionsButton->Bind(wxEVT_BUTTON, &AssetPopout::OnTransactionClick, this);
    buttonSizer->Add(addDistributionButton);
    buttonSizer->Add(assetLevelMovementOfCapitalButton);
    buttonSizer->Add(addValuationButton);
    buttonSizer->Add(addPositionButton);
    buttonSizer->Add(executeDistributionButton);
    buttonSizer->Add(viewTransactionsButton);

    bottomSizer->Add(buttonSizer,5,wxALL|wxEXPAND,3);

    mainSizer->Add(bottomSizer,1,wxALL|wxEXPAND,5);

    #ifdef __WXMAC__
    wxFont font = wxFont(14, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
    wxColour bgColor = wxColor(255,255,255);
    wxColour fgColor = wxColor(0,0,0);  
    this->SetBackgroundColour(bgColor);

    investorPositionDisplayVirtualListControl->SetFont(font);
    investorPositionDisplayVirtualListControl->SetBackgroundColour(bgColor);
    investorPositionDisplayVirtualListControl->SetForegroundColour(fgColor);  

    valuationListControl->SetFont(font);
    valuationListControl->SetBackgroundColour(bgColor);
    valuationListControl->SetForegroundColour(fgColor);

    distributionListControl->SetFont(font);
    distributionListControl->SetBackgroundColour(bgColor);
    distributionListControl->SetForegroundColour(fgColor);

    addDistributionButton->SetForegroundColour(fgColor);
    addDistributionButton->SetBackgroundColour(bgColor);
    addDistributionButton->SetFont(font);
    addValuationButton->SetForegroundColour(fgColor);
    addValuationButton->SetBackgroundColour(bgColor);
    addValuationButton->SetFont(font);
    addPositionButton->SetForegroundColour(fgColor);
    addPositionButton->SetBackgroundColour(bgColor);
    addPositionButton->SetFont(font);
    assetLevelMovementOfCapitalButton->SetForegroundColour(fgColor);
    assetLevelMovementOfCapitalButton->SetBackgroundColour(bgColor);
    assetLevelMovementOfCapitalButton->SetFont(font);
    executeDistributionButton->SetForegroundColour(fgColor);
    executeDistributionButton->SetBackgroundColour(bgColor);
    executeDistributionButton->SetFont(font);
    viewTransactionsButton->SetForegroundColour(fgColor);
    viewTransactionsButton->SetBackgroundColour(bgColor);
    viewTransactionsButton->SetFont(font);

    assetIRR->SetForegroundColour(fgColor);
    assetIRR->SetFont(font);
    numInvestorsText->SetForegroundColour(fgColor);
    numInvestorsText->SetFont(font);
    totalCommittedText->SetForegroundColour(fgColor);
    totalCommittedText->SetFont(font);
    totalDeployedCapitalText->SetForegroundColour(fgColor);
    totalDeployedCapitalText->SetFont(font);
    totalReserveCapitalText->SetForegroundColour(fgColor);
    totalReserveCapitalText->SetFont(font);
    totalPromoteFeesGeneratedText->SetForegroundColour(fgColor);
    totalPromoteFeesGeneratedText->SetFont(font);
    totalMgmtFeesDueText->SetFont(font);
    totalMgmtFeesDueText->SetForegroundColour(fgColor);
    totalMgmtFeesGeneratedText->SetFont(font);
    totalMgmtFeesGeneratedText->SetForegroundColour(fgColor);
    totalPaidText->SetForegroundColour(fgColor);
    totalPaidText->SetFont(font);
    totalReturnedCapitalText->SetForegroundColour(fgColor);
    totalReturnedCapitalText->SetFont(font);

    #endif
    
    this->SetSizer(mainSizer);
    this->Layout();
}

void AssetPopout::UpdateDisplayTextValues(){
    asset->TriggerUpdateDerivedValues();
    double numInvestors = asset->GetTotalInvestors();
    double totalPaid = 0.0;
    for(const auto&pos:asset->GetPositions()){
        totalPaid+=pos->GetPaid();
    }
    std::string formattedTotalPaid = utilities::formatDollarAmount(totalPaid);
    double totalDeployed = asset->GetTotalAssetDeployed();
    std::string formattedTotalDeployed = utilities::formatDollarAmount(totalDeployed);
    double totalCommitted = asset->GetTotalCommitted();
    std::string formattedCommitted = utilities::formatDollarAmount(totalCommitted);
    double totalReserveCapital = asset->GetTotalAssetReserve();
    std::string formattedReserve = utilities::formatDollarAmount(totalReserveCapital);
    double totalReturnedCapital = asset->GetTotalReturnOfCapital();
    std::string formattedReturnedCapital = utilities::formatDollarAmount(totalReturnedCapital);
    double totalPromoteFees = asset->GetTotalPromoteFeesEarned();
    std::string formatedPromoteFees = utilities::formatDollarAmount(totalPromoteFees);
    double totalMgmtFees = asset->GetTotalMgmtFeesEarned();
    std::string formatedTotalMgmtFees = utilities::formatDollarAmount(totalMgmtFees);
    double totalMgmtFeesDue = asset->GetTotalMgmtFeesDue();
    std::string formattedTotalMgmtFeesDue = utilities::formatDollarAmount(totalMgmtFeesDue);
    std::string assetIRRformated = utilities::FormatPercentage(asset->GetIrr());


    assetIRR->SetLabel("Asset Gross IRR: "+assetIRRformated);
    assetIRR->SetForegroundColour(wxColor(0,0,0));

    totalCommittedText->SetLabel("Total Subscribed Amount: "+formattedCommitted);
    totalCommittedText->SetForegroundColour(wxColor(0,0,0));

    totalPaidText->SetLabel("Total Paid Amount: "+formattedTotalPaid);
    totalPaidText->SetForegroundColour(wxColor(0,0,0));

    totalDeployedCapitalText->SetLabel("Total Deployed Amount: "+formattedTotalDeployed);
    totalDeployedCapitalText->SetForegroundColour(wxColor(0,0,0));

    numInvestorsText->SetLabel(wxString::Format("Total Investors: %.2f", numInvestors));
    numInvestorsText->SetForegroundColour(wxColor(0,0,0));

    totalReserveCapitalText->SetLabel("Total Reserve Amount: "+formattedReserve);
    totalReserveCapitalText->SetForegroundColour(wxColor(0,0,0));

    totalReturnedCapitalText->SetLabel("Total Returned Amount: "+formattedReturnedCapital);
    totalReturnedCapitalText->SetForegroundColour(wxColor(0,0,0));

    totalMgmtFeesGeneratedText->SetLabel("Total Management Fees Earned: "+formatedTotalMgmtFees);
    totalMgmtFeesGeneratedText->SetForegroundColour(wxColor(0,0,0));

    totalMgmtFeesDueText->SetLabel("Total Management Fees Due: "+ formattedTotalMgmtFeesDue);
    totalMgmtFeesDueText->SetForegroundColour(wxColor(0,0,0));

    totalPromoteFeesGeneratedText->SetLabel("Total Promote Fees Earned: "+formatedPromoteFees);
    totalPromoteFeesGeneratedText->SetForegroundColour(wxColor(0,0,0));
}

void AssetPopout::OnAddDistributionClicked(wxCommandEvent &e){
    wxDateTime today = wxDateTime::Today();
    double zero = 0.0;
    DistributionDialog addDistroWindow(this,false,today, zero);
    int retValue = addDistroWindow.ShowModal();
    if(retValue == wxID_OK){
        Distribution newDistribution;
        newDistribution.distribution.first = addDistroWindow.GetDistributionDate();
        newDistribution.distribution.second = addDistroWindow.GetDistributionAmount();
        newDistribution.paid = false;
        asset->AddDistribution(newDistribution);
        distributionListControl->setItems(asset->GetDistributions());
        distributionListControl->Update();
        UpdateDisplayTextValues();
        UpdateChartDistribution();
        this->Refresh();
        UpdateExecuteDistributionButton();
        wxDateTime transactionDate = newDistribution.distribution.first;
        double amount = newDistribution.distribution.second;
        std::shared_ptr<Position> positionPtr = nullptr;
        std::string type = "Gross Distribution";
        std::string name = asset->GetAssetName().ToStdString();
        std::string note = addDistroWindow.GetNote();
        Transaction newTransaction(transactionDate,name,amount,type,note);
        asset->AddNewTransaction(newTransaction);
    }
}

void AssetPopout::OnCapitalMovement(wxCommandEvent &e){
    MoveDeploy DeployMovementWindow(this,asset);
    int retValue = DeployMovementWindow.ShowModal();
    if(retValue == wxID_OK){
        wxDateTime dateOfMovement = DeployMovementWindow.GetDate();
        double amountMoved = DeployMovementWindow.GetAmountMoved();
        wxString selectedMovementDirection = DeployMovementWindow.GetSelectedMovementDirection();
        std::string note = DeployMovementWindow.GetNote();
        if(selectedMovementDirection == "Reserve to Deploy"){
            asset->MoveReserveToDeploy(dateOfMovement,amountMoved);//could maybe call this from inside the MoveReserveToDeploy function
            asset->SetCurrentValue();
            asset->SetPositionValues();   
            wxDateTime transactionDate = dateOfMovement;
            double amount = amountMoved;
            std::string type = "Reserve->Deploy";
            std::string name = asset->GetAssetName().ToStdString();
            Transaction newTransaction(transactionDate,name,amount,type,note);
            asset->AddNewTransaction(newTransaction);
            for(auto pos : asset->GetPositionsForIDP()){
                pos->UpdateManagementFees(dateOfMovement);
            }
        }else if(selectedMovementDirection == "Deploy to Reserve"){
            asset->MoveDeployToReserve(dateOfMovement,amountMoved);
            asset->SetCurrentValue();//could maybe call this from inside the MoveDeployToReserve function
            asset->SetPositionValues();
            wxDateTime transactionDate = dateOfMovement;
            double amount = amountMoved;
            std::string type = "Deploy->Reserve";
            std::string name = asset->GetAssetName().ToStdString();
            Transaction newTransaction(transactionDate,name,amount,type,note);
            asset->AddNewTransaction(newTransaction);
            for(auto pos : asset->GetPositionsForIDP()){
                pos->UpdateManagementFees(dateOfMovement);
            }
        }else if(selectedMovementDirection == "Reserve to ROC"){
            asset->MoveReserveToReturnOfCapital(dateOfMovement,amountMoved);
            asset->UpdateCommitted();
            asset->SetPositionValues();
            wxDateTime transactionDate = dateOfMovement;
            double amount = amountMoved;
            std::string type = "Reserve->Return Of Capital";
            std::string name = asset->GetAssetName().ToStdString();
            Transaction newTransaction(transactionDate,name,amount,type,note);
            asset->AddNewTransaction(newTransaction);
        }
        investorPositionDisplayVirtualListControl->Refresh();
        UpdateChartValuationDeploy();
        asset->TriggerUpdateDerivedValues();
        UpdateDisplayTextValues();
        this->Refresh();
    }else if(retValue == wxID_CANCEL){
        //do nothing close
    }
}

void AssetPopout::OnAddValuation(wxCommandEvent &e){
    wxDateTime today = wxDateTime::Today();
    double setValue = 0.0;
    ValuationDialog addValuationDialog(this, false,today, setValue);
    int retVal = addValuationDialog.ShowModal();
    if(retVal == wxID_OK){
        wxDateTime valuationDate = addValuationDialog.GetDate();
        double valuationAmount = addValuationDialog.GetValuation();
        Valuation newValuation;
        newValuation.valuationDate = valuationDate;
        newValuation.valuation = valuationAmount;
        asset->AddNewValuation(newValuation);
        valuationListControl->setItems(asset->GetValuationsNonConst());
        asset->SetCurrentValue();
        asset->SetPositionValues();
        investorPositionDisplayVirtualListControl->Refresh();
        portfolio.ValuationDialog();
        UpdateChartValuationDeploy();
        asset->TriggerUpdateDerivedValues();
        UpdateDisplayTextValues();
        this->Layout();
        wxDateTime transactionDate = valuationDate;
        double amount = valuationAmount;
        std::string note = addValuationDialog.GetNote();
        std::string name = asset->GetAssetName().ToStdString();
        std::string type = "Valuation";
        Transaction newTransaction(transactionDate,name,amount,type,note);
        asset->AddNewTransaction(newTransaction);
    }else if(retVal == wxID_CANCEL){

    }
    AllowBindingOnValuationLC();
}

void AssetPopout::OnClose(wxCloseEvent &e){
    wxCommandEvent evt(ASSET_POPOUT_CLOSED, wxID_ANY);
    wxPostEvent(GetParent(), evt);
    e.Skip();
}

void AssetPopout::OnDistributionEdit(wxListEvent &e){
    //simply edits the distribution that is selected, does not effect positions anymore
    long listIndex = e.GetIndex();
    long dataIndex = distributionListControl->orderedIndices[listIndex];
    auto selectedDistribution = asset->GetDistributionsNonConst()[dataIndex];
    wxDateTime distributionDate = selectedDistribution.distribution.first;
    double distributionAmount = selectedDistribution.distribution.second;
    bool paid = selectedDistribution.paid;
    std::string type = "Gross Distribution";
    double amount = distributionAmount;
    wxDateTime date = distributionDate;
    asset->RemoveTransaction(type,date,amount);
    DistributionDialog distributionEditwindow(this,true, distributionDate, distributionAmount);
    int retVal = distributionEditwindow.ShowModal();
    if(retVal == wxID_OK){
        selectedDistribution.distribution.first = distributionEditwindow.GetDistributionDate();
        selectedDistribution.distribution.second = distributionEditwindow.GetDistributionAmount();
        selectedDistribution.paid = paid;
        asset->RemoveDistribution(dataIndex);
        asset->AddDistribution(selectedDistribution);
        distributionListControl->setItems(asset->GetDistributionsNonConst());
        distributionListControl->Update();
        double amount = selectedDistribution.distribution.second;
        std::string name = asset->GetAssetName().ToStdString();
        std::string type = "Gross Distribution";
        std::string note = distributionEditwindow.GetNote();
        Transaction newTransaction(date,name,amount,type,note);
        UpdateDisplayTextValues();
        UpdateChartDistribution();
        this->Refresh();
    }else if(retVal == MY_CUSTOM_DELETE_CODE){
        if(dataIndex >= 0 && dataIndex < asset->GetDistributions().size()){
            asset->RemoveDistribution(dataIndex);
            wxDateTime date = selectedDistribution.distribution.first;
            double amount = selectedDistribution.distribution.second;
            std::string type = "Gross Distribution";
            asset->RemoveTransaction(type,date,amount);
            UpdateExecuteDistributionButton();
        }
        distributionListControl->setItems(asset->GetDistributions());
        distributionListControl->Update();
        UpdateDisplayTextValues();
        UpdateChartDistribution();
        this->Refresh();
    }
}

void AssetPopout::OnValuationEdit(wxListEvent &e){
    long listIndex = e.GetIndex();
    long dataIndex = valuationListControl->orderedIndices[listIndex];
    Valuation valuationToEdit = asset->GetValuationsNonConst()[dataIndex];
    std::cout<<"List Index: "<<listIndex<<" Data Index: "<<dataIndex<<std::endl;
    wxDateTime setDate  = valuationToEdit.valuationDate;
    double setValue = valuationToEdit.valuation;
    std::string type = "Valuation";
    wxDateTime date = valuationToEdit.valuationDate;
    double amount = valuationToEdit.valuation;
    asset->RemoveTransaction(type,date,amount);
    ValuationDialog valuationWindow(this, true, setDate, setValue);
    int retValue = valuationWindow.ShowModal();
    if(retValue == wxID_OK){
        valuationToEdit.valuation = valuationWindow.GetValuation();
        valuationToEdit.valuationDate = valuationWindow.GetDate();
        asset->RemoveValuation(dataIndex);
        asset->AddNewValuation(valuationToEdit);
        valuationListControl->setItems(asset->GetValuationsNonConst());
        valuationListControl->Update();
        asset->SetCurrentValue();
        asset->SetPositionValues();
        investorPositionDisplayVirtualListControl->Refresh();
        UpdateChartValuationDeploy();
        UpdateDisplayTextValues();
        wxDateTime date = valuationToEdit.valuationDate;
        double amount = valuationToEdit.valuation;
        std::string name = asset->GetAssetName().ToStdString();
        std::string note = valuationWindow.GetNote();
        std::string type = "Valuation";
        Transaction newTransaction(date,name,amount,type,note);
        asset->AddNewTransaction(newTransaction);
        this->Refresh();
    }else if(retValue == MY_VALUATION_DELETE_CODE){
        if(dataIndex>=0 && dataIndex < asset->GetValuations().size()){
            asset->RemoveValuation(dataIndex);
        }
        valuationListControl->setItems(asset->GetValuations());
        valuationListControl->Update();
        asset->SetCurrentValue();
        asset->SetPositionValues();
        investorPositionDisplayVirtualListControl->Refresh();
        UpdateChartValuationDeploy();
        UpdateDisplayTextValues();
        this->Refresh();
    }
    this->Refresh();
    this->Layout();
    AllowBindingOnValuationLC();
}

Chart* AssetPopout::PopulateDrawChartValuationDeploy(){
    wxPen *valuationPen = new wxPen(wxColor(3, 252, 123),2);
    wxPen *deploymentPen = new wxPen(wxColor(238, 3, 255),2);
    wxColor *gridLineColor = new wxColor(0,0,0);
    wxColor *textColor = new wxColor(5, 45, 247);
    wxPen *gridLinePen = new wxPen(*gridLineColor);

    asset->PopulateValuationsDeploymentsForPlotting();
    if (asset->GetValuationsForPlotting().empty() && asset->GetDeploymentsForPlotting().empty()) {
        return nullptr;
    }

    std::set<wxDateTime> allDates;
    for (const auto& val : asset->GetValuationsForPlotting()) {
        allDates.insert(val.first);
    }
    for (const auto& dep : asset->GetDeploymentsForPlotting()) {
        allDates.insert(dep.first);
    }
    std::vector<std::pair<wxDateTime, double>> newValuations, newDeployments;
    double lastValuation = 0.0, lastDeployment = 0.0;
    bool isFirstValuationHandled = false; 

    for (const auto& date : allDates) {
        auto valIt = std::find_if(asset->GetValuationsForPlotting().begin(), asset->GetValuationsForPlotting().end(),
                                [date](const std::pair<wxDateTime, double>& val) { return val.first == date; });
        if (valIt != asset->GetValuationsForPlotting().end()) {
            lastValuation = valIt->second; 
        }
        newValuations.push_back({date, lastValuation});

       
        auto depIt = std::find_if(asset->GetDeploymentsForPlotting().begin(), asset->GetDeploymentsForPlotting().end(),
                                [date](const std::pair<wxDateTime, double>& dep) { return dep.first == date; });
        if (depIt != asset->GetDeploymentsForPlotting().end()) {
            lastDeployment = depIt->second; 
        }
        newDeployments.push_back({date, lastDeployment}); 
    }

    if (!newValuations.empty() && newValuations.front().second == 0.0 && !newDeployments.empty()) {
        newValuations.front().second = newDeployments.front().second;
    }
    asset->UpdateValuationsForPlotting(std::move(newValuations));
    asset->UpdateDeploymentsForPlotting(std::move(newDeployments));

    XYPlot *xyPlot = new XYPlot();

    if (!asset->GetValuationsForPlotting().empty()) {
        size_t count = asset->GetValuationsForPlotting().size();
        double *data = new double[count];
        time_t* times = new time_t[count];
        for(size_t i = 0; i < count; i++) { 
            data[i] = asset->GetValuationsForPlotting()[i].second;
            times[i] = asset->GetValuationsForPlotting()[i].first.GetTicks();
            std::cout<<"Valuation:i = "<<i<<" Data[i] = "<<data[i]<< "times[i] = "<<asset->GetValuationsForPlotting()[i].first.FormatISODate()<<std::endl;
        }
        TimeSeriesDataset* assetValuationTimeSeries = new TimeSeriesDataset(data, times, count);
        XYLineRenderer* assetValuationLineRender = new XYLineRenderer();
        assetValuationLineRender->SetSeriePen(0, valuationPen);
        assetValuationTimeSeries->SetRenderer(assetValuationLineRender);
        xyPlot->AddDataset(assetValuationTimeSeries);
    }

    if (!asset->GetDeploymentsForPlotting().empty()) {
        size_t count2 = asset->GetDeploymentsForPlotting().size();
        double *data2 = new double[count2];
        time_t *times2 = new time_t[count2];
        for(size_t i = 0; i < count2; i++) {
            data2[i] = asset->GetDeploymentsForPlotting()[i].second;
            times2[i] = asset->GetDeploymentsForPlotting()[i].first.GetTicks();
            std::cout<<"Deployment:i = "<<i<<" Data[i] = "<<data2[i]<< "times[i] = "<<asset->GetDeploymentsForPlotting()[i].first.FormatISODate()<<std::endl;
        }
        TimeSeriesDataset* assetDeployTimeSeries = new TimeSeriesDataset(data2, times2, count2);
        XYLineRenderer* assetDeployLineRender = new XYLineRenderer();
        assetDeployLineRender->SetSeriePen(0, deploymentPen);
        assetDeployTimeSeries->SetRenderer(assetDeployLineRender);
        xyPlot->AddDataset(assetDeployTimeSeries);
    }


    wxPen* chartBorderPen = new wxPen(wxColor(0,0,0));
    wxBrush* chartFillBrush = new wxBrush(wxColor(255,255,255));

    FillAreaDraw* chartFillArea = new FillAreaDraw(*chartBorderPen, *chartFillBrush);
    xyPlot->SetDrawGrid(true, true);

    wxFont axisFont = *wxNORMAL_FONT;
    axisFont.SetPointSize(14);
    wxFont labelFont = *wxNORMAL_FONT;
    labelFont.SetPointSize(12);

    NumberAxis *leftAxis = new NumberAxis(AXIS_LEFT);
    leftAxis->SetTitle("Amount in $");
    leftAxis->SetTitleColour(*textColor);
    leftAxis->SetLabelTextColour(*textColor);
    leftAxis->SetMajorGridlinePen(*gridLinePen);
    leftAxis->SetMinorGridlinePen(*gridLinePen);
    leftAxis->SetLabelPen(*gridLinePen);
    leftAxis->SetTitleFont(axisFont);
    leftAxis->SetLabelTextFont(labelFont);

    DateAxis *bottomAxis = new DateAxis(AXIS_BOTTOM);
    bottomAxis->SetTitle("Dates");
    bottomAxis->SetTitleColour(*textColor);
    bottomAxis->SetLabelTextColour(*textColor);
    bottomAxis->SetVerticalLabelText(true);
    bottomAxis->SetDateFormat(wxT("%b-%Y"));
    bottomAxis->SetMajorGridlinePen(*gridLinePen);
    bottomAxis->SetLabelPen(*gridLinePen);
    bottomAxis->SetTitleFont(axisFont);
    bottomAxis->SetLabelTextFont(labelFont);
    bottomAxis->SetMargins(wxCoord(15),wxCoord(35));

    xyPlot->AddAxis(leftAxis);
    xyPlot->AddAxis(bottomAxis);
    xyPlot->LinkDataHorizontalAxis(0,0);
    xyPlot->LinkDataHorizontalAxis(1,0);
    xyPlot->LinkDataVerticalAxis(0,0);  
    xyPlot->LinkDataVerticalAxis(1,0);  

    xyPlot->SetBackground(chartFillArea);

    if(xyPlot->GetDatasetCount()==0){
        delete xyPlot;
        return nullptr;
    }

    Chart* myChart = new Chart(xyPlot, "Valuations & Deployed Capital");
    wxString titleText = "Valuations & Deployed Capital";
    wxFont titleFont = *wxNORMAL_FONT;
    titleFont.SetPointSize(14);

    TextElement *chartTitle = new TextElement(titleText, wxALIGN_CENTER_HORIZONTAL, titleFont);
    chartTitle->SetColour(*textColor);
    Header *myHeader = new Header(*chartTitle);
    myChart->SetHeader(myHeader);
    wxPen *chartPen = new wxPen(*textColor);
    wxBrush *chartBrush = new wxBrush(wxColor(255,255,255));
    FillAreaDraw *chartFillArea2 = new FillAreaDraw(*chartBorderPen, *chartFillBrush);
    myChart->SetBackground(chartFillArea2);
    return myChart;
}

void AssetPopout::UpdateChartValuationDeploy(){
    Chart* updatedChart = PopulateDrawChartValuationDeploy();
    if(updatedChart!=nullptr){
        chartPanelHolderPanel->DestroyChildren();

        wxChartPanel *newChartPanel = new wxChartPanel(chartPanelHolderPanel, wxID_ANY);
        newChartPanel->SetBackgroundColour(wxColor(255,255,255));
        newChartPanel->SetChart(updatedChart);

        wxBoxSizer* holderSizer = new wxBoxSizer(wxVERTICAL);
        chartPanelHolderPanel->SetSizer(holderSizer);

        holderSizer->Add(newChartPanel, 1, wxEXPAND);

        chartPanelHolderPanel->Update();
        chartPanelHolderPanel->Layout();
    }
    this->Layout();
}

Chart* AssetPopout::PopulateDrawChartDistribution(){
    asset->PopulateDistributionForPlotting();

    wxPen *distributionPen = new wxPen(wxColor(5, 45, 247),2);
    wxColor *gridLineColor = new wxColor(0,0,0);
    wxColor *textColor = new wxColor(5, 45, 247);
    wxPen *gridLinePen = new wxPen(*gridLineColor);   
    wxBrush* chartFillBrush = new wxBrush(wxColor(255,255,255));

    if(asset->GetDistributions().empty()){
        return nullptr;
    }
    BarPlot *barPlot = new BarPlot();

    size_t count = asset->GetDistributionsForPlotting().size();
    wxString* names = new wxString[count];
    double* values = new double[count];
    for(size_t i = 0;i<count; i++){
        values[i] = asset->GetDistributionsForPlotting()[i].second;
        names[i] = asset->GetDistributionsForPlotting()[i].first.Format("%b-%Y");
    }
    if (count == 1) {
        //work around solution to issue rendering barchart when there is only 1 distribution 
        //caused bar to be rendered off chart and not visible
        count += 2; 
        delete[] names; 
        delete[] values;
        names = new wxString[count];
        values = new double[count];

        names[0] = (asset->GetDistributionsForPlotting()[0].first - wxTimeSpan::Days(60)).Format("%b-%Y");
        values[0] = 0.0;
        names[1] = asset->GetDistributionsForPlotting()[0].first.Format("%b-%Y"); 
        values[1] = asset->GetDistributionsForPlotting()[0].second;
        names[2] = (asset->GetDistributionsForPlotting()[0].first + wxTimeSpan::Days(60)).Format("%b-%Y"); 
        values[2] = 0.0; 
    }

    CategorySimpleDataset* distributionDataSet = new CategorySimpleDataset(names,count);
    wxString barSerie = "BAR SERIES";
    CategorySerie* distributionBarSerie = new CategorySerie(barSerie, values, count);
    distributionDataSet->AddSerie(distributionBarSerie);
    BarType *barType = new NormalBarType(15);
    BarRenderer *barRender = new BarRenderer(barType);

    wxBrush* barBrush = new wxBrush(*textColor);
    FillAreaDraw *barAreas = new FillAreaDraw(*distributionPen, *barBrush);
    barRender->SetBarDraw(0,barAreas);
    distributionDataSet->SetRenderer(barRender);
    FillAreaDraw* chartFillArea = new FillAreaDraw(*gridLinePen, *chartFillBrush);
    wxPen chartGridLinePen(wxColor(51,245,12),1,wxPENSTYLE_SOLID);

    wxFont axisFont = *wxNORMAL_FONT;
    axisFont.SetPointSize(14);
    wxFont labelFont = *wxNORMAL_FONT;
    labelFont.SetPointSize(12);

    NumberAxis *leftAxis = new NumberAxis(AXIS_LEFT);
    leftAxis->SetTitle("Amount in $");
    leftAxis->SetTitleColour(*textColor);
    leftAxis->SetLabelTextColour(*textColor);
    leftAxis->SetLabelPen(*gridLinePen);
    leftAxis->SetMajorGridlinePen(*gridLinePen);
    leftAxis->SetTitleFont(axisFont);
    leftAxis->SetLabelTextFont(labelFont);
    barPlot->AddAxis(leftAxis);

    CategoryAxis *bottomAxis = new CategoryAxis(AXIS_BOTTOM);
    bottomAxis->SetTitle("Dates");
    bottomAxis->SetTitleColour(*textColor);
    bottomAxis->SetVerticalLabelText(true);
    bottomAxis->SetLabelTextColour(*textColor);
    bottomAxis->SetLabelPen(*gridLinePen);
    bottomAxis->SetTitleFont(axisFont);
    bottomAxis->SetLabelTextFont(labelFont);
    bottomAxis->SetMargins(wxCoord(15),wxCoord(35));
    barPlot->AddAxis(bottomAxis);

    barPlot->AddDataset(distributionDataSet);

    barPlot->SetBackground(chartFillArea);
    barPlot->LinkDataHorizontalAxis(0, 0);
    barPlot->LinkDataVerticalAxis(0, 0);
    Chart* chart = new Chart(barPlot, "");
    wxString titleText = "Distributions By Q";
    wxFont titleFont = *wxNORMAL_FONT;
    titleFont.SetPointSize(16);
    TextElement *chartTitle = new TextElement(titleText, wxALIGN_CENTER_HORIZONTAL, titleFont);
    chartTitle->SetColour(*textColor);
    Header *myHeader = new Header(*chartTitle);
    chart->SetHeader(myHeader);
    wxPen *chartPen = new wxPen(*textColor);
    wxBrush *chartBrush = new wxBrush(wxColor(0,0,0));
    FillAreaDraw *chartFillArea2 = new FillAreaDraw(*gridLinePen, *chartFillBrush);
    chart->SetBackground(chartFillArea2);
    
    return chart;
}

void AssetPopout::UpdateChartDistribution(){
    
    Chart* updatedChart = PopulateDrawChartDistribution();

    if(updatedChart!=nullptr){
        distributionChartPanelHolder->DestroyChildren();

        wxChartPanel *newChartPanel = new wxChartPanel(distributionChartPanelHolder, wxID_ANY);
        newChartPanel->SetBackgroundColour(wxColor(0,0,0));
        newChartPanel->SetChart(updatedChart);

        wxBoxSizer* holderSizer = new wxBoxSizer(wxVERTICAL);
        distributionChartPanelHolder->SetSizer(holderSizer);

        holderSizer->Add(newChartPanel, 1, wxEXPAND);

        distributionChartPanelHolder->Update();
        distributionChartPanelHolder->Layout();
    }else{
        distributionChartPanelHolder->DestroyChildren();
    }
    this->Layout();
}

void AssetPopout::OnAddPosition(wxCommandEvent &e){
    AddPositionDialog addPositionDialog(this, portfolio, asset);
    int retvalue = addPositionDialog.ShowModal();
    if(retvalue == wxID_OK){
        asset->ClearInvestorPositionDisplays();
        for(auto& position : asset->GetPositionsForIDP()){
            auto investorPositionDisplay = std::make_shared<InvestorPositionDisplay>(position);
            asset->AddInvestorPositionDisplay(investorPositionDisplay);
            investorPositionDisplayVirtualListControl->setItems(asset->GetIPDVector());
        }
        UpdateChartValuationDeploy();
        UpdateDisplayTextValues();
        this->Refresh();
    }
}

void AssetPopout::OnExecuteDistribution(wxCommandEvent &e){
    ExecuteDistribution dialog(this, asset);
    int retValue = dialog.ShowModal();
    if(retValue == wxID_OK){
        double amountToDistribute = dialog.GetDistributionAmount();
        double amountToReserve = dialog.GetReserveAmount();
        wxDateTime dateOfDistribution = dialog.GetDate();
        Distribution newDistribution;
        newDistribution.distribution.first = dateOfDistribution;
        newDistribution.distribution.second = amountToDistribute;
        newDistribution.paid = true;
        for(auto&distribution : asset->GetDistributionsNonConst()){
            if(distribution.distribution.first <= dateOfDistribution){
                distribution.paid = true;
            }
        }
        asset->PassDistributionToPositions(newDistribution);
        asset->AddQuarterlyDistribution(newDistribution);
        asset->AddNewReserve(amountToReserve);
        std::string type = "Client Distribution Recorded";
        std::string name = asset->GetAssetName().ToStdString();
        std::string note = dialog.GetNote();
        Transaction newTransaction(dateOfDistribution,name,amountToDistribute,type,note);
        asset->AddNewTransaction(newTransaction);
        if(amountToReserve>0){
            investorPositionDisplayVirtualListControl->setItems(asset->GetIPDVector());
            asset->SetPositionValues();
        }
        UpdateDisplayTextValues();
        this->Refresh();
        this->Layout();
    }else{
        //do nothing and close;
    }
}

void AssetPopout::UpdateExecuteDistributionButton(){
    bool hasDistribution = !asset->GetDistributions().empty();
    executeDistributionButton->Enable(hasDistribution);
    if(hasDistribution){
        distributionListControl->Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, &AssetPopout::OnDistributionEdit, this);
    }
}

void AssetPopout::AllowBindingOnValuationLC(){
    bool hasValuation = !asset->GetValuations().empty();
    if(hasValuation){
        valuationListControl->Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, &AssetPopout::OnValuationEdit,this);
    }
}

void AssetPopout::OnTransactionClick(wxCommandEvent &e){
    auto* transactionPopout = new TransactionPopout(this,asset->GetAssetName(),wxDefaultPosition,wxSize(FromDIP(1200),FromDIP(800)),asset);
    transactionPopout->Show(true);
}