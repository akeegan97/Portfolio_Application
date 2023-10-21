#include "asset.hpp"


void to_json(json &j, const Asset &as) {
    j = {
        {"Asset Name", as.assetName.ToStdString()},
        {"Asset Exit Date", as.assetExitDate.FormatISODate()},
        {"Valuations", as.valuations},
        {"Investors",as.investors},
        {"Positions",as.positions}
    };
}

void from_json(const json &j, Asset &as) {
    as.assetName = j["Asset Name"].get<std::string>().c_str();
    wxString dateStr = wxString::FromUTF8(j["Asset Exit Date"].get<std::string>().c_str());
    wxDateTime dateParse;
    dateParse.ParseDate(dateStr);
    as.assetExitDate = dateParse;
    as.valuations = j["Valuations"].get<std::vector<Valuation>>();
    as.investors = j["Investors"].get<std::vector<Investor>>();
    as.positions = j["Positions"].get<std::vector<Position>>();
}

std::vector<wxString> Asset::columnNames = {"Asset Name","Exit Date","Total Invested Capital","Number of Investors","Current Value"};
std::vector<int> Asset::columnWidths = {150,75,100,100,100};

wxVariant Asset::GetValue(int col)const{
    switch(col){
        case 0: return wxVariant(assetName);
        case 1: return wxVariant(assetExitDate);
        case 2: return wxVariant(totalInvestedCapital);
        case 3: return wxVariant(countOfInvestors);
        case 4: return wxVariant(currentValue);
        default: return wxVariant();
    }
}

void Asset::SetValue(int col, const wxVariant &v){
    switch(col){
        case 0: assetName = v.GetString();break;
        case 1: assetExitDate = v.GetDateTime();break;
        case 2: totalInvestedCapital = v.GetDouble();break;
        case 3: countOfInvestors = v.GetDouble();break;
        case 4: currentValue = v.GetDouble();break;
    }
}