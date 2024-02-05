#include "baseclasses/asset.hpp"
#include "baseclasses/investor.hpp"
#include "baseclasses/portfolio.hpp"
#include "baseclasses/distribution.hpp"
#include <unordered_set>

void to_json(json &j, const Asset &as) {
    j = {
        {"Asset Name", as.assetName.ToStdString()},
        {"Asset Exit Date", as.assetExitDate.FormatISODate()},
        {"Valuations", as.valuations},
        {"Events", json::array({})},
        {"Distributions", json::array({})}
    };
    for (const auto& evtPtr : as.events) {
        if(evtPtr){
            j["Events"].push_back(*evtPtr);
        }
    }
    for(const auto&distribution:as.distributions){
        json distributionJson;
        to_json(distributionJson, distribution);
        j["Distributions"].push_back(distributionJson);
    }
}

void from_json(const json &j, Asset &as, Portfolio &porf) {
    if (j.contains("Valuations") && j["Valuations"].is_array()) {
        as.valuations = j["Valuations"].get<std::vector<Valuation>>();
    }

    if (j.contains("Events") && j["Events"].is_array()) {
        for (const auto &evtJson : j["Events"]) {
            auto event = std::make_shared<AssetEvent>(evtJson.get<AssetEvent>());
            as.events.push_back(event);
        }
    }
    if(j.contains("Distributions") && j["Distributions"].is_array()){
        for(const auto &distributionJson : j["Distributions"]){
            Distribution distribution;
            from_json(distributionJson, distribution);
            as.distributions.push_back(distribution);
        }
    }
}


std::vector<wxString> Asset::columnNames = {"Asset Name","Exit Date","Total Deployed","# Investors","Current Value"};
std::vector<int> Asset::columnWidths = {150,75,100,100,100};

wxVariant Asset::GetValue(int col)const{
    switch(col){
        case 0: return wxVariant(assetName);
        case 1: return wxVariant(assetExitDate);
        case 2: return wxVariant(totalInvestedCapital);
        case 3: return wxVariant(countOfInvestors);
        case 4: return wxVariant(currentValue);
        default: return wxVariant();
    }
}

void Asset::SetValue(int col, const wxVariant &v){
    switch(col){
        case 0: assetName = v.GetString();break;
        case 1: assetExitDate = v.GetDateTime();break;
        case 2: totalInvestedCapital = v.GetDouble();break;
        case 3: countOfInvestors = v.GetDouble();break;
        case 4: currentValue = v.GetDouble();break;
    }
}

double Asset::CalculateDeployedCapital()const{
    double totalInvested = 0;
    for(const auto&pos:positions){
        totalInvested+=pos->deployed;
    }
    return totalInvested;
}

double Asset::CalculateNumberOfInvestors() const {
    std::unordered_set<std::string> uniqueInvestors;

    for (const auto& position : positions) {
        if (position->investorPtr) {
            uniqueInvestors.insert(position->investorPtr->clientName.ToStdString());
        }
    }

    return static_cast<double>(uniqueInvestors.size());
}

double Asset::GetLastValuation() const {
    if(valuations.empty()){
        double deployedCap = 0.0;
        for(const auto&pos: positions){
            deployedCap+=pos->deployed;
        }
        return deployedCap;
    }else {
        auto sortedValuations = valuations; 
        std::sort(sortedValuations.begin(), sortedValuations.end(), 
            [](const Valuation& a, const Valuation& b) {
                return a.valuationDate.IsEarlierThan(b.valuationDate);
            }
        );
        return sortedValuations.back().valuation;
    }
}

void Asset::UpdateDerivedValues(){
    countOfInvestors = CalculateNumberOfInvestors();
    totalInvestedCapital = CalculateDeployedCapital();
    currentValue = GetLastValuation();
    totalMgmtFees = GetTotalMgmtFeesGenerated();
    SetOwnershipOfPositions();
    totalMgmtFeesDue = GetTotalMgmtFeesDue();
}

