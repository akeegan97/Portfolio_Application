#include "models/entities/position.hpp"

//Getter Definitions
wxDateTime Position::GetDateInvested()const{
    return m_dateInvested;
}
double Position::GetReserve()const{
    return m_reserve;
}
double Position::GetPaid()const{
    return m_paid;
}
void Position::SetId(size_t id){
    m_id = id;
}
size_t Position::GetId(){
    return m_id;
}
double Position::GetDeployed()const{
    return m_deployed;
}
double Position::GetReturnOfCapital()const{
    return m_returnOfCapital;
}
double Position::GetOwnership()const{
    return m_percentOwnership;
}
double Position::GetManagementFeesDue()const{
    return m_managementFeesDue;
}
std::vector<ManagementFee> Position::GetManagementFees()const{
    return m_managementFees;
}
std::vector<PromoteFee> Position::GetPromoteFees()const{
    return m_promoteFees;
}
const std::map<wxDateTime, double> Position::GetPositionValuations()const{
    return m_valuationOfPosition;
}
std::map<wxDateTime, double> Position::GetMovedFromDeploy()const{
    return m_movedOutOfDeploy;
}
std::map<wxDateTime, double> Position::GetMovedToDeploy()const{
    return m_movedToDeploy;
}
double Position::GetCommitted()const{
    return m_committed;
}
//Setter Definitions
void Position::SetDeployed(double deployed){
    m_deployed = deployed;
}
void Position::SetReserve(double reserve){
    m_reserve = reserve;
}
void Position::SetReturnOfCapital(double returnOfCapital){
    m_returnOfCapital = returnOfCapital;
}
std::shared_ptr<Asset> Position::GetAssetPointer()const{
    return m_assetPtr;
}
std::map<wxDateTime, double> Position::GetReturnOfCapitalMap(){
    return m_returnOfCapitalMap;
}
//methods
double Position::CalculateCommittedUpToDate(const wxDateTime &date)const{
    if(m_dateInvested > date){
        return 0.0;
    }
    double committed = m_paid;
    for(const auto& rocEvent : m_returnOfCapitalMap){
        if(rocEvent.first<=date){
            committed-=rocEvent.second;
        }
    }
    return committed > 0 ? committed : 0;
}

double Position::CalculateOwnershipAtDate(const wxDateTime &date){
    double totalPaidCapitalAtDate = 0.0;
    double committed = CalculateCommittedUpToDate(date);
    for(auto&pos:m_assetPtr->GetPositions()){
        if(pos->GetDateInvested() <= date){
            totalPaidCapitalAtDate += pos->CalculateCommittedUpToDate(date);
        }
    }
    return (totalPaidCapitalAtDate >0)? committed / totalPaidCapitalAtDate: 0.0;
}

void Position::UpdateFinancesPostDistributionChanges(Distribution &distribution, PromoteFee &promoteFee){
    m_netIncome.push_back(distribution);
    m_promoteFees.push_back(promoteFee);
}

double Position::CalculateManagementFeesDue(const Distribution &currentDistribution) {
    // Determine the quarter end date of the distribution
    wxDateTime distributionDate = currentDistribution.distribution.first;
    wxDateTime distributionQEndDate = utilities::GetQuarterEndDate(distributionDate);
    
    // Initialize total fees due
    double totalFeesDue = 0.0;
    
    // Determine if this is the first distribution of the quarter
    bool isFirstDistributionOfQuarter = true;
    for (const auto& pastDistribution : m_netIncome) {
        if (utilities::AreSameQuarter(pastDistribution.distribution.first, distributionDate)) {
            isFirstDistributionOfQuarter = false;
            break; // Already had a distribution this quarter
        }
    }

    // Apply fees only if this is the first distribution of the quarter
    if (isFirstDistributionOfQuarter) {
        for (const auto& fee : m_managementFees) {
            // Apply fees from the start of the investment quarter to the end of the distribution quarter
            wxDateTime feeDate = fee.managementFeesAsset.first;
            if (feeDate >= utilities::GetQuarterStartDate(m_dateInvested) && feeDate <= distributionQEndDate) {
                totalFeesDue += fee.managementFeesAsset.second;
            }
        }
    }

    return totalFeesDue;
} 

double Position::GetInvestorPromoteFee(){
    return m_investorPtr->GetPromoteFeePercentage();
}

void Position::ClearNetIncomePromoteFees(){
    m_netIncome.clear();
    m_promoteFees.clear();
}

void Position::SetAssetPtr(std::shared_ptr<Asset> &assetPointer){
    m_assetPtr = assetPointer;
}
void Position::SetInvestorPtr(std::shared_ptr<Investor> &investorPointer){
    m_investorPtr = investorPointer;
}
void Position::SetDateInvested(wxDateTime &date){
    m_dateInvested = date;
}
void Position::SetPaid(double &paid){
    m_paid = paid;
}
void Position::SetManagementFeesDue(double &mgmtFeeDue){
    m_managementFeesDue = mgmtFeeDue;
}
void Position::AddMovedFromDeployEntry(std::pair<wxDateTime, double> &movement){
    m_movedOutOfDeploy[movement.first] = movement.second;
}
void Position::AddMovedToDeployEntry(std::pair<wxDateTime, double> &movement){
    m_movedToDeploy[movement.first] = movement.second;
}
void Position::AddRocMovement(std::pair<wxDateTime, double> &movement){
    m_returnOfCapitalMap[movement.first] = movement.second;
}

