#include "baseclasses/position.hpp"

void to_json(json&j, const Position &pos){
    j=json{
        {"AssetName", pos.assetPtr ? pos.assetPtr->assetName.ToStdString() : "Unknown"},//check to see if we are attempting to deref a nullPtr
        {"InvestorName",pos.investorPtr? pos.investorPtr->clientName.ToStdString():"NoName"},
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
    wxString investorName = wxString::FromUTF8(j["InvestorName"].get<std::string>().c_str());
    for(const auto& invPtr: porf.allInvestorPtrs){
        if(invPtr->clientName == investorName){
            pos.investorPtr = invPtr;
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
    std::pair<wxDateTime, wxDateTime> qdates = utilities::GetCurrentQuarterDates(date);
    double endingDeployedCapital = position.deployed;
    double totalMovedToDeploy=0, totalMovedFromDeploy = 0;

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

    if(position.dateInvested > qdates.first && position.dateInvested <qdates.second){//checking if we are in the first Q that the position was entered if it is our starting date is == to the date invested else
        segmentStartDate = position.dateInvested;
    }else{
        segmentStartDate = qdates.first;    
    }

    double totalFee = 0;
    for(const auto&movement:quarterMovements){
        if(movement.first!=segmentStartDate){
            segmentEndDate = movement.first;
            double daysInSegment = utilities::CalculateDaysBetween(segmentStartDate, segmentEndDate);
            double feeForSegment = deployedCapital * managementFeePercentage *(daysInSegment/365);
            totalFee+=feeForSegment;
            segmentStartDate=segmentEndDate;
            deployedCapital+=movement.second;
        }else{
            deployedCapital+=movement.second;
        }
    }

    if(qdates.second>segmentStartDate){
        double daysInFinalSegment = utilities::CalculateDaysBetween(segmentStartDate, qdates.second);
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
    std::pair<wxDateTime, wxDateTime> startingQDates = utilities::GetCurrentQuarterDates(dateInvested);
    wxDateTime startingQDate = startingQDates.first;
    wxDateTime endingQDate;

    while(startingQDate < currentDate){
        std::pair<wxDateTime, wxDateTime> qDates = utilities::GetCurrentQuarterDates(startingQDate);
        endingQDate = qDates.second;
        if(this->dateInvested > startingQDate && this->dateInvested < endingQDate){
            ManagementFee feeForQuarter = CalculatePositionManagementFees(*this, managementFeePercentage,dateInvested);//might need a separate function to handle this case
            startingQDate = utilities::GetNextQuarterStartDate(endingQDate);
            managementFees.push_back(feeForQuarter);
        }else{
            ManagementFee feeForQuarter = CalculatePositionManagementFees(*this, managementFeePercentage,startingQDate);
            startingQDate = utilities::GetNextQuarterStartDate(endingQDate);
            managementFees.push_back(feeForQuarter);
        }
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

    wxDateTime lastDistributionQuarterEnd = wxDateTime::Today();
    lastDistributionQuarterEnd.SetYear(1900); 
    double unpaidMgmtFees = 0.0;
    bool mgmtFeesCoveredThisQ = false;

    for (const auto& distribution : distributions) {
        if(!distribution.distribution.first.IsEarlierThan(dateInvested)){
            std::pair<wxDateTime, wxDateTime> currentQuarter = utilities::GetCurrentQuarterDates(distribution.distribution.first);
            wxDateTime quarterStartDate = currentQuarter.first;
            wxDateTime quarterEndDate = currentQuarter.second;

            if (quarterEndDate != lastDistributionQuarterEnd) {
                mgmtFeesCoveredThisQ = false;
                mgmtFeesDue = unpaidMgmtFees;
                unpaidMgmtFees = 0.0; 
                for (const auto& fee : managementFees) {
                    if (fee.managementFeesAsset.first > lastDistributionQuarterEnd && fee.managementFeesAsset.first <= quarterEndDate) {
                        mgmtFeesDue += fee.managementFeesAsset.second;
                    }
                }
                lastDistributionQuarterEnd = quarterEndDate;
            }
            double percentOwnershipInLoop = 0.0;
            double totalPaidInLoop = 0.0;
            for(const auto& pos:assetPtr->positions){
                if(pos->dateInvested.IsEarlierThan(distribution.distribution.first)){
                    totalPaidInLoop+= pos->paid;
                }
            }
            percentOwnershipInLoop = paid / totalPaidInLoop;

            double proportionalShare = distribution.distribution.second * percentOwnershipInLoop;
            double totalFeesDue = mgmtFeesDue;

            // Check if distribution covers the management fees
            if (proportionalShare < totalFeesDue && !mgmtFeesCoveredThisQ) {
                unpaidMgmtFees = totalFeesDue - proportionalShare; 
                proportionalShare = 0.0; 
            } else {
                if(!mgmtFeesCoveredThisQ){
                    unpaidMgmtFees = 0.0; 
                    proportionalShare -= totalFeesDue; 
                    mgmtFeesCoveredThisQ = true;
                }
            }

            double promoteFee = 0.0;
            double netIncomeAmount = 0.0;

            if (proportionalShare > 0) {
                promoteFee = proportionalShare * promoteFeePercentage;
                netIncomeAmount = proportionalShare - promoteFee;
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
            lastDistributionQuarterEnd = quarterEndDate;
        }

    }
    wxDateTime currentQuarterEnd = utilities::GetCurrentQuarterDates(wxDateTime::Today()).second;
    if (!distributions.empty() && lastDistributionQuarterEnd < currentQuarterEnd) {
        wxDateTime nextQuarterStartDate = utilities::GetNextQuarterStartDate(lastDistributionQuarterEnd);
        for (const auto& fee : managementFees) {
            if (fee.managementFeesAsset.first >= nextQuarterStartDate && fee.managementFeesAsset.first <= currentQuarterEnd) {
                unpaidMgmtFees += fee.managementFeesAsset.second;
            }
        }
    }else if(distributions.empty()){
        for(auto &fee: managementFees){
            unpaidMgmtFees += fee.managementFeesAsset.second;
        }
    }
    mgmtFeesDue = unpaidMgmtFees;
}

