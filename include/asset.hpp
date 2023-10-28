#ifndef ASSET_HPP
#define ASSET_HPP
#include <wx/string.h>
#include <wx/datetime.h>
#include <wx/variant.h>
#include "valuation.hpp"
#include "investor.hpp"
#include "investorpositiondisplay.hpp"
#include <json.hpp>
using json = nlohmann::json;
class Portfolio;

class Asset{
    public:
        wxString assetName;
        wxDateTime assetExitDate;
        double totalInvestedCapital;
        double countOfInvestors;
        double currentValue;

        std::vector<Valuation> valuations;
        std::vector<InvestorPositionDisplay> investorsPositionsDisplays;
        std::vector<Investor> investors;

        static std::vector<wxString> columnNames;
        static std::vector<int> columnWidths;

        Asset()=default;
        Asset(const wxString &assetName, const wxDateTime &assetExitDate):
            assetName(assetName),assetExitDate(assetExitDate){};
        bool operator<(const Asset& other) const {
            return assetName < other.assetName; 
        }

        wxVariant GetValue(int col)const;
        void SetValue(int col, const wxVariant &v);
};  
void to_json(json &j, const Asset &as);
void from_json(const json&j, Asset &as,Portfolio &porf);

#endif
