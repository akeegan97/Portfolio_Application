#include "baseclasses/investor.hpp"
#include "baseclasses/position.hpp"
#include "baseclasses/portfolio.hpp"

void to_json(json &j, const Investor &inv) {
    std::cout << "Serializing Investor: " << inv.clientName.ToStdString() << ", Positions Count: " << inv.positions.size() << std::endl;

    j = {
        {"Client Name", inv.clientName.ToStdString()},
        {"Type", inv.type.ToStdString()},
        {"Management Fee Percentage", inv.managementFeePercentage},
        {"Promote Fee Percentage",inv.promoteFeePercentage},
        {"Positions", json::array()}
    };

    for(const auto &pos: inv.positions){
        json posJson;
        to_json(posJson, *pos);
        j["Positions"].push_back(posJson);
    }
}

void from_json(const json &j, Investor &inv, Portfolio &porf) {
    inv.clientName =  wxString::FromUTF8(j["Client Name"].get<std::string>());
    inv.type = wxString::FromUTF8(j["Type"].get<std::string>());
    inv.managementFeePercentage = j["Management Fee Percentage"].get<double>();
    inv.promoteFeePercentage = j["Promote Fee Percentage"].get<double>();   
}

wxVariant Investor::GetValue(int col)const{
    switch(col){
        case 0: return wxVariant(clientName);break;
        case 1: return wxVariant(type);break;
        case 2: return wxVariant(managementFeePercentage);break;
        case 3: return wxVariant(promoteFeePercentage);break;
        default: return wxVariant();break;
    }
}

void Investor::SetValue(int col, const wxVariant &v){
    switch(col){
        case 0: clientName = v.GetString();break;
        case 1: type = v.GetString();break;
        case 3: managementFeePercentage = v.GetDouble();break;
        case 4: promoteFeePercentage = v.GetDouble();break;
    }
}

std::vector<wxString> Investor::columnNames = {"Name", "Type","Management Fee", "Promote Fee"};
std::vector<int> Investor::columnWidths = {100,100,150,150};



void Investor::PopulateDeploymentsForPlotting(){

}

void Investor::PopulateDistributionsForPlotting(){

}

void Investor::PopulateValuationsForPlotting(){
    valuationsForPlotting.clear();
    std::vector<wxString> uniqueAssets;
    std::map<wxString, std::map<wxDateTime, double>> lastValuationofAsset;

}