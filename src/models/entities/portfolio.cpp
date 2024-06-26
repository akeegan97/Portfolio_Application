#include "models/entities/portfolio.hpp"
#include "models/entities/asset.hpp"


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
            std::shared_ptr<Investor> investor= std::make_shared<Investor>();
            from_json(investorJson, *investor, por);
            por.allInvestorPtrs.push_back(investor);
        }
    }

    if(j.contains("Assets") && j["Assets"].is_array()){
        for(const auto &assetJson : j["Assets"]){
            auto asset = std::make_shared<Asset>();
            if(assetJson.contains("Asset Name")){
                wxString assetName = assetJson["Asset Name"].get<std::string>().c_str();
                asset->DeserializeSetAssetName(assetName);
            }
            
            if(assetJson.contains("Asset Exit Date")){
                wxString dateStr = wxString::FromUTF8(assetJson["Asset Exit Date"].get<std::string>().c_str());
                wxDateTime dateParse;
                dateParse.ParseDate(dateStr);
                asset->DeserializeSetAssetExitDate(dateParse);
            }
            if(assetJson.contains("Asset Sponser")){
                wxString sponser = assetJson["Asset Sponser"].get<std::string>().c_str();
                asset->DeserializeSetAssetSponser(sponser);
            }
            from_json(assetJson, *asset, por);
            por.assetPtrs.push_back(asset);
        }
    }
    for (auto& investorJson : j["Investors"]) {
        auto investorName = investorJson["Client Name"].get<std::string>();
        auto investor = std::find_if(por.allInvestorPtrs.begin(), por.allInvestorPtrs.end(),
                                     [&investorName](const std::shared_ptr<Investor>& inv) {
                                         return inv->GetName() == investorName;
                                     });

        if (investor != por.allInvestorPtrs.end()) {
            for (const auto& positionJson : investorJson["Positions"]) {
                auto position = std::make_shared<Position>();
                from_json(positionJson, *position, por);

                position->SetInvestorPtr(*investor);

                auto assetName = positionJson["AssetName"].get<std::string>();
                auto asset = std::find_if(por.assetPtrs.begin(), por.assetPtrs.end(),
                                          [&assetName](const std::shared_ptr<Asset>& as) {
                                              return as->GetAssetName() == assetName;
                                          });

                if (asset != por.assetPtrs.end()) {
                    position->SetAssetPtr(*asset);
                    (*investor)->AddPosition(position);
                    (*asset)->AddPosition(position);
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
    for(auto& asset: assetPtrs){
        asset->SetPositionValues();
        asset->SetCurrentValue();
    }
}


double Portfolio::TotalInvestedCapital(){
    double totalInvestedCapital = 0;
    for(const auto &asset: assetPtrs){
        totalInvestedCapital+= asset->GetTotalAssetDeployed();
    }
    return totalInvestedCapital;
}

double Portfolio::TotalInvestors(){
    double totalInvestors = 0;
    for(const auto &asset:assetPtrs){
        totalInvestors+=asset->GetTotalInvestors();
    }
    return totalInvestors;
}

double Portfolio::TotalValuation(){
    double totalValuation = 0;
    for(const auto&asset:assetPtrs){
        totalValuation+=asset->GetLastValuationAmountOrCommittedCapital();
    }
    return totalValuation;
}

void Portfolio::ValuationDialog() {
    double totalValuation = 0.0;
    wxDateTime latestDate = wxDateTime(1, wxDateTime::Jan, 1900);

    for (auto& assetPtr : assetPtrs) {
        double assetValuation = 0.0;
        if (!assetPtr->GetValuations().empty()) {
            std::vector<Valuation> valuationCopy = assetPtr->GetValuations();
            std::sort(valuationCopy.begin(),valuationCopy.end(),
                        [](const Valuation &a, const Valuation &b){
                            return a.valuationDate.IsEarlierThan(b.valuationDate);
                        });
            Valuation& lastValuation = valuationCopy.back();
            assetValuation = lastValuation.valuation;
            if (lastValuation.valuationDate.IsLaterThan(latestDate)) {
                latestDate = lastValuation.valuationDate;
            }
        } else {
            // If no valuation, use deployed capital
            assetValuation = assetPtr->GetLastValuationAmountOrCommittedCapital();
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
    if (asset->GetValuations().empty()) {
        return asset->GetTotalAssetDeployed();
    }

    // Find the last valuation before the given date
    double lastValuationAmount = asset->GetTotalAssetDeployed();
    for (const auto& valuation : asset->GetValuations()) {
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
        for(const auto&pos:assetPtr->GetPositions()){
            if(pos->GetDateInvested().IsEarlierThan(oldestInvestedDate)){
                oldestInvestedDate = pos->GetDateInvested();
            }
        }
    }
    wxDateTime qEndDate = utilities::GetQuarterEndDate(oldestInvestedDate);
    wxDateTime currentQDate = wxDateTime::Today();
    wxDateTime currentQStartDate = utilities::GetQuarterStartDate(currentQDate);

    while (qEndDate.IsEarlierThan(currentQStartDate)) {
        double quarterValuation = 0.0;

        for (const auto& assetPtr : assetPtrs) {
            double assetQuarterValuation = 0.0;
            bool valuationFound = false;

            for (const auto& val : assetPtr->GetValuations()) {
                if ((val.valuationDate.IsEarlierThan(qEndDate))) {
                    assetQuarterValuation = val.valuation;
                    valuationFound = true;
                } else if (val.valuationDate < qEndDate) {
                    assetQuarterValuation = val.valuation;
                }
            }

            if (!valuationFound) {
                for(auto& movement : assetPtr->GetMovementsToFromDeploy()){
                    if(movement.first.IsEarlierThan(qEndDate) || movement.first == qEndDate){
                        assetQuarterValuation+=movement.second;
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
    if(previousQMap.empty()){
        return;
    }
    double currentValuation = previousQMap.rbegin()->second; 
    wxDateTime today = wxDateTime::Today();
    std::map<wxString, double> assetLastValuationMap; 
    for (auto& asset : assetPtrs) {
        auto lastValuationIt = asset->GetValuations().rbegin();
        if(lastValuationIt != asset->GetValuations().rend() /*&& utilities::IsWithinQuarter(lastValuationIt->valuationDate, today)*/) {
            assetLastValuationMap[asset->GetAssetName()] = lastValuationIt->valuation;
        } else {
            assetLastValuationMap[asset->GetAssetName()] = asset->GetTotalAssetDeployed();
        }
    }
    std::vector<std::pair<wxDateTime, std::pair<wxString, double>>> currentQValuations;
    for (auto& asset : assetPtrs) {
        for (auto& val : asset->GetValuations()) {
            if (utilities::IsWithinQuarter(val.valuationDate, today)) {
                currentQValuations.push_back({val.valuationDate, {asset->GetAssetName(), val.valuation}});
            }
        }
    }
    std::sort(currentQValuations.begin(), currentQValuations.end(), [](const auto& a, const auto& b) {
        return a.first.IsEarlierThan(b.first);
    });
    double accumulatedValuation = std::accumulate(
        assetLastValuationMap.begin(), assetLastValuationMap.end(), 0.0,
        [](double sum, const std::pair<wxString, double>& assetVal) {
            return sum + assetVal.second;
        }
    );
    wxDateTime currentQEndDate = utilities::GetQuarterEndDate(today);
    currentValuation = accumulatedValuation;
    currentQMap[currentQEndDate] = currentValuation;
}


std::vector<std::shared_ptr<Investor>>& Portfolio::GetInvestors(){
    return allInvestorPtrs;
}

void Portfolio::AddInvestor(std::shared_ptr<Investor> &investor){
    allInvestorPtrs.push_back(investor);
}

std::shared_ptr<Investor> Portfolio::GetInvestorByName(const std::string &investorName) const{
    for (const auto& investorPtr : allInvestorPtrs) { 
        if (investorPtr->GetName() == investorName) {
            return investorPtr;
        }
    }
    return nullptr; 
}

void Portfolio::AddAsset(std::shared_ptr<Asset> &newAsset){
    assetPtrs.push_back(newAsset);
}

void Portfolio::EnsureFundPositionExists(){
    auto fundPositionIt = std::find_if(allInvestorPtrs.begin(),allInvestorPtrs.end(),
                                        [](const std::shared_ptr<Investor> &investor){
                                            return investor->GetName() == "Fund Position";
                                        });
    if(fundPositionIt == allInvestorPtrs.end()){
        auto fundPosition = std::make_shared<Investor>("Fund Position","Fund",0.0,0.0);
        allInvestorPtrs.push_back(fundPosition);
    }
}

double Portfolio::CalculateFundIrr(){
    std::vector<CashFlow> allAssetCashFlows;
    for(auto asset:assetPtrs){
        std::vector<CashFlow> tempCashFlow;
        tempCashFlow = asset->GetAssetCashFlow();
        std::move(std::begin(tempCashFlow),std::end(tempCashFlow),std::back_inserter(allAssetCashFlows));
    }
    std::sort(allAssetCashFlows.begin(),allAssetCashFlows.end(),
            [](const CashFlow &a, const CashFlow &b){
                return a.date < b.date;
            });
    double m_irr = 0; 
    double zero = 0.0;
    double npvAtZero = CalculateNPV(allAssetCashFlows, zero); 
    double direction = npvAtZero > 0 ? 1.0 : -1.0; 

    int maxIterationsPerGuess = 100; 
    double precision = 0.000001; 
    bool foundIRR = false; 


    for (double initialGuess = 0.1 * direction; std::fabs(initialGuess) <= 1.0; initialGuess += 0.1 * direction) {
        double x1 = initialGuess; 

        for (int i = 0; i < maxIterationsPerGuess; ++i) {
            double npv = CalculateNPV(allAssetCashFlows, x1); 
            double x1AddPrecision = x1+precision;
            double npvPrime = (CalculateNPV(allAssetCashFlows, x1AddPrecision) - npv) / precision; 

            if (std::fabs(npvPrime) < 1e-6) {
                break; 
            }
            double xNext = x1 - npv / npvPrime; 
            if (std::fabs(xNext - x1) <= precision) {
                m_irr = xNext; 
                foundIRR = true; 
                std::cout << "IRR found: " << m_irr << std::endl;
                break; 
            }
            x1 = xNext; 
        }

        if (foundIRR) {
            return m_irr;
            break; 
        }else{
            return 0.0;
        }
    }
}

double Portfolio::CalculateNPV(std::vector<CashFlow> &cashFlows, double &rate){
    double npv = 0.0;
    if(cashFlows.empty()){
        return npv;
    }
    wxDateTime firstDate = cashFlows[0].date;
    for(const auto& cf : cashFlows){
        wxTimeSpan timeSpan = cf.date - firstDate;
        double years = timeSpan.GetDays() / 365.25;
        npv += cf.amount / std::pow(1+rate, years);
    }
    return npv;
}