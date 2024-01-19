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
    if(j.contains("Assets") && j["Assets"].is_array()){
        for(const auto &assetJson : j["Assets"]){
            auto asset = std::make_shared<Asset>();
            if(assetJson.contains("Asset Name")){
                asset->assetName = assetJson["Asset Name"].get<std::string>().c_str();
            }
            if(assetJson.contains("Asset Exit Date")){
                wxString dateStr = wxString::FromUTF8(assetJson["Asset Exit Date"].get<std::string>().c_str());
                wxDateTime dateParse;
                dateParse.ParseDate(dateStr);
                asset->assetExitDate = dateParse;
            }
            from_json(j["Assets"], *asset, por);
            por.assetPtrs.push_back(asset);
        }
    }
    if(j.contains("Investors") && j["Investors"].is_array()){
        for(const auto& investorJson: j["Investors"]){
            std::shared_ptr<Investor> investor= std::make_shared<Investor>();
            from_json(investorJson, *investor, por);
            por.allInvestorPtrs.push_back(investor);
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
    por.SetAssetPositions();
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



wxDateTime Portfolio::GetQuarterEndDate(wxDateTime &currentDate){
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

wxDateTime Portfolio::GetQuarterStartDate(wxDateTime &date){
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

wxDateTime Portfolio::GetNextQuarterEndDate(wxDateTime &currentEndDate){
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


bool Portfolio::IsWithinQuarter(const wxDateTime&date,const wxDateTime &quarterEndDate){
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
        for(const auto&investorPtr: allInvestorPtrs){
            for(const auto&pos:investorPtr->positions){
                if(pos->assetPtr == assetPtr){
                    if(pos->dateInvested.IsEarlierThan(oldestInvestedDate)){
                        oldestInvestedDate = pos->dateInvested;
                    }
                }
            }
        }
    } 
    //Getting the first date of deployed capital
    //Get the End Date of the Q that corresponds to the oldest date
    wxDateTime qEndDate = GetQuarterEndDate(oldestInvestedDate);
    wxDateTime today = wxDateTime::Today();
    wxDateTime currentQStartDate = GetQuarterStartDate(today);
    double lastQuarterValuation = 0.0; // Initialize to zero or a starting value

    while (qEndDate.IsEarlierThan(currentQStartDate)) {
        double quarterValuation = 0.0;
        bool hasNewDataInQuarter = false;

        for (const auto& assetPointer : assetPtrs) {
            bool hasValuationInQuarter = false;
            double assetsValuation = 0.0;

            // Check for valuations in this quarter
            for (const auto& valuation : assetPointer->valuations) {
                if (IsWithinQuarter(valuation.valuationDate, qEndDate)) {
                    assetsValuation += valuation.valuation;
                    hasValuationInQuarter = true;
                }
            }

            // If no valuation, check for deployed capital in this quarter
            if (!hasValuationInQuarter) {
                double deployedCapital = 0.0;
                for (const auto& inv : allInvestorPtrs) {
                    for (const auto& pos : inv->positions) {
                        if (pos->assetPtr == assetPointer && IsWithinQuarter(pos->dateInvested, qEndDate)) {
                            deployedCapital += pos->deployed;
                        }
                    }
                }
                if (deployedCapital > 0) {
                    assetsValuation += deployedCapital;
                    hasNewDataInQuarter = true;
                }
            }

            quarterValuation += assetsValuation;
        }

        // Update the quarter valuation
        if (hasNewDataInQuarter) {
            quarterValuation += lastQuarterValuation; // Add to previous quarter's valuation
            lastQuarterValuation = quarterValuation;  // Update for next iteration
        } else {
            quarterValuation = lastQuarterValuation; // Carry forward if no new data
        }

        previousQMap[qEndDate] = quarterValuation;
        qEndDate = GetNextQuarterEndDate(qEndDate);
    }
}


void Portfolio::PopulateAndProcessCurrentQValuations() {
    std::map<wxString, double> assetLastValuationMap; // Last valuation for each asset
    double currentValuation = previousQMap.rbegin()->second; // Start with the last quarter's valuation
    wxDateTime today = wxDateTime::Today();
    wxDateTime currentQStartDate = GetQuarterStartDate(today);

    // Initialize assetLastValuationMap with the deployed capital for each asset
    for (auto& asset : assetPtrs) {
        assetLastValuationMap[asset->assetName] = asset->CalculateDeployedCapital();
    }

    // Sort all valuations within the current quarter
    std::vector<std::pair<wxDateTime, std::pair<wxString, double>>> sortedValuations;
    for (auto& asset : assetPtrs) {
        for (auto& val : asset->valuations) {
            if (IsWithinQuarter(val.valuationDate, today)) {
                sortedValuations.push_back({val.valuationDate, {asset->assetName, val.valuation}});
            }
        }
    }
    std::sort(sortedValuations.begin(), sortedValuations.end(), 
              [](const auto& a, const auto& b) { return a.first.IsEarlierThan(b.first); });

    // Process sorted valuations
    for (auto& [date, assetVal] : sortedValuations) {
        const auto& [assetName, valuationAmount] = assetVal;
        double lastValuation = assetLastValuationMap[assetName];
        double differential = valuationAmount - lastValuation;
        currentValuation += differential; // Update current total valuation
        currentQMap[date] = currentValuation; // Store in map
        assetLastValuationMap[assetName] = valuationAmount; // Update last valuation for the asset
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