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
        std::vector<std::shared_ptr<Investor>> allInvestorPtrs;

        std::vector<std::shared_ptr<AssetEvent>> assetEventPtrs;
        std::vector<std::pair<wxDateTime, double>> valuationVectorPlotting;

        std::set<std::string> addedInvestorsName;

        std::map<wxDateTime, double> previousQMap;
        std::map<wxDateTime, double> currentQMap;
        
        Portfolio()=default;
        void SavePortfolioToFile(const Portfolio &portfolio, const std::string &filePath);
        void LoadFromFile(const std::string &filePath);

        //add helper function to be called to populate assetEventPtrs and valuations for getting valuations to display
        void PopulateEvents();
        void SetAssetPositions();        
        void ValuationDialog();//
        void PopulateValuationMaps();//call when any valuations are added/edited/deleted
        //functions for portfolio to calculate key figures used in the mainframe wxStaticText control
        double TotalInvestedCapital();
        double TotalInvestors();
        double TotalValuation();
        wxDateTime GetQuarterEndDate(wxDateTime &currentdate);
        wxDateTime GetNextQuarterEndDate(wxDateTime &currentEndDate);
        bool IsWithinQuarter(const wxDateTime&date,const wxDateTime &quarterEndDate);
        double GetLastValuationOrDeployedCapital(std::shared_ptr<Asset> &asset, const wxDateTime &date);
        wxDateTime GetQuarterStartDate(wxDateTime &date);
        void PopulatePreviousQValuations();
        void PopulateAndProcessCurrentQValuations();
};
void to_json(json &j, const Portfolio &por);
void from_json(const json &j, Portfolio &por);
#endif