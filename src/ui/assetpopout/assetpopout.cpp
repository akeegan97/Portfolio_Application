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


void AssetPopout::SetupLayout(){
    //sizers
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
    investorPositionDisplayVirtualListControl = new VListControl<std::shared_ptr<InvestorPositionDisplay>>(this,wxID_ANY,FromDIP(wxDefaultPosition),FromDIP(wxDefaultSize));
    if(!asset->GetIPDVector().empty()){
        investorPositionDisplayVirtualListControl->setItems(asset->GetIPDVector());
    }
    topSizer->Add(investorPositionDisplayVirtualListControl, wxALL| wxEXPAND,5);
    mainSizer->Add(topSizer,2,wxALL|wxEXPAND);

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

    mainSizer->Add(middleChartSizer,4,wxEXPAND,5);

    //add VLCs for Valuations/Distributions removal of Events
    valuationListControl = new VListControl<Valuation>(this, wxID_ANY,FromDIP(wxDefaultPosition),FromDIP(wxDefaultSize));
    if(!asset->GetValuations().empty()){
        valuationListControl->setItems(asset->GetValuationsNonConst());
    }
    valuationListControl->Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, &AssetPopout::OnValuationEdit, this);
    distributionListControl = new VListControl<Distribution>(this, wxID_ANY,FromDIP(wxDefaultPosition),FromDIP(wxDefaultSize));
    if(!asset->GetDistributions().empty()){
        distributionListControl->setItems(asset->GetDistributionsNonConst());
    }
    distributionListControl->Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, &AssetPopout::OnDistributionEdit, this);
    middleVLCSizer->Add(valuationListControl,5,wxALL|wxEXPAND,5);
    middleVLCSizer->Add(distributionListControl,5,wxALL|wxEXPAND,5);

    mainSizer->Add(middleVLCSizer, 2, wxALL|wxEXPAND, 5);

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
    wxButton *setAssetValues = new wxButton(this, wxID_ANY, "Set Asset Values");
    setAssetValues->Bind(wxEVT_BUTTON, &AssetPopout::OnSetAssetValues, this);
    buttonSizer->Add(addDistributionButton);
    buttonSizer->Add(assetLevelMovementOfCapitalButton);
    buttonSizer->Add(addValuationButton);
    buttonSizer->Add(addPositionButton);
    buttonSizer->Add(setAssetValues);

    bottomSizer->Add(buttonSizer,5,wxALL|wxEXPAND,3);

    mainSizer->Add(bottomSizer,2,wxALL|wxEXPAND,5);
    this->SetSizer(mainSizer);
    this->Layout();

    //TODO hookup bindings for buttons/vlcs

}

void AssetPopout::UpdateDisplayTextValues(){
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

    std::string assetIRRformated = std::to_string(asset->GetIrr());

    assetIRR->SetLabel("Asset Gross IRR: "+assetIRRformated);
    assetIRR->SetForegroundColour(wxColor(51,245,12));

    totalCommittedText->SetLabel("Total Subscribed Amount: "+formattedCommitted);
    totalCommittedText->SetForegroundColour(wxColor(51,245,12));

    totalPaidText->SetLabel("Total Paid Amount: "+formattedTotalPaid);
    totalPaidText->SetForegroundColour(wxColor(51,245,12));

    totalDeployedCapitalText->SetLabel("Total Deployed Amount: "+formattedTotalDeployed);
    totalDeployedCapitalText->SetForegroundColour(wxColor(51,245,12));

    numInvestorsText->SetLabel(wxString::Format("Total Investors: %.2f", numInvestors));
    numInvestorsText->SetForegroundColour(wxColor(51,245,12));

    totalReserveCapitalText->SetLabel("Total Reserve Amount: "+formattedReserve);
    totalReserveCapitalText->SetForegroundColour(wxColor(51,245,12));

    totalReturnedCapitalText->SetLabel("Total Returned Amount: "+formattedReturnedCapital);
    totalReturnedCapitalText->SetForegroundColour(wxColor(51,245,12));

    totalMgmtFeesGeneratedText->SetLabel("Total Management Fees Earned: "+formatedTotalMgmtFees);
    totalMgmtFeesGeneratedText->SetForegroundColour(wxColor(51,245,12));

    totalMgmtFeesDueText->SetLabel("Total Management Fees Due: "+ formattedTotalMgmtFeesDue);
    totalMgmtFeesDueText->SetForegroundColour(wxColor(51,245,12));

    totalPromoteFeesGeneratedText->SetLabel("Total Promote Fees Earned: "+formatedPromoteFees);
    totalPromoteFeesGeneratedText->SetForegroundColour(wxColor(51,245,12));
}

