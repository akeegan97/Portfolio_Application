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
#include "asset_event.hpp"
#include <utility>
#include <memory>
#include <set>

using json = nlohmann::json;
class Asset;
class AssetEvent;
class Investor;
class Portfolio{
    public:
        std::vector<std::shared_ptr<Asset>> assetPtrs;
        std::vector<std::shared_ptr<AssetEvent>> assetEventPtrs;
        std::vector<std::pair<wxDateTime, double>> valuationVectorPlotting;
        std::vector<std::shared_ptr<Investor>> allInvestorPtrs;
        std::set<std::string> addedInvestorsName;
        Portfolio()=default;
        void SavePortfolioToFile(const Portfolio &portfolio, const std::string &filePath);
        void LoadFromFile(const std::string &filePath);

        //add helper function to be called to populate assetEventPtrs and valuations for getting valuations to display
        void PopulateEvents();
        void PopulateInvestors();        
        void addValuation();
        //functions for portfolio to calculate key figures used in the mainframe wxStaticText control

        double TotalInvestedCapital();
        double TotalInvestors();
        double TotalValuation();

};
void to_json(json &j, const Portfolio &por);
void from_json(const json &j, Portfolio &por);
#endif