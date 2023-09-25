#include "assetEvent.hpp"

wxVariant AssetEvent::GetValue(int col) const{
    switch(col){
        case 0: return wxVariant(eventDate);
        case 1: return wxVariant(notes);
        case 2: return wxVariant(type);
        default: return wxVariant();
    }
}

void AssetEvent::SetValue(int col, const wxVariant &v){
    switch(col){
        case 0: eventDate = v.GetDateTime();break;
        case 1: notes = v.GetString();break;
        case 2: type = v.GetString();break;
    }
}

void to_json(json&j, const AssetEvent &ae){
    j=json{
        {"Event Date", ae.eventDate.FormatISODate()},
        {"Notes",ae.notes},
        {"Type",ae.type}
    };
}

void from_json(const json&j, AssetEvent &ae){
    wxString dateStr = wxString::FromUTF8(j["Event Date"].get<std::string>().c_str());
    wxDateTime dateParse;
    dateParse.ParseDate(dateStr);
    ae.eventDate = dateParse;
    ae.notes = j["Notes"].get<std::string>().c_str();
    ae.type = j["Type"].get<std::string>().c_str();
}