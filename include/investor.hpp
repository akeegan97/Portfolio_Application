#ifndef INVESTOR_HPP
#define INVESTOR_HPP

#include <wx/string.h>
#include <wx/variant.h>
#include <wx/datetime.h>
#include <wx/tokenzr.h>
#include <wx/wxcrt.h>
#include <json.hpp>
#include <asset.hpp>
#include <position.hpp>


using json = nlohmann::json;
class Position;
class Portfolio;
class Investor{
    public:
        wxString clientName;
        wxString type;
        std::vector<Position> positions;
        double managementFeePercentage;
        double promoteFeePercentage;
        static std::vector<wxString> columnNames;
        static std::vector<int> columnWidths;
        
        Investor()=default;
        Investor(const wxString &clientName, const wxString &type):
                clientName(clientName),type(type){};
        wxVariant GetValue(int col)const;
        void SetValue(int col, const wxVariant &v);
};

void to_json(json &j, const Investor &inv);
void from_json(const json &j, Investor &inv, Portfolio &porf);

#endif