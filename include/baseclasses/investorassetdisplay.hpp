#ifndef INVESTORPOPOUTDISPLAY_HPP
#define INVESTORPOPOUTDISPLAY_HPP
#include <wx/variant.h>
#include "json.hpp"
#include <memory>
#include "baseclasses/investor2.hpp"
#include "baseclasses/position2.hpp"
#include "baseclasses/asset2.hpp"
#include "baseclasses/cashflow.hpp"

class Investor2;
class Position2;

class InvestorAssetDisplay{
    public:
        InvestorAssetDisplay(std::shared_ptr<Asset2>assetPtr, std::shared_ptr<Investor2>investorPtr):
        assetPtr(std::move(assetPtr)),investorPtr(std::move(investorPtr)){
            PopulateITDNetDistribution();
            SetDerivedValues();
            PopulateIRR();
        };
        std::shared_ptr<Asset2> assetPtr;
        std::shared_ptr<Investor2> investorPtr;
        double totalSubscribed;
        double totalDeployed;
        double totalReserve;
        double totalPaid;
        double totalReturnOfCapital;
        double irr;
        double itdNetDistribution;
        
        static std::vector<wxString> columnNames;
        static std::vector<int> columnWidths;

        wxVariant GetValue(int col)const;
        void SetValue(int col, const wxVariant &v);
    private:
        void PopulateIRR();
        void PopulateITDNetDistribution();
        void SetDerivedValues();
        double CalculateNPV(std::vector<CashFlow> &cashFlows, double &rate);
};

#endif
