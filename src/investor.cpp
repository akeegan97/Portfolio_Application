#include "investor.hpp"



void to_json(json &j, const Investor &inv) {
    j = {
        {"Client Name", inv.clientName.ToStdString()},
        {"Type", inv.type.ToStdString()},
        {"Assets", {}}
    };
    for (const auto &pair : inv.assets) {
        const Asset &asset = pair.first;
        const AssetEntry &entry = pair.second;
        j["Assets"][asset.assetName.ToStdString()] = {
            {"Invested Date", entry.first.FormatISODate()},
            {"Position", entry.second}
        };
    }
}

void from_json(const json &j, Investor &inv) {
    inv.clientName = j["Client Name"].get<std::string>();
    inv.type = j["Type"].get<std::string>();
    for (const auto &[assetName, assetData] : j["Assets"].items()) {
        Asset asset;
        asset.assetName = assetName;
        
        wxDateTime dateParse;
        wxString dateStr = wxString::FromUTF8(assetData["Invested Date"].get<std::string>().c_str());
        dateParse.ParseDate(dateStr);
        
        Position pos = assetData["Position"].get<Position>();
        
        inv.assets[asset] = {dateParse, pos};
    }
}
