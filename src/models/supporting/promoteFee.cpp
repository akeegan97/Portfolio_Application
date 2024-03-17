#include "models/supporting/promoteFee.hpp"
#include "helpers/utilities.hpp"

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

wxVariant PromoteFee::GetValue(int col)const{
    switch(col){
        case 0: return wxVariant(promotefee.first.FormatISODate());break;
        case 1: return wxVariant(utilities::formatDollarAmount(promotefee.second));break;
        default: return wxVariant();break;
    }
}

std::vector<wxString> PromoteFee::columnNames = {"Promote Fee Date","Promote"};
std::vector<int> PromoteFee::columnWidths = {150,100};