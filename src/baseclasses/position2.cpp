#include "position2.hpp"

//Getter Definitions
wxDateTime Position2::GetDateInvested()const{
    return m_dateInvested;
}
double Position2::GetSubscribed()const{
    return m_subscribed;
}
double Position2::GetReserve()const{
    return m_reserve;
}
double Position2::GetDeployed()const{
    return m_deployed;
}
double Position2::GetReturnOfCapital()const{
    return m_returnOfCapital;
}
double Position2::GetOwnership()const{
    return m_percentOwnership;
}
double Position2::GetManagementFeesDue()const{
    return m_managementFeesDue;
}
std::vector<ManagementFee> Position2::GetManagementFees()const{
    return m_managementFees;
}
std::vector<PromoteFee> Position2::GetPromoteFees()const{
    return m_promoteFees;
}
const std::map<wxDateTime, double> Position2::GetPositionValuations()const{
    return m_valuationOfPosition;
}
std::map<wxDateTime, double> Position2::GetMovedFromDeploy()const{
    return m_movedOutOfDeploy;
}
std::map<wxDateTime, double> Position2::GetMovedToDeploy()const{
    return m_movedToDeploy;
}
double Position2::GetCommitted()const{
    return m_committed;
}
//Setter Definitions
void Position2::SetDeployed(double deployed){
    m_deployed = deployed;
}
void Position2::SetReserve(double reserve){
    m_reserve = reserve;
}
void Position2::SetReturnOfCapital(double returnOfCapital){
    m_returnOfCapital = returnOfCapital;
}
//methods
double Position2::CalculateCommittedUpToDate(const wxDateTime &date)const{
    double committed = m_paid;
    for(const auto& rocEvent : m_returnOfCapitalMap){
        if(rocEvent.first<=date){
            committed-=rocEvent.second;
        }
    }
    return committed > 0 ? committed : 0;
}

double Position2::CalculateOwnershipAtDate(const wxDateTime &date){
    double totalPaidCapitalAtDate = 0.0;
    double committed = CalculateCommittedUpToDate(date);
    for(auto&pos:m_assetPtr->GetPositions()){
        if(pos.GetDateInvested() <= date){
            totalPaidCapitalAtDate += pos.CalculateCommittedUpToDate(date);
        }
    }
    return (totalPaidCapitalAtDate >0)? committed / totalPaidCapitalAtDate: 0.0;
}

void Position2::UpdateFinancesPostDistributionChanges(Distribution &distribution, PromoteFee &promoteFee){
    m_netIncome.push_back(distribution);
    m_promoteFees.push_back(promoteFee);
}

double Position2::CalculateManagementFeesDue(const Distribution &currentDistribution){
    wxDateTime distributionDate = currentDistribution.distribution.first;
    wxDateTime currentQStartDate = utilities::GetQuarterStartDate(distributionDate);
    wxDateTime lastDistributionDate = !m_netIncome.empty() ? m_netIncome.back().distribution.first : m_dateInvested;
    wxDateTime lastQuarterStart = utilities::GetQuarterStartDate(lastDistributionDate);
    double totalFeesDue = 0.0;
    for (const auto& fee : m_managementFees) {
        if ((m_netIncome.empty() && fee.managementFeesAsset.first <= currentQStartDate) || 
            (!m_netIncome.empty() && fee.managementFeesAsset.first > lastQuarterStart && fee.managementFeesAsset.first <= currentQStartDate)) {
            totalFeesDue += fee.managementFeesAsset.second;
        }
    }
    return totalFeesDue;
}   

double Position2::GetInvestorPromoteFee(){
    return m_investorPtr->GetPromoteFeePercentage();
}

void Position2::ClearNetIncomePromoteFees(){
    m_netIncome.clear();
    m_promoteFees.clear();
}

