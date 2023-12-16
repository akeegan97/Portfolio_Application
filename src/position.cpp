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
        {"Ownership", pos.percentOwnership}
    };

    json feesJson = json::array();
    for(const auto& fee: pos.managementFees){
        json feeJson;
        to_json(fee, feeJson);
        feesJson.push_back(feeJson);
    }

    j["Fees"] = feesJson;

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

    if(j.find("Fees")!=j.end()){
        for(const auto&feeJson : j["Fees"]){
            ManagementFee fee;
            from_json(feeJson, fee);
            pos.managementFees.push_back(fee);
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
            for(const auto &position : investor.positions){
                if(position.assetPtr == assetPtr){
                    totalDeployed+=position.deployed;
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

ManagementFee Position::CalculatePositionManagementFees(const Position&position, const double &managementFeePercentage){
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
    wxDateTime feeDate = fee.managementFeesAsset.first;
    
    auto it = std::find_if(managementFees.begin(), managementFees.end(),
                           [&feeDate](const ManagementFee& existingFee) {
                               return existingFee.managementFeesAsset.first == feeDate;
                           });

    if (it != managementFees.end()) {
        *it = fee;
    } else {
        managementFees.push_back(fee);
    }
}