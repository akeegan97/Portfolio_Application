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
    for (const auto& assetPtr:porf.assetPtrs){
        for (const auto& investor:assetPtr->investors){
            std::cout<<"Investor: "<<investor.clientName<<std::endl;
            for(const auto& position:investor.positions){
                std::cout<<"Position Asset Ptr: "<<position.assetPtr->assetName<<std::endl;
            }
            //bug found, investors.position.assetPtr is not being set correctly resulting in a segmentation fault
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
            totalInvested+=position.investedAmountUp;
            totalInvested+=position.investedAmountDown;
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