double Asset::CalculateReserveCapital(){
    double reserveCapital = 0;
    for(const auto&pos:positions){
        reserveCapital+=pos->reserve;
    }
    return reserveCapital;
}

double Asset::CalculatePaidCapital(){
    double paidCapital = 0;
    for(const auto&pos: positions){
        paidCapital+=pos->paid;
    }
    return paidCapital;
}
double Asset::CalculateSubscribedCapital(){
    double subscribed = 0;
    for(const auto&pos:positions){
        subscribed+=pos->subscribed;
    }
    return subscribed;
}
double Asset::CalculateReturnedCapital(){
    double returnedCapital = 0;
    for(const auto&pos:positions){
        returnedCapital+=pos->returnOfCapital;
    }
    return returnedCapital;
}

double Asset::GetTotalMgmtFeesGenerated(){
    double totalMgmtFees=0;
    for(const auto&pos:positions){
        for(const auto&fee:pos->managementFees){
            totalMgmtFees+=fee.managementFeesAsset.second;
        }
    }
    return totalMgmtFees;
}

double Asset::GetTotalMgmtFeesDue(){
    double totalDue = 0;
    for(const auto&pos:positions){
        totalDue+=pos->mgmtFeesDue;
    }
    return totalDue;
}

double Asset::GetTotalPromoteFeesGenerated(){
    double totalPromoteFees=0;
    for(const auto&pos: positions){
        for(const auto&pfee:pos->promoteFees){
            totalPromoteFees+=pfee.promotefee.second;
        }
    }
    return totalPromoteFees;
}

void Asset::SetOwnershipOfPositions(){
    double totalPaid = 0.0;
    for(const auto&pos:positions){
        totalPaid+=pos->paid;
    }
    for(auto&pos:positions){
        pos->percentOwnership = pos->paid/totalPaid;
    }
}

void Asset::PopulateValuationDeploymentForPlotting(){
    currentQDeployMap.clear();
    previousQDeployMap.clear();
    currentQValuationMap.clear();
    previousQValuationMap.clear();

    valuationsForPlotting.clear();
    deploymentsForPlotting.clear();

    PopulatePreviousQValuations();
    PopulatePreviousQDeploys();

    PopulateCurrentQValuations();
    PopulateCurrentQDeploys();

    for(const auto&entry:previousQDeployMap){
        deploymentsForPlotting.push_back(entry);
    }
    for(const auto&entry:currentQDeployMap){
        deploymentsForPlotting.push_back(entry);
    }
    for(const auto&entry:previousQValuationMap){
        valuationsForPlotting.push_back(entry);
    }
    for(const auto&entry:currentQValuationMap){
        valuationsForPlotting.push_back(entry);
    }

    std::sort(valuationsForPlotting.begin(), valuationsForPlotting.end(),
                [](const std::pair<wxDateTime, double>&a, const std::pair<wxDateTime, double>&b){
                    return a.first.IsEarlierThan(b.first);
                });

    std::sort(deploymentsForPlotting.begin(), deploymentsForPlotting.end(),
            [](const std::pair<wxDateTime, double>&a, const std::pair<wxDateTime, double>&b){
                return a.first.IsEarlierThan(b.first);
            });
}

void Asset::PopulatePreviousQValuations(){
    previousQValuationMap.clear();
    wxDateTime oldestInvestedDate = wxDateTime::Today();
    for(const auto &position: positions){
        if(position->dateInvested.IsEarlierThan(oldestInvestedDate)){
            oldestInvestedDate = position->dateInvested;
        }
    }

    wxDateTime qEndDate = utilities::GetQuarterEndDate(oldestInvestedDate);
    wxDateTime currentQDate = wxDateTime::Today();
    wxDateTime currentQStartDate = utilities::GetQuarterStartDate(currentQDate);

    //sorting valuations for this asset
    std::sort(valuations.begin(), valuations.end(),
        [](const Valuation& a, const Valuation& b) {
            return a.valuationDate < b.valuationDate;
        }
    );

    while(qEndDate.IsEarlierThan(currentQStartDate)){
        double quarterValuation = 0.0;
        for(const auto& val:valuations){
            if(val.valuationDate.IsEarlierThan(qEndDate) || val.valuationDate == qEndDate){
                quarterValuation = val.valuation;
            }
        }

        previousQValuationMap[qEndDate] = quarterValuation;
        qEndDate = utilities::GetNextQuarterEndDate(qEndDate);
    }

}

