#include "models/entities/asset.hpp"
#include "helpers/utilities.hpp"

void Asset::SortDistributions(std::vector<Distribution> &distributions){
    std::sort(distributions.begin(), distributions.end(),
    [](const Distribution &a, const Distribution &b) {
        return a.distribution.first < b.distribution.first;
    });  
}
void Asset::SortDistributions2(){
    std::sort(m_distributions.begin(), m_distributions.end(),
    [](const Distribution &a, const Distribution &b) {
        return a.distribution.first < b.distribution.first;
    });  
}

void Asset::SetPositionID(){
    size_t id = 0;
    for(auto position:m_positions){
        position->SetId(id);
        id++;
    }
}

std::vector<std::pair<Distribution, bool>> Asset::GetQuarterDistributions()const{
    return m_qDistributions;
}   


void Asset::ProcessDistributionsForPosition(){
    SortDistributions(m_distributions);
    for(auto &pos:m_positions){
        pos->ClearNetIncomePromoteFees();
    }
    for(const auto& distribution : m_distributions){
        for(auto &position: m_positions){
            if(position->GetDateInvested()< distribution.distribution.first){
                double ownershipForThisDistribution = position->CalculateOwnershipAtDate(distribution.distribution.first);
                double feesForThisDistribution = position->CalculateManagementFeesDue(distribution);
                double proportionalShareGross = distribution.distribution.second * ownershipForThisDistribution;
                double remainder = proportionalShareGross - feesForThisDistribution;
                double promoteFeePercentage = position->GetInvestorPromoteFee();
                PromoteFee newPromoteFee;
                newPromoteFee.promotefee.first = distribution.distribution.first;
                newPromoteFee.promotefee.second = remainder * promoteFeePercentage;
                Distribution newDistribution;
                newDistribution.distribution.first = distribution.distribution.first;
                newDistribution.distribution.second = remainder - newPromoteFee.promotefee.second;
                position->UpdateFinancesPostDistributionChanges(newDistribution, newPromoteFee);
            }
        }
    }
}

void Asset::TriggerUpdateOfDistributionsForPositions(){
    ProcessDistributionsForPosition();
}

const std::vector<Valuation>& Asset::GetValuations()const{
    return m_valuations;
}

void Asset::AddNewValuation(Valuation &newValuation){
    m_valuations.push_back(newValuation);
    UpdatePositionValuations();
}

void Asset::UpdatePositionValuations(){
    for(auto& position: m_positions){
        position->RepopulateValuations();
    }
}

void Asset::SetCurrentValue(){
    if(m_valuations.empty()){
        m_currentValue = m_assetDeployedCapital;
    }else{
        SortValuations2();
        m_currentValue = m_valuations.back().valuation;
    }
}

double Asset::GetCurrentValue()const{
    return m_currentValue;
}

double Asset::GetValuationInQuarter(wxDateTime &date)const{
    auto quarterEnd = utilities::GetQuarterEndDate(date);
    double valuationAmount = 0;
    for(const auto&valuation: m_valuations){
        if(valuation.valuationDate >= date && valuation.valuationDate <= quarterEnd){
            valuationAmount = valuation.valuation;
        }
    }
    return valuationAmount;
}

void Asset::UpdateTotalMgmtFeesEarned(){
    double totalMgmtFeesEarned = 0.0;
    for(const auto& position:m_positions){
        for(const auto&mgmtfee: position->GetManagementFees()){
            if(mgmtfee.paid == TRUE){
                m_totalMgmtFeesEarned += mgmtfee.amount;
            }
        }
    }
}

void Asset::UpdateTotalPromoteFeesEarned(){
    double totalPromoteFeesEarned = 0.0;
    for(const auto&pos:m_positions){
        for(const auto&pf : pos->GetPromoteFees()){
            totalPromoteFeesEarned+=pf.promotefee.second;
        }
    }
    m_totalPromoteFeesEarned = totalPromoteFeesEarned;
}