void Position::SetCommitted(){
    m_committed = m_paid - m_returnOfCapital;
}
void Position::UpdateROC(){
    double totalRoc = 0;
    for(auto movement: m_returnOfCapitalMap){
        totalRoc +=movement.second;
    }
    m_returnOfCapital = totalRoc;
}
void Position::SetReserve(){
    m_reserve = m_assetPtr->GetTotalAssetReserve() * m_percentOwnership;
}

void Position::SetDeployed(){
    m_deployed = m_assetPtr->GetTotalAssetDeployed() * m_percentOwnership;
}

void Position::SetOwnership(){
    m_percentOwnership = m_committed / m_assetPtr->GetTotalCommitted();
}

void Position::SetCurrentValue(){
    m_currentValue = m_percentOwnership * m_assetPtr->GetCurrentValue();
}

void Position::SetRocMovements(){
    for(auto movement: m_assetPtr->GetROCMovements()){
        if(movement.first > m_dateInvested){
            m_returnOfCapitalMap[movement.first] = movement.second * CalculateOwnershipAtDate(movement.first);
            SetReturnOfCapital(movement.second * CalculateOwnershipAtDate(movement.first));
        }
    }
}

void Position::SetMovedToDeploy(){
    for(auto movement:m_assetPtr->GetMovementsToFromDeploy()){
        if(movement.first > m_dateInvested){
            if(movement.second > 0){
                m_movedToDeploy[movement.first] = movement.second * CalculateOwnershipAtDate(movement.first);
            }
        }
    }
}

void Position::SetMovedFromDeploy(){
    for(auto movement:m_assetPtr->GetMovementsToFromDeploy()){
        if(movement.first > m_dateInvested){
            if(movement.second < 0){
                m_movedOutOfDeploy[movement.first] = movement.second * CalculateOwnershipAtDate(movement.first);
            }
        }
    }
}

void Position::PopulateManagementFeeVector() {
    double managementFeePercentage = m_investorPtr->GetManagementFeePercentage();
    wxDateTime investmentStartDate = m_dateInvested; 
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

ManagementFee Position::CalculatePositionManagementFees(const double &managementFeePercentage, const wxDateTime &quarterStartDate) {
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

void Position::TriggerUpdateOfManagementFeeVector(){
    PopulateManagementFeeVector();
}

void Position::RepopulateValuations() {
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

std::vector<Distribution> Position::GetNetIncome()const{
    return m_netIncome;
}

std::shared_ptr<Investor> Position::GetInvestorPtr()const{
    return m_investorPtr;
}

double Position::GetPositionValue()const{
    return m_currentValue;
}
void from_json(const json&j, Position &position, Portfolio &port){
    wxString assetName = wxString::FromUTF8(j["AssetName"].get<std::string>().c_str());
    for(auto &assetPointer : port.assetPtrs){
        if(assetPointer->GetAssetName() == assetName){
            position.SetAssetPtr(assetPointer);
            break;
        }
    }
    wxString investorName = wxString::FromUTF8(j["InvestorName"].get<std::string>().c_str());
    for(auto& investorPointer : port.allInvestorPtrs){
        if(investorPointer->GetName()==investorName){
            position.SetInvestorPtr(investorPointer);
            break;
        }
    }
    wxString dateStr = wxString::FromUTF8(j["Date Invested"].get<std::string>().c_str());
    wxDateTime dateParse;
    dateParse.ParseDate(dateStr);
    position.SetDateInvested(dateParse);
    double paid = j["Paid"].get<double>();
    position.SetPaid(paid);
    double mgmtFeesDue = j["Management Fees Due"].get<double>();
    position.SetManagementFeesDue(mgmtFeesDue);
    double returnOfCapital = j["ROC"].get<double>();
    position.SetReturnOfCapital(returnOfCapital);
    if(j.contains("MovedToDeploy")){
        for(const auto&[dateStr, amount]: j["MovedToDeploy"].items()){
            wxDateTime date;
            date.ParseISODate(dateStr);
            double amountMoved = amount;
            std::pair<wxDateTime, double> movement = std::make_pair(date, amountMoved);
            position.AddMovedToDeployEntry(movement);
        }
    }
    if(j.contains("MovedOutDeploy")){
        for(const auto&[dateStr, amount]: j["MovedOutDeploy"].items()){
            wxDateTime date;
            date.ParseISODate(dateStr);
            double amountMoved = amount;
            std::pair<wxDateTime, double> movement = std::make_pair(date, amountMoved);
            position.AddMovedFromDeployEntry(movement);
        }
    }
    if(j.contains("ROC Movements")){
        for(const auto&[dateStr, amount]:j["ROC Movements"].items()){
            wxDateTime date;
            date.ParseISODate(dateStr);
            double amountMoved = amount;
            std::pair<wxDateTime, double> movement = std::make_pair(date, amountMoved);
            position.AddRocMovement(movement);
        }
    }

}