#include "baseclasses/portfolio.hpp"
#include "baseclasses/asset.hpp"


void to_json(json &j, const Portfolio &por) {
    std::vector<json> assetsJson;
    for (const auto& assetPtr : por.assetPtrs) {
        json assetJson;
        to_json(assetJson, *assetPtr);
        assetsJson.push_back(assetJson);
    }
    j["Assets"] = assetsJson;

    std::vector<json> investorsJson; 
    for(const auto& investorPtr : por.allInvestorPtrs) {
        json singleInvestorJson;
        to_json(singleInvestorJson, *investorPtr);
        investorsJson.push_back(singleInvestorJson); 
    }
    j["Investors"] = investorsJson; 

    json valuationArray = json::array();
    for(const auto& pair : por.valuationVectorPlotting) {
        std::string dateStr = pair.first.FormatISODate().ToStdString();
        double amount = pair.second;
        json pairJson = {{"Date", dateStr}, {"Amount", amount}};
        valuationArray.push_back(pairJson);
    }

    j["valuationVectorPlotting"] = valuationArray;
}


void from_json(const json &j, Portfolio &por) {
    if(j.contains("Investors") && j["Investors"].is_array()){
        for(const auto& investorJson: j["Investors"]){
            std::shared_ptr<Investor2> investor= std::make_shared<Investor2>();
            from_json(investorJson, *investor, por);
            por.allInvestorPtrs.push_back(investor);
        }
    }

    if(j.contains("Assets") && j["Assets"].is_array()){
        for(const auto &assetJson : j["Assets"]){
            auto asset = std::make_shared<Asset2>();
            if(assetJson.contains("Asset Name")){
                wxString assetName = assetJson["Asset Name"].get<std::string>().c_str();
                asset->DeserializeSetAssetName(assetName);
            }
            
            if(assetJson.contains("Asset Exit Date")){
                wxString dateStr = wxString::FromUTF8(assetJson["Asset Exit Date"].get<std::string>().c_str());
                wxDateTime dateParse;
                dateParse.ParseDate(dateStr);
                asset->assetExitDate = dateParse;
            }
            from_json(assetJson, *asset, por);
            por.assetPtrs.push_back(asset);
        }
    }
    for (auto& investorJson : j["Investors"]) {
        auto investorName = investorJson["Client Name"].get<std::string>();
        auto investor = std::find_if(por.allInvestorPtrs.begin(), por.allInvestorPtrs.end(),
                                     [&investorName](const std::shared_ptr<Investor>& inv) {
                                         return inv->clientName == investorName;
                                     });

        if (investor != por.allInvestorPtrs.end()) {
            for (const auto& positionJson : investorJson["Positions"]) {
                auto position = std::make_shared<Position>();
                from_json(positionJson, *position, por);

                position->investorPtr = *investor;

                auto assetName = positionJson["AssetName"].get<std::string>();
                auto asset = std::find_if(por.assetPtrs.begin(), por.assetPtrs.end(),
                                          [&assetName](const std::shared_ptr<Asset>& as) {
                                              return as->assetName == assetName;
                                          });

                if (asset != por.assetPtrs.end()) {
                    position->assetPtr = *asset;
                    (*investor)->m_positions.push_back(position);
                    (*asset)->m_positions.push_back(position);
                }
            }
        }
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
    //por.SetAssetPositions();
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
}//call this any time there is an edit/deletion/addition of an event 


double Portfolio::TotalInvestedCapital(){
    double totalInvestedCapital = 0;
    for(const auto &asset: assetPtrs){
        totalInvestedCapital+= asset->CalculateDeployedCapital();
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

void Portfolio::ValuationDialog() {
    double totalValuation = 0.0;
    wxDateTime latestDate = wxDateTime(1, wxDateTime::Jan, 1900);

    for (auto& assetPtr : assetPtrs) {
        double assetValuation = 0.0;
        if (!assetPtr->valuations.empty()) {
            Valuation& lastValuation = assetPtr->valuations.back();
            assetValuation = lastValuation.valuation;
            if (lastValuation.valuationDate.IsLaterThan(latestDate)) {
                latestDate = lastValuation.valuationDate;
            }
        } else {
            // If no valuation, use deployed capital
            assetValuation = assetPtr->CalculateDeployedCapital();
        }
        totalValuation += assetValuation;
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
//call this anytime there is an edit/addition/deletion of a valuation for any asset
 
void Portfolio::PopulateValuationMaps(){
    currentQMap.clear();
    previousQMap.clear();
    valuationVectorPlotting.clear();
    PopulatePreviousQValuations();
    PopulateAndProcessCurrentQValuations();

    // Insert all entries from previousQMap
    for (const auto& entry : previousQMap) {
        valuationVectorPlotting.push_back(entry);
    }

    // Insert or update entries from currentQMap
    for (const auto& entry : currentQMap) {
        auto it = std::find_if(valuationVectorPlotting.begin(), valuationVectorPlotting.end(),
                               [&entry](const std::pair<wxDateTime, double>& existingEntry) {
                                   return existingEntry.first == entry.first;
                               });
        if (it != valuationVectorPlotting.end()) {
            // Update existing entry
            it->second = entry.second;
        } else {
            // Insert new entry
            valuationVectorPlotting.push_back(entry);
        }
    }

    // Sort valuationVectorPlotting by date
    std::sort(valuationVectorPlotting.begin(), valuationVectorPlotting.end(),
              [](const std::pair<wxDateTime, double>& a, const std::pair<wxDateTime, double>& b) {
                  return a.first.IsEarlierThan(b.first);
              });
}

double Portfolio::GetLastValuationOrDeployedCapital(std::shared_ptr<Asset>& asset, const wxDateTime& date) {
    if (asset->valuations.empty()) {
        return asset->CalculateDeployedCapital();
    }
    // Sort valuations by date
    std::sort(asset->valuations.begin(), asset->valuations.end(), 
              [](const Valuation& a, const Valuation& b) {
                  return a.valuationDate.IsEarlierThan(b.valuationDate);
              });

    // Find the last valuation before the given date
    double lastValuationAmount = asset->CalculateDeployedCapital();
    for (const auto& valuation : asset->valuations) {
        if (valuation.valuationDate.IsSameDate(date) || valuation.valuationDate.IsLaterThan(date)) {
            break;
        }
        lastValuationAmount = valuation.valuation;
    }
    return 0.0;
}

void Portfolio::PopulatePreviousQValuations() {
    previousQMap.clear();
    wxDateTime oldestInvestedDate = wxDateTime::Today();
    for(const auto&assetPtr: assetPtrs){
        for(const auto&pos:assetPtr->positions){
            if(pos->dateInvested.IsEarlierThan(oldestInvestedDate)){
                oldestInvestedDate = pos->dateInvested;
            }
        }
    }
    wxDateTime qEndDate = utilities::GetQuarterEndDate(oldestInvestedDate);
    wxDateTime currentQDate = wxDateTime::Today();
    wxDateTime currentQStartDate = utilities::GetQuarterStartDate(currentQDate);

    for (const auto& assetPtr : assetPtrs) {
        std::sort(assetPtr->valuations.begin(), assetPtr->valuations.end(),
            [](const Valuation& a, const Valuation& b) {
                return a.valuationDate < b.valuationDate;
            });
    }

    while (qEndDate.IsEarlierThan(currentQStartDate)) {
        double quarterValuation = 0.0;

        for (const auto& assetPtr : assetPtrs) {
            double assetQuarterValuation = 0.0;
            bool valuationFound = false;

            for (const auto& val : assetPtr->valuations) {
                if ((val.valuationDate.IsEarlierThan(qEndDate))) {
                    assetQuarterValuation = val.valuation;
                    valuationFound = true;
                } else if (val.valuationDate < qEndDate) {
                    assetQuarterValuation = val.valuation;
                }
            }

            if (!valuationFound) {
                for (const auto& pos : assetPtr->positions) {
                    if ((pos->dateInvested.IsEarlierThan(qEndDate))) {
                        assetQuarterValuation += pos->paid; 
                    }
                }
            }

            quarterValuation += assetQuarterValuation;
        }

        previousQMap[qEndDate] = quarterValuation;
        qEndDate = utilities::GetNextQuarterEndDate(qEndDate);
    }
}

void Portfolio::PopulateAndProcessCurrentQValuations() {
    std::map<wxString, double> assetLastValuationMap; 
    double currentValuation = previousQMap.rbegin()->second; 
    wxDateTime today = wxDateTime::Today();
    wxDateTime currentQStartDate = utilities::GetQuarterStartDate(today);

    for (auto& asset : assetPtrs) {
        assetLastValuationMap[asset->assetName] = asset->CalculateDeployedCapital();
    }

    std::vector<std::pair<wxDateTime, std::pair<wxString, double>>> sortedValuations;
    for (auto& asset : assetPtrs) {
        for (auto& val : asset->valuations) {
            if (utilities::IsWithinQuarter(val.valuationDate, today)) {
                sortedValuations.push_back({val.valuationDate, {asset->assetName, val.valuation}});
            }
        }
    }
    std::sort(sortedValuations.begin(), sortedValuations.end(), 
              [](const auto& a, const auto& b) { return a.first.IsEarlierThan(b.first); });

    for (auto& [date, assetVal] : sortedValuations) {
        const auto& [assetName, valuationAmount] = assetVal;
        double lastValuation = assetLastValuationMap[assetName];
        double differential = valuationAmount - lastValuation;
        currentValuation += differential; 
        currentQMap[date] = currentValuation; 
        assetLastValuationMap[assetName] = valuationAmount; 
    }
}

void Portfolio::SetAssetPositions(){
    for(auto& asset:assetPtrs){
        for(auto& investor: allInvestorPtrs){
            for(auto&position: investor->positions){
                if(position->assetPtr->assetName == asset->assetName){
                    asset->positions.push_back(position);
                }
            }
        }
    }
}

