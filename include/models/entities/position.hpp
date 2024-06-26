#ifndef POSITION_HPP
#define POSITION_HPP
#include <memory>
#include <map>
#include <utility>
#include <wx/variant.h>
#include <wx/string.h>
#include <wx/datetime.h>
#include <wx/tokenzr.h>
#include <wx/wxcrt.h>
#include <json.hpp>

#include "models/entities/asset.hpp"
#include "models/entities/portfolio.hpp"
#include "models/entities/investor.hpp"

#include "models/supporting/managementFee.hpp"
#include "models/supporting/promoteFee.hpp"
#include "models/supporting/distribution.hpp"
#include "models/supporting/valuation.hpp"
#include "models/supporting/cashflow.hpp"


class Portfolio;
class Distribution;
class Asset;
class Investor;
class CashFlow;
using json = nlohmann::json;


class Position{
    private:
    //pointers to tie to assigned asset/investor
        std::shared_ptr<Asset> m_assetPtr;
        std::shared_ptr<Investor> m_investorPtr;
    //collections of data specific to this position
        std::vector<ManagementFee> m_managementFees;//need to serde this also 
        std::vector<PromoteFee> m_promoteFees;
        std::vector<Distribution> m_netIncome;

        std::map<wxDateTime, double> m_valuationOfPosition;
        std::map<wxDateTime, double> m_movementsDeploy;
        std::map<wxDateTime, double> m_returnOfCapitalMap;
    //positional specifics 
        wxDateTime m_dateInvested;
        double m_paid;//this is for the actual initial money sent
        double m_committed;//this is the amount that is still in the asset (deploy + reserve)
        double m_reserve;
        double m_deployed;
        double m_returnOfCapital;
        double m_percentOwnership;
        double m_managementFeesDue;
        double m_currentValue;
        size_t m_id;
    //private Setter
        void PopulateManagementFeeVector();
    //private Methods
        //new mgmt fee function
        ManagementFee CalculatePositionManagementFee(const double &mgmtFeePercent, const wxDateTime &qStart);
        void RecalculateManagementFeesFrom(const wxDateTime &qStartDate);
    public:
    //constructors
        Position()=default;
        Position(const wxDateTime &dateInvested, const double &subscribed,
        const double &paid, const double &reserve, const double &deployed,
        const double &returnOfCapital, const double &percentOwnership, const double &id):
        m_dateInvested(dateInvested),m_paid(paid),m_id(id),
        m_reserve(reserve),m_deployed(deployed),m_returnOfCapital(returnOfCapital),
        m_percentOwnership(percentOwnership),m_assetPtr(nullptr),m_investorPtr(nullptr){
        };
    //public getters
        wxDateTime GetDateInvested()const;
        double GetPaid()const;
        double GetReserve()const;
        double GetCommitted()const;
        double GetDeployed()const;
        double GetReturnOfCapital()const;
        double GetOwnership()const;
        double GetManagementFeesDue()const;
        std::vector<ManagementFee> GetManagementFees()const;
        std::vector<PromoteFee> GetPromoteFees()const;
        std::vector<Distribution> GetNetIncome()const;
        const std::map<wxDateTime, double> GetPositionValuations()const;
        std::map<wxDateTime, double> GetMovementsDeploy()const;
        std::shared_ptr<Investor> GetInvestorPtr()const;
        std::shared_ptr<Asset> GetAssetPointer()const;
        double GetPositionValue()const;
        std::vector<ManagementFee>& GetManagementFeesReference();
    //public setters
        void SetDeployed(double deployed);
        void SetReserve(double reserve);
        void SetReturnOfCapital(double returnOfCapital);
        void TriggerUpdateOfManagementFeeVector();
        void SetAssetPtr(std::shared_ptr<Asset> &assetPointer);
        void SetInvestorPtr(std::shared_ptr<Investor> &investorPointer);
        void SetDateInvested(wxDateTime &date);
        void SetPaid(double &paid);
        void SetManagementFeesDue(double &mgmtFeeDue);
        void AddMovementDeploy(std::pair<wxDateTime, double> &movement);
        void AddRocMovement(std::pair<wxDateTime, double> &movement);
        void SetCommitted();
        void SetReserve();
        void SetDeployed();
        void SetCurrentValue();
        void SetOwnership();
        void UpdateROC();
        void SetMgmtFeeVector(std::vector<ManagementFee> fees);
        void UpdateManagementFees(const wxDateTime &date);
        void SetPromoteFees(std::vector<PromoteFee> pfs);
        void SetNetIncome(std::vector<Distribution> netIncome);
    //public methods in general
        void UpdateFinancesPostDistributionChanges(Distribution &distribution, PromoteFee &promoteFee);
        double CalculateManagementFeesDue(const Distribution&currentDistribution);
        double CalculateOwnershipAtDate(const wxDateTime &date)const;
        double GetInvestorPromoteFee();
        void ClearNetIncomePromoteFees();
        double CalculateCommittedUpToDate(const wxDateTime &date)const;
        void RepopulateValuations();
        void SetRocMovements();
        void SetId(size_t id);
        size_t GetId();
        std::map<wxDateTime, double> GetReturnOfCapitalMap();
        std::map<wxDateTime, double> GetROCMapConstant()const;
        double CalculateCapitalDays(const Asset &asset, const wxDateTime &qEndDate);
        double GetInitialDeployedBefore(const wxDateTime &date);
        void AddNetIncome(Distribution &distribution);
        void AddPromoteFee(PromoteFee &pf);
        std::vector<CashFlow> ReturnCashFlowToFromDate(const wxDateTime &startDate, const wxDateTime &endDate)const;
};

void to_json(json &j, const Position &pos);
void from_json(const json&j, Position &pos, Portfolio &port);
#endif