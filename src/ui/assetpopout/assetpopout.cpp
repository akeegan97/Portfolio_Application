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


void AssetPopout::SetupLayout(){
    //sizers
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *middleChartSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *middleVLCSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *bottomSizer = new wxBoxSizer(wxHORIZONTAL);

    for(auto& position : asset->GetPositions()){
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
        valuationListControl->setItems(asset->GetValuations());
    }
    distributionListControl = new VListControl<Distribution>(this, wxID_ANY,FromDIP(wxDefaultPosition),FromDIP(wxDefaultSize));
    if(!asset->GetDistributions().empty()){
        distributionListControl->setItems(asset->GetDistributions());
    }
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
    assetLevelMovementOfCapitalButton = new wxButton(this, wxID_ANY, "Movement Of Capital");
    addValuationButton = new wxButton(this, wxID_ANY, "Valuation");
    buttonSizer->Add(addDistributionButton);
    buttonSizer->Add(assetLevelMovementOfCapitalButton);
    buttonSizer->Add(addValuationButton);

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

void AssetPopout::OnInvestorPositionClick(wxListEvent &e){
    InvestorPositionEditWindow editWindow(this);
    editWindow.SetBackgroundColour(wxColor(0,0,0));
    int returnValue = editWindow.ShowModal();

    if(returnValue==wxID_OK){
        long listIndex = e.GetIndex();
        auto& selectedInvestorPosition = investorPositionDisplayVirtualListControl->GetItemAtListIndex(listIndex);

        wxDateTime investedDate = editWindow.GetInvestmentDate();
        wxString clientName = editWindow.GetClientName();
        wxString clientType = editWindow.GetClientType();
        double clientSubscribed = editWindow.GetSubscribed();
        double clientPaid = editWindow.GetPaid();
        double clientDeployed = editWindow.GetDeployed();
        double clientReserve = editWindow.GetReserve();
        double clientReturnOfCapital = editWindow.GetReserve();

        if(investedDate != wxDateTime(01,wxDateTime::Jan, 2001)){
            selectedInvestorPosition->positionPtr->dateInvested = investedDate;
        }
        if(clientName.ToStdString().size()!=0){
            selectedInvestorPosition->positionPtr->investorPtr->clientName = clientName;
        }
        if(clientType.ToStdString().size()!=0){
            selectedInvestorPosition->positionPtr->investorPtr->type = clientType;
        }
        if(clientSubscribed != 0){
            selectedInvestorPosition->positionPtr->subscribed = clientSubscribed;
        }
        if(clientPaid != 0){
            selectedInvestorPosition->positionPtr->paid = clientPaid;
        }
        if(clientDeployed != 0){
            selectedInvestorPosition->positionPtr->deployed = clientDeployed;
        }
        if(clientReserve != 0){
            selectedInvestorPosition->positionPtr->reserve = clientReserve;
        }
        if(clientReturnOfCapital !=0){
            selectedInvestorPosition->positionPtr->returnOfCapital = clientReturnOfCapital;
        }
        for(auto&ipd : asset->investorsPositionsDisplays){
            ipd->positionPtr->assetPtr->SetOwnershipOfPositions();
        }
        investorPositionDisplayVirtualListControl->Refresh();
        UpdateChartValuationDeploy();
        UpdateDisplayTextValues();
        this->Refresh();
    }else if(returnValue == wxID_ANY){
        //exit 
    }
}

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
        asset->distributions.push_back(newDistribution);
        for(auto&pos:asset->positions){
            pos->CalculateHistoricalManagementFees(pos->investorPtr->managementFeePercentage);
            pos->UpdateFinancesPostDistributionChanges(asset->distributions, pos->investorPtr->promoteFeePercentage, pos->investorPtr->managementFeePercentage);
        }

        distributionListControl->setItems(asset->distributions);
        distributionListControl->Update();
        UpdateDisplayTextValues();
        UpdateChartDistribution();
        asset->PopulateIRR();
        UpdateDisplayTextValues();
        this->Refresh();
    }
}
//Will need to change this if mgmt fees go to paid/committed regardless if in deployed/reserve and only omit fees on ROC
void AssetPopout::OnCapitalMovement(wxCommandEvent &e){
    MoveDeploy DeployMovementWindow(this);
    DeployMovementWindow.SetBackgroundColour(wxColor(0,0,0));
    int retValue = DeployMovementWindow.ShowModal();
    if(retValue == wxID_OK){
        wxDateTime dateOfMovement = DeployMovementWindow.GetDate();
        double amountMoved = DeployMovementWindow.GetAmountMoved();
        wxString selectedMovementDirection = DeployMovementWindow.GetSelectedMovementDirection();
        if(selectedMovementDirection == "Reserve to Deploy"){
            for(auto&pos:asset->positions){
                pos->reserve -=amountMoved * pos->percentOwnership;
                pos->deployed+=amountMoved * pos->percentOwnership;
                pos->movedToDeploy[dateOfMovement] = amountMoved * pos->percentOwnership;
                pos->CalculateHistoricalManagementFees(pos->investorPtr->managementFeePercentage);
                UpdateChartValuationDeploy();
            }
        }else if(selectedMovementDirection == "Deploy to Reserve"){
            for(auto&pos:asset->positions){
                pos->reserve+= amountMoved * pos->percentOwnership;
                pos->deployed-=amountMoved * pos->percentOwnership;
                pos->movedOutOfDeployed[dateOfMovement] = amountMoved * pos->percentOwnership;
                pos->CalculateHistoricalManagementFees(pos->investorPtr->managementFeePercentage);
                UpdateChartValuationDeploy();
            }
        }else if(selectedMovementDirection == "Reserve to Return of Capital"){
            for(auto& pos:asset->positions){
                pos->reserve-= amountMoved * pos->percentOwnership;
                pos->returnOfCapital+=amountMoved * pos->percentOwnership;
                UpdateChartValuationDeploy();
            }
        }else if(selectedMovementDirection == "Deploy to Return of Capital"){   
            for(auto&pos:asset->positions){
                pos->deployed-=amountMoved * pos->percentOwnership;
                pos->movedOutOfDeployed[dateOfMovement] = amountMoved * pos->percentOwnership;
                pos->returnOfCapital += amountMoved * pos->percentOwnership;
                //since mgmt fees are calculated based on deployed capital need to readjust them based on this movement as well
                pos->CalculateHistoricalManagementFees(pos->investorPtr->managementFeePercentage);
                UpdateChartValuationDeploy();
            }
        }
        investorPositionDisplayVirtualListControl->Refresh();
        UpdateChartValuationDeploy();
        asset->PopulateIRR();
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
        asset->valuations.push_back(newValuation);
        valuationListControl->setItems(asset->valuations);
        portfolio.ValuationDialog();
        UpdateChartValuationDeploy();
        asset->PopulateIRR();
        UpdateDisplayTextValues();
        this->Layout();
    }else if(retVal == wxID_CANCEL){
        //do nothing and exit
    }
}

