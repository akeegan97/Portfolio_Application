#ifndef POSITION_HPP
#define POSITION_HPP
#include <memory>
#include <wx/string.h>
#include <wx/variant.h>
#include <wx/datetime.h>
#include <wx/tokenzr.h>
#include <wx/wxcrt.h>
#include <json.hpp>
#include "asset.hpp"
#include "portfolio.hpp"
using json = nlohmann::json;

class Asset;
class Portfolio;
class Position{
    public:
        std::shared_ptr<Asset> assetPtr;
        wxDateTime dateInvested;
        double committedAmountUp;
        double committedAmountDown;
        double calledCapitalUp;
        double calledCapitalDown;
        double investedAmountUp;
        double investedAmountDown;
        double waitingDeployUp;
        double waitingDeployDown;

        Position()=default;
        Position(const wxDateTime &dateInvested, const double &committedAmountUp, const double &committedAmountDown, const double &calledCapitalUp, 
        const double &calledCapitalDown, const double &investedAmountUp, const double &investedAmountDown, const double &waitingDeployUp, const double &waitingDeployDown):
        dateInvested(dateInvested),committedAmountUp(committedAmountUp),committedAmountDown(committedAmountDown),calledCapitalUp(calledCapitalUp),calledCapitalDown(calledCapitalDown),
        investedAmountUp(investedAmountUp),investedAmountDown(investedAmountDown),waitingDeployUp(waitingDeployUp),waitingDeployDown(waitingDeployDown){};
};
void to_json(json &j, const Position &pos);
void from_json(const json &j, Position &pos,Portfolio &porf);
#endif