#include "managementFee.hpp"


void to_json(json&j,const ManagementFee &fee){
    std::string date = fee.managementFeesAsset.first.FormatISODate().ToStdString();
    j=json{{"Management Fee Date",date},{"Management Fee Amount", fee.managementFeesAsset.second}};
}

void from_json(const json&j, ManagementFee &fee){
    std::string dateStr;
    double amount;
    if(j.find("Management Fee Date")!=j.end() && j.find("Management Fee Amount")!=j.end()){
        dateStr = j["Management Fee Date"].get<std::string>();
        amount = j["Management Fee Amount"].get<double>();
        wxDateTime date;
        date.ParseISODate(dateStr);
        fee.managementFeesAsset = std::make_pair(date, amount);
    }
}