void Asset::UpdateTotalCountOfInvestors(){
    std::unordered_set<std::string> uniqueInvestors;
    for(const auto&pos:m_positions){
        if(pos->GetInvestorPtr()){
            std::shared_ptr investorPtr =  pos->GetInvestorPtr();
            std::string investorName = investorPtr->GetName();
            uniqueInvestors.insert(investorName);
        }
    }
    m_countOfInvestors = uniqueInvestors.size();
}

void Asset::UpdateTotalInvestedCapital(){
    double totalInvested = 0.0;
    for(const auto&pos:m_positions){
        totalInvested += pos->GetCommitted();
    }
    m_totalInvestedCapital = totalInvested;
}

wxString Asset::GetAssetName()const{
    return m_assetName;
}
wxString Asset::GetAssetSponserName()const{
    return m_assetSponserName;
}
wxDateTime Asset::GetAssetExitDate()const{
    return m_assetExitDate;
}
double Asset::GetTotalInvestedCapital()const{
    return m_totalInvestedCapital;
}
double Asset::GetTotalAssetDeployed()const{
    return m_assetDeployedCapital;
}
double Asset::GetTotalAssetReserve()const{
    return m_assetReserveCapital;
}
double Asset::GetTotalReturnOfCapital()const{
    double returnedCapital = 0.0;
    for(const auto&position:m_positions){
        // for(const auto& rocMovement: position->GetReturnOfCapitalMap()){
        //     returnedCapital+=rocMovement.second;
        // }
        returnedCapital+=position->GetReturnOfCapital();
    }
    return m_assetReturnOfCapital;
}
std::shared_ptr<Position> Asset::GetPositionByID(size_t id){
    for(auto position: m_positions){
        if(position->GetId()==id){
            return position;
        }
    }
    return nullptr;
}
const std::vector<std::shared_ptr<Position>>& Asset::GetPositions()const{
    return m_positions;
}
std::vector<wxString> Asset::columnNames = {"Sponser","Name","Exit Date","Total Deployed","# Investors","Current Value"};
std::vector<int> Asset::columnWidths = {100,125,100,150,125,140};

wxVariant Asset::GetValue(int col)const{
    switch(col){
        case 0: return wxVariant(m_assetSponserName);
        case 1: return wxVariant(m_assetName);
        case 2: return wxVariant(m_assetExitDate.FormatISODate());
        case 3: return wxVariant(utilities::formatDollarAmount(m_assetDeployedCapital));
        case 4: return wxVariant(m_countOfInvestors);
        case 5: return wxVariant(utilities::formatDollarAmount(m_currentValue));
        default: return wxVariant();
    }
}

std::vector<Valuation>& Asset::SortValuations(std::vector<Valuation> &valuations){
    std::sort(valuations.begin(),valuations.end(),
        [](const Valuation &a, const Valuation &b){
            return a.valuationDate < b.valuationDate;
        });
    return valuations;
}

void Asset::SortValuations2(){
    std::sort(m_valuations.begin(),m_valuations.end(),
                [](const Valuation &a, const Valuation &b){
                    return a.valuationDate < b.valuationDate;
                });
}

void Asset::UpdateCurrentvalue(){
    if(!m_valuations.empty()){
        SortValuations(m_valuations);
        m_currentValue = m_valuations.back().valuation;
    }else{
        m_currentValue = m_assetDeployedCapital;
    }
    
}

void Asset::DeserializeSetAssetName(wxString &assetName){
    m_assetName = assetName;
}
void Asset::DeserializeSetAssetSponser(wxString &assetSponserName){
    m_assetSponserName = assetSponserName;
}
void Asset::DeserializeSetAssetExitDate(wxDateTime &exitDate){
    m_assetExitDate = exitDate;
}
void Asset::DeserializeSetAssetCommittedCapital(double &committedCapital){
    m_assetCommittedCapital = committedCapital;
}
void Asset::DeserializeSetAssetDeployedCapital(double &deployedCapital){
    m_assetDeployedCapital = deployedCapital;
}
void Asset::DeserializeSetAssetReserveCapital(double &reserveCapital){
    m_assetReserveCapital = reserveCapital;
}
void Asset::DeserializeSetAssetReturnOfCapital(double &returnOfCapital){
    m_assetReturnOfCapital = returnOfCapital;
}
void Asset::DeserializeSetValuations(std::vector<Valuation> &valuations){
    m_valuations.clear();
    m_valuations = valuations;
}
void Asset::DeserializeSetDistributions(std::vector<Distribution> &distributions){
    m_distributions.clear();
    m_distributions = distributions;
}
void Asset::AddPosition(std::shared_ptr<Position> &position){
    m_positions.push_back(position);
}

