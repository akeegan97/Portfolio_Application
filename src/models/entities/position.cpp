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
    double mfdue = 0.0;
    for(auto&mf:m_managementFees){
        if(mf.paid == false){
            mfdue+=mf.amount;
        }
    }
    return mfdue;
}
std::vector<ManagementFee> Position::GetManagementFees()const{
    return m_managementFees;
}
std::vector<ManagementFee>& Position::GetManagementFeesReference(){
    return this->m_managementFees;
}
std::vector<PromoteFee> Position::GetPromoteFees()const{
    return m_promoteFees;
}
const std::map<wxDateTime, double> Position::GetPositionValuations()const{
    return m_valuationOfPosition;
}
std::map<wxDateTime, double> Position::GetMovementsDeploy()const{
    return m_movementsDeploy;
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
    
    // bool isFirstDistributionOfQuarter = true;
    // for (const auto& pastDistribution : m_netIncome) {
    //     if (utilities::AreSameQuarter(pastDistribution.distribution.first, distributionDate)) {
    //         isFirstDistributionOfQuarter = false;
    //         break;
    //     }
    // }

    // if (isFirstDistributionOfQuarter) {
    //     for (const auto& fee : m_managementFees) {
    //         // Apply fees from the start of the investment quarter to the end of the distribution quarter
    //         wxDateTime feeDate = fee.managementFeesAsset.first;
    //         if (feeDate >= utilities::GetQuarterStartDate(m_dateInvested) && feeDate <= distributionQEndDate) {
    //             totalFeesDue += fee.managementFeesAsset.second;
    //         }
    //     }
    // }

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
void Position::AddMovementDeploy(std::pair<wxDateTime, double> &movement){
    auto it = m_movementsDeploy.find(movement.first);
    if(it!=m_movementsDeploy.end()){
        it->second+=movement.second;
    }else{
        m_movementsDeploy[movement.first] = movement.second;    
    }
}

void Position::AddRocMovement(std::pair<wxDateTime, double> &movement){
    auto it = m_returnOfCapitalMap.find(movement.first);
    if(it != m_returnOfCapitalMap.end()){
        it->second += movement.second;
    }else{
        m_returnOfCapitalMap[movement.first] = movement.second;    
    }
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

void Position::PopulateManagementFeeVector() {
    double managementFeePercentage = m_investorPtr->GetManagementFeePercentage();
    wxDateTime investmentStartDate = m_dateInvested; 
    wxDateTime currentDate = wxDateTime::Today();
    wxDateTime currentQuarterEndDate = utilities::GetQuarterEndDate(currentDate);

    m_managementFees.clear();

    for (wxDateTime quarterStartDate = investmentStartDate; quarterStartDate <= currentQuarterEndDate; quarterStartDate = utilities::GetNextQuarterStartDate(quarterStartDate)) {
        ManagementFee feeForQuarter = CalculatePositionManagementFee(managementFeePercentage, quarterStartDate);

        if(feeForQuarter.amount >0){
            m_managementFees.push_back(feeForQuarter);
        }
    }
}

//--New MGMT FEE Calculations--//

ManagementFee Position::CalculatePositionManagementFee(const double &mgmtFeePercent, const wxDateTime &qStart) {
    std::pair<wxDateTime, wxDateTime> qDates = utilities::GetCurrentQuarterDates(qStart);
    wxDateTime qStartDate = qDates.first;
    wxDateTime qEndDate = qDates.second;

    double totalMgmtFee = 0.0;
    double currentlyDeployed = 0.0;
    wxDateTime calculationStartDate = qStartDate;
    wxDateTime lastMovementDate = qStartDate;

    if (!m_movementsDeploy.empty()) {
        auto firstMovementDate = m_movementsDeploy.begin()->first;
        if (firstMovementDate > qStartDate && firstMovementDate <= qEndDate) {
            calculationStartDate = firstMovementDate;
        }
    }
    double deployedBeforeQuarter = 0.0;
    for (const auto& movement : m_movementsDeploy) {
        if (movement.first < calculationStartDate) {
            deployedBeforeQuarter += movement.second;
        }
    }

    currentlyDeployed = deployedBeforeQuarter;

    for (const auto& movement : m_movementsDeploy) {
        if (movement.first > qEndDate) break;

        if (movement.first >= calculationStartDate) {
            double daysInSegment = (movement.first - lastMovementDate).GetDays();
            double annualizedFactor = daysInSegment / 365.25;
            totalMgmtFee += currentlyDeployed * mgmtFeePercent * annualizedFactor;
            
            currentlyDeployed += movement.second;
            lastMovementDate = movement.first;
        }
    }

    double daysInSegment = (qEndDate - lastMovementDate).GetDays() + 1; // Include end date
    double annualizedFactor = daysInSegment / 365.25;
    totalMgmtFee += currentlyDeployed * mgmtFeePercent * annualizedFactor;

    return ManagementFee{qEndDate, totalMgmtFee, false};
}

void Position::UpdateManagementFees(const wxDateTime &date) {
    wxDateTime qEndDate = utilities::GetQuarterEndDate(date);

    auto isOnOrAfterQuarter = [qEndDate](const ManagementFee& fee) {
        return fee.quarter >= qEndDate; 
    };

    auto iter = std::find_if(m_managementFees.begin(), m_managementFees.end(), isOnOrAfterQuarter);

    if (iter != m_managementFees.end()) {
        m_managementFees.erase(iter, m_managementFees.end());
    }
    wxDateTime qStartDate = utilities::GetQuarterStartDate(date);

    RecalculateManagementFeesFrom(qStartDate);
}

void Position::RecalculateManagementFeesFrom(const wxDateTime &qStartDate){
    double managementFeePercentage = m_investorPtr->GetManagementFeePercentage();
    wxDateTime currentQuarterEndDate = utilities::GetQuarterEndDate(wxDateTime::Today());
    wxDateTime quarterStartDate = qStartDate;  

    while (quarterStartDate <= currentQuarterEndDate) {
        ManagementFee feeForQuarter = CalculatePositionManagementFee(managementFeePercentage, quarterStartDate);
        if(feeForQuarter.amount > 0) {
            m_managementFees.push_back(feeForQuarter);
        }
        quarterStartDate = utilities::GetNextQuarterStartDate(quarterStartDate);
    }
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

void to_json(json &j, const Position &pos){
    j={
        {"AssetName", pos.GetAssetPointer()->GetAssetName().ToStdString()},
        {"InvestorName", pos.GetInvestorPtr()->GetName()},
        {"Date Invested", pos.GetDateInvested().FormatISODate()},
        {"Paid", pos.GetPaid()},
        {"Management Fees Due", pos.GetManagementFeesDue()},
        {"ROC", pos.GetReturnOfCapital()},
        {"Movements Deploy", json::array()},
        {"Management Fees",json::array()},
        {"ROC Movements", json::array()},
        {"Net Income",json::array()},
        {"Promote Fees",json::array()}
    };
    std::vector<json> feesJson;
    for (const auto& fee : pos.GetManagementFees()) { 
        json feeJson;
        to_json(feeJson, fee); 
        feesJson.push_back(feeJson); 
    }
    j["Management Fees"] = feesJson; 

    json movementsDeploy;
    for(const auto&movement:pos.GetMovementsDeploy()){
        movementsDeploy.push_back({
            movement.first.FormatISODate().ToStdString(), movement.second
        });
    }
    j["Movements Deploy"] = movementsDeploy;
    json movedFromDeployJson;
    for(const auto&movement:pos.GetMovementsDeploy()){
        movementsDeploy.push_back({
            movement.first.FormatISODate().ToStdString(), movement.second
        });
    }
    json rocMovementsJson;
    for (const auto& movement : pos.GetROCMapConstant()) {
        rocMovementsJson.push_back({
            {movement.first.FormatISODate().ToStdString(), movement.second}
        });
    }
    j["ROC Movements"] = rocMovementsJson;
    json netIncomeJson;
    for(const auto& ni: pos.GetNetIncome()){
        json netIncome;
        to_json(netIncome,ni);
        netIncomeJson.push_back(netIncome);
    }
    j["Net Income"] = netIncomeJson;
    json promotefeeJson;
    for(const auto&pf:pos.GetPromoteFees()){
        json pfjson;
        to_json(pfjson,pf);
        promotefeeJson.push_back(pfjson);
    }
    j["Promote Fees"] = promotefeeJson;

}
std::map<wxDateTime, double> Position::GetROCMapConstant()const{
    return m_returnOfCapitalMap;
}
void from_json(const json&j, Position &position, Portfolio &port){//also deserialize the net income/promote fees from here
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
    if(j.contains("Movements Deploy")) {
        for(const auto& entry : j["Movements Deploy"]) {
            if(entry.size() == 2) { 
                wxDateTime date;
                std::string dateStr = entry[0];
                date.ParseISODate(dateStr);
                double amountMoved = entry[1];
                std::pair<wxDateTime, double> movement = std::make_pair(date, amountMoved);
                position.AddMovementDeploy(movement); 
            }
        }
    }
    if(j.contains("ROC Movements") && j["ROC Movements"].is_array()){
        std::vector<std::pair<wxDateTime, double>> movements;
        for(const auto& item : j["ROC Movements"]){
            for(auto&[dateStr, amount] : item.items()){
                wxDateTime date;
                date.ParseISODate(dateStr);
                double amountMoved = amount.get<double>(); 
                std::pair<wxDateTime, double> movement = std::make_pair(date, amountMoved);
                position.AddRocMovement(movement);
            }
        }
    }
    if (j.contains("Management Fees") && j["Management Fees"].is_array()) {
        std::vector<ManagementFee> fees;
        for (const auto& feeJson : j["Management Fees"]) {
            ManagementFee fee;
            from_json(feeJson, fee); 
            fees.push_back(fee); 
        }
        position.SetMgmtFeeVector(fees);
    }
    if(j.contains("Net Income") && j["Net Income"].is_array()){
        std::vector<Distribution> netIncomeVector;
        for(const auto& niJson:j["Net Income"]){
            Distribution netIncome;
            from_json(niJson,netIncome);
            netIncomeVector.push_back(netIncome);
        }
        position.SetNetIncome(netIncomeVector);
    }
    if(j.contains("Promote Fees") && j["Promote Fees"].is_array()){
        std::vector<PromoteFee> promoteFees;
        for(const auto& pfjson:j["Promote Fees"]){
            PromoteFee promoteFee;
            from_json(pfjson, promoteFee);
            promoteFees.push_back(promoteFee);
        }
        position.SetPromoteFees(promoteFees);
    }
}

void Position::SetMgmtFeeVector(std::vector<ManagementFee> fees){
    m_managementFees = fees;
}

double Position::CalculateCapitalDays(const Asset &asset, const wxDateTime &qEndDate) {
    wxDateTime startDate = utilities::GetQuarterStartDate(qEndDate);
    wxDateTime assetStartDate = asset.GetMovementsToFromDeploy().begin()->first;
    if (assetStartDate > startDate) {
        startDate = assetStartDate;
    }
    double positionsDeployed = this->GetInitialDeployedBefore(startDate);
    double capitalizedDays = 0.0;
    wxDateTime lastDate = startDate; 
    // Assume GetInitialDeployedBefore() calculates total deployed before the quarter
    double initialDeployed = this->GetInitialDeployedBefore(startDate);

    std::map<wxDateTime, double> segmentMap;
    segmentMap[startDate] = initialDeployed; // Starting capital for the quarter

    // Add movements to the segment map, adjusting deployed capital
    double currentDeployed = initialDeployed;
    for (const auto& movement : this->GetMovementsDeploy()) {
        if (movement.first >= startDate && movement.first <= qEndDate) {
            currentDeployed += movement.second;
            segmentMap[movement.first] = currentDeployed;
        }
    }

    // Calculate capital days for each segment

    for (const auto& [date, deployed] : segmentMap) {
        int days = (date - lastDate).GetDays();
        capitalizedDays += days * segmentMap[lastDate]; // Use the previous segment's deployed capital
        lastDate = date;
    }

    // Add the days from the last movement to the end of the quarter
    int finalDays = (qEndDate - lastDate).GetDays() + 1; // Include the end date
    capitalizedDays += finalDays * segmentMap[lastDate];

    return capitalizedDays;
}

double Position::GetInitialDeployedBefore(const wxDateTime &date) {
    double deployed = 0;
    for (const auto &movement : this->GetMovementsDeploy()) {
        if (movement.first < date) {
            deployed += movement.second;
        }
    }
    return deployed;
}

void Position::AddNetIncome(Distribution &distribution){
    m_netIncome.push_back(distribution);
}

void Position::AddPromoteFee(PromoteFee &pf){
    m_promoteFees.push_back(pf);
}

void Position::SetPromoteFees(std::vector<PromoteFee> pfs){
    m_promoteFees = pfs;
}

void Position::SetNetIncome(std::vector<Distribution> netIncome){
    m_netIncome = netIncome;
}