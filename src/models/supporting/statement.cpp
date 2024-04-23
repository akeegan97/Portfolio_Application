#include "models/supporting/statement.hpp"

Statement::Statement(wxDateTime &startDate, wxDateTime &endDate,std::shared_ptr<Investor> &investorPtr):
    m_QStartDate(startDate),m_EndingDate(endDate),m_InvestorPtr(investorPtr){
        SetInternalValues();
        SetAssetDetails();
    };

//setters

void Statement::SetInternalValues(){
    double beginningBalance = 0.0;
    double endingBalance = 0.0;
    double additionalCapital = 0.0;
    double returnedCapital = 0.0;
    double returnedPrincipal = 0.0;
    double netIncomeThisPeriod = 0.0;
    double netIncomeAlltime = 0.0;
    double paidCapital = 0.0;
    double returnAmount = 0.0;
    double committed = 0.0;
    double combinedIrr = 0.0;
    std::vector<CashFlow> cashflow;

    for(const auto& pos : m_InvestorPtr->GetPositions()){
        if(pos->GetDateInvested() > m_EndingDate) continue;
        CashFlow newCashFlow;
        double paid = pos->GetPaid();
        newCashFlow.amount = -paid;
        newCashFlow.date = pos->GetDateInvested();
        cashflow.push_back(newCashFlow);
        committed += pos->GetPaid();//think this needs to be scoped to this loop instead and passed at the end to global comitted var
        double valuationAtStart = 0.0;
        double valuationAtEnd = 0.0;

        // Only consider the asset valuation at start if it was invested before the quarter start
        if(pos->GetDateInvested() < m_QStartDate){
            double assetValuationAtStart = pos->GetAssetPointer()->GetValuationOnDate(m_QStartDate);
            valuationAtStart = assetValuationAtStart * pos->CalculateOwnershipAtDate(m_QStartDate);
        }

        double assetValuationAtEnd = pos->GetAssetPointer()->GetValuationOnDate(m_EndingDate);
        double ownershipAtEnd = pos->CalculateOwnershipAtDate(m_EndingDate);
        valuationAtEnd = assetValuationAtEnd * ownershipAtEnd;

        double deployedAtStart = 0.0;
        double deployedAtEnd = 0.0;
        for(const auto& movement : pos->GetMovementsDeploy()){
            if(movement.first < m_QStartDate){
                deployedAtStart += movement.second;
            }
            if(movement.first <= m_EndingDate){
                deployedAtEnd += movement.second;
            }
        }

        double returnedCapitalAtStart = 0.0;
        double returnedCapitalAtEnd = 0.0;
        for(const auto& ROC : pos->GetROCMapConstant()){
            if(ROC.first < m_QStartDate){
                returnedCapitalAtStart += ROC.second;
            }
            if(ROC.first < m_EndingDate){
                returnedCapitalAtEnd += ROC.second;
                committed-=ROC.second;
                CashFlow newCashFlow;
                newCashFlow.amount = ROC.second;
                newCashFlow.date = ROC.first;
                cashflow.push_back(newCashFlow);
            }
        }

        double reserveAtStart = paid - deployedAtStart - returnedCapitalAtStart;
        double reserveAtEnd = paid - deployedAtEnd - returnedCapitalAtEnd;

        if(pos->GetDateInvested() < m_QStartDate){
            beginningBalance += valuationAtStart + reserveAtStart;
        }

        endingBalance += valuationAtEnd + reserveAtEnd;

        if(pos->GetDateInvested() >= m_QStartDate && pos->GetDateInvested() <= m_EndingDate){
            additionalCapital += pos->GetPaid();
        }

        for(const auto& ROC : pos->GetROCMapConstant()){
            if(ROC.first >= m_QStartDate && ROC.first < m_EndingDate){
                returnedCapital += ROC.second;
            }
            if(ROC.first < m_EndingDate){
                returnedPrincipal += ROC.second;
            }
        }
        for(const auto ni : pos->GetNetIncome()){
            if(ni.distribution.first >=m_QStartDate && ni.distribution.first <=m_EndingDate){
                netIncomeThisPeriod+=ni.distribution.second;
            }
            if(ni.distribution.first <m_EndingDate){
                netIncomeAlltime+=ni.distribution.second;
                CashFlow newCashFlow;
                newCashFlow.amount = ni.distribution.second;
                newCashFlow.date = ni.distribution.first;
                cashflow.push_back(newCashFlow);
            }
        }
        paidCapital+=pos->GetPaid();
    }
    CashFlow newCashFlow;
    newCashFlow.amount = endingBalance;
    newCashFlow.date = m_EndingDate;
    cashflow.push_back(newCashFlow);

    double changeInValuationThisPeriod = endingBalance - (beginningBalance + additionalCapital - returnedCapital);
    returnAmount = changeInValuationThisPeriod + netIncomeThisPeriod;
    double returnPercent = returnAmount / committed;
    double totalGain = netIncomeAlltime + (endingBalance - committed);
    SetBeginningBalance(beginningBalance);
    SetEndingBalance(endingBalance);
    SetAdditionalCapital(additionalCapital);
    SetReturnedCapital(returnedCapital);
    SetReturnedPrincipal(returnedPrincipal);
    SetChangeInValuationThisPeriod(changeInValuationThisPeriod);
    SetNetIncomeThisPeriod(netIncomeThisPeriod);
    SetTotalNetIncome(netIncomeAlltime);
    SetReturnThisPeriod(returnAmount);
    SetReturnPercentThisPeriod(returnPercent);
    SetPaid(paidCapital);
    SetTotalGain(totalGain);
    if(!cashflow.empty()){
        combinedIrr = CalculateIrr(cashflow);
    }
    SetCombinedIrr(combinedIrr);

}