void AssetPopout::OnAddEvent(wxCommandEvent &e){
    EventDialog addEventDialog(this,false);
    addEventDialog.SetBackgroundColour(wxColor(0,0,0));
    int retVal = addEventDialog.ShowModal();
    if(retVal == wxID_OK){
        wxDateTime eventDate = addEventDialog.GetDate();
        bool eventHasHappened = addEventDialog.GetHasHappened();
        wxString eventDetails = addEventDialog.GetDescription();

        AssetEvent newEvent;
        newEvent.eventDate = eventDate;
        newEvent.hasHappened = eventHasHappened;
        newEvent.eventDetails = eventDetails;
        std::shared_ptr<AssetEvent> newEventPtr = std::make_shared<AssetEvent>(newEvent);
        asset->events.push_back(newEventPtr);
        portfolio.assetEventPtrs.push_back(newEventPtr);

        eventsVirtualListControl->setItems(asset->events);
    }else if(retVal== wxID_CANCEL){
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
    Distribution& selectedDistribution = asset->distributions[dataIndex];
    wxDateTime distributionDate = selectedDistribution.distribution.first;
    double distributionAmount = selectedDistribution.distribution.second;
    DistributionDialog distributionEditwindow(this,true, distributionDate, distributionAmount);

    distributionEditwindow.SetBackgroundColour(wxColor(0,0,0));
    int retVal = distributionEditwindow.ShowModal();
    if(retVal == wxID_OK){
        selectedDistribution.distribution.first = distributionEditwindow.GetDistributionDate();
        selectedDistribution.distribution.second = distributionEditwindow.GetDistributionAmount();
        for(auto &pos:asset->positions){
            pos->UpdateFinancesPostDistributionChanges(asset->distributions, pos->investorPtr->promoteFeePercentage, pos->investorPtr->managementFeePercentage);
        }
        distributionListControl->setItems(asset->distributions);
        distributionListControl->Update();
        UpdateDisplayTextValues();
        UpdateChartDistribution();
        this->Refresh();
    }else if(retVal == MY_CUSTOM_DELETE_CODE){
        if(dataIndex >= 0 && dataIndex < asset->distributions.size()){
            std::swap(asset->distributions[dataIndex], asset->distributions.back());
            asset->distributions.pop_back();
        }
        for(auto&pos : asset->positions){
            pos->UpdateFinancesPostDistributionChanges(asset->distributions, pos->investorPtr->promoteFeePercentage, pos->investorPtr->managementFeePercentage);
        }
        distributionListControl->setItems(asset->distributions);
        distributionListControl->Update();
        UpdateDisplayTextValues();
        UpdateChartDistribution();
        asset->PopulateIRR();
        UpdateDisplayTextValues();
        this->Refresh();
    }
    UpdateChartDistribution();
}

void AssetPopout::OnValuationEdit(wxListEvent &e){
    long listIndex = e.GetIndex();
    long dataIndex = valuationListControl->orderedIndices[listIndex];
    Valuation& valuationToEdit = asset->valuations[dataIndex];
    wxDateTime setDate  = valuationToEdit.valuationDate;
    double setValue = valuationToEdit.valuation;
    ValuationDialog valuationWindow(this, true, setDate, setValue);
    valuationWindow.SetBackgroundColour(wxColor(0,0,0));
    int retValue = valuationWindow.ShowModal();
    if(retValue == wxID_OK){
        valuationToEdit.valuation = valuationWindow.GetValuation();
        valuationToEdit.valuationDate = valuationWindow.GetDate();
        valuationListControl->setItems(asset->valuations);
        valuationListControl->Update();
        UpdateChartValuationDeploy();
        asset->PopulateIRR();
        UpdateDisplayTextValues();
        this->Refresh();
    }else if(retValue == MY_VALUATION_DELETE_CODE){
        if(dataIndex>=0 && dataIndex < asset->valuations.size()){
            std::swap(asset->valuations[dataIndex], asset->valuations.back());
            asset->valuations.pop_back();
        }
        valuationListControl->setItems(asset->valuations);
        valuationListControl->Update();
        UpdateChartValuationDeploy();
        asset->PopulateIRR();
        UpdateDisplayTextValues();
        this->Refresh();
    }
    this->Refresh();
    this->Layout();
}

void AssetPopout::OnEventEdit(wxListEvent &e){
    long listIndex = e.GetIndex();
    long dataIndex = eventsVirtualListControl->orderedIndices[listIndex];
    std::shared_ptr<AssetEvent> eventToEdit = asset->events[dataIndex];
    EventDialog eventDialog(this, true);
    eventDialog.SetBackgroundColour(wxColor(0,0,0));
    int retVal = eventDialog.ShowModal();
    if(retVal == wxID_OK){
        eventToEdit->eventDate = eventDialog.GetDate();
        eventToEdit->hasHappened = eventDialog.GetHasHappened();
        eventToEdit->eventDetails = eventDialog.GetDescription();
        eventsVirtualListControl->setItems(asset->events);
        this->Refresh();
        this->Update();
    }else if(retVal == DELETE_CODE_EVENT){
        if(dataIndex >=0 && dataIndex < asset->events.size()){
            std::swap(asset->events[dataIndex], asset->events.back());
            asset->events.pop_back();
        }
        eventsVirtualListControl->setItems(asset->events);
        eventsVirtualListControl->Refresh();
        eventsVirtualListControl->Update();
        this->Update();
    }
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
        std::cout<<"Distribution:i = "<<i<<" Data[i] = "<<values[i]<< "times[i] = "<<asset->GetDistributionsForPlotting()[i].first.FormatISODate()<<std::endl;
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
    
}
void AssetPopout::CreateNewInvestor(){

}