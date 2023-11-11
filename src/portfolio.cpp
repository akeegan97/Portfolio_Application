#include "portfolio.hpp"
#include "asset.hpp"


void to_json(json &j, const Portfolio &por) {
    std::vector<json> assetsJson;
    for (const auto& assetPtr : por.assetPtrs) {
        json assetJson;
        to_json(assetJson, *assetPtr);
        assetsJson.push_back(assetJson);
    }
    j["Assets"] = assetsJson;
    json valuationArray = json::array();
    for(const auto&pair:por.valuationVectorPlotting){
        std::string dateStr = pair.first.FormatISODate().ToStdString();
        double amount = pair.second;
        json pairJson = {{"Date", dateStr},{"Amount",amount}};
        valuationArray.push_back(pairJson);
    }

    j["valuationVectorPlotting"] = valuationArray;
}


void from_json(const json &j, Portfolio &por) {
    if (!j.contains("Assets") || !j["Assets"].is_array()) {
        throw std::runtime_error("JSON does not contain 'Assets' or 'Assets' is not an array.");
    }

    for(const auto &assetJson : j["Assets"]){
        auto asset = std::make_shared<Asset>();
        from_json(assetJson, *asset, por);
        por.assetPtrs.push_back(asset);
    }
    if(j.contains("valuationVectorPlotting")){
        for(const auto &pair:j["valuationVectorPlotting"]){
            std::string dateStr = pair["Date"];
            double amount = pair["Amount"];

            wxDateTime date;
            date.ParseDate(dateStr);
            por.valuationVectorPlotting.push_back(std::make_pair(date, amount));
        }
    }
}


void Portfolio::SavePortfolioToFile(const Portfolio &portfolio, const std::string &filePath){
    json j;
    to_json(j,portfolio);
    std::ofstream file(filePath,std::ios::trunc);
    file << j.dump(4);
}

void Portfolio::LoadFromFile(const std::string &filePath) {
    std::ifstream file(filePath);
    json j;
    if (!file) {
        throw std::runtime_error("Failed to open the file: " + filePath);
    }
    std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    try {
        j = json::parse(fileContent);
    } catch(const json::exception& e) {
        throw std::runtime_error("JSON parsing error: " + std::string(e.what()));
    }
    try {
        from_json(j, *this); // Populate the current object
    } catch (const std::exception& e) {
        std::cerr << "Deserialization error: " << e.what() << std::endl;
        throw;
    }
}


void Portfolio::PopulateEvents(){
    assetEventPtrs.clear();
    for(const auto& asset:assetPtrs){
        for(const auto& eventPtr : asset->events){
            assetEventPtrs.push_back(eventPtr);
        }
    }
}

double Portfolio::TotalInvestedCapital(){
    double totalInvestedCapital = 0;
    for(const auto &asset: assetPtrs){
        totalInvestedCapital+= asset->CalculateInvestedCapital();
    }
    return totalInvestedCapital;
}

double Portfolio::TotalInvestors(){
    double totalInvestors = 0;
    for(const auto &asset:assetPtrs){
        totalInvestors+=asset->CalculateNumberOfInvestors();
    }
    return totalInvestors;
}

double Portfolio::TotalValuation(){
    double totalValuation = 0;
    for(const auto&asset:assetPtrs){
        totalValuation+=asset->GetLastValuation();
    }
    return totalValuation;
}

void Portfolio::addValuation(){
    double totalValuation = 0.0;
    wxDateTime latestDate = wxDateTime(1, wxDateTime::Jan, 1900); 


    for(auto&assetPtr: assetPtrs){
        if(!assetPtr->valuations.empty()){
            Valuation& lastValuation = assetPtr->valuations.back();
            totalValuation +=lastValuation.valuation;
            if(lastValuation.valuationDate.IsLaterThan(latestDate)){
                latestDate = lastValuation.valuationDate;
            }
        }
    }

    auto it = std::find_if(valuationVectorPlotting.begin(), valuationVectorPlotting.end(),
                           [&latestDate](const std::pair<wxDateTime, double>& entry) {
                               return entry.first == latestDate;
                           });
    if (it != valuationVectorPlotting.end()) {
        it->second = totalValuation;
    } else {
        valuationVectorPlotting.push_back(std::make_pair(latestDate, totalValuation));
    }


}
