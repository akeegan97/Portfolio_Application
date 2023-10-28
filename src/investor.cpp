#include "investor.hpp"
#include "position.hpp"
#include "portfolio.hpp"

void to_json(json &j, const Investor &inv) {
    j = {
        {"Client Name", inv.clientName.ToStdString()},
        {"Type", inv.type.ToStdString()},
        {"Positions", json::array()}
    };
    for(const auto &pos: inv.positions){
        json posJson;
        to_json(posJson, pos);
        j["Positions"].push_back(posJson);
    }
}

void from_json(const json &j, Investor &inv, Portfolio &porf) {
    inv.clientName =  wxString::FromUTF8(j["Client Name"].get<std::string>());
    inv.type = wxString::FromUTF8(j["Type"].get<std::string>());
    if(j.contains("Positions")&&j["Positions"].is_array()){
        for(const auto& posJson : j["Positions"]){
            Position pos;
            from_json(posJson, pos, porf);
            inv.positions.push_back(pos);
        }
    }
}