// void AssetPopout::OnInvestorPositionClick(wxListEvent &e){
//     InvestorPositionEditWindow editWindow(this);
//     editWindow.SetBackgroundColour(wxColor(0,0,0));
//     int returnValue = editWindow.ShowModal();

//     if(returnValue==wxID_OK){
//         long listIndex = e.GetIndex();
//         auto& selectedInvestorPosition = investorPositionDisplayVirtualListControl->GetItemAtListIndex(listIndex);

//         wxDateTime investedDate = editWindow.GetInvestmentDate();
//         wxString clientName = editWindow.GetClientName();
//         wxString clientType = editWindow.GetClientType();
//         double clientSubscribed = editWindow.GetSubscribed();
//         double clientPaid = editWindow.GetPaid();
//         double clientDeployed = editWindow.GetDeployed();
//         double clientReserve = editWindow.GetReserve();
//         double clientReturnOfCapital = editWindow.GetReserve();

//         if(investedDate != wxDateTime(01,wxDateTime::Jan, 2001)){
//             selectedInvestorPosition->positionPtr->dateInvested = investedDate;
//         }
//         if(clientName.ToStdString().size()!=0){
//             selectedInvestorPosition->positionPtr->investorPtr->clientName = clientName;
//         }
//         if(clientType.ToStdString().size()!=0){
//             selectedInvestorPosition->positionPtr->investorPtr->type = clientType;
//         }
//         if(clientSubscribed != 0){
//             selectedInvestorPosition->positionPtr->subscribed = clientSubscribed;
//         }
//         if(clientPaid != 0){
//             selectedInvestorPosition->positionPtr->paid = clientPaid;
//         }
//         if(clientDeployed != 0){
//             selectedInvestorPosition->positionPtr->deployed = clientDeployed;
//         }
//         if(clientReserve != 0){
//             selectedInvestorPosition->positionPtr->reserve = clientReserve;
//         }
//         if(clientReturnOfCapital !=0){
//             selectedInvestorPosition->positionPtr->returnOfCapital = clientReturnOfCapital;
//         }
//         for(auto&ipd : asset->investorsPositionsDisplays){
//             ipd->positionPtr->assetPtr->SetOwnershipOfPositions();
//         }
//         investorPositionDisplayVirtualListControl->Refresh();
//         UpdateChartValuationDeploy();
//         UpdateDisplayTextValues();
//         this->Refresh();
//     }else if(returnValue == wxID_ANY){
//         //exit 
//     }
// }

