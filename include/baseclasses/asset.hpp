#ifndef ASSET_HPP
#define ASSET_HPP
#include <wx/datetime.h>
#include "portfolio.hpp"
#include "valuation.hpp"
#include "investor.hpp"
#include "investorpositiondisplay.hpp"
#include "asset_event.hpp"
#include "distribution.hpp"
#include <json.hpp>
#include "position.hpp"
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
        std::vector<Distribution> distributions;
        std::vector<std::shared_ptr<InvestorPositionDisplay>> investorsPositionsDisplays;
        std::vector<std::shared_ptr<Position>> positions;
        std::vector<std::shared_ptr<AssetEvent>> events;

        static std::vector<wxString> columnNames;
        static std::vector<int> columnWidths;

        Asset()=default;
        Asset(const wxString &assetName, const wxDateTime &assetExitDate):
            assetName(assetName),assetExitDate(assetExitDate){};
        bool operator<(const Asset& other) const {
            return assetName < other.assetName; 
        }

        wxVariant GetValue(int col)const;///<
        void SetValue(int col, const wxVariant &v);

        double CalculateNumberOfInvestors()const;
        double CalculateDeployedCapital()const;
        double CalculateReturnedCapital();
        double CalculateSubscribedCapital();
        double CalculateReserveCapital();
        double CalculatePaidCapital();
        double GetLastValuation()const;
        void UpdateDerivedValues();
        double GetTotalMgmtFeesGenerated();
        double GetTotalPromoteFeesGenerated();
        void SetOwnershipOfPositions();
};  

void to_json(json &j, const Asset &as);
void from_json(const json&j, Asset &as,Portfolio &porf);

#endif