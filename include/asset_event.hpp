#ifndef ASSET_EVENT_HPP
#define ASSET_EVENT_HPP
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/variant.h>
#include "json.hpp"

using json = nlohmann::json;

class AssetEvent{
    public:
        wxDateTime eventDate;
        wxString eventDetails;
        bool hasHappened;
        AssetEvent()=default;
        AssetEvent(const wxDateTime &eventDate, const wxString &eventDetails, const bool &hasHappened):
            eventDate(eventDate),eventDetails(eventDetails),hasHappened(hasHappened){};
        static std::vector<wxString> columnNames;
        static std::vector<int> columnWidths;
        wxVariant GetValue(int col)const;
        void SetValue(int col, const wxVariant &v);
};

void to_json(json&j, const AssetEvent &ae);
void from_json(const json&j, AssetEvent &ae);



#endif