#include "models/supporting/distribution.hpp"
#include "helpers/utilities.hpp"


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
        case 0: return wxVariant(distribution.first.FormatISODate());break;
        case 1: return wxVariant(utilities::formatDollarAmount(distribution.second));break;
        default: return wxVariant();break;
    }
}



std::vector<wxString> Distribution::columnNames = {"Distribution Date","Amount"};
std::vector<int> Distribution::columnWidths = {150,100};

