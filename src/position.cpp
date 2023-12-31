#include "position.hpp"

void to_json(json&j, const Position &pos){
    j=json{
        {"AssetName", pos.assetPtr ? pos.assetPtr->assetName.ToStdString() : "Unknown"},//check to see if we are attempting to deref a nullPtr
        {"Date Invested",pos.dateInvested.FormatISODate()},
        {"Subscribed", pos.subscribed},
        {"Paid", pos.paid},
        {"Reserve", pos.reserve},
        {"Deployed",pos.deployed},
        {"ROC", pos.returnOfCapital},
        {"Ownership", pos.percentOwnership},
        {"Management Fees Due", pos.mgmtFeesDue},
        {"Promote Fees",json::array({})},
        {"Management Fees",json::array({})}
    };
    for(const auto&mgmtFee : pos.managementFees){
        json mgmtFeeJson;
        to_json(mgmtFeeJson, mgmtFee);
        j["Management Fees"].push_back(mgmtFeeJson);
    }
    for(const auto&promoteFee : pos.promoteFees){
        json promoteJson;
        to_json(promoteJson, promoteFee);
        j["Promote Fees"].push_back(promoteJson);
    }
    for(const auto&distribution : pos.netIncome){
        json netIncomeJson;
        to_json(netIncomeJson, distribution);
        j["Net Income"].push_back(netIncomeJson);
    }
    json movedToDeployJson;
    for(const auto&[date, amount]:pos.movedToDeploy){
        movedToDeployJson[date.FormatISODate().ToStdString()] = amount;
    }
    j["MovedToDeploy"] = movedToDeployJson;

    json movedOutDeployJson;
    for(const auto&[date, amount]:pos.movedOutOfDeployed){
        movedOutDeployJson[date.FormatISODate().ToStdString()]=amount;
    }
    j["MovedOutDeploy"] = movedOutDeployJson;

}

void from_json(const json &j, Position &pos, Portfolio &porf){
    wxString assetName = wxString::FromUTF8(j["AssetName"].get<std::string>().c_str());
    for(const auto &assetPtr: porf.assetPtrs){
        if(assetPtr->assetName == assetName){
            pos.assetPtr = assetPtr;
            break;
        }
    }
    wxString dateStr = wxString::FromUTF8(j["Date Invested"].get<std::string>().c_str());
    wxDateTime dateParse;
    dateParse.ParseDate(dateStr);
    pos.dateInvested = dateParse;
    pos.subscribed = j["Subscribed"].get<double>();
    pos.paid = j["Paid"].get<double>();
    pos.reserve = j["Reserve"].get<double>();
    pos.deployed = j["Deployed"].get<double>();///
    pos.returnOfCapital = j["ROC"].get<double>();
    pos.percentOwnership = j["Ownership"].get<double>();
    pos.mgmtFeesDue = j["Management Fees Due"].get<double>();

    if(j.find("Management Fees")!=j.end()){
        for(const auto&feeJson : j["Management Fees"]){
            ManagementFee fee;
            from_json(feeJson, fee);
            pos.managementFees.push_back(fee);
        }
    }
    if(j.find("Promote Fees")!=j.end()){
        for(const auto&promoteFeeJson : j["Promote Fees"]){
            PromoteFee pFee;
            from_json(promoteFeeJson, pFee);
            pos.promoteFees.push_back(pFee);
        }
    }
    if(j.find("Net Income")!=j.end()){
        for(const auto &netIncomeJson : j["Net Income"]){
            Distribution netIncome;
            from_json(netIncomeJson, netIncome);
            pos.netIncome.push_back(netIncome);
        }
    }
    if(j.contains("MovedToDeploy")){
        for(const auto&[dateStr, amount]: j["MovedToDeploy"].items()){
            wxDateTime date;
            date.ParseISODate(dateStr);
            pos.movedToDeploy[date] = amount.get<double>();
        }
    }
    if(j.contains("MovedOutDeploy")){
        for(const auto&[dateStr, amount]: j["MovedOutDeploy"].items()){
            wxDateTime date;
            date.ParseISODate(dateStr);
            pos.movedOutOfDeployed[date] = amount.get<double>();
        }
    }
}

void Position::calculateOwnership(Portfolio &portfolio){
    double totalDeployed = 0;
    for(const auto& assetPointer : portfolio.assetPtrs){
        for(const auto &investor: assetPointer->investors){
            for(const auto &position : investor->positions){
                if(position->assetPtr == assetPtr){
                    totalDeployed+=position->deployed;
                }
            }
        }
    }
    percentOwnership = (deployed/totalDeployed);
}

