#include "baseclasses/asset2.hpp"

void Asset2::SortDistributions(std::vector<Distribution> &distributions){
    std::sort(m_distributions.begin(), m_distributions.end(),
    [](const Distribution &a, const Distribution &b) {
        return a.distribution.first < b.distribution.first;
    });  
}

void Asset2::ProcessDistributionsForPosition(){
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

const std::vector<Valuation>& Asset2::GetValuations()const{
    return m_valuations;
}

void Asset2::AddNewValuation(const wxDateTime &valuationDate, double valuationAmount){
    Valuation newValuation;
    newValuation.valuation=valuationAmount;
    newValuation.valuationDate=valuationDate;
    m_valuations.push_back(newValuation);
    UpdatePositionValuations();
}

void Asset2::UpdatePositionValuations(){
    for(auto& position: m_positions){
        position->RepopulateValuations();
    }
}

double Asset2::GetValuationInQuarter(wxDateTime &date)const{
    auto quarterEnd = utilities::GetQuarterEndDate(date);
    double valuationAmount = 0;
    for(const auto&valuation: m_valuations){
        if(valuation.valuationDate >= date && valuation.valuationDate <= quarterEnd){
            valuationAmount = valuation.valuation;
        }
    }
    return valuationAmount;
}

void Asset2::UpdateTotalMgmtFeesEarned(){
    double totalMgmtFeesEarned = 0.0;
    for(const auto& position:m_positions){
        for(const auto&mgmtfee: position->GetManagementFees()){
            totalMgmtFeesEarned+=mgmtfee.managementFeesAsset.second;
        }
    }
    m_totalMgmtFeesEarned = totalMgmtFeesEarned;
}

void Asset2::UpdateTotalPromoteFeesEarned(){
    double totalPromoteFeesEarned = 0.0;
    for(const auto&pos:m_positions){
        for(const auto&pf : pos->GetPromoteFees()){
            totalPromoteFeesEarned+=pf.promotefee.second;
        }
    }
    m_totalPromoteFeesEarned = totalPromoteFeesEarned;
}

void Asset2::UpdateTotalCountOfInvestors(){
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

void Asset2::UpdateTotalInvestedCapital(){
    double totalInvested = 0.0;
    for(const auto&pos:m_positions){
        totalInvested += pos->GetCommitted();
    }
    m_totalInvestedCapital = totalInvested;
}

wxString Asset2::GetAssetName()const{
    return m_assetName;
}
wxString Asset2::GetAssetSponserName()const{
    return m_assetSponserName;
}
wxDateTime Asset2::GetAssetExitDate()const{
    return m_assetExitDate;
}
double Asset2::GetTotalInvestedCapital()const{
    return m_totalInvestedCapital;
}
double Asset2::GetTotalAssetDeployed()const{
    return m_assetDeployedCapital;
}
double Asset2::GetTotalAssetReserve()const{
    return m_assetReserveCapital;
}
double Asset2::GetTotalReturnOfCapital()const{
    return m_assetReserveCapital;
}

const std::vector<std::shared_ptr<Position2>>& Asset2::GetPositions()const{
    return m_positions;
}
std::vector<wxString> Asset2::columnNames = {"Asset Name","Exit Date","Total Deployed","# Investors","Current Value"};
std::vector<int> Asset2::columnWidths = {150,75,100,100,100};

wxVariant Asset2::GetValue(int col)const{
    switch(col){
        case 0: return wxVariant(m_assetName);
        case 1: return wxVariant(m_assetExitDate);
        case 2: return wxVariant(m_totalInvestedCapital);
        case 3: return wxVariant(m_countOfInvestors);
        case 4: return wxVariant(m_currentValue);
        default: return wxVariant();
    }
}

void Asset2::SetValue(int col, const wxVariant &v){
    switch(col){
        case 0: m_assetName = v.GetString();break;
        case 1: m_assetExitDate = v.GetDateTime();break;
        case 2: m_totalInvestedCapital = v.GetDouble();break;
        case 3: m_countOfInvestors = v.GetDouble();break;
        case 4: m_currentValue = v.GetDouble();break;
    }
}
void Asset2::SortValuations(std::vector<Valuation> &valuations){
    std::sort(valuations.begin(),valuations.end(),
        [](const Valuation &a, const Valuation &b){
            return a.valuationDate < b.valuationDate;
        });
}

void Asset2::UpdateCurrentvalue(){
    if(!m_valuations.empty()){
        SortValuations(m_valuations);
        m_currentValue = m_valuations.back().valuation;
    }else{
        m_currentValue = m_totalInvestedCapital;
    }
    
}