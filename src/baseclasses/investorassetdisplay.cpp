#include "baseclasses/investorassetdisplay.hpp"

void InvestorAssetDisplay::PopulateIRR(){
    std::vector<CashFlow> cashFlow;
    //Getting Initial Dates of investment and amounts
    for(const auto&pos:investorPtr->positions){
        if(pos->assetPtr == assetPtr){
            CashFlow newCashFlow;
            newCashFlow.amount = -pos->paid;
            newCashFlow.date = pos->dateInvested;
            cashFlow.push_back(newCashFlow);
        }
    }//getting dates and amounts of net distributions
    for(const auto& pos: investorPtr->positions){
        if(pos->assetPtr == assetPtr){
            for(const auto& dis : pos->netIncome){
                CashFlow newCashFlow;
                newCashFlow.amount = dis.distribution.second;
                newCashFlow.date = dis.distribution.first;
                cashFlow.push_back(newCashFlow);
            }
        }
    }//Getting date/amount of last valuation of asset * pos->ownership or total Paid
    if(!assetPtr->valuations.empty()){
        double ownership = 0;
        CashFlow newCashFlow;
        for(const auto&pos : investorPtr->positions){
            if(pos->assetPtr == assetPtr){
                ownership += pos->percentOwnership;
            }
        }
        std::sort(assetPtr->valuations.begin(), assetPtr->valuations.end(),
            [](const Valuation&a, const Valuation &b){
                return a.valuationDate.IsEarlierThan(b.valuationDate);
            });
        newCashFlow.amount = assetPtr->valuations.back().valuation * ownership;
        newCashFlow.date = assetPtr->valuations.back().valuationDate;
        cashFlow.push_back(newCashFlow);
    }else{
        CashFlow newCashFlow;
        double paidCapital = 0;
        for(const auto&pos:investorPtr->positions){
            if(pos->assetPtr == assetPtr){
                paidCapital += pos->paid;
            }
        }
        newCashFlow.amount = paidCapital;
        newCashFlow.date = wxDateTime::Today();
    }
    //starting calculation around XIRR:
    irr = 0.0;
    double guess = 0.1;
    double x1 = 0.0;
    int maxIterations = 100;
    double precision = 0.000001;
    for(int i = 0; i< maxIterations; i++){
        double npv = CalculateNPV(cashFlow, guess);
        double guessAddPrecision = guess + precision;
        double npvPrime = (CalculateNPV(cashFlow,guessAddPrecision) - npv) / precision;
        x1 = guess - npv / npvPrime;
        if(std::fabs(x1 - guess)<=precision){
            irr = x1;
            break;
        }
        guess = x1;
    }
}

void InvestorAssetDisplay::PopulateITDNetDistribution(){
    double returns = 0;
    for(const auto pos: investorPtr->positions){
        if(pos->assetPtr == assetPtr){
            for(auto&income : pos->netIncome){
                returns+=income.distribution.second;
            }
        }
    }
    itdNetDistribution = returns;
}

void InvestorAssetDisplay::SetDerivedValues(){
    totalDeployed=0;
    totalSubscribed=0;
    totalReserve=0;
    totalReturnOfCapital=0;
    for(const auto pos:investorPtr->positions){
        if(pos->assetPtr == assetPtr){
            totalSubscribed +=pos->subscribed;
            totalDeployed +=pos->deployed;
            totalReserve +=pos->reserve;
            totalReturnOfCapital +=pos->returnOfCapital;
        }
    }
}

double InvestorAssetDisplay::CalculateNPV(std::vector<CashFlow> &cashFlows, double &rate){
    double npv = 0.0;
    wxDateTime firstDate = cashFlows[0].date;
    for(const auto& cf : cashFlows){
        wxTimeSpan timeSpan = cf.date - firstDate;
        double years = timeSpan.GetDays() / 365.25;
        npv += cf.amount / std::pow(1+rate, years);
    }
}