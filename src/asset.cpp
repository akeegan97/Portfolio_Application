#include "asset.hpp"
#include "investor.hpp"
#include "portfolio.hpp"


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

void from_json(const json &j, Asset &as, Portfolio &porf) {
    if (j.contains("Asset Name")) {
        as.assetName = j["Asset Name"].get<std::string>().c_str();
    }
    
    if (j.contains("Asset Exit Date")) {
        wxString dateStr = wxString::FromUTF8(j["Asset Exit Date"].get<std::string>().c_str());
        wxDateTime dateParse;
        dateParse.ParseDate(dateStr);
        as.assetExitDate = dateParse;
    }

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
    //changing to make_shared
    if (j.contains("Events") && j["Events"].is_array()) {
        for (const auto &evtJson : j["Events"]) {
            auto event = std::make_shared<AssetEvent>(evtJson.get<AssetEvent>());
            as.events.push_back(event);
        }
    }
}


std::vector<wxString> Asset::columnNames = {"Asset Name","Exit Date","Total Invested Capital","Number of Investors","Current Value"};
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

double Asset::CalculateInvestedCapital()const{
    double totalInvested = 0;
    for(const auto& investor: investors){
        for(const auto&position:investor.positions){
            totalInvested+=position.investedAmountUp;
            totalInvested+=position.investedAmountDown;
        }
    }
    return totalInvested;
}

double Asset::CalculateNumberOfInvestors()const{
    return investors.size();
}

double Asset::GetLastValuation()const{
    if(valuations.empty()){
        return 0.0;
    }else{
        return valuations.back().valuation;
    }
}

void Asset::UpdateDerivedValues(){
    countOfInvestors = CalculateNumberOfInvestors();
    totalInvestedCapital = CalculateInvestedCapital();
    currentValue = GetLastValuation();
}