void Statement::SetBeginningBalance(double value){
    m_BeginningBalance = value;
}
void Statement::SetAdditionalCapital(double value){
    m_AdditionalCapitalThisPeriod = value;
}
void Statement::SetReturnedCapital(double value){
    m_ReturnedCapitalThisPeriod = value;
}
void Statement::SetChangeInValuationThisPeriod(double value){
    m_ChangeValuationThisPeriod = value;
}
void Statement::SetEndingBalance(double value){
    m_EndingBalance = value;
}
void Statement::SetPaid(double value){
    m_PaidCapital = value;
}
void Statement::SetReturnedPrincipal(double value){
    m_ReturnedPrincipal = value;
}
void Statement::SetNetIncomeThisPeriod(double value){
    m_NetIncomeThisPeriod = value;
}
void Statement::SetReturnThisPeriod(double value){
    m_ReturnThisPeriodAmount = value;
}
void Statement::SetReturnPercentThisPeriod(double value){
    m_ReturnThisPeriodPercent = value;
}
void Statement::SetTotalNetIncome(double value){
    m_TotalNetDistributions = value;
}
void Statement::SetTotalChangeInValuation(double value){
    m_ChangeInValuationToEndDate = value;
}
void Statement::SetTotalGain(double value){
    m_TotalGain = value;
}
void Statement::SetCombinedIrr(double value){
    m_CombinedIRR = value;
}


