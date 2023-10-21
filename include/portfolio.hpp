#ifndef PORTFOLIO_HPP
#define PORTFOLIO_HPP
#include <fstream>
#include <iostream>
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
        Portfolio()=default;
        void SavePortfolioToFile(const Portfolio &portfolio, const std::string &filePath);
        Portfolio LoadFromFile(const std::string &filePath);

};
void to_json(json &j, const Portfolio &por);
void from_json(const json &j, Portfolio &por);
#endif