void AssetPopout::OnAddDistributionClicked(wxCommandEvent &e){
    wxDateTime today = wxDateTime::Today();
    double zero = 0.0;
    DistributionDialog addDistroWindow(this,false,today, zero);
    addDistroWindow.SetBackgroundColour(wxColor(0,0,0));
    int retValue = addDistroWindow.ShowModal();
    if(retValue == wxID_OK){
        Distribution newDistribution;
        newDistribution.distribution.first = addDistroWindow.GetDistributionDate();
        newDistribution.distribution.second = addDistroWindow.GetDistributionAmount();
        //here specify if multiple distributions per Q are okay
        asset->AddDistribution(newDistribution);
        for(auto&pos:asset->GetPositions()){
            asset->TriggerUpdateOfDistributionsForPositions();
        }
        distributionListControl->setItems(asset->GetDistributions());
        distributionListControl->Update();
        UpdateDisplayTextValues();
        UpdateChartDistribution();
        //asset->PopulateIRR(); TODO Implement new IRR function 
        UpdateDisplayTextValues();
        this->Refresh();
        std::cout<<"New Distribution Occured!"<<std::endl;
        for(const auto& pos: asset->GetPositions()){
            std::cout<<"Position InvestorPtr: "<<pos->GetInvestorPtr()->GetName()<<std::endl;
            for(auto& netIncome: pos->GetNetIncome()){
                std::cout<<"Net Income: "<<"Date: "<<netIncome.distribution.first.FormatISODate()<<
                "Amount: "<<netIncome.distribution.second<<std::endl;
            }
        }
    }
}

void AssetPopout::OnCapitalMovement(wxCommandEvent &e){//Changing to be the "Setter" from the UI that controls what the assets deployed/reserve values are
    MoveDeploy DeployMovementWindow(this);
    DeployMovementWindow.SetBackgroundColour(wxColor(0,0,0));
    int retValue = DeployMovementWindow.ShowModal();
    if(retValue == wxID_OK){
        wxDateTime dateOfMovement = DeployMovementWindow.GetDate();
        double amountMoved = DeployMovementWindow.GetAmountMoved();
        wxString selectedMovementDirection = DeployMovementWindow.GetSelectedMovementDirection();
        if(selectedMovementDirection == "Reserve to Deploy"){
            asset->MoveReserveToDeploy(dateOfMovement,amountMoved);
            asset->SetPositionValues();
        }else if(selectedMovementDirection == "Deploy to Reserve"){
            asset->MoveDeployToReserve(dateOfMovement,amountMoved);
            asset->SetPositionValues();
        }
        investorPositionDisplayVirtualListControl->Refresh();
        UpdateChartValuationDeploy();
        // asset->PopulateIRR();
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
    addValuationDialog.SetBackgroundColour(wxColor(0,0,0));
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
        //asset->PopulateIRR();
        UpdateDisplayTextValues();
        this->Layout();
    }else if(retVal == wxID_CANCEL){
        //do nothing and exit
    }
}

void AssetPopout::OnClose(wxCloseEvent &e){
    wxCommandEvent evt(ASSET_POPOUT_CLOSED, wxID_ANY);
    wxPostEvent(GetParent(), evt);
    e.Skip();
}

void AssetPopout::OnDistributionEdit(wxListEvent &e){
    long listIndex = e.GetIndex();
    long dataIndex = distributionListControl->orderedIndices[listIndex];
    std::cout<<"List Index: "<<listIndex<<" Data Index: "<<dataIndex<<std::endl;
    auto selectedDistribution = asset->GetDistributionsNonConst()[dataIndex];
    wxDateTime distributionDate = selectedDistribution.distribution.first;
    double distributionAmount = selectedDistribution.distribution.second;
    DistributionDialog distributionEditwindow(this,true, distributionDate, distributionAmount);

    distributionEditwindow.SetBackgroundColour(wxColor(0,0,0));
    int retVal = distributionEditwindow.ShowModal();
    if(retVal == wxID_OK){
        selectedDistribution.distribution.first = distributionEditwindow.GetDistributionDate();
        selectedDistribution.distribution.second = distributionEditwindow.GetDistributionAmount();
        asset->RemoveDistribution(dataIndex);
        asset->AddDistribution(selectedDistribution);
        asset->TriggerUpdateOfDistributionsForPositions();
        distributionListControl->setItems(asset->GetDistributionsNonConst());
        distributionListControl->Update();
        UpdateDisplayTextValues();
        UpdateChartDistribution();
        this->Refresh();
    }else if(retVal == MY_CUSTOM_DELETE_CODE){
        if(dataIndex >= 0 && dataIndex < asset->GetDistributions().size()){
            asset->RemoveDistribution(dataIndex);
        }
        asset->TriggerUpdateOfDistributionsForPositions();
        distributionListControl->setItems(asset->GetDistributions());
        distributionListControl->Update();
        UpdateDisplayTextValues();
        UpdateChartDistribution();
        //asset->PopulateIRR();
        UpdateDisplayTextValues();
        this->Refresh();
        for(const auto& pos: asset->GetPositions()){
            std::cout<<"Position InvestorPtr: "<<pos->GetInvestorPtr()->GetName()<<std::endl;
            for(auto& netIncome: pos->GetNetIncome()){
                std::cout<<"Net Income: "<<"Date: "<<netIncome.distribution.first.FormatISODate()<<
                "Amount: "<<netIncome.distribution.second<<std::endl;
            }
        }
    }
    UpdateChartDistribution();
}