std::pair<wxDateTime, wxDateTime> Position::GetCurrentQuarterDates(const wxDateTime &currentDate){
    int year = currentDate.GetYear();
    wxDateTime quarterStart, quarterEnd;

    if (currentDate >= wxDateTime(1, wxDateTime::Jan, year) && currentDate < wxDateTime(1, wxDateTime::Apr, year)) {
        // Q1
        quarterStart = wxDateTime(1, wxDateTime::Jan, year);
        quarterEnd = wxDateTime(31, wxDateTime::Mar, year);
    } else if (currentDate >= wxDateTime(1, wxDateTime::Apr, year) && currentDate < wxDateTime(1, wxDateTime::Jul, year)) {
        // Q2
        quarterStart = wxDateTime(1, wxDateTime::Apr, year);
        quarterEnd = wxDateTime(30, wxDateTime::Jun, year);
    } else if (currentDate >= wxDateTime(1, wxDateTime::Jul, year) && currentDate < wxDateTime(1, wxDateTime::Oct, year)) {
        // Q3
        quarterStart = wxDateTime(1, wxDateTime::Jul, year);
        quarterEnd = wxDateTime(30, wxDateTime::Sep, year);
    } else {
        // Q4
        quarterStart = wxDateTime(1, wxDateTime::Oct, year);
        quarterEnd = wxDateTime(31, wxDateTime::Dec, year);
    }

    return {quarterStart, quarterEnd};
}

void Position::ReCalculateTotalManagementFeesDue(wxDateTime distributionDate) {
    double feesDueUpToDistribution = 0.0;
    for (const auto& fee : managementFees) {
        if (fee.managementFeesAsset.first <= distributionDate) {
            feesDueUpToDistribution += fee.managementFeesAsset.second;
        }
    }
    mgmtFeesDue = feesDueUpToDistribution;
}

ManagementFee Position::CalculatePositionManagementFees(Position&position, const double &managementFeePercentage, wxDateTime &date){
    ManagementFee feeThisQuarter;
    std::pair<wxDateTime, wxDateTime> qdates = GetCurrentQuarterDates(date);
    double endingDeployedCapital = position.deployed;
    double totalMovedToDeploy, totalMovedFromDeploy = 0;

    for(const auto &pair:position.movedToDeploy){
        if(pair.first>=qdates.first && pair.first <= qdates.second){
            totalMovedToDeploy +=pair.second;
        }
    }

    for(const auto&pair:position.movedOutOfDeployed){
        if(pair.first>=qdates.first && pair.first <= qdates.second){
            totalMovedFromDeploy+=pair.second;
        }
    }

    double deployedCapital = endingDeployedCapital - totalMovedToDeploy + totalMovedFromDeploy;

    std::vector<std::pair<wxDateTime, double>> quarterMovements;
    for(const auto&pair:position.movedToDeploy){
        if(pair.first>=qdates.first && pair.first <= qdates.second){
            quarterMovements.push_back(pair);
        }
    }

    for(const auto&pair:position.movedOutOfDeployed){
        if(pair.first>=qdates.first && pair.first <= qdates.second){
            quarterMovements.push_back({pair.first, -pair.second});
        }
    }

    std::sort(quarterMovements.begin(), quarterMovements.end());

    wxDateTime segmentStartDate, segmentEndDate;
    segmentStartDate = qdates.first;
    double totalFee = 0;

    for(const auto&movement:quarterMovements){
        if(movement.first!=segmentStartDate){
            segmentEndDate = movement.first;
            double daysInSegment = calculateDaysBetween(segmentStartDate, segmentEndDate);
            double feeForSegment = deployedCapital * managementFeePercentage *(daysInSegment/365);
            totalFee+=feeForSegment;
            segmentStartDate=segmentEndDate;
            deployedCapital+=movement.second;
        }else{
            deployedCapital+=movement.second;
        }
    }

    if(qdates.second>segmentStartDate){
        double daysInFinalSegment = calculateDaysBetween(segmentStartDate, qdates.second);
        double finalSegmentFees = deployedCapital * managementFeePercentage * (daysInFinalSegment/365);
        totalFee+=finalSegmentFees;
    }

    feeThisQuarter.managementFeesAsset.first = qdates.second;
    feeThisQuarter.managementFeesAsset.second = totalFee;
    
    return feeThisQuarter;
}

