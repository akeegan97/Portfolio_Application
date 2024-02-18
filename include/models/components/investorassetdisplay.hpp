#ifndef INVESTORPOPOUTDISPLAY_HPP
#define INVESTORPOPOUTDISPLAY_HPP
#include <wx/variant.h>
#include "json.hpp"
#include <memory>
#include "models/entities/investor.hpp"
#include "models/entities/position.hpp"
#include "models/entities/asset.hpp"
#include "models/supporting/distribution.hpp"

class Investor;
class Position;

class InvestorAssetDisplay{
    public:
        InvestorAssetDisplay(std::shared_ptr<Asset>assetPtr, std::shared_ptr<Investor>investorPtr):
        assetPtr(std::move(assetPtr)),investorPtr(std::move(investorPtr)){
            PopulateITDNetDistribution();
            SetDerivedValues();
            PopulateIRR();
        };
        std::shared_ptr<Asset> assetPtr;
        std::shared_ptr<Investor> investorPtr;
        double totalCommitted;
        double totalDeployed;
        double totalReserve;
        double totalPaid;
        double totalReturnOfCapital;
        double irr;
        double itdNetDistribution;
        
        static std::vector<wxString> columnNames;
        static std::vector<int> columnWidths;

        wxVariant GetValue(int col)const;
        
    private:
        void PopulateIRR();
        void PopulateITDNetDistribution();
        void SetDerivedValues();
        double CalculateNPV(std::vector<CashFlow> &cashFlows, double &rate);
};

#endif