void Asset::DeserializeSetRocMovements(std::map<std::string, double> &movements){
    for(const auto&[dateStr, value]: movements){
        wxDateTime date;
        date.ParseISODate(dateStr);
        m_rocMovements[date] = value;
    }
}

void Asset::DeserializeSetMovementsDeploy(std::map<std::string, double> &movements){
    for(const auto&[dateStr, value]:movements){
        wxDateTime date;
        date.ParseISODate(dateStr);
        m_movementsToFromDeploy[date] = value;
    }
}

double Asset::GetTotalInvestors()const{
    return m_countOfInvestors;
}

double Asset::GetLastValuationAmountOrCommittedCapital()const{
    if(m_valuations.empty()){
        double committedCapital = 0.0;
        for(const auto&pos:m_positions){
            committedCapital += pos->GetCommitted();
        }
        return committedCapital;
    }else{
        double lastValuation = 0.0;
        return lastValuation = m_valuations.back().valuation;
    }
}

void from_json(const json&j, Asset &asset, Portfolio &port){
    if(j.contains("Valuations") && j["Valuations"].is_array()){
        std::vector<Valuation> valuations = j["Valuations"].get<std::vector<Valuation>>();
        asset.DeserializeSetValuations(valuations);
    }
    if(j.contains("Distributions") && j["Distributions"].is_array()){
        std::vector<Distribution> distributions = j["Distributions"].get<std::vector<Distribution>>();
        asset.DeserializeSetDistributions(distributions);
    }
    if(j.contains("Asset Committed Capital")){
        double committed = j["Asset Committed Capital"].get<double>();
        asset.DeserializeSetAssetCommittedCapital(committed);
    }
    if(j.contains("Asset Deployed Capital")){
        double deployed = j["Asset Deployed Capital"].get<double>();
        asset.DeserializeSetAssetDeployedCapital(deployed);
    }
    if(j.contains("Asset Reserve Capital")){
        double reserve = j["Asset Reserve Capital"].get<double>();
        asset.DeserializeSetAssetReserveCapital(reserve);
    }
    if(j.contains("Asset ROC")){
        double returnOfCapital = j["Asset ROC"].get<double>();
        asset.DeserializeSetAssetReturnOfCapital(returnOfCapital);
    }
    if(j.contains("Asset ROC Movements")) {
        const auto& rocMovementsJson = j["Asset ROC Movements"];
        std::map<std::string, double> movements;
        for (auto it = rocMovementsJson.begin(); it != rocMovementsJson.end(); ++it) {
            movements[it.key()] = it.value();
        }
        asset.DeserializeSetRocMovements(movements);
    }
    if(j.contains("Asset Movement To From Deploy") && j["Asset Movement To From Deploy"].is_array()){
        std::map<std::string, double> movements;
        for(const auto& item: j["Asset Movement To From Deploy"]){
            std::string dateStr = item["Date"].get<std::string>();
            double amount = item["Amount"].get<double>();
            movements[dateStr] = amount;
        }
        asset.DeserializeSetMovementsDeploy(movements);
    }
}

