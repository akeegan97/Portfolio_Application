#include "fee.hpp"


void to_json(const Fee &fee, json&j){
    json managementFeeJson;
    for(const auto&pair:fee.managementFeesAsset){
        std::string date;
        date = pair.first.FormatISODate().ToStdString();
        managementFeeJson[date] = pair.second;
    }
    json promoteFeeJson;
    for(const auto&pair:fee.promoteFees){
        std::string date;
        date = pair.first.FormatISODate().ToStdString();
        promoteFeeJson[date] = pair.second;
    }

    j=json{{"Management Fee", managementFeeJson},{"Promote Fee", promoteFeeJson}};
}

void from_json(const json&j, Fee &fee){
    if(j.find("Management Fee")!=j.end()){
        const auto& managementFeeJson = j.at("Management Fee");
        for(const auto&pair:managementFeeJson.items()){
            wxDateTime date;
            date.ParseISODate(pair.key());
            fee.managementFeesAsset[date] = pair.value().get<double>();
        }
    }
    if(j.find("Promote Fee")!= j.end()){
        const auto&promoteFeeJson = j.at("Promote Fee");
        for(const auto&pair:promoteFeeJson.items()){
            wxDateTime date;
            date.ParseISODate(pair.key());
            fee.promoteFees[date] = pair.value().get<double>();
        }
    }
}