void Position::CalculateHistoricalManagementFees(const double &managementFeePercentage){
    managementFees.clear();
    wxDateTime dateInvested = this->dateInvested;
    wxDateTime currentDate = wxDateTime::Today();
    std::pair<wxDateTime, wxDateTime> startingQDates = GetCurrentQuarterDates(dateInvested);
    wxDateTime startingQDate = startingQDates.first;
    wxDateTime endingQDate;

    while(startingQDate < currentDate){
        std::pair<wxDateTime, wxDateTime> qDates = GetCurrentQuarterDates(startingQDate);
        endingQDate = qDates.second;
        ManagementFee feeForQuarter = CalculatePositionManagementFees(*this, managementFeePercentage,startingQDate);
        startingQDate = GetNextQuarterStartDate(endingQDate);
        managementFees.push_back(feeForQuarter);
    }
    ReCalculateTotalManagementFeesDue(startingQDate);
}

void Position::UpdateFinancesPostDistributionChanges(std::vector<Distribution>& distributions, double& promoteFeePercentage, double& mgmtFeePercentage){
    netIncome.clear();
    promoteFees.clear();

    std::sort(distributions.begin(), distributions.end(), 
        [](const Distribution& a, const Distribution& b) {
            return a.distribution.first < b.distribution.first;
        }
    );

    // Set the initial last processed quarter to a time before any possible distributions
    wxDateTime lastQuarterProcessed = wxDateTime::Now();
    lastQuarterProcessed.SetYear(1900); // Example: Setting to a date way in the past
    double totalFeesDue = 0.0;

    for (const auto& distribution : distributions) {
        std::pair<wxDateTime, wxDateTime> currentQuarter = GetCurrentQuarterDates(distribution.distribution.first);
        wxDateTime quarterStartDate = currentQuarter.first;
        wxDateTime quarterEndDate = currentQuarter.second;

        // Check if we are in a new quarter and accumulate management fees
        if (quarterStartDate > lastQuarterProcessed) {
            totalFeesDue = 0.0; // Reset fees for a new quarter
            for (const auto& fee : managementFees) {
                if (fee.managementFeesAsset.first > lastQuarterProcessed && fee.managementFeesAsset.first <= quarterEndDate) {
                    totalFeesDue += fee.managementFeesAsset.second;
                }
            }
            lastQuarterProcessed = quarterEndDate;
        }

        double proportionalShare = distribution.distribution.second * percentOwnership;
        double amountAfterMgmtFees = proportionalShare - totalFeesDue;
        double promoteFee = 0;
        double netIncomeAmount = 0;

        if (amountAfterMgmtFees > 0) {
            promoteFee = amountAfterMgmtFees * promoteFeePercentage;
            netIncomeAmount = amountAfterMgmtFees - promoteFee;
        }

        if (netIncomeAmount > 0) {
            Distribution netIncomeDistribution;
            netIncomeDistribution.distribution = std::make_pair(distribution.distribution.first, netIncomeAmount);
            netIncome.push_back(netIncomeDistribution);
        }

        if (promoteFee > 0) {
            PromoteFee newPromoteFee;
            newPromoteFee.promotefee = std::make_pair(distribution.distribution.first, promoteFee);
            promoteFees.push_back(newPromoteFee);
        }

        // Debug prints
        std::cout << "Date of Distribution Pushed: " << distribution.distribution.first.FormatISODate() << std::endl;
        std::cout << "PROPORTIONAL SHARE OF GROSS DISTR: " << proportionalShare << std::endl;
        std::cout << "TOTAL MGMT FEES DUE: " << totalFeesDue << std::endl;
        std::cout << "NET INCOME AFTER MGMT FEES: " << netIncomeAmount << std::endl;
        std::cout << "PROMOTE FEE: " << promoteFee << std::endl;

        // Reset total fees due for the next distribution
        totalFeesDue = 0.0;
    }
}

double Position::calculateDaysBetween(const wxDateTime &start, const wxDateTime &end){

    if(end.IsEarlierThan(start)){
        return 0;
    }else{
        wxTimeSpan span = end - start;
        return span.GetDays();
    }
}

wxDateTime Position::GetNextQuarterStartDate(wxDateTime &date){
    wxDateTime nextQStartDate;
    int year =date.GetYear();
    if(date.GetMonth()<= wxDateTime::Mar){
        nextQStartDate = wxDateTime(1, wxDateTime::Apr, year);
    }else if(date.GetMonth()<=wxDateTime::Jun){
        nextQStartDate = wxDateTime(1, wxDateTime::Jul, year);
    }else if(date.GetMonth()<=wxDateTime::Sep){
        nextQStartDate = wxDateTime(1,wxDateTime::Oct, year);
    }else if(date.GetMonth()<=wxDateTime::Dec){
        nextQStartDate = wxDateTime(1, wxDateTime::Jan, year+1);
    }
    return nextQStartDate;
}

