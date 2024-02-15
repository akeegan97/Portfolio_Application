#ifndef PORTFOLIO_HPP
#define PORTFOLIO_HPP
#include <fstream>
#include <iostream>
#include <wx/string.h>
#include <wx/datetime.h>
#include <wx/variant.h>
#include "json.hpp"
#include "baseclasses/investor2.hpp"
#include "baseclasses/asset2.hpp"
#include <utility>
#include <memory>
#include <set>
#include "helpers/utilities.hpp"

using json = nlohmann::json;
class Asset2;
class Investor2;
class Portfolio{
    public:
        std::vector<std::shared_ptr<Asset2>> assetPtrs;
        std::vector<std::shared_ptr<Investor2>> allInvestorPtrs;
        std::vector<std::pair<wxDateTime, double>> valuationVectorPlotting;

        std::set<std::string> addedInvestorsName;

        std::map<wxDateTime, double> previousQMap;
        std::map<wxDateTime, double> currentQMap;
        
        Portfolio()=default;
        void SavePortfolioToFile(const Portfolio &portfolio, const std::string &filePath);
        void LoadFromFile(const std::string &filePath);

        //add helper function to be called to populate assetEventPtrs and valuations for getting valuations to display
        void SetAssetPositions();        
        void ValuationDialog();//
        void PopulateValuationMaps();//call when any valuations are added/edited/deleted
        
        //functions for portfolio to calculate key figures used in the mainframe wxStaticText control

        double TotalInvestedCapital();
        double TotalInvestors();
        double TotalValuation();

        double GetLastValuationOrDeployedCapital(std::shared_ptr<Asset2> &asset, const wxDateTime &date);
        void PopulatePreviousQValuations();
        void PopulateAndProcessCurrentQValuations();
};
void to_json(json &j, const Portfolio &por);
void from_json(const json &j, Portfolio &por);
#endif