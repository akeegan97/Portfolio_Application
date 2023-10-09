#include "asset.hpp"


void to_json(json &j, const Asset &as) {
    j = {
        {"Asset Name", as.assetName.ToStdString()},
        {"Asset Exit Date", as.assetExitDate.FormatISODate()},
        {"Valuations", as.valuations}
    };
}

void from_json(const json &j, Asset &as) {
    as.assetName = j["Asset Name"].get<std::string>().c_str();
    wxString dateStr = wxString::FromUTF8(j["Asset Exit Date"].get<std::string>().c_str());
    wxDateTime dateParse;
    dateParse.ParseDate(dateStr);
    as.assetExitDate = dateParse;
    as.valuations = j["Valuations"].get<std::vector<Valuation>>();
}