#ifndef PORTFOLIO_HPP
#define PORTFOLIO_HPP
#include <wx/string.h>
#include <wx/datetime.h>
#include <wx/variant.h>
#include "json.hpp"
#include "investor.hpp"
#include "asset.hpp"

using json = nlohmann::json;

class Portfolio{
    public:
        std::vector<Asset> assets;
        std::vector<Investor> investors;
        Portfolio()=default;

};
void to_json(json &j, const Portfolio &por);
void from_json(const json &j, Portfolio &por);
#endif