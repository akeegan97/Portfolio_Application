#include "baseclasses/promoteFee.hpp"

void to_json(json&j,const PromoteFee &promoteFee){
    std::string date = promoteFee.promotefee.first.FormatISODate().ToStdString();
    j=json{{"Promote Fee Date",date},{"Promote Fee Amount", promoteFee.promotefee.second}};
}

void from_json(const json&j,PromoteFee &promoteFee){
    std::string dateStr;
    double amount;
    if(j.find("Promote Fee Date")!=j.end() && j.find("Promote Fee Amount")!=j.end()){
        dateStr = j["Promote Fee Date"].get<std::string>();
        amount = j["Promote Fee Amount"].get<double>();

        wxDateTime date;
        date.ParseISODate(dateStr);
        promoteFee.promotefee = std::make_pair(date, amount);
    }
}