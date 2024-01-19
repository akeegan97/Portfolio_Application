#ifndef POSITION_HPP
#define POSITION_HPP
#include <memory>
#include <map>
#include <utility>
#include <wx/string.h>
#include <wx/variant.h>
#include <wx/datetime.h>
#include <wx/tokenzr.h>
#include <wx/wxcrt.h>
#include <json.hpp>
#include "portfolio.hpp"
#include "asset.hpp"
#include "managementFee.hpp"
#include "promoteFee.hpp"
#include "distribution.hpp"
#include "investor.hpp"

using json = nlohmann::json;
class Portfolio;
class Asset;
class Investor;

class Position{
    public:
        std::shared_ptr<Asset> assetPtr;
        std::shared_ptr<Investor> investorPtr;
        wxDateTime dateInvested;
        double subscribed;
        double paid;
        double reserve;
        double deployed;
        double returnOfCapital;
        double percentOwnership;
        double mgmtFeesDue;
        std::vector<ManagementFee> managementFees;
        std::vector<PromoteFee> promoteFees;
        std::map<wxDateTime, double> movedToDeploy;
        std::map<wxDateTime, double> movedOutOfDeployed;
        std::vector<Distribution> netIncome;
    
        Position() = default;
        Position(const wxDateTime &dateInvested, const double &subscribed, 
        const double &paid, const double &reserve,const double deployed, const double &returnOfCapital, 
        const double &percentOwnership): dateInvested(dateInvested), subscribed(subscribed),
        paid(paid), reserve(reserve), deployed(deployed),
        returnOfCapital(returnOfCapital),percentOwnership(percentOwnership){};

        void calculateOwnership(Portfolio &portfolio);
        ManagementFee CalculatePositionManagementFees(Position&position, const double &managementFeePercentage, wxDateTime &date);
        void CalculateHistoricalManagementFees(const double &managementFeePercentage);
        void ReCalculateTotalManagementFeesDue(wxDateTime distributionDate);
        void UpdateFinancesPostDistributionChanges(std::vector<Distribution>& distributions, double& promoteFeePercentage, double&mgmtFeePercentage);

        std::pair<wxDateTime, wxDateTime> GetCurrentQuarterDates(const wxDateTime &currentDate);
        wxDateTime GetNextQuarterStartDate(wxDateTime &date);
        double calculateDaysBetween(const wxDateTime &start, const wxDateTime &end);
};
void to_json(json &j, const Position &pos);
void from_json(const json &j, Position &pos,Portfolio &porf);

#endif
