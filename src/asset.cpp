#include "asset.hpp"
#include "investor.hpp"
#include "portfolio.hpp"
#include "distribution.hpp"
/**
*@brief Convert the Asset object to JSON format.
*
*@param j - Reference to a JSON object.
*@param as - The Asset object to be converted.
*/
void to_json(json &j, const Asset &as) {
    j = {
        {"Asset Name", as.assetName.ToStdString()},
        {"Asset Exit Date", as.assetExitDate.FormatISODate()},
        {"Valuations", as.valuations},
        {"Investors", json::array({})},
        {"Events", json::array({})},
        {"Distributions", json::array({})}
    };
    for(const auto&investorPtr: as.investors){
        if(investorPtr){
            j["Investors"].push_back(*investorPtr);
        }
    }
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
/**
*@brief Convert JSON object to Asset.
*
*@param j - JSON object to be converted.
*@param as - Reference to an Asset Object.
*param porf - Reference to Portfolio Object.
*/
void from_json(const json &j, Asset &as, Portfolio &porf) {
    if (j.contains("Valuations") && j["Valuations"].is_array()) {
        as.valuations = j["Valuations"].get<std::vector<Valuation>>();
    }

    if (j.contains("Investors") && j["Investors"].is_array()) {
        for (const auto &invJson : j["Investors"]) {
            auto inv = std::make_shared<Investor>();
            from_json(invJson, *inv, porf);
            as.investors.push_back(inv);
        }
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
//Definitions of static members of Asset class
std::vector<wxString> Asset::columnNames = {"Asset Name","Exit Date","Total Deployed","# Investors","Current Value"};
std::vector<int> Asset::columnWidths = {150,75,100,100,100};
/**
*@brief Get the value of a specified column for the Asset being displayed in a VLC
*
*@param col - The Column index of the VLC
*@return wxVariant - The Value at that column.
*/
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
/**
*@brief Set the value for a specified column for the Asset.
*
*@param col - The column index.
*@param v - the value to be set.
*
*/
void Asset::SetValue(int col, const wxVariant &v){
    switch(col){
        case 0: assetName = v.GetString();break;
        case 1: assetExitDate = v.GetDateTime();break;
        case 2: totalInvestedCapital = v.GetDouble();break;
        case 3: countOfInvestors = v.GetDouble();break;
        case 4: currentValue = v.GetDouble();break;
    }
}
/**
*@brief Calculate total invested capital in the Asset.
*
*@return double total amount of invested capital in asset
*
*/
double Asset::CalculateInvestedCapital()const{
    double totalInvested = 0;
    for(const auto& investor: investors){
        for(const auto&position:investor->positions){
            totalInvested+=position->deployed;
        }
    }
    return totalInvested;
}
/**
*@brief Calculate total number of investors in Asset.
*

*@return double size of investors vector.
*/
double Asset::CalculateNumberOfInvestors()const{
    return investors.size();
}
/**
*@brief Gets latest valuation for Asset
*
*@return double last valuation added to valuations vector.
*/
double Asset::GetLastValuation() const {
    if (valuations.empty()) {
        double deployedCapital = 0.0;
        for (const auto& inv : investors) {
            for (const auto& pos : inv->positions) {
                if (pos->assetPtr.get() == this) {
                    deployedCapital += pos->deployed;
                }
            }
        }
        return deployedCapital;
    } else {
        // Make a copy of valuations for sorting if the original vector is const
        auto sortedValuations = valuations; 
        std::sort(sortedValuations.begin(), sortedValuations.end(), 
            [](const Valuation& a, const Valuation& b) {
                return a.valuationDate.IsEarlierThan(b.valuationDate);
            }
        );
        return sortedValuations.back().valuation;
    }
}
/**
*@brief Update the values of the Asset based off of the functions
*
*/
void Asset::UpdateDerivedValues(){
    countOfInvestors = CalculateNumberOfInvestors();
    totalInvestedCapital = CalculateInvestedCapital();
    currentValue = GetLastValuation();
}

double Asset::CalculateReserveCapital(){
    double reserveCapital = 0;
    for(const auto&investor:investors){
        for(const auto&position : investor->positions){
            reserveCapital+=position->reserve;
        }
    }
    return reserveCapital;
}

double Asset::CalculatePaidCapital(){
    double paidCapital = 0;
    for(const auto&investor:investors){
        for(const auto&position : investor->positions){
            paidCapital+=position->paid;
        }
    }
    return paidCapital;
}
double Asset::CalculateSubscribedCapital(){
    double subscribed = 0;
    for(const auto&investor:investors){
        for(const auto&position : investor->positions){
            subscribed+=position->subscribed;
        }
    }
    return subscribed;
}
double Asset::CalculateReturnedCapital(){
    double returnedCapital = 0;
    for(const auto&investor:investors){
        for(const auto&position : investor->positions){
            returnedCapital+=position->returnOfCapital;
        }
    }
    return returnedCapital;
}

double Asset::GetTotalMgmtFeesGenerated(){
    double totalMgmtFees=0;
    for(const auto&inv : investors){
        for(const auto&pos:inv->positions){
            for(const auto &mgmtFee : pos->managementFees){
                totalMgmtFees+= mgmtFee.managementFeesAsset.second;
            }
        }
    }
    return totalMgmtFees;
}

double Asset::GetTotalPromoteFeesGenerated(){
    double totalPromoteFees=0;
    for(const auto&inv : investors){
        for(const auto&pos:inv->positions){
            for(const auto &pfee : pos->promoteFees){
                totalPromoteFees+= pfee.promotefee.second;
            }
        }
    }
    return totalPromoteFees;
}