void Position2::PopulateManagementFeeVector() {
    double managementFeePercentage = m_investorPtr->GetManagementFeePercentage();
    wxDateTime investmentStartDate = utilities::GetQuarterStartDate(m_dateInvested); // Ensure starting from the beginning of the quarter of investment
    wxDateTime currentDate = wxDateTime::Today();
    wxDateTime currentQuarterEndDate = utilities::GetQuarterEndDate(currentDate);

    m_managementFees.clear();

    for (wxDateTime quarterStartDate = investmentStartDate; quarterStartDate <= currentQuarterEndDate; quarterStartDate = utilities::GetNextQuarterStartDate(quarterStartDate)) {
        ManagementFee feeForQuarter = CalculatePositionManagementFees(managementFeePercentage, quarterStartDate);

        if (feeForQuarter.managementFeesAsset.second > 0) {
            m_managementFees.push_back(feeForQuarter);
        }

    }
}

ManagementFee Position2::CalculatePositionManagementFees(const double &managementFeePercentage, const wxDateTime &quarterStartDate) {
    std::pair<wxDateTime, wxDateTime> quarterDates = utilities::GetCurrentQuarterDates(quarterStartDate);
    wxDateTime quarterStart = quarterDates.first;
    wxDateTime quarterEnd = quarterDates.second;

    wxDateTime calculationStartDate = std::max(m_dateInvested, quarterStart);

    double committedCapitalStart = CalculateCommittedUpToDate(calculationStartDate);
    double totalFee = 0.0;
    double lastCapital = committedCapitalStart;
    wxDateTime lastDate = calculationStartDate;

    for (const auto& rocEvent : m_returnOfCapitalMap) {
        if (rocEvent.first > calculationStartDate && rocEvent.first <= quarterEnd) {

            double daysBeforeROC = (rocEvent.first - lastDate).GetDays();
            double annualizedFactor = daysBeforeROC / 365.0;
            totalFee += lastCapital * managementFeePercentage * annualizedFactor;

            lastCapital -= rocEvent.second;
            lastDate = rocEvent.first;
        }
    }
    double daysAfterLastROC = (quarterEnd - lastDate).GetDays() + 1; 
    double annualizedFactor = daysAfterLastROC / 365.0;
    totalFee += lastCapital * managementFeePercentage * annualizedFactor;

    ManagementFee feeThisQuarter;
    feeThisQuarter.managementFeesAsset.first = quarterEnd;
    feeThisQuarter.managementFeesAsset.second = totalFee;

    return feeThisQuarter;
}

void Position2::TriggerUpdateOfManagementFeeVector(){
    PopulateManagementFeeVector();
}

void Position2::RepopulateValuations() {
    m_valuationOfPosition.clear();

    wxDateTime quarterDate = utilities::GetQuarterStartDate(m_dateInvested);
    wxDateTime currentDate = wxDateTime::Today();
    wxDateTime lastValuationDate = quarterDate; 
    double lastKnownValuation = CalculateCommittedUpToDate(m_dateInvested);

    while (quarterDate <= currentDate) {
        wxDateTime nextQuarterDate = utilities::GetNextQuarterStartDate(quarterDate);
        double valuationAmount = 0.0;
        bool valuationFound = false;

    for (const auto& valuation : m_assetPtr->GetValuations()) {
        if (valuation.valuationDate >= quarterDate && valuation.valuationDate < nextQuarterDate) {
            valuationAmount = valuation.valuation;
            lastKnownValuation = valuationAmount;
            lastValuationDate = valuation.valuationDate;
        }
    }

        if (!valuationFound && quarterDate > m_dateInvested) { 
            valuationAmount = lastKnownValuation;
        }

        double ownershipAtThisValuation = CalculateOwnershipAtDate(lastValuationDate);
        double positionValuation = valuationAmount * ownershipAtThisValuation;

        m_valuationOfPosition[quarterDate] = positionValuation;

        quarterDate = nextQuarterDate;
    }
}

std::vector<Distribution> Position2::GetNetIncome()const{
    return m_netIncome;
}

std::shared_ptr<Investor2> Position2::GetInvestorPtr()const{
    return m_investorPtr;
}