#include "models/entities/asset.hpp"

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

void Asset::ProcessDistributionsForPosition(){
    SortDistributions(m_distributions);
    for(auto &pos:m_positions){
        pos->ClearNetIncomePromoteFees();
    }
    for(const auto& distribution : m_distributions){
        for(auto &position: m_positions){
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
            newDistribution.distribution.second = remainder * - newPromoteFee.promotefee.second;
            position->UpdateFinancesPostDistributionChanges(newDistribution, newPromoteFee);
        }
    }
}

const std::vector<Valuation>& Asset::GetValuations()const{
    return m_valuations;
}

void Asset::AddNewValuation(const wxDateTime &valuationDate, double valuationAmount){
    Valuation newValuation;
    newValuation.valuation=valuationAmount;
    newValuation.valuationDate=valuationDate;
    m_valuations.push_back(newValuation);
    UpdatePositionValuations();
}

void Asset::UpdatePositionValuations(){
    for(auto& position: m_positions){
        position->RepopulateValuations();
    }
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
            totalMgmtFeesEarned+=mgmtfee.managementFeesAsset.second;
        }
    }
    m_totalMgmtFeesEarned = totalMgmtFeesEarned;
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
    return m_assetReserveCapital;
}

const std::vector<std::shared_ptr<Position>>& Asset::GetPositions()const{
    return m_positions;
}
std::vector<wxString> Asset::columnNames = {"Asset Name","Exit Date","Total Deployed","# Investors","Current Value"};
std::vector<int> Asset::columnWidths = {150,75,100,100,100};

wxVariant Asset::GetValue(int col)const{
    switch(col){
        case 0: return wxVariant(m_assetName);
        case 1: return wxVariant(m_assetExitDate);
        case 2: return wxVariant(m_totalInvestedCapital);
        case 3: return wxVariant(m_countOfInvestors);
        case 4: return wxVariant(m_currentValue);
        default: return wxVariant();
    }
}

void Asset::SortValuations(std::vector<Valuation> &valuations){
    std::sort(valuations.begin(),valuations.end(),
        [](const Valuation &a, const Valuation &b){
            return a.valuationDate < b.valuationDate;
        });
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
        m_currentValue = m_totalInvestedCapital;
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
    if(j.contains("Asset Committed")){
        double committed = j["Asset Committed"].get<double>();
        asset.DeserializeSetAssetCommittedCapital(committed);
    }
    if(j.contains("Asset Deployed")){
        double deployed = j["Asset Deployed"].get<double>();
        asset.DeserializeSetAssetDeployedCapital(deployed);
    }
    if(j.contains("Asset Reserve")){
        double reserve = j["Asset Reserve"].get<double>();
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
    double totalMgmtFeesDue = 0.0;
    if(m_distributions.empty()){
        return totalMgmtFeesDue;
    }
    for(const auto&position:m_positions){
        for(const auto&mf:position->GetManagementFees()){
            if(mf.managementFeesAsset.first <= m_distributions.back().distribution.second){
                totalMgmtFeesDue+=mf.managementFeesAsset.second;
            }
        }
    }
    return totalMgmtFeesDue;
    //questionable this assumes there will always be a distribution that can take care of
    //all mgmtfees eventually might not actually happen in production 
    //might require keeping a second vector<ManagementFee> in position
    //that tracks when a mf was paid once a distribution happens
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

    SortValuations2();

    while(qEndDate.IsEarlierThan(currentQStartDate)){
        double qValuation = 0.0;
        for(const auto&valuation:m_valuations){
            if(valuation.valuationDate.IsEarlierThan(qEndDate)|| valuation.valuationDate == qEndDate){
                qValuation = valuation.valuation;
            }
        }
        m_previousQValuationMap[qEndDate] = qValuation;
        qEndDate = utilities::GetNextQuarterEndDate(qEndDate);
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

void Asset::PopulatePreviousQDeploys(){
    m_previousQDeploymentMap.clear();
    wxDateTime oldestInvestedDate = wxDateTime::Today();
    for(const auto&position:m_positions){
        if(position->GetDateInvested().IsEarlierThan(oldestInvestedDate)){
            oldestInvestedDate = position->GetDateInvested();
        }
    }
    wxDateTime qEndDate = utilities::GetQuarterEndDate(oldestInvestedDate);
    wxDateTime today = wxDateTime::Today();
    wxDateTime currentQStartDate = utilities::GetQuarterStartDate(today);
    while(qEndDate.IsEarlierThan(currentQStartDate)){
        double adjustedDeployedCapiatl = m_assetDeployedCapital;//currently deployed today
        for(auto& movement: m_movementsToFromDeploy){
            //reverse all movements out if they happened on or after the Q end date to get the ending balance
            if(movement.first >= qEndDate){
                adjustedDeployedCapiatl+=movement.second;
            }
        }
        m_previousQDeploymentMap[qEndDate] = adjustedDeployedCapiatl;
        qEndDate = utilities::GetNextQuarterEndDate(qEndDate);
    }
}

void Asset::PopulateCurrentQDeploys(){
    m_currentQDeploymentMap.clear();
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
}

void Asset::PopulateValuationsDeploymentsForPlotting(){

    m_valuationsForPlotting.clear();
    m_deploymentsForPlotting.clear();
    PopulatePreviousQValuations();
    if(!m_previousQValuationMap.empty()){
        PopulateCurrentQValuations();
    }
    PopulatePreviousQDeploys();
    if(!m_previousQDeploymentMap.empty()){
        PopulateCurrentQDeploys(); 
    }
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


double Asset::GetTotalCommitted()const{
    return m_assetCommittedCapital;
}