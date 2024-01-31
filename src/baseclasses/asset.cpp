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

