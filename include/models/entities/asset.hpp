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
        std::map<wxDateTime, double> m_movementsToFromDeploy;//negative for movements out of deploy to reserve, positive for movements in to deploy and out of reserve
    //for plotting
        std::vector<std::pair<wxDateTime, double>> m_valuationsForPlotting;
        std::vector<std::pair<wxDateTime, double>> m_deploymentsForPlotting;
        std::vector<std::pair<wxDateTime, double>> m_distributionsForBarChart;
        
        std::map<wxDateTime, double> m_previousQValuationMap;
        std::map<wxDateTime, double> m_currentQValuationMap;

        std::map<wxDateTime, double> m_previousQDeploymentMap;
        std::map<wxDateTime, double> m_currentQDeploymentMap;
    //private helper functions 
        void UpdateTotalMgmtFeesEarned();/////
        void UpdateTotalPromoteFeesEarned();/////
        void UpdateTotalCountOfInvestors();/////
        void UpdateTotalInvestedCapital();/////
        void UpdateCurrentvalue();///////
        void ProcessDistributionsForPosition();//
        void UpdatePositionValuations();//
        void PopulatePreviousQValuations();
        void PopulateCurrentQValuations();
        void PopulatePreviousQDeploys();
        void PopulateCurrentQDeploys();

    public:
        Asset() = default;
        Asset(const wxString &assetName, const wxString &assetSponserName, wxDateTime &assetExitDate):
            m_assetName(assetName),
            m_assetSponserName(assetSponserName),
            m_assetExitDate(assetExitDate),
            m_assetCommittedCapital(0.0),
            m_assetDeployedCapital(0.0),
            m_assetReserveCapital(0.0),
            m_assetReturnOfCapital(0.0),
            m_currentValue(0.0),
            m_totalInvestedCapital(0.0),
            m_countOfInvestors(0),
            m_totalMgmtFeesEarned(0.0),
            m_totalMgmtFeesDue(0.0),
            m_totalPromoteFeesEarned(0.0),
            m_irr(0.0){};
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
        std::vector<std::shared_ptr<Position>>& GetPositionsForIDP();
        double GetValuationInQuarter(wxDateTime &date)const;
        const std::vector<Valuation>& GetValuations()const;
        std::vector<Valuation>& GetValuationsNonConst();
        double GetTotalInvestors()const;
        double GetLastValuationAmountOrCommittedCapital()const;
        std::vector<std::shared_ptr<InvestorPositionDisplay>> GetIPDVector();
        const std::vector<std::pair<wxDateTime, double>>& GetValuationsForPlotting();
        const std::vector<std::pair<wxDateTime, double>>& GetDeploymentsForPlotting();
        const std::vector<Distribution>& GetDistributions();
        std::vector<Distribution>& GetDistributionsNonConst();
        const std::vector<std::pair<wxDateTime, double>>& GetDistributionsForPlotting();
        double GetTotalPromoteFeesEarned()const;
        double GetTotalMgmtFeesDue()const;
        double GetTotalMgmtFeesEarned()const;
        double GetIrr()const;
        double GetCurrentValue()const;
        const std::map<wxDateTime, double> GetROCMovements()const;
        const std::map<wxDateTime, double> GetMovementsToFromDeploy()const;
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
        void MoveReserveToDeploy(wxDateTime &date, double amount);
        void MoveDeployToReserve(wxDateTime &date, double amount);
        void MoveReserveToReturnOfCapital(wxDateTime &date, double amount);
        void MoveDeployToReturnOfCapital(wxDateTime &date,double amount);
        void SetCurrentValue();
        void SetNewCommittedOnNewPosition(double additionalCapital);
        void SetDeployedCapital(double &startingDeployed);
        void SetReserveCapital(double &startingReserve);
        void AddMovement(std::pair<wxDateTime, double>& movement);
        void TriggerUpdateDerivedValues();
        void AddNewDeployed(double &newDeployed);
        void AddNewReserve(double &newReserve);

    //methods to be used by VLC Templated Class
        wxVariant GetValue(int col)const;
        static std::vector<wxString> columnNames;
        static std::vector<int> columnWidths;
    //other public methods
        void SortDistributions(std::vector<Distribution> &distributions);
        std::vector<Valuation>& SortValuations(std::vector<Valuation> &valuations);
        void AddNewValuation(Valuation &newValuation);
        void SortValuations2();
        void SortDistributions2();
        void PopulateValuationsDeploymentsForPlotting();
        void PopulateDistributionForPlotting();
        void UpdateValuationsForPlotting(std::vector<std::pair<wxDateTime, double>> &&newValuations);
        void UpdateDeploymentsForPlotting(std::vector<std::pair<wxDateTime, double>> &&newDeployments);
        void SetPositionValues();
        void ClearInvestorPositionDisplays();
        void RemoveValuation(size_t index);
        void RemoveDistribution(size_t index);
        void AddNewPositionAdditionalCapital(Position &position);
};

void to_json(json &j, const Asset &asset);
void from_json(const json &j, Asset &asset, Portfolio &port);

#endif