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
#include <utility>
#include <set>

using json = nlohmann::json;
class Portfolio;

class Asset{
    public:
        wxString assetName;
        wxDateTime assetExitDate;
        double totalInvestedCapital;
        double countOfInvestors;
        double currentValue;
        double totalMgmtFees;

        std::vector<Valuation> valuations;
        std::vector<Distribution> distributions;
        std::vector<std::shared_ptr<InvestorPositionDisplay>> investorsPositionsDisplays;
        std::vector<std::shared_ptr<Position>> positions;
        std::vector<std::shared_ptr<AssetEvent>> events;

        //for plotting

        std::vector<std::pair<wxDateTime, double>> valuationsForPlotting;
        std::vector<std::pair<wxDateTime, double>> deploymentsForPlotting;

        std::map<wxDateTime, double>previousQValuationMap;
        std::map<wxDateTime, double>currentQValuationMap;

        std::map<wxDateTime, double>previousQDeployMap;
        std::map<wxDateTime, double>currentQDeployMap;

        std::vector<std::pair<wxDateTime, double>> distributionsForPlottingBarChart;

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

        void PopulateValuationDeploymentForPlotting();
        void PopulatePreviousQValuations();
        void PopulateCurrentQValuations();
        void PopulatePreviousQDeploys();
        void PopulateCurrentQDeploys();

        void PopulateDistributionsForPlotting();

        //functions to move to a single namespace 
        wxDateTime GetQuarterEndDate(wxDateTime &currentDate);
        bool IsWithinQuarter(const wxDateTime &date, const wxDateTime &quarterEndDate);
        wxDateTime GetQuarterStartDate(wxDateTime &date);
        wxDateTime GetNextQuarterEndDate(wxDateTime &currentEndDate);
};  

void to_json(json &j, const Asset &as);
void from_json(const json&j, Asset &as,Portfolio &porf);

#endif
