#ifndef ASSET2_HPP
#define ASSET2_HPP
#include <wx/datetime.h>
#include <wx/string.h>
#include <json.hpp>
#include <wx/variant.h>
#include <utility>
#include <set>
#include <math.h>
#include <memory>
#include <unordered_set>
#include "baseclasses/valuation.hpp"
#include "baseclasses/portfolio.hpp"
#include "baseclasses/position2.hpp"
#include "helpers/utilities.hpp"

using json = nlohmann::json;
class Portfolio;

class Asset2{
    private:
    //asset specifics
        wxString m_assetName;
        wxString m_assetSponserName;
        wxDateTime m_assetExitDate;
        double m_assetCommittedCapital;
        double m_assetDeployedCapital;
        double m_assetReserveCapital;
        double m_assetReturnOfCapital;
        double m_currentValue;
    //positional specifics
        double m_totalInvestedCapital;
        double m_countOfInvestors;
        double m_totalMgmtFeesEarned;
        double m_totalMgmtFeesDue;
        double m_totalPromoteFeesEarned;
        double m_irr;
    //collections 
        std::vector<Valuation> m_valuations;
        std::vector<Distribution> m_distributions;
        std::vector<std::shared_ptr<Position2>> m_positions;
        std::map<wxDateTime,double> m_rocMovements;
    //for plotting
        std::vector<std::pair<wxDateTime, double>> m_valuationsForPlotting;
        std::vector<std::pair<wxDateTime, double>> m_deploymentsForPlotting;
        std::vector<std::pair<wxDateTime, double>> m_distributionsForBarChart;
        
        std::map<wxDateTime, double> m_previousQValuationMap;
        std::map<wxDateTime, double> m_currentQValuationMap;

        std::map<wxDateTime, double> m_previousQDeploymentMap;
        std::map<wxDateTime, double> m_currentQDeploymentMap;
    //private helper functions 
        void UpdateTotalMgmtFeesEarned();
        void UpdateTotalPromoteFeesEarned();
        void UpdateTotalCountOfInvestors();
        void UpdateTotalInvestedCapital();
        void ProcessDistributionsForPosition();
        void UpdatePositionValuations();
        void UpdateCurrentvalue();

    public:
        Asset2() = default;
        Asset2(const wxString &assetName, const wxString &assetSponserName, wxDateTime &assetExitDate):
            m_assetName(assetName),m_assetSponserName(assetSponserName),m_assetExitDate(assetExitDate){};
        bool operator < (const Asset2 & other)const{
            return m_assetName < other.m_assetName;
        }
    //public Getters
        wxString GetAssetName()const;
        wxString GetAssetSponserName()const;
        wxDateTime GetAssetExitDate()const;
        double GetTotalInvestedCapital()const;
        double GetTotalAssetDeployed()const;
        double GetTotalAssetReserve()const;
        double GetTotalReturnOfCapital()const;
        const std::vector<std::shared_ptr<Position2>>& GetPositions()const;
        double GetValuationInQuarter(wxDateTime &date)const;
        const std::vector<Valuation>& GetValuations()const;
        double GetTotalInvestors()const;
        double GetLastValuationAmountOrCommittedCapital()const;
    //public setters
        void DeserializeSetAssetName(wxString &assetName);
        void DeserializeSetAssetSponser(wxString &assetSponserName);
        void DeserializeSetAssetExitDate(wxDateTime &exitDate);
        void DeserializeSetAssetCommittedCapital(double &committedCapital);
        void DeserializeSetAssetDeployedCapital(double &deployedCapital);
        void DeserializeSetAssetReserveCapital(double &reserveCapital);
        void DeserializeSetAssetReturnOfCapital(double &returnOfCapital);
        void DeserializeSetValuations(std::vector<Valuation> &valuations);
        void DeserializeSetDistributions(std::vector<Distribution> &distributions);
        void AddPosition(std::shared_ptr<Position2> &position);
        void DeserializeSetRocMovements(std::map<wxDateTime, double> &movements);

    //methods to be used by VLC Templated Class
        wxVariant GetValue(int col)const;
        void SetValue(int col, const wxVariant &v);
        static std::vector<wxString> columnNames;
        static std::vector<int> columnWidths;
    //other public methods
        void SortDistributions(std::vector<Distribution> &distributions);
        void SortValuations(std::vector<Valuation> &valuations);
        void AddNewValuation(const wxDateTime &valuationDate, double valuationAmount);

};

void to_json(json &j, const Asset2 &asset);
void from_json(const json &j, Asset2 &asset, Portfolio &port);

#endif