void to_json(json &j, const Asset &asset){
    j = {
        {"Asset Name", asset.GetAssetName().ToStdString()},
        {"Asset Sponser", asset.GetAssetSponserName().ToStdString()},
        {"Asset Exit Date", asset.GetAssetExitDate().FormatISODate().ToStdString()},
        {"Asset Committed Capital", asset.GetTotalCommitted()},
        {"Asset Deployed Capital", asset.GetTotalAssetDeployed()},
        {"Asset Reserve Capital", asset.GetTotalAssetReserve()},
        {"Current Value", asset.GetCurrentValue()},
        {"Valuations", json::array()},
        {"Distributions", json::array()},
        {"Asset ROC Movements", json::array()},
        {"Asset Movement To From Deploy", json::array()}
    };
    json rocMovementsJson;
    for(const auto& movement : asset.GetROCMovements()){
        std::string dateStr = movement.first.FormatISODate().ToStdString();
        double amount = movement.second;
        rocMovementsJson.push_back({{"Date", dateStr}, {"Amount", amount}});
    }
    j["Asset ROC Movements"] = rocMovementsJson;
    json distributionJson = json::array();
    for(const auto& dist:asset.GetDistributionsConst()){
        json distJson;
        to_json(distJson, dist);
        distributionJson.push_back(distJson);
    }
    json valuationsJson = json::array();
    for(const auto& valuation: asset.GetValuations()){
        json jsonValuation;
        to_json(jsonValuation, valuation);
        valuationsJson.push_back(jsonValuation);
    }
    j["Valuations"] = valuationsJson;
    j["Distributions"] = distributionJson;
    json movementsToFromDeployJson;
    for(const auto& movement: asset.GetMovementsToFromDeploy()){
        std::string dateStr = movement.first.FormatISODate().ToStdString();
        double amount = movement.second;
        movementsToFromDeployJson.push_back({{"Date", dateStr},{"Amount", amount}});
    }
    j["Asset Movement To From Deploy"] = movementsToFromDeployJson;
}


std::vector<std::shared_ptr<InvestorPositionDisplay>> Asset::GetIPDVector(){
    return m_investorPositionDisplays;
}

void Asset::AddInvestorPositionDisplay(std::shared_ptr<InvestorPositionDisplay> &idp){
    m_investorPositionDisplays.push_back(idp);
}

const std::vector<std::pair<wxDateTime, double>>& Asset::GetValuationsForPlotting(){
    return m_valuationsForPlotting;
}
const std::vector<std::pair<wxDateTime, double>>& Asset::GetDeploymentsForPlotting(){
    return m_deploymentsForPlotting;
}
void Asset::UpdateValuationsForPlotting(std::vector<std::pair<wxDateTime, double>> &&newValuations){
    m_valuationsForPlotting = std::move(newValuations);
}
void Asset::UpdateDeploymentsForPlotting(std::vector<std::pair<wxDateTime, double>> &&newDeployments){
    m_deploymentsForPlotting = std::move(newDeployments);
}
const std::vector<Distribution> & Asset::GetDistributions(){
    return m_distributions;
}
const std::vector<std::pair<wxDateTime, double>>& Asset::GetDistributionsForPlotting(){
    return m_distributionsForBarChart;
}
double Asset::GetTotalPromoteFeesEarned()const{
    double promoteFees = 0.0;
    for(const auto&position:m_positions){
        for(const auto&pf:position->GetPromoteFees()){
            promoteFees+=pf.promotefee.second;
        }
    }
    return promoteFees;
}
double Asset::GetTotalMgmtFeesDue()const{
    double totalMgmtFeesDue = 0.0;
    for(const auto&position:m_positions){
        totalMgmtFeesDue+=position->GetManagementFeesDue();
    }
    return totalMgmtFeesDue;
}
double Asset::GetTotalMgmtFeesEarned()const{
    double mgmtFeesEarned = 0.0;
    for(auto pos : GetPositions()){
        for(auto mf : pos->GetManagementFees()){
            if(mf.paid == TRUE){
                mgmtFeesEarned+=mf.amount;
            }
        }
    }
    return mgmtFeesEarned;
}

double Asset::GetIrr()const{
    return m_irr;
}

void Asset::PopulatePreviousQValuations(){
    m_previousQValuationMap.clear();
    wxDateTime oldestInvestedDate = wxDateTime::Today();

    for(const auto& position: m_positions){
        if(position->GetDateInvested().IsEarlierThan(oldestInvestedDate)){
            oldestInvestedDate = position->GetDateInvested();
        }
    }
    wxDateTime qEndDate = utilities::GetQuarterEndDate(oldestInvestedDate);
    wxDateTime currentQDate = wxDateTime::Today();
    wxDateTime currentQStartDate = utilities::GetQuarterStartDate(currentQDate);
    if(!m_valuations.empty()){
        std::vector<Valuation> valuations = GetValuations();
        while(qEndDate.IsEarlierThan(currentQStartDate)){
            double qValuation = 0.0;
            for(const auto&valuation:valuations){
                if(valuation.valuationDate.IsEarlierThan(qEndDate)|| valuation.valuationDate == qEndDate){
                    qValuation = valuation.valuation;
                }
            }
            m_previousQValuationMap[qEndDate] = qValuation;
            qEndDate = utilities::GetNextQuarterEndDate(qEndDate);
        }
    }
}

