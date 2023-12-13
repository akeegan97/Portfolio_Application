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
    for(const auto& fee: pos.fees){
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
            Fee fee;
            from_json(feeJson, fee);
            pos.fees.push_back(fee);
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