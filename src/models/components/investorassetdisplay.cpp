#include "models/components/investorassetdisplay.hpp"
#include "helpers/utilities.hpp"

void InvestorAssetDisplay::PopulateIRR(){
    std::vector<CashFlow> cashFlow;
    //Getting Initial Dates of investment and amounts
    for(const auto&pos:investorPtr->GetPositions()){
        if(pos->GetAssetPointer() == assetPtr){
            CashFlow newCashFlow;
            newCashFlow.amount = -pos->GetPaid();
            newCashFlow.date = pos->GetDateInvested();
            cashFlow.push_back(newCashFlow);
        }
    }//getting dates and amounts of net distributions
    for(const auto& pos: investorPtr->GetPositions()){
        if(pos->GetAssetPointer() == assetPtr){
            for(const auto& dis : pos->GetNetIncome()){
                CashFlow newCashFlow;
                newCashFlow.amount = dis.distribution.second;
                newCashFlow.date = dis.distribution.first;
                cashFlow.push_back(newCashFlow);
            }
        }
    }
    for(const auto&pos: investorPtr->GetPositions()){
        if(pos->GetAssetPointer()== assetPtr){
            for(const auto& roc: pos->GetReturnOfCapitalMap()){
                CashFlow newCashFlow;
                newCashFlow.amount = roc.second;
                newCashFlow.date = roc.first;
                cashFlow.push_back(newCashFlow);
            }
        }
    }
    if(!assetPtr->GetValuations().empty()){
        CashFlow newCashFlow;
        double ownership = 0;
        double committedAmount = 0;
        double reserveCapital = 0;
        assetPtr->SortValuations2();
        auto valuations = assetPtr->GetValuations();
        auto lastvaluation = valuations.back();
        for(const auto&pos : investorPtr->GetPositions()){
            if(pos->GetAssetPointer() == assetPtr){
                ownership+=pos->CalculateOwnershipAtDate(lastvaluation.valuationDate);
                committedAmount+=pos->GetCommitted();
                reserveCapital+=pos->GetReserve();
            }
        }
        newCashFlow.amount = lastvaluation.valuation * ownership + reserveCapital;
        newCashFlow.date = wxDateTime::Today();
        if(newCashFlow.amount != 0){
            cashFlow.push_back(newCashFlow);    
        }else{
            newCashFlow.amount = committedAmount;
            newCashFlow.date = wxDateTime::Today();
            cashFlow.push_back(newCashFlow);
        }
    }else{
        CashFlow newCashFlow;
        double committedCapital = 0;
        for(const auto&pos:investorPtr->GetPositions()){
            if(pos->GetAssetPointer() == assetPtr){
                committedCapital += pos->GetCommitted();
            }
        }
        newCashFlow.amount = committedCapital;
        newCashFlow.date = wxDateTime::Today();
        cashFlow.push_back(newCashFlow);
    }
    std::sort(cashFlow.begin(),cashFlow.end(),
            [](const CashFlow &a, const CashFlow &b){
                return a.date < b.date;
            });
    std::cout<<"Called From InvestorAssetDisplay"<<std::endl;
    for(const auto&cf:cashFlow){
        std::cout<<"Cash Flow Date: "<<cf.date.FormatISODate().ToStdString()<<" | Cash Flow Amount: "<<cf.amount<<std::endl;
    }
    irr = 0; 
    double zero = 0.0;
    double npvAtZero = CalculateNPV(cashFlow, zero); 
    double direction = npvAtZero > 0 ? 1.0 : -1.0; 

    int maxIterationsPerGuess = 100; 
    double precision = 0.000001; 
    bool foundIRR = false; 


    for (double initialGuess = 0.1 * direction; std::fabs(initialGuess) <= 1.0; initialGuess += 0.1 * direction) {
        double x1 = initialGuess; 

        for (int i = 0; i < maxIterationsPerGuess; ++i) {
            double npv = CalculateNPV(cashFlow, x1); 
            double x1AddPrecision = x1+precision;
            double npvPrime = (CalculateNPV(cashFlow, x1AddPrecision) - npv) / precision; 

            if (std::fabs(npvPrime) < 1e-6) {
                break; 
            }
            double xNext = x1 - npv / npvPrime; 
            if (std::fabs(xNext - x1) <= precision) {
                irr = xNext; 
                foundIRR = true; 
                std::cout << "IRR found: " << irr << std::endl;
                break; 
            }
            x1 = xNext; 
        }

        if (foundIRR) {
            break; 
        }
    }

    if (!foundIRR) {
        std::cout << "IRR not found within the tested range." << std::endl;
    }
}

void InvestorAssetDisplay::PopulateITDNetDistribution(){
    double returns = 0;
    for(const auto pos: investorPtr->GetPositions()){
        std::cout<<"Investor Ptr Position Size: "<<investorPtr->GetPositions().size()<<std::endl;
        if(pos->GetAssetPointer() == assetPtr){
            for(auto&income : pos->GetNetIncome()){
                returns+=income.distribution.second;
            }
        }
    }
    itdNetDistribution = returns;
}

void InvestorAssetDisplay::SetDerivedValues(){
    totalDeployed=0;
    totalCommitted=0;
    totalReserve=0;
    totalReturnOfCapital=0;
    totalCurrrentValue = 0;
    for(const auto pos:investorPtr->GetPositions()){
        if(pos->GetAssetPointer() == assetPtr){
            totalCommitted +=pos->GetCommitted();
            totalDeployed +=pos->GetDeployed();
            totalReserve +=pos->GetDeployed();
            totalReturnOfCapital +=pos->GetReturnOfCapital();
            totalPaid +=pos->GetPaid();
            totalCurrrentValue+=pos->GetPositionValue();
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
    return npv;
}

wxVariant InvestorAssetDisplay::GetValue(int col)const{
    switch(col){
        case 0: return wxVariant(assetPtr->GetAssetName());break;
        case 1: return wxVariant(utilities::formatDollarAmount(totalPaid));break;
        case 2: return wxVariant(utilities::formatDollarAmount(totalReturnOfCapital));break;
        case 3: return wxVariant(utilities::formatDollarAmount(totalCommitted));break;
        case 4: return wxVariant(utilities::formatDollarAmount(totalDeployed));break;
        case 5: return wxVariant(utilities::formatDollarAmount(itdNetDistribution));break;
        case 6: return wxVariant(utilities::FormatPercentage(irr));break;
        case 7: return wxVariant(utilities::formatDollarAmount(totalCurrrentValue));break;
        default: return wxVariant();break;
    }
}


std::vector<wxString> InvestorAssetDisplay::columnNames={"Asset Name","Paid","ROC","Committed","Deployed","ITD Distributions","IRR","Current Value"};
std::vector<int> InvestorAssetDisplay::columnWidths = {100,110,80,100,100,145,80,125};