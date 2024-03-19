#include "models/supporting/managementFee.hpp"
#include "helpers/utilities.hpp"


void to_json(json& j, const ManagementFee& fee) {
    j = json{
        {"Management Fee Date", fee.quarter.FormatISODate().ToStdString()},
        {"Management Fee Amount", fee.amount},
        {"Paid", fee.paid}
    };
}

void from_json(const json& j, ManagementFee& fee) {
    std::string dateStr;
    if (j.find("Management Fee Date") != j.end()) {
        dateStr = j["Management Fee Date"].get<std::string>();
        wxDateTime date;
        date.ParseISODate(dateStr);
        fee.quarter = date; 
    }
    if (j.find("Management Fee Amount") != j.end()) {
        fee.amount = j["Management Fee Amount"].get<double>();
    }

    if (j.find("Paid") != j.end()) {
        fee.paid = j["Paid"].get<bool>();
    }
}

wxVariant ManagementFee::GetValue(int col)const{
    switch(col){
        case 0: return wxVariant(quarter.FormatISODate());break;
        case 1: return wxVariant(utilities::formatDollarAmount(amount));break;
        default: return wxVariant();break;
    }
}


std::vector<wxString> ManagementFee::columnNames = {"Mgmt Fee Date","Amount"};
std::vector<int> ManagementFee::columnWidths = {125,125};