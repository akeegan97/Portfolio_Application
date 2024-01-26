#include "baseclasses/managementFee.hpp"


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

wxVariant ManagementFee::GetValue(int col)const{
    switch(col){
        case 0: return wxVariant(managementFeesAsset.first);break;
        case 1: return wxVariant(managementFeesAsset.second);break;
        default: return wxVariant();break;
    }
}

void ManagementFee::SetValue(int col, const wxVariant &v){
    switch(col){
        case 0: managementFeesAsset.first = v.GetDateTime();break;
        case 1: managementFeesAsset.second = v.GetDouble();break;
    }
}

std::vector<wxString> ManagementFee::columnNames = {"Mgmt Fee Date","Mgmt Fee Amount"};
std::vector<int> ManagementFee::columnWidths = {150,150};