void Statement::SetAssetDetails(){
    std::set<std::shared_ptr<Asset>> allAssets;
    for(auto pos : m_InvestorPtr->GetPositions()){
        allAssets.insert(pos->GetAssetPointer());
    }
    for(auto &asset : allAssets){
        Details assetDetails;
        double committed = 0.0;//
        double returnedCapital = 0.0;//
        double endingValuation = 0.0;//
        double totalDistributions = 0.0;//
        double totalReturn = 0.0;//
        double irr = 0.0;//
        double allocationPercent = 0.0;
        std::vector<CashFlow> cashFlow;
        std::shared_ptr<Asset> assetPtr = nullptr;
        for(auto &pos: m_InvestorPtr->GetPositions()){
            if(pos->GetDateInvested()>m_EndingDate)continue;
            if(pos->GetAssetPointer()==asset){
                std::vector<CashFlow> tempCashFlow;
                CashFlow newCashFlow;
                assetPtr=pos->GetAssetPointer();
                double paid = pos->GetPaid();
                committed+=paid;
                newCashFlow.amount = -paid;
                newCashFlow.date = pos->GetDateInvested();
                tempCashFlow.push_back(newCashFlow);

                double deployedAtEnd = 0.0;
                for(auto& move : pos->GetMovementsDeploy()){
                    if(move.first < m_EndingDate){
                        deployedAtEnd+=move.second;
                    }
                }
                double returnedCapitalAtEnd=0.0;
                for(auto&ROC: pos->GetROCMapConstant()){
                    if(ROC.first < m_EndingDate){
                        CashFlow newCashFlow;
                        newCashFlow.amount = ROC.second;
                        newCashFlow.date = ROC.first;
                        tempCashFlow.push_back(newCashFlow);
                        returnedCapitalAtEnd+=ROC.second;
                    }
                }
                returnedCapital+=returnedCapitalAtEnd;
                double reserveAtEnd = paid - deployedAtEnd - returnedCapitalAtEnd;
                for(auto&ni : pos->GetNetIncome()){
                    if(ni.distribution.first < m_EndingDate){
                        CashFlow newCashFlow;
                        newCashFlow.amount = ni.distribution.second;
                        newCashFlow.date = ni.distribution.first;
                        tempCashFlow.push_back(newCashFlow);
                        totalDistributions+=ni.distribution.second;
                    }
                }
                double assetValuationAtEnd = pos->GetAssetPointer()->GetValuationOnDate(m_EndingDate);
                double ownershipAtEnd = pos->CalculateOwnershipAtDate(m_EndingDate);
                double positionsValueAtEnd = assetValuationAtEnd * ownershipAtEnd;
                double positionsScaledValueAtEnd = positionsValueAtEnd + reserveAtEnd;
                CashFlow endingCashFlow;
                endingCashFlow.amount = positionsScaledValueAtEnd;
                endingCashFlow.date = m_EndingDate;
                tempCashFlow.push_back(endingCashFlow);
                endingValuation += positionsScaledValueAtEnd;
                std::move(std::begin(tempCashFlow),std::end(tempCashFlow),std::back_inserter(cashFlow));
            }
        }
        if(!cashFlow.empty()){
            irr = CalculateIrr(cashFlow);
        }else{
            irr=0.0;
        }
        assetDetails.m_AssetPtr = assetPtr;
        assetDetails.m_CommittedAmount = committed;
        assetDetails.m_IRR = irr;
        assetDetails.m_NetDistributionsExecuted = totalDistributions;
        assetDetails.m_ReturnedCapital = returnedCapital;
        assetDetails.m_ChangeInValuation = endingValuation - (committed - returnedCapital);
        assetDetails.m_AllocationPercent = 0.0;
        assetDetails.m_TotalReturn = assetDetails.m_ChangeInValuation + assetDetails.m_NetDistributionsExecuted;
        m_ITDDetails.push_back(assetDetails);
    }
    double totalCommitted = 0;
    for(auto detail : m_ITDDetails){
        totalCommitted += detail.m_CommittedAmount;
    }
    for(auto &detail : m_ITDDetails){
        detail.m_AllocationPercent = detail.m_CommittedAmount / totalCommitted;
    }

}
double Statement::CalculateIrr(std::vector<CashFlow> &cashFlow){
    double irr = 0;
    double zero = 0;
    double npvAtZero = CalculateNPV(cashFlow, zero);
    double direction = npvAtZero >0 ? 1.0 : -1.0;
    int maxIterations = 100;
    double precision = 0.000001;
    bool foundIrr = false;
    for(const auto&cf:cashFlow){
        std::cout<<"*Cash Flow Date: "<<cf.date.FormatISODate().ToStdString()<<" | Cash Flow Amount: "<<cf.amount<<std::endl;
    }
    for(double initialGuess = 0.1 * direction; std::fabs(initialGuess)<=1.0; initialGuess +=0.1*direction){
        double x1 = initialGuess;
        for(int i = 0; i< maxIterations; i++){
            double npv = CalculateNPV(cashFlow, x1);
            double x1AddPrecision = x1 + precision;
            double npvPrime = (CalculateNPV(cashFlow, x1AddPrecision) - npv) / precision;

            if(std::fabs(npvPrime) < 1e-6){
                break;
            }
            double xNext = x1 - npv / npvPrime; 
            if (std::fabs(xNext - x1) <= precision) {
                irr = xNext; 
                foundIrr = true; 
                std::cout << "IRR found: " << irr << std::endl;
                break; 
            }
            x1 = xNext; 
        }
        if (foundIrr) {
            return irr;
            break; 
        }else{
            return 0.0;
        }
    }
}
double Statement::CalculateNPV(std::vector<CashFlow> &cashFlows, double rate){
    double npv = 0.0;
    if(cashFlows.empty()){
        return npv;
    }
    wxDateTime firstDate = cashFlows[0].date;
    for(const auto& cf : cashFlows){
        wxTimeSpan timeSpan = cf.date - firstDate;
        double years = timeSpan.GetDays() / 365.25;
        npv += cf.amount / std::pow(1+rate, years);
    }
    return npv;
}
wxVariant Details::GetValue(int col)const{
    switch(col){
        case 0: return wxVariant(m_AssetPtr->GetAssetName());break;
        case 1: return wxVariant(utilities::formatDollarAmount(m_CommittedAmount));break;
        case 2: return wxVariant(utilities::formatDollarAmount(m_ReturnedCapital));break;
        case 3: return wxVariant(utilities::formatDollarAmount(m_ChangeInValuation));break;
        case 4: return wxVariant(utilities::formatDollarAmount(m_NetDistributionsExecuted));break;
        case 5: return wxVariant(utilities::formatDollarAmount(m_TotalReturn));break;
        case 6: return wxVariant(utilities::FormatPercentage(m_AllocationPercent));break;
        case 7: return wxVariant(utilities::FormatPercentage(m_IRR));break;
        default : return wxVariant();break;
    }
}

