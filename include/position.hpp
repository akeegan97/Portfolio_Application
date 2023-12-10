#ifndef POSITION_HPP
#define POSITION_HPP
#include <memory>
#include <wx/string.h>
#include <wx/variant.h>
#include <wx/datetime.h>
#include <wx/tokenzr.h>
#include <wx/wxcrt.h>
#include <json.hpp>
#include "portfolio.hpp"
#include "asset.hpp"

using json = nlohmann::json;
class Portfolio;
class Asset;

class Position{
    public:
        std::shared_ptr<Asset> assetPtr;
        wxDateTime dateInvested;
        double subscribed;
        double paid;
        double reserve;
        double deployed;
        double returnOfCapital;
        double percentOwnership;
    
    Position() = default;
    Position(const wxDateTime &dateInvested, const double &subscribed, 
    const double &paid, const double &reserve,const double deployed, const double &returnOfCapital, 
    const double &percentOwnership): dateInvested(dateInvested), subscribed(subscribed),
    paid(paid), reserve(reserve), deployed(deployed),
    returnOfCapital(returnOfCapital),percentOwnership(percentOwnership){};

    void calculateOwnership(Portfolio &portfolio);

};
void to_json(json &j, const Position &pos);
void from_json(const json &j, Position &pos,Portfolio &porf);

#endif
