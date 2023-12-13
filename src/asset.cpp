#include "asset.hpp"
#include "investor.hpp"
#include "portfolio.hpp"
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
        {"Investors", as.investors},
        {"Events", json::array({})}
    };
    
    for (const auto& evtPtr : as.events) {
        j["Events"].push_back(*evtPtr);
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
            Investor inv;
            from_json(invJson, inv, porf);
            as.investors.push_back(inv);
        }
    }

    if (j.contains("Events") && j["Events"].is_array()) {
        for (const auto &evtJson : j["Events"]) {
            auto event = std::make_shared<AssetEvent>(evtJson.get<AssetEvent>());
            as.events.push_back(event);
        }
    }
}
//Definitions of static members of Asset class
std::vector<wxString> Asset::columnNames = {"Asset Name","Exit Date","Total Invested Capital","Number of Investors","Current Value"};
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
        for(const auto&position:investor.positions){
            totalInvested+=position.deployed;
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
double Asset::GetLastValuation()const{
    if(valuations.empty()){
        return 0.0;
    }else{
        return valuations.back().valuation;
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
        for(const auto&position : investor.positions){
            reserveCapital+=position.reserve;
        }
    }
    return reserveCapital;
}

double Asset::CalculatePaidCapital(){
    double paidCapital = 0;
    for(const auto&investor:investors){
        for(const auto&position : investor.positions){
            paidCapital+=position.paid;
        }
    }
    return paidCapital;
}
double Asset::CalculateSubscribedCapital(){
    double subscribed = 0;
    for(const auto&investor:investors){
        for(const auto&position : investor.positions){
            subscribed+=position.subscribed;
        }
    }
    return subscribed;
}
double Asset::CalculateReturnedCapital(){
    double returnedCapital = 0;
    for(const auto&investor:investors){
        for(const auto&position : investor.positions){
            returnedCapital+=position.returnOfCapital;
        }
    }
    return returnedCapital;
}

std::pair<wxDateTime, wxDateTime> getCurrentQuarterDates(const wxDateTime& currentDate) {
    int year = currentDate.GetYear();
    wxDateTime quarterStart, quarterEnd;

    if (currentDate >= wxDateTime(1, wxDateTime::Jan, year) && currentDate < wxDateTime(1, wxDateTime::Apr, year)) {
        // Q1
        quarterStart = wxDateTime(1, wxDateTime::Jan, year);
        quarterEnd = wxDateTime(31, wxDateTime::Mar, year);
    } else if (currentDate >= wxDateTime(1, wxDateTime::Apr, year) && currentDate < wxDateTime(1, wxDateTime::Jul, year)) {
        // Q2
        quarterStart = wxDateTime(1, wxDateTime::Apr, year);
        quarterEnd = wxDateTime(30, wxDateTime::Jun, year);
    } else if (currentDate >= wxDateTime(1, wxDateTime::Jul, year) && currentDate < wxDateTime(1, wxDateTime::Oct, year)) {
        // Q3
        quarterStart = wxDateTime(1, wxDateTime::Jul, year);
        quarterEnd = wxDateTime(30, wxDateTime::Sep, year);
    } else {
        // Q4
        if (currentDate >= wxDateTime(1, wxDateTime::Oct, year)) {
            quarterStart = wxDateTime(1, wxDateTime::Oct, year);
            quarterEnd = wxDateTime(31, wxDateTime::Dec, year);
        } else {
            // If the current date is in January or February of the new year, set to previous year's Q4
            quarterStart = wxDateTime(1, wxDateTime::Oct, year - 1);
            quarterEnd = wxDateTime(31, wxDateTime::Dec, year - 1);
        }
    }

    return {quarterStart, quarterEnd};
}

int Asset::calculateDaysBetween(const wxDateTime &start, const wxDateTime &end){
    if(end.IsEarlierThan(start)){
        return 0;
    }else{
        wxTimeSpan span = end - start;
        return span.GetDays();
    }
}
