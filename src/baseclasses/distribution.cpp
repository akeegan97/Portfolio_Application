#include "baseclasses/distribution.hpp"


void to_json(json &j,const Distribution &distribution){
    std::string date = distribution.distribution.first.FormatISODate().ToStdString();
    j=json{{"Distribution Date",date},{"Distribution Amount", distribution.distribution.second}};
}
void from_json(const json &j, Distribution &distribution){
    std::string dateStr;
    double amount;

    if(j.find("Distribution Date")!=j.end() && j.find("Distribution Amount")!=j.end()){
        dateStr = j["Distribution Date"].get<std::string>();
        amount = j["Distribution Amount"].get<double>();
        wxDateTime date;
        date.ParseISODate(dateStr);
        distribution.distribution = std::make_pair(date, amount);
    }
}

//methods and members for VLC

wxVariant Distribution::GetValue(int col)const{
    switch(col){
        case 0: return wxVariant(distribution.first);break;
        case 1: return wxVariant(distribution.second);break;
        default: return wxVariant();break;
    }
}

void Distribution::SetValue(int col, const wxVariant &v){
    switch(col){
        case 0: distribution.first = v.GetDateTime();break;
        case 1: distribution.second = v.GetDouble();break;
    }
}


std::vector<wxString> Distribution::columnNames = {"Distribution Date","Distribution Amount"};
std::vector<int> Distribution::columnWidths = {100,100};