#include "asset_event.hpp"

wxVariant AssetEvent::GetValue(int col)const{
    switch(col){
        case 0: return wxVariant(eventDate);
        case 1: return wxVariant(hasHappened);
        case 2: return wxVariant(eventDetails);
        default: return wxVariant();
    }
}

void AssetEvent::SetValue(int col, const wxVariant &v){
    switch(col){
        case 0: eventDate = v.GetDateTime();break;
        case 1: hasHappened = v.GetBool();break;
        case 2: eventDetails = v.GetString();break;
    }
}

std::vector<wxString> AssetEvent::columnNames {"Event Date","Has Happened","Event Details"};
std::vector<int> AssetEvent::columnWidths{100,50,200};

void to_json(json&j, const AssetEvent &ae){
    j=json{
        {"Event Date",ae.eventDate.FormatISODate()},
        {"Event Details",ae.eventDetails.ToStdString()},
        {"Has Happened",ae.hasHappened}
    };
}
void from_json(const json&j, AssetEvent &ae){
    wxString dateStr = wxString::FromUTF8(j["Event Date"].get<std::string>().c_str());
    wxDateTime dateParsed;
    dateParsed.ParseDate(dateStr);
    ae.eventDate = dateParsed;
    ae.eventDetails = wxString::FromUTF8(j["Event Details"].get<std::string>().c_str());
    ae.hasHappened = j["Has Happened"].get<bool>();
}