void Asset::PopulateCurrentQValuations(){
    m_currentQValuationMap.clear();
    double currentValuation = m_previousQValuationMap.rbegin()->second;
    wxDateTime today = wxDateTime::Today();
    wxDateTime currentQStartDate = utilities::GetQuarterStartDate(today);
    std::vector<std::pair<wxDateTime, double>> valuationsInCurrentQ;

    double lastValuation = currentValuation;
    for(auto &val:m_valuations){
        if(val.valuationDate>=currentQStartDate){
            double differential = val.valuation-lastValuation;
            currentValuation +=differential;
            m_currentQValuationMap[val.valuationDate] = currentValuation;
            lastValuation = val.valuation;
        }
    }
}

void Asset::PopulatePreviousQDeploys() {
    m_previousQDeploymentMap.clear();
    if (m_movementsToFromDeploy.empty()) {
        return; 
    }
    double deployedCapital = 0;
    wxDateTime oldestMovement = m_movementsToFromDeploy.begin()->first;
    wxDateTime qEndingDate = utilities::GetQuarterEndDate(oldestMovement);
    wxDateTime currentQDate = wxDateTime::Today();
    wxDateTime currentQstartDate = utilities::GetQuarterStartDate(currentQDate);

    while(qEndingDate <= currentQstartDate){
        for(const auto& movement:m_movementsToFromDeploy){
            if(utilities::IsWithinQuarter(movement.first, qEndingDate)){
                deployedCapital += movement.second;
            }
        }
        m_previousQDeploymentMap[qEndingDate] = deployedCapital;
        qEndingDate = utilities::GetNextQuarterEndDate(qEndingDate);
    }
}
void Asset::PopulateCurrentQDeploys(){
    m_currentQDeploymentMap.clear();
    if(m_previousQDeploymentMap.empty()){
        wxDateTime today = wxDateTime::Today();
        m_currentQDeploymentMap[today] = m_assetCommittedCapital;
    }
    double currentDeploy = m_previousQDeploymentMap.rbegin()->second;
    wxDateTime today = wxDateTime::Today();
    wxDateTime currentQStartDate = utilities::GetQuarterStartDate(today);
    wxDateTime currentQEndDate = utilities::GetQuarterEndDate(today);

    for(const auto& movement: m_movementsToFromDeploy){
        if(movement.first>=currentQStartDate){
            currentDeploy+=movement.second;
            m_currentQDeploymentMap[movement.first] = currentDeploy;
        }
    }
    m_currentQDeploymentMap[currentQStartDate] = currentDeploy;
}

void Asset::PopulateValuationsDeploymentsForPlotting(){

    m_valuationsForPlotting.clear();
    m_deploymentsForPlotting.clear();
    PopulatePreviousQValuations();
    if(!m_previousQValuationMap.empty()){
        PopulateCurrentQValuations();
    }
    PopulatePreviousQDeploys();
    PopulateCurrentQDeploys(); 
    
    for(const auto&entry:m_previousQValuationMap){
        m_valuationsForPlotting.push_back(entry);
    }
    for(const auto&entry:m_currentQValuationMap){
        m_valuationsForPlotting.push_back(entry);
    }
    for(const auto&entry:m_previousQDeploymentMap){
        m_deploymentsForPlotting.push_back(entry);
    }
    for(const auto&entry:m_currentQDeploymentMap){
        m_deploymentsForPlotting.push_back(entry);
    }
}

