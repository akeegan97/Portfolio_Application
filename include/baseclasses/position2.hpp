#ifndef POSITION2_HPP
#define POSITION2_HPP
#include <memory>
#include <map>
#include <utility>
#include <wx/variant.h>
#include <wx/string.h>
#include <wx/datetime.h>
#include <wx/tokenzr.h>
#include <wx/wxcrt.h>
#include <json.hpp>
#include "baseclasses/asset2.hpp"
#include "baseclasses/managementFee.hpp"
#include "baseclasses/promoteFee.hpp"
#include "baseclasses/distribution.hpp"
#include "baseclasses/valuation.hpp"
#include "baseclasses/portfolio.hpp"
#include "baseclasses/investor2.hpp"

class Portfolio;
class Distribution;
class Asset2;
class Investor2;
using json = nlohmann::json;


class Position2{
    private:
    //pointers to tie to assigned asset/investor
        std::shared_ptr<Asset2> m_assetPtr;
        std::shared_ptr<Investor2> m_investorPtr;
    //collections of data specific to this position
        std::vector<ManagementFee> m_managementFees;
        std::vector<PromoteFee> m_promoteFees;
        std::vector<Distribution> m_netIncome;

        std::map<wxDateTime, double> m_valuationOfPosition;
        std::map<wxDateTime, double> m_movedOutOfDeploy;
        std::map<wxDateTime, double> m_movedToDeploy;
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
    //private Setter
        void PopulateManagementFeeVector();
    //private Methods
        ManagementFee CalculatePositionManagementFees(const double &mgmtfeePercent,const wxDateTime &qStart);

    public:
    //constructors
        Position2()=default;
        Position2(const wxDateTime &dateInvested, const double &subscribed,
        const double &paid, const double &reserve, const double &deployed,
        const double &returnOfCapital, const double &percentOwnership):
        m_dateInvested(dateInvested),m_paid(paid),
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
        std::map<wxDateTime, double> GetMovedFromDeploy()const;
        std::map<wxDateTime, double> GetMovedToDeploy()const;
        std::shared_ptr<Investor2> GetInvestorPtr()const;
        std::shared_ptr<Asset2> GetAssetPointer()const;
    //public setters
        void SetDeployed(double deployed);
        void SetReserve(double reserve);
        void SetReturnOfCapital(double returnOfCapital);
        void TriggerUpdateOfManagementFeeVector();
        void SetAssetPtr(std::shared_ptr<Asset2> &assetPointer);
        void SetInvestorPtr(std::shared_ptr<Investor2> &investorPointer);
        void SetDateInvested(wxDateTime &date);
        void SetPaid(double &paid);
        void SetManagementFeesDue(double &mgmtFeeDue);
        void AddMovedToDeployEntry(std::pair<wxDateTime, double> &movement);
        void AddMovedFromDeployEntry(std::pair<wxDateTime, double> &movement);
        void AddRocMovement(std::pair<wxDateTime, double> &movement);
    //public methods in general
        void UpdateFinancesPostDistributionChanges(Distribution &distribution, PromoteFee &promoteFee);
        double CalculateManagementFeesDue(const Distribution&currentDistribution);
        double CalculateOwnershipAtDate(const wxDateTime &date);
        double GetInvestorPromoteFee();
        void ClearNetIncomePromoteFees();
        double CalculateCommittedUpToDate(const wxDateTime &date)const;
        void RepopulateValuations();
};

void to_json(json &j, const Position2 &pos);
void from_json(const json&j, Position2 &pos, Portfolio &port);
#endif