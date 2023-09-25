#ifndef ASSETEVENT_HPP
#define ASSETEVENT_HPP
#include <wx/string.h>
#include <wx/datetime.h>
#include <wx/variant.h>
#include <json.hpp>
using json = nlohmann::json;

class AssetEvent{
    public:
        wxDateTime eventDate;
        wxString notes;
        wxString type;

        wxVariant GetValue(int col)const;
        void SetValue(int col, const wxVariant &v);

        static std::vector<std::string> columnNames;
        static std::vector<int> columnWidths;
};
void to_json(json &j, const AssetEvent &ie);
void from_json(const json &j, AssetEvent &ie);
#endif