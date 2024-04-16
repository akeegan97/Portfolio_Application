#ifndef PORTFOLIO_HPP
#define PORTFOLIO_HPP
#include <fstream>
#include <iostream>
#include <utility>
#include <memory>
#include <set>
#include <wx/string.h>
#include <wx/datetime.h>
#include <wx/variant.h>
#include "json.hpp"

#include "models/entities/investor.hpp"
#include "models/entities/asset.hpp"
#include "helpers/utilities.hpp"

using json = nlohmann::json;
class Asset;
class Investor;
class Portfolio{
    public:
        std::vector<std::shared_ptr<Asset>> assetPtrs;
        std::vector<std::shared_ptr<Investor>> allInvestorPtrs;
        std::vector<std::pair<wxDateTime, double>> valuationVectorPlotting;

        std::set<std::string> addedInvestorsName;

        std::map<wxDateTime, double> previousQMap;
        std::map<wxDateTime, double> currentQMap;
        
        Portfolio()=default;
        void SavePortfolioToFile(const Portfolio &portfolio, const std::string &filePath);
        void LoadFromFile(const std::string &filePath);  
        void ValuationDialog();//
        void PopulateValuationMaps();//call when any valuations are added/edited/deleted

        double TotalInvestedCapital();
        double TotalInvestors();
        double TotalValuation();

        void AddInvestor(std::shared_ptr<Investor> &investor);
        void AddAsset(std::shared_ptr<Asset> &newAsset);

        double GetLastValuationOrDeployedCapital(std::shared_ptr<Asset> &asset, const wxDateTime &date);
        void PopulatePreviousQValuations();
        void PopulateAndProcessCurrentQValuations();

        std::vector<std::shared_ptr<Investor>>& GetInvestors();
        std::shared_ptr<Investor> GetInvestorByName(const std::string &investorName) const;
        void EnsureFundPositionExists();

        double CalculateFundIrr();
        double CalculateNPV(std::vector<CashFlow> &cashFlows, double &rate);
};
void to_json(json &j, const Portfolio &por);
void from_json(const json &j, Portfolio &por);
#endif