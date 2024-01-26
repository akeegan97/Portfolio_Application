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

wxVariant PromoteFee::GetValue(int col)const{
    switch(col){
        case 0: return wxVariant(promotefee.first);break;
        case 1: return wxVariant(promotefee.second);break;
        default: return wxVariant();break;
    }
}

void PromoteFee::SetValue(int col, const wxVariant &v){
    switch(col){
        case 0: promotefee.first = v.GetDateTime();break;
        case 1: promotefee.second = v.GetDouble();break;
    }
}

std::vector<wxString> PromoteFee::columnNames = {"Promote Date","Promote Amount"};
std::vector<int> PromoteFee::columnWidths = {150,150};