std::vector<wxString> Details::columnNames = {"Asset Name","Committed","ROC","Valuation Delta","ITD Income","Total Return","Allocate %","IRR%"};
std::vector<int> Details::columnWidths = {100,110,80,120,125,120,100,80};

double Statement::GetBeginningBalance(){
    return m_BeginningBalance;
}
double Statement::GetAdditionalCapital(){
    return m_AdditionalCapitalThisPeriod;
}
double Statement::GetReturnedCapital(){
    return m_ReturnedCapitalThisPeriod;
}
double Statement::GetChangeInValuationThisPeriod(){
    return m_ChangeValuationThisPeriod;
}
double Statement::GetEndingBalance(){
    return m_EndingBalance;
}

double Statement::GetPaidCapital(){
    return m_PaidCapital;
}
double Statement::GetReturnedPrincipal(){
    return m_ReturnedPrincipal;
}

double Statement::GetNetIncomeThisPeriod(){
    return m_NetIncomeThisPeriod;
}
double Statement::GetReturnAmountThisPeriod(){
    return m_ReturnThisPeriodAmount;
}
double Statement::GetReturnPercentThisPeriod(){
    return m_ReturnThisPeriodPercent;
}

double Statement::GetTotalNetIncomeToEndDate(){
    return m_TotalNetDistributions;
}
double Statement::GetChangeInValuationToEndDate(){
    return m_ChangeInValuationToEndDate;
}
double Statement::GetTotalGain(){
    return m_TotalGain;
}
double Statement::GetCombinedIrr(){
    return m_CombinedIRR;
}

std::vector<Details> Statement::GetDetails(){
    return m_ITDDetails;
}

wxString Statement::ToCsv()const{
    return wxString::Format("Beginning Balance,%.2f\n"
    "Additional Capiatl, %.2f\n"
    "Returned Capital, %.2f\n"
    "Ending Balance, %.2f\n"
    "Change In Valuation,%.2f\n"
    "Distributions This Period, %.2f\n"
    "Return This Period,%.2f\n"
    "Return % This Period ,%.2f%%\n"
    "Paid Capital,%.2f\n"
    "Returned Principal,%.2f\n"
    "Ending Balance, %.2f\n"
    "ITD Distributions,%.2f\n"
    "Total Return,%.2f\n"
    "IRR, %.5f\n",
    m_EndingBalance, m_AdditionalCapitalThisPeriod, m_ReturnedCapitalThisPeriod, m_EndingBalance,m_ChangeValuationThisPeriod,
    m_NetIncomeThisPeriod,m_ReturnThisPeriodAmount,m_ReturnThisPeriodPercent,m_PaidCapital,m_ReturnedPrincipal,
    m_EndingBalance,m_TotalNetDistributions,m_TotalGain,m_CombinedIRR);
}

wxString Details::ToCsv()const{
    return wxString::Format("%s,%.2f,%.2f,%.2f,%.2f,%.2f,%.5f,%.2f\n",
    m_AssetPtr->GetAssetName(),m_CommittedAmount,m_ReturnedCapital,m_ChangeInValuation,m_NetDistributionsExecuted,
    m_TotalReturn,m_IRR,m_AllocationPercent);
}

wxString Statement::WriteCsv()const{
    wxString statement = ToCsv();
    wxString detailHeaders = "Asset Name, Committed Amount, Returned Capital, Change In Valuation, ITD Distributions, Total Return, IRR, Allocation\n";
    wxString details;
    for(const auto&detail : m_ITDDetails){
        details+=detail.ToCsv();
    }
    wxString csvData = statement+detailHeaders+details;
    return csvData;
}