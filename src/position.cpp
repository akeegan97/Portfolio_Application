#include "position.hpp"
#include "portfolio.hpp"

void to_json(json&j, const Position &pos){
    j=json{
        {"AssetName", pos.assetPtr ? pos.assetPtr->assetName.ToStdString() : "Unknown"},//check to see if we are attempting to deref a nullPtr
        {"Date Invested",pos.dateInvested.FormatISODate()},
        {"Committed Up",pos.committedAmountUp},
        {"Committed Down",pos.committedAmountDown},
        {"Called Up",pos.calledCapitalUp},
        {"Called Down",pos.calledCapitalDown},
        {"Invested Up",pos.investedAmountUp},
        {"Invested Down",pos.investedAmountDown},
        {"Waiting Up",pos.waitingDeployUp},
        {"Waiting Down",pos.waitingDeployDown}
    };
}

void from_json(const json &j, Position &pos, Portfolio &porf){
    wxString assetName = wxString::FromUTF8(j["AssetName"].get<std::string>().c_str());
    std::cout << "Looking for Asset: " << assetName << " (length: " << assetName.length() << ")" << std::endl;
    std::cout << "Portfolio's Assets:" << std::endl;
    for (const auto& assetPtr : porf.assetPtrs) {
        std::cout << " - " << assetPtr->assetName << " (length: " << assetPtr->assetName.length() << ")" << std::endl;
    }
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
    pos.committedAmountUp = j["Committed Up"].get<double>();
    pos.committedAmountDown = j["Committed Down"].get<double>();
    pos.calledCapitalUp = j["Called Up"].get<double>();
    pos.calledCapitalDown = j["Called Down"].get<double>();
    pos.investedAmountUp = j["Invested Up"].get<double>();
    pos.investedAmountDown = j["Invested Down"].get<double>();
    pos.waitingDeployUp = j["Waiting Up"].get<double>();
    pos.waitingDeployDown = j["Waiting Down"].get<double>();
}