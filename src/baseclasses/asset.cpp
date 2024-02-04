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

    wxDateTime qEndDate = GetQuarterEndDate(oldestInvestedDate);
    wxDateTime currentQDate = wxDateTime::Today();
    wxDateTime currentQStartDate = GetQuarterStartDate(currentQDate);

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
        qEndDate = GetNextQuarterEndDate(qEndDate);
    }

}

void Asset::PopulateCurrentQValuations(){
    double currentValuation = previousQValuationMap.rbegin()->second;
    wxDateTime today = wxDateTime::Today();
    wxDateTime currentQStartDate = GetQuarterStartDate(today);

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

    wxDateTime qEndDate = GetQuarterEndDate(oldestInvestedDate);
    wxDateTime currentQDate = wxDateTime::Today();
    wxDateTime currentQStartDate = GetQuarterStartDate(currentQDate);

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
        qEndDate = GetNextQuarterEndDate(qEndDate);
    }
}

void Asset::PopulateCurrentQDeploys(){
    double currentDeploy = previousQDeployMap.rbegin()->second;
    wxDateTime today = wxDateTime::Today();
    wxDateTime currentQStartDate = GetQuarterStartDate(today);
    wxDateTime currentQEndDate = GetQuarterEndDate(today);
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

wxDateTime Asset::GetQuarterEndDate(wxDateTime &currentDate){
    int year = currentDate.GetYear();

    wxDateTime quarterEnd;
    if (currentDate >= wxDateTime(1, wxDateTime::Jan, year) && currentDate < wxDateTime(1, wxDateTime::Apr, year)) {
        // Q1
        quarterEnd = wxDateTime(31, wxDateTime::Mar, year);
    } else if (currentDate >= wxDateTime(1, wxDateTime::Apr, year) && currentDate < wxDateTime(1, wxDateTime::Jul, year)) {
        // Q2
        quarterEnd = wxDateTime(30, wxDateTime::Jun, year);
    } else if (currentDate >= wxDateTime(1, wxDateTime::Jul, year) && currentDate < wxDateTime(1, wxDateTime::Oct, year)) {
        // Q3
        quarterEnd = wxDateTime(30, wxDateTime::Sep, year);
    } else {
        // Q4
        quarterEnd = wxDateTime(31, wxDateTime::Dec, year);
    }
    return quarterEnd;
}

bool Asset::IsWithinQuarter(const wxDateTime&date,const wxDateTime &quarterEndDate){
    wxDateTime qStart, qEnd;
    int year = quarterEndDate.GetYear();
    if (quarterEndDate >= wxDateTime(1, wxDateTime::Jan, year) && quarterEndDate < wxDateTime(1, wxDateTime::Apr, year)) {
        // Q1
        qStart = wxDateTime(1, wxDateTime::Jan, year);
        qEnd = wxDateTime(31, wxDateTime::Mar, year);
    } else if (quarterEndDate >= wxDateTime(1, wxDateTime::Apr, year) && quarterEndDate < wxDateTime(1, wxDateTime::Jul, year)) {
        // Q2
        qStart = wxDateTime(1, wxDateTime::Apr, year);
        qEnd = wxDateTime(30, wxDateTime::Jun, year);
    } else if (quarterEndDate >= wxDateTime(1, wxDateTime::Jul, year) && quarterEndDate < wxDateTime(1, wxDateTime::Oct, year)) {
        // Q3
        qStart = wxDateTime(1, wxDateTime::Jul, year);
        qEnd = wxDateTime(30, wxDateTime::Sep, year);
    } else {
        // Q4
        qStart = wxDateTime(1, wxDateTime::Oct, year);
        qEnd = wxDateTime(31, wxDateTime::Dec, year);
    }

    return date.IsBetween(qStart, qEnd);
}

wxDateTime Asset::GetQuarterStartDate(wxDateTime &date){
    int year = date.GetYear();

    wxDateTime quarterStartDate;
    if (date >= wxDateTime(1, wxDateTime::Jan, year) && date < wxDateTime(1, wxDateTime::Apr, year)) {
        // Q1
        quarterStartDate = wxDateTime(1, wxDateTime::Jan, year);
    } else if (date >= wxDateTime(1, wxDateTime::Apr, year) && date < wxDateTime(1, wxDateTime::Jul, year)) {
        // Q2
        quarterStartDate = wxDateTime(1, wxDateTime::Apr, year);
    } else if (date >= wxDateTime(1, wxDateTime::Jul, year) && date < wxDateTime(1, wxDateTime::Oct, year)) {
        // Q3
        quarterStartDate = wxDateTime(1, wxDateTime::Jul, year);
    } else {
        // Q4
        quarterStartDate = wxDateTime(1, wxDateTime::Oct, year);
    }
    return quarterStartDate;
}

wxDateTime Asset::GetNextQuarterEndDate(wxDateTime &currentEndDate){
    wxDateTime nextEndingQuarter;
    int year = currentEndDate.GetYear();
    if(currentEndDate.GetMonth()<= wxDateTime::Mar){
        nextEndingQuarter = wxDateTime(30, wxDateTime::Jun, year);
    }else if(currentEndDate.GetMonth()<=wxDateTime::Jun){
        nextEndingQuarter = wxDateTime(30, wxDateTime::Sep, year);
    }else if(currentEndDate.GetMonth()<=wxDateTime::Sep){
        nextEndingQuarter = wxDateTime(31,wxDateTime::Dec, year);
    }else if(currentEndDate.GetMonth()<=wxDateTime::Dec){
        nextEndingQuarter = wxDateTime(31, wxDateTime::Mar, year+1);
    }
    return nextEndingQuarter;
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
    wxDateTime qEndDate = GetQuarterEndDate(oldestDistribution);
    wxDateTime lastQEndDate = GetQuarterEndDate(newestDistribution);
    while(qEndDate <= lastQEndDate){
        wxDateTime currentQStartDate = GetQuarterStartDate(qEndDate);
        double currentQDistributedAmount = 0.0;
        for(const auto&dist:distributions){
            if(dist.distribution.first >= currentQStartDate && dist.distribution.first <=qEndDate){
                currentQDistributedAmount+=dist.distribution.second;
            }
        }
        distributionsForPlottingBarChart.push_back({qEndDate,currentQDistributedAmount});
        qEndDate = GetNextQuarterEndDate(qEndDate);
    }
}