void Asset::PopulateCurrentQValuations(){
    double currentValuation = previousQValuationMap.rbegin()->second;
    wxDateTime today = wxDateTime::Today();
    wxDateTime currentQStartDate = utilities::GetQuarterStartDate(today);

    std::vector<std::pair<wxDateTime, double>> valuationsInCurrentQ;
    for(auto&val:valuations){
        if(val.valuationDate>= currentQStartDate){
            valuationsInCurrentQ.push_back({val.valuationDate,val.valuation});
        }
    }
    //now valuationsInCurrentQ only have valuations that are in the currentQ at runtime
    double lastValuation = currentValuation;
    for(auto& val:valuations){
        if(val.valuationDate>=currentQStartDate){
            double differential = val.valuation - lastValuation;
            currentValuation+=differential;
            currentQValuationMap[val.valuationDate] = currentValuation;
            lastValuation = val.valuation;
        }
    }
}

void Asset::PopulatePreviousQDeploys() {
    previousQDeployMap.clear();
    wxDateTime oldestInvestedDate = wxDateTime::Today();
    for (const auto& position : positions) {
        if (position->dateInvested.IsEarlierThan(oldestInvestedDate)) {
            oldestInvestedDate = position->dateInvested;
        }
    }

    wxDateTime qEndDate = utilities::GetQuarterEndDate(oldestInvestedDate);
    wxDateTime currentQDate = wxDateTime::Today();
    wxDateTime currentQStartDate = utilities::GetQuarterStartDate(currentQDate);

    while (qEndDate.IsEarlierThan(currentQStartDate)) {
        double deployedCapitalThisQ = 0.0;
        for (const auto& position : positions) {
            if(position->dateInvested<=qEndDate){
                double adjustedDeployed = position->deployed;
                for (const auto& move : position->movedToDeploy) {
                    if (move.first > qEndDate) {
                        adjustedDeployed -= move.second;
                    }
                }
                for (const auto& move : position->movedOutOfDeployed) {
                    if (move.first > qEndDate) {
                        adjustedDeployed += move.second;
                    }
                }
                deployedCapitalThisQ += adjustedDeployed;                
            }
        }
        previousQDeployMap[qEndDate] = deployedCapitalThisQ;
        qEndDate = utilities::GetNextQuarterEndDate(qEndDate);
    }
}

void Asset::PopulateCurrentQDeploys(){
    double currentDeploy = previousQDeployMap.rbegin()->second;
    wxDateTime today = wxDateTime::Today();
    wxDateTime currentQStartDate = utilities::GetQuarterStartDate(today);
    wxDateTime currentQEndDate = utilities::GetQuarterEndDate(today);
    std::map<wxDateTime, double> aggregatedMovements;
    std::vector<std::pair<wxDateTime, double>> deployedInCurrentQ;
    for(const auto&position:positions){
       for(const auto& [date, amount]:position->movedToDeploy){
            if(date >= currentQStartDate && date<= currentQEndDate){
                aggregatedMovements[date] +=amount;
            }
        }
        for(const auto&[date, amount]:position->movedOutOfDeployed){
            if(date>=currentQStartDate && date<= currentQEndDate){
                aggregatedMovements[date] -=amount;
            }
        }
    }
    for(const auto&[date, movement]:aggregatedMovements){
        currentDeploy+=movement;
        currentQDeployMap[date] = currentDeploy;
    }
}

//populating barchart vector

