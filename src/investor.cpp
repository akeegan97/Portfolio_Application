#include "investor.hpp"

void to_json(json&j, const Investor &inv){
    j=json{
        {"Client Name", inv.clientName.ToStdString()},
        {"Type", inv.type.ToStdString()}
    };
}

void from_json(const json&j, Investor &inv){
    inv.clientName = j["Client Name"].get<std::string>();
    inv.type = j["Type"].get<std::string>();
}