void AssetPopout::OnValuationEdit(wxListEvent &e){
    long listIndex = e.GetIndex();
    long dataIndex = valuationListControl->orderedIndices[listIndex];
    Valuation valuationToEdit = asset->GetValuationsNonConst()[dataIndex];
    std::cout<<"List Index: "<<listIndex<<" Data Index: "<<dataIndex<<std::endl;
    wxDateTime setDate  = valuationToEdit.valuationDate;
    double setValue = valuationToEdit.valuation;
    ValuationDialog valuationWindow(this, true, setDate, setValue);
    valuationWindow.SetBackgroundColour(wxColor(0,0,0));
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
        //asset->PopulateIRR();
        UpdateDisplayTextValues();
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
        //asset->PopulateIRR();
        UpdateDisplayTextValues();
        this->Refresh();
    }
    this->Refresh();
    this->Layout();
}

Chart* AssetPopout::PopulateDrawChartValuationDeploy(){
    asset->PopulateValuationsDeploymentsForPlotting();
    if (asset->GetValuationsForPlotting().empty() && asset->GetDeploymentsForPlotting().empty()) {
        // Both datasets are empty, return nullptr to indicate no chart should be drawn
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

    for (const auto& date : allDates) {
        auto valIt = std::find_if(asset->GetValuationsForPlotting().begin(), asset->GetValuationsForPlotting().end(),
                                [date](const std::pair<wxDateTime, double>& val) { return val.first == date; });
        if (valIt != asset->GetValuationsForPlotting().end()) {
            lastValuation = valIt->second;
            newValuations.push_back(*valIt);
        } else {
            newValuations.push_back({date, lastValuation}); 
        }

        auto depIt = std::find_if(asset->GetDeploymentsForPlotting().begin(), asset->GetDeploymentsForPlotting().end(),
                                [date](const std::pair<wxDateTime, double>& dep) { return dep.first == date; });
        if (depIt != asset->GetDeploymentsForPlotting().end()) {
            lastDeployment = depIt->second;
            newDeployments.push_back(*depIt);
        } else {
            newDeployments.push_back({date, lastDeployment}); 
        }
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
        wxPen* assetValuationPen = new wxPen(wxColor(51,245,12),2); // Green for valuation
        assetValuationLineRender->SetSeriePen(0, assetValuationPen);
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
        wxPen* assetDeployLinePen = new wxPen(wxColor(0,0,252),2); // blue for deployment
        assetDeployLineRender->SetSeriePen(0, assetDeployLinePen);
        assetDeployTimeSeries->SetRenderer(assetDeployLineRender);
        xyPlot->AddDataset(assetDeployTimeSeries);
    }


    wxPen* chartBorderPen = new wxPen(wxColor(51,245,12));
    wxBrush* chartFillBrush = new wxBrush(wxColor(0,0,0));

    FillAreaDraw* chartFillArea = new FillAreaDraw(*chartBorderPen, *chartFillBrush);
    xyPlot->SetDrawGrid(true, true);
    wxPen chartGridLinePen(wxColor(51,245,12),1,wxPENSTYLE_SOLID);

    NumberAxis *leftAxis = new NumberAxis(AXIS_LEFT);
    leftAxis->SetTitle("Amount in $");
    wxColor myColor = wxColor(51,245,12);
    leftAxis->SetTitleColour(myColor);
    leftAxis->SetLabelTextColour(myColor);
    leftAxis->SetMajorGridlinePen(chartGridLinePen);
    leftAxis->SetMinorGridlinePen(chartGridLinePen);
    leftAxis->SetLabelPen(chartGridLinePen);

    DateAxis *bottomAxis = new DateAxis(AXIS_BOTTOM);
    bottomAxis->SetTitle("Dates");
    bottomAxis->SetTitleColour(myColor);
    bottomAxis->SetLabelTextColour(myColor);
    bottomAxis->SetVerticalLabelText(true);
    bottomAxis->SetDateFormat(wxT("%m-%d-%Y"));
    bottomAxis->SetMajorGridlinePen(chartGridLinePen);
    bottomAxis->SetLabelPen(chartGridLinePen);

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
    chartTitle->SetColour(myColor);
    Header *myHeader = new Header(*chartTitle);
    myChart->SetHeader(myHeader);
    wxPen *chartPen = new wxPen(myColor);
    wxBrush *chartBrush = new wxBrush(wxColor(0,0,0));
    FillAreaDraw *chartFillArea2 = new FillAreaDraw(*chartBorderPen, *chartFillBrush);
    myChart->SetBackground(chartFillArea2);

    return myChart;
}

void AssetPopout::UpdateChartValuationDeploy(){

    Chart* updatedChart = PopulateDrawChartValuationDeploy();

    if(updatedChart!=nullptr){
        chartPanelHolderPanel->DestroyChildren();

        wxChartPanel *newChartPanel = new wxChartPanel(chartPanelHolderPanel, wxID_ANY);
        newChartPanel->SetBackgroundColour(wxColor(0,0,0));
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

    if(asset->GetDistributions().empty()){
        return nullptr;
    }
    BarPlot *barPlot = new BarPlot();

    size_t count = asset->GetDistributionsForPlotting().size();
    wxString* names = new wxString[count];
    double* values = new double[count];
    for(size_t i = 0;i<count; i++){
        values[i] = asset->GetDistributionsForPlotting()[i].second;
        names[i] = asset->GetDistributionsForPlotting()[i].first.FormatISODate();
        // std::cout<<"Distribution:i = "<<i<<" Data[i] = "<<values[i]<< "times[i] = "<<asset->GetDistributionsForPlotting()[i].first.FormatISODate()<<std::endl;
    }

    CategorySimpleDataset* distributionDataSet = new CategorySimpleDataset(names,count);
    wxString barSerie = "BAR SERIES";
    CategorySerie* distributionBarSerie = new CategorySerie(barSerie, values, count);
    distributionDataSet->AddSerie(distributionBarSerie);
    BarType *barType = new NormalBarType(15);
    BarRenderer *barRender = new BarRenderer(barType);
    wxColor *myColor = new wxColor(127, 197, 250);
    wxPen*barPen = new wxPen(*myColor);
    wxBrush* barBrush = new wxBrush(*myColor);
    FillAreaDraw *barAreas = new FillAreaDraw(*barPen, *barBrush);
    barRender->SetBarDraw(0,barAreas);
    distributionDataSet->SetRenderer(barRender);

    wxPen* chartBorderPen = new wxPen(wxColor(51,245,12));
    wxBrush* chartFillBrush = new wxBrush(wxColor(0,0,0));
    FillAreaDraw* chartFillArea = new FillAreaDraw(*chartBorderPen, *chartFillBrush);
    wxPen chartGridLinePen(wxColor(51,245,12),1,wxPENSTYLE_SOLID);

    NumberAxis *leftAxis = new NumberAxis(AXIS_LEFT);
    leftAxis->SetTitle("Amount in $");
    leftAxis->SetTitleColour(*myColor);
    //leftAxis->SetMargins(5, 0);
    leftAxis->SetLabelTextColour(*myColor);
    leftAxis->SetLabelPen(chartGridLinePen);
    leftAxis->SetMajorGridlinePen(chartGridLinePen);
    barPlot->AddAxis(leftAxis);

    CategoryAxis *bottomAxis = new CategoryAxis(AXIS_BOTTOM);
    bottomAxis->SetTitle("Dates");
    bottomAxis->SetTitleColour(*myColor);
    bottomAxis->SetVerticalLabelText(true);
    bottomAxis->SetLabelTextColour(*myColor);
    bottomAxis->SetLabelPen(chartGridLinePen);
    barPlot->AddAxis(bottomAxis);

    barPlot->AddDataset(distributionDataSet);

    barPlot->SetBackground(chartFillArea);
    barPlot->LinkDataHorizontalAxis(0, 0);
    barPlot->LinkDataVerticalAxis(0, 0);
    Chart* chart = new Chart(barPlot, "");
    wxString titleText = "Distributions By Q";
    wxFont titleFont = *wxNORMAL_FONT;
    titleFont.SetPointSize(14);
    TextElement *chartTitle = new TextElement(titleText, wxALIGN_CENTER_HORIZONTAL, titleFont);
    chartTitle->SetColour(*myColor);
    Header *myHeader = new Header(*chartTitle);
    chart->SetHeader(myHeader);
    wxPen *chartPen = new wxPen(*myColor);
    wxBrush *chartBrush = new wxBrush(wxColor(0,0,0));
    FillAreaDraw *chartFillArea2 = new FillAreaDraw(*chartBorderPen, *chartFillBrush);
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
    }
    this->Layout();
}

void AssetPopout::OnAddPosition(wxCommandEvent &e){
    AddPositionDialog addPositionDialog(this, portfolio, asset);
    int retvalue = addPositionDialog.ShowModal();
    if(retvalue == wxID_OK){
        asset->ClearInvestorPositionDisplays();
        for(auto& position : asset->GetPositionsForIDP()){
            position->TriggerUpdateOfManagementFeeVector();
            auto investorPositionDisplay = std::make_shared<InvestorPositionDisplay>(position);
            asset->AddInvestorPositionDisplay(investorPositionDisplay);
            investorPositionDisplayVirtualListControl->setItems(asset->GetIPDVector());
        }
        asset->TriggerUpdateOfDistributionsForPositions();
        // UpdateChartValuationDeploy();
        // UpdateChartDistribution();
        UpdateDisplayTextValues();
        this->Refresh();
    }
    
}

void AssetPopout::OnSetAssetValues(wxCommandEvent &e){
    SetAssetDeployReserveDialog dialog(this);
    int retValue = dialog.ShowModal();
    if(retValue == wxID_OK){
        wxDateTime dateOfDeploy = dialog.GetDate();
        double amountDeploy = dialog.GetDeploy();
        double amountReserve = dialog.GetReserve();
        std::pair<wxDateTime, double> movement = std::make_pair(dateOfDeploy, amountDeploy);
        asset->AddMovement(movement);
        asset->SetDeployedCapital(amountDeploy);
        asset->SetReserveCapital(amountReserve);
        asset->SetPositionValues();
        UpdateDisplayTextValues();
        UpdateChartValuationDeploy();
        UpdateChartDistribution();
        this->Refresh();
        
        std::cout<<"Asset: Deployed "<<asset->GetTotalAssetDeployed()<<std::endl;
    }
}