void Asset::PopulateDistributionsForPlotting(){
    distributionsForPlottingBarChart.clear();
    if(distributions.empty()){
        return;
    }
    std::sort(distributions.begin(),distributions.end(),
            [](const Distribution &a, const Distribution &b){
                return a.distribution.first.IsEarlierThan(b.distribution.first);
            });
    wxDateTime oldestDistribution = distributions.front().distribution.first;
    wxDateTime newestDistribution = distributions.back().distribution.first;
    wxDateTime qEndDate = utilities::GetQuarterEndDate(oldestDistribution);
    wxDateTime lastQEndDate = utilities::GetQuarterEndDate(newestDistribution);
    while(qEndDate <= lastQEndDate){
        wxDateTime currentQStartDate = utilities::GetQuarterStartDate(qEndDate);
        double currentQDistributedAmount = 0.0;
        for(const auto&dist:distributions){
            if(dist.distribution.first >= currentQStartDate && dist.distribution.first <=qEndDate){
                currentQDistributedAmount+=dist.distribution.second;
            }
        }
        distributionsForPlottingBarChart.push_back({qEndDate,currentQDistributedAmount});
        qEndDate = utilities::GetNextQuarterEndDate(qEndDate);
    }
}

void Asset::PopulateIRR(){
    std::vector<CashFlow> cashflows;
    //get the initial deployed amount each investor -negative for asset XIRR calculation
    for(const auto&pos:positions){
        double initialDeployment = pos->deployed;
        for(const auto&movement:pos->movedToDeploy){
            initialDeployment-=movement.second;
        }
        for(const auto&movement:pos->movedOutOfDeployed){
            initialDeployment+=movement.second;
        }
        CashFlow newCashFlow;
        newCashFlow.date = pos->dateInvested;
        newCashFlow.amount = -initialDeployment;
        cashflows.push_back(newCashFlow);
    }
    //get positive cashflows ie gross distributions
    for(const auto&dist : distributions){
        CashFlow newCashFlow;
        newCashFlow.amount = dist.distribution.second;
        newCashFlow.date = dist.distribution.first;
        cashflows.push_back(newCashFlow);
    }
    //need to also include the negative amounts/positive amounts that come from deploy movements
    //get either last valuation or summed currently deployed amount
    if(!valuations.empty()){
        std::sort(valuations.begin(), valuations.end(),
                [](const Valuation &a, const Valuation &b){
                    return a.valuationDate.IsEarlierThan(b.valuationDate);
                });
        CashFlow newCashflow;
        newCashflow.amount = valuations.back().valuation;
        newCashflow.date = wxDateTime::Today();
        cashflows.push_back(newCashflow);
    }else{
        CashFlow newCashFlow;
        double totalDeployed = 0.0;
        for(const auto &pos:positions){
            totalDeployed+=pos->deployed;
        }
        newCashFlow.amount = totalDeployed;
        newCashFlow.date = wxDateTime::Today();
        cashflows.push_back(newCashFlow);
    }
    //now that we have a full cashflow list
    std::sort(cashflows.begin(), cashflows.end(),
    [](const CashFlow &a, const CashFlow &b) {
        return a.date < b.date;
    });
    for(const auto&cf:cashflows){
        std::cout<<"Asset: Cashflow Date: "<<cf.date.FormatISODate().ToStdString()<<" | Cashflow Amount: "<<cf.amount<<std::endl;
    //debug prints
    }
    irr = 0.0;
    double guess = 0.1;
    double x1 = 0.0;
    int maxIterations = 100;
    double precision = 0.000001;
    for(int i =0;i<maxIterations;i++){
        double npv = CalculateNPV(cashflows,guess);
        double guessAddPrecision = guess+precision;
        double npvPrime = (CalculateNPV(cashflows, guessAddPrecision)-npv)/precision;
        x1 = guess - npv / npvPrime;
        if(std::fabs(x1-guess)<=precision){
            irr= x1;
            break;
        }
        guess = x1;
    }

}

double Asset::CalculateNPV(std::vector<CashFlow>&cashflows, double &rate){
    double npv = 0.0;
    wxDateTime firstDate = cashflows[0].date;
    for(const auto&cf:cashflows){
        wxTimeSpan timespan = cf.date-firstDate;
        double years = timespan.GetDays()/365.25;
        npv+=cf.amount /std::pow(1+rate, years);
    }
    return npv;
}
