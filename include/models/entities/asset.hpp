#ifndef ASSET_HPP
#define ASSET_HPP
#include <wx/datetime.h>
#include <wx/string.h>
#include <json.hpp>
#include <wx/variant.h>
#include <wx/chart.h>
#include <utility>
#include <set>
#include <math.h>
#include <memory>
#include <unordered_set>
#include "models/supporting/valuation.hpp"
#include "models/entities/portfolio.hpp"
#include "models/entities/position.hpp"
#include "models/components/investorpositiondisplay.hpp"
#include "helpers/utilities.hpp"

using json = nlohmann::json;
class Portfolio;
class InvestorPositionDisplay;

class Asset{
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
        std::vector<std::shared_ptr<InvestorPositionDisplay>> m_investorPositionDisplays;
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
        std::vector<std::shared_ptr<Position>> m_positions;
        std::map<wxDateTime,double> m_rocMovements;
        std::map<wxDateTime, double> m_movementsToFromDeploy;
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
        void PopulatePreviousQValuations();
        void PopulateCurrentQValuations();
        void PopulatePreviousQDeploys();
        void PopulateCurrentQDeploys();

    public:
        Asset() = default;
        Asset(const wxString &assetName, const wxString &assetSponserName, wxDateTime &assetExitDate):
            m_assetName(assetName),m_assetSponserName(assetSponserName),m_assetExitDate(assetExitDate){};
        bool operator < (const Asset & other)const{
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
        double GetTotalCommitted()const;
        const std::vector<std::shared_ptr<Position>>& GetPositions()const;
        double GetValuationInQuarter(wxDateTime &date)const;
        const std::vector<Valuation>& GetValuations()const;
        double GetTotalInvestors()const;
        double GetLastValuationAmountOrCommittedCapital()const;
        std::vector<std::shared_ptr<InvestorPositionDisplay>> GetIPDVector();
        const std::vector<std::pair<wxDateTime, double>>& GetValuationsForPlotting();
        const std::vector<std::pair<wxDateTime, double>>& GetDeploymentsForPlotting();
        const std::vector<Distribution>& GetDistributions();
        const std::vector<std::pair<wxDateTime, double>>& GetDistributionsForPlotting();
        double GetTotalPromoteFeesEarned()const;
        double GetTotalMgmtFeesDue()const;
        double GetTotalMgmtFeesEarned()const;
        double GetIrr()const;
        double GetCurrentValue()const;
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
        void AddPosition(std::shared_ptr<Position> &position);
        void DeserializeSetRocMovements(std::map<std::string, double> &movements);
        void AddInvestorPositionDisplay(std::shared_ptr<InvestorPositionDisplay> &ipd);
        void AddDistribution(Distribution &distribution);
        void TriggerUpdateOfDistributionsForPositions();

    //methods to be used by VLC Templated Class
        wxVariant GetValue(int col)const;
        static std::vector<wxString> columnNames;
        static std::vector<int> columnWidths;
    //other public methods
        void SortDistributions(std::vector<Distribution> &distributions);
        void SortValuations(std::vector<Valuation> &valuations);
        void AddNewValuation(const wxDateTime &valuationDate, double valuationAmount);
        void SortValuations2();
        void SortDistributions2();
        void PopulateValuationsDeploymentsForPlotting();
        void PopulateDistributionForPlotting();
        void UpdateValuationsForPlotting(std::vector<std::pair<wxDateTime, double>> &&newValuations);
        void UpdateDeploymentsForPlotting(std::vector<std::pair<wxDateTime, double>> &&newDeployments);
        void SetPositionValues();
};

void to_json(json &j, const Asset &asset);
void from_json(const json &j, Asset &asset, Portfolio &port);

#endif