void Asset::PopulateDistributionForPlotting(){
    m_distributionsForBarChart.clear();
    if(m_distributions.empty()){
        return;
    }
    SortDistributions(m_distributions);
    wxDateTime oldestDistribution = m_distributions.front().distribution.first;
    wxDateTime newestDistribution = m_distributions.back().distribution.first;

    wxDateTime qEndDate = utilities::GetQuarterEndDate(oldestDistribution);
    wxDateTime lastQEndDate = utilities::GetQuarterEndDate(newestDistribution);

    while(qEndDate <= lastQEndDate){
        wxDateTime currentQStartDate = utilities::GetQuarterStartDate(qEndDate);
        double currentQDistributedAmount = 0.0;
        for(const auto& distribution:m_distributions){
            if(distribution.distribution.first >= currentQStartDate && 
                distribution.distribution.first <=qEndDate){
                    currentQDistributedAmount+=distribution.distribution.second;
            }
        }
        m_distributionsForBarChart.push_back({qEndDate,currentQDistributedAmount});
        qEndDate = utilities::GetNextQuarterEndDate(qEndDate);
    }
}


void Asset::AddDistribution(Distribution &distribution){
    m_distributions.push_back(distribution);
    SortDistributions2();
}
const std::vector<Distribution> Asset::GetDistributionsConst()const{
    return m_distributions;
}


double Asset::GetTotalCommitted()const{
    return m_assetCommittedCapital;
}

void Asset::ClearInvestorPositionDisplays(){
    m_investorPositionDisplays.clear();
}

void Asset::SetPositionValues(){
    for(auto&pos:m_positions){
        pos->SetCommitted();
        pos->SetOwnership();
        pos->SetReserve();
        pos->SetDeployed();
        pos->SetCurrentValue();
    }
}

const std::map<wxDateTime, double> Asset::GetROCMovements()const{
    return m_rocMovements;
}
const std::map<wxDateTime, double> Asset::GetMovementsToFromDeploy()const{
    return m_movementsToFromDeploy;
}

void Asset::MoveReserveToDeploy(wxDateTime &date, double amount){
    m_assetReserveCapital -= amount;
    m_assetDeployedCapital +=amount;
    m_movementsToFromDeploy[date] = amount;
    for(auto&pos:m_positions){
        std::pair<wxDateTime, double> movement = std::make_pair(date, (amount * pos->GetOwnership()));
        pos->AddMovementDeploy(movement);
    }
}

void Asset::MoveDeployToReserve(wxDateTime &date, double amount){
    m_assetReserveCapital += amount;
    m_assetDeployedCapital -= amount;
    m_movementsToFromDeploy[date] = -amount;
    for(auto&pos:m_positions){
        std::pair<wxDateTime, double> movement = std::make_pair(date, (amount * pos->GetOwnership()));
        pos->AddMovementDeploy(movement);
    }

}

void Asset::MoveReserveToReturnOfCapital(wxDateTime &date, double amount){
    m_assetReserveCapital -= amount;
    m_assetReturnOfCapital += amount;
    m_rocMovements[date] = amount;
    for(auto&pos:m_positions){
        pos->SetRocMovements();
    }
}

void Asset::MoveDeployToReturnOfCapital(wxDateTime &date, double amount){
    m_assetDeployedCapital -=amount;
    m_assetReturnOfCapital +=amount;
    m_rocMovements[date] = amount;
    for(auto&pos:m_positions){
        pos->SetRocMovements();
    }
}


void Asset::RemoveValuation(size_t index){
    std::swap(m_valuations[index],m_valuations.back());
    m_valuations.pop_back();
}




std::vector<std::shared_ptr<Position>>& Asset::GetPositionsForIDP(){
    return m_positions;
}


std::vector<Valuation>& Asset::GetValuationsNonConst(){
    return m_valuations;
}

std::vector<Distribution>& Asset::GetDistributionsNonConst(){
    return m_distributions;
}

void Asset::RemoveDistribution(size_t index){
    std::swap(m_distributions[index],m_distributions.back());
    m_distributions.pop_back();
}


void Asset::AddNewPositionAdditionalCapital(Position &position){
    std::shared_ptr<Position> newPosition = std::make_shared<Position>(position);
    m_positions.push_back(newPosition);

}

void Asset::SetNewCommittedOnNewPosition(double additionalCapital){
    m_assetCommittedCapital += additionalCapital;
}

void Asset::SetDeployedCapital(double &startingDeployed){
    m_assetDeployedCapital = startingDeployed;
}
void Asset::SetReserveCapital(double &startingReserve){
    m_assetReserveCapital = startingReserve;
}

void Asset::AddMovement(std::pair<wxDateTime, double>& movement){
    auto it = m_movementsToFromDeploy.find(movement.first);
    if( it != m_movementsToFromDeploy.end()){
        it->second +=movement.second;
    }else{
        m_movementsToFromDeploy[movement.first] = movement.second;    
    }
}
void Asset::AddNewDeployed(double &newDeployed){
    m_assetDeployedCapital +=newDeployed;
}

void Asset::AddNewReserve(double &newReserve){
    m_assetReserveCapital+=newReserve;
}

void Asset::TriggerUpdateDerivedValues(){
    UpdateTotalMgmtFeesEarned();
    UpdateTotalPromoteFeesEarned();
    UpdateTotalInvestedCapital();
    UpdateTotalCountOfInvestors();
    UpdateCurrentvalue();
    CalculateIrr();
}

void Asset::CalculateIrr(){
    std::vector<CashFlow> cashFlow;
    for(const auto& movement: m_movementsToFromDeploy){
        CashFlow newCashFlow;
        newCashFlow.amount = -movement.second;
        newCashFlow.date = movement.first;
        cashFlow.push_back(newCashFlow);
    }
    for(const auto& distr : m_distributions){
        CashFlow newCashFlow;
        newCashFlow.amount = distr.distribution.second;
        newCashFlow.date = distr.distribution.first;
        cashFlow.push_back(newCashFlow);
    }
    if(!m_valuations.empty()){
        CashFlow newCashFlow;
        SortValuations2();
        auto valuation = m_valuations.back();
        newCashFlow.amount = valuation.valuation;
        newCashFlow.date = valuation.valuationDate;
        cashFlow.push_back(newCashFlow);
    }else{
        CashFlow newCashFlow;
        newCashFlow.amount = m_assetDeployedCapital;
        newCashFlow.date = wxDateTime::Today();
        cashFlow.push_back(newCashFlow);
    }
    std::sort(cashFlow.begin(),cashFlow.end(),
                [](const CashFlow &a, const CashFlow &b){
                    return a.date < b.date;
                });
    for(const auto&cf:cashFlow){
        std::cout<<"Cash Flow Date: "<<cf.date.FormatISODate().ToStdString()<<" | Cash Flow Amount: "<<cf.amount<<std::endl;
    }
    m_irr = 0;
    double guess = 0.1;
    double x1 = 0.0;
    int maxIterations = 100;
    double precision = 0.000001;
    for(int i = 0; i<maxIterations;i++){
        double npv = CalculateNPV(cashFlow, guess);
        double guessAddPrecision = guess + precision;
        double npvPrime = (CalculateNPV(cashFlow, guessAddPrecision)- npv)/precision;
        x1 =guess - npv / npvPrime;
        if(std::fabs(x1-guess)<=precision){
            m_irr = x1;
            break;
        }
        guess = x1;
    }
}

double Asset::CalculateNPV(std::vector<CashFlow> &cashFlows, double &rate){
    double npv = 0.0;
    wxDateTime firstDate = cashFlows[0].date;
    for(const auto& cf : cashFlows){
        wxTimeSpan timeSpan = cf.date - firstDate;
        double years = timeSpan.GetDays() / 365.25;
        npv += cf.amount / std::pow(1+rate, years);
    }
    return npv;
}

void Asset::PassDistributionToPositions(Distribution &distribution){
    double distributionAmount = distribution.distribution.second;
    double totalCapdays = 0.0;
    std::map<std::shared_ptr<Position>, double> positionsCapitalizedDays;
    for(const auto&pos: this->GetPositions()){
        double positionsCapitalDays =0.0;
        positionsCapitalDays = pos->CalculateCapitalDays(*this,distribution.distribution.first);
        positionsCapitalizedDays[pos] = positionsCapitalDays;
        totalCapdays+=positionsCapitalDays;
        std::cout<<"Position CAP DAYS: "<<positionsCapitalDays<<std::endl;
    }
}