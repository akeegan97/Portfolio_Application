#ifndef ASSETVALUATION_HPP
#define ASSETVALUATION_HPP

#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/variant.h>
#include <json.hpp>

using json = nlohmann::json;

class Valuation{
    public:
        wxDateTime valuationDate;
        double valuation;
        Valuation()=default;
        Valuation(const wxDateTime &valuationDate, const double &valuation):
            valuationDate(valuationDate),valuation(valuation){};
        static std::vector<wxString> columnNames;
        static std::vector<int> columnWidths;
        wxVariant GetValue(int col)const;
        void SetValue(int col, const wxVariant &v);        
};

void to_json(json&j, const Valuation &val);
void to_json(const json &j, Valuation &val);

#endif