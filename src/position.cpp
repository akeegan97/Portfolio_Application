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

ManagementFee Position::CalculatePositionManagementFees(Position&position, const double &managementFeePercentage){
    ManagementFee feeThisQuarter;
    wxDateTime now = wxDateTime::Today();
    std::pair<wxDateTime, wxDateTime> qdates = GetCurrentQuarterDates(now);
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

double Position::calculateDaysBetween(const wxDateTime &start, const wxDateTime &end){
    if(end.IsEarlierThan(start)){
        return 0;
    }else{
        wxTimeSpan span = end - start;
        return span.GetDays();
    }
}

void Position::PushFeeToVector(const ManagementFee& fee) {
    std::cout << "PushFeeToVector called with fee: " << fee.managementFeesAsset.second << std::endl;

    wxDateTime feeDate = fee.managementFeesAsset.first;
    std::cout<<"FeeDATE: "<<feeDate.FormatISODate().ToStdString()<<std::endl;
    std::cout<<"Number of Fees in Management Fee Vector: "<<managementFees.size()<<std::endl;
    auto it = std::find_if(managementFees.begin(), managementFees.end(),
                           [&feeDate](const ManagementFee& existingFee) {
                               return existingFee.managementFeesAsset.first == feeDate;
                           });

    if (it != managementFees.end()) {
        std::cout << "Existing fee found for date " << feeDate.FormatISODate().ToStdString() << std::endl;
        std::cout << "Existing fee amount: " << it->managementFeesAsset.second << std::endl;
        std::cout<<"fee.managementFeesAsset: "<<fee.managementFeesAsset.second<<std::endl;
        if (it->managementFeesAsset.second != fee.managementFeesAsset.second) {
            std::cout << "Fee amount has changed. Adjusting mgmtFeesDue." << std::endl;
            mgmtFeesDue -= it->managementFeesAsset.second;
            mgmtFeesDue += fee.managementFeesAsset.second;
        }
        *it = fee; // Replace the existing fee
    } else {
        std::cout << "No existing fee found for date. Adding new fee." << std::endl;
        managementFees.push_back(fee); // Add the new fee
        mgmtFeesDue += fee.managementFeesAsset.second;
    }

    std::cout << "Updated mgmtFeesDue: " << mgmtFeesDue << std::endl;
}

void Position::CalculatePositionNetIncome(const Distribution &distribution, const double promoteFeePercentage){
    wxDateTime now = wxDateTime::Today();
    std::pair<wxDateTime, wxDateTime> QDates =  GetCurrentQuarterDates(now);

    if(distribution.distribution.first>=QDates.first && distribution.distribution.first <= QDates.second){
        double proportionalShare = distribution.distribution.second * percentOwnership;

        double amountAfterMgmtFees = proportionalShare - mgmtFeesDue;
        if(amountAfterMgmtFees<=0){
            mgmtFeesDue = - amountAfterMgmtFees;
            return;
        }
        double promoteFee = amountAfterMgmtFees * promoteFeePercentage;
        PromoteFee promoteFeeEntry;
        promoteFeeEntry.promotefee = std::make_pair(distribution.distribution.first, promoteFee);
        promoteFees.push_back(promoteFeeEntry);

        double netIncomeAmount = amountAfterMgmtFees - promoteFee;

        Distribution netIncomeDistribution;
        netIncomeDistribution.distribution = std::make_pair(distribution.distribution.first, netIncomeAmount);
        netIncome.push_back(netIncomeDistribution);
    }

}
