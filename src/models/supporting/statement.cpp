#include "models/supporting/statement.hpp"

Statement::Statement(wxDateTime &startDate, wxDateTime &endDate):
    m_QStartDate(startDate),m_EndingDate(endDate){

    };

//setters

void Statement::SetInternalValues(){
    double committedCapital = 0.0;//
    double paidcapital = 0.0;//
    double returnedCapital=0.0;//
    double returnedPrincipal=0.0;//
    double startingValuation=0.0;//
    double endingValuation=0.0;//
    double changeInValuation=0.0;//
    double additionalCapital=0.0;//
    double totalExecutedDistributionsInPeriod=0.0;//
    double totalIncomeUptoEndDate = 0.0;//
    double totalGainUptoEndDate = 0.0;
    double totalGainInPeriod=0.0;//
    double totalGainInValuation=0.0;//
    double beginningValuation = 0.0;//

    for(auto &pos:m_InvestorPtr->GetPositions()){
        wxDateTime dateInvested = pos->GetDateInvested();
        committedCapital += pos->CalculateCommittedUpToDate(m_EndingDate);
        paidcapital+=pos->GetPaid();
        beginningValuation +=pos->GetAssetPointer()->GetValuationOnDate(dateInvested) * pos->CalculateOwnershipAtDate(pos->GetDateInvested());
        startingValuation += pos->GetAssetPointer()->GetValuationOnDate(m_QStartDate) * pos->CalculateOwnershipAtDate(m_QStartDate);
        endingValuation +=pos->GetAssetPointer()->GetValuationOnDate(m_EndingDate) * pos->CalculateOwnershipAtDate(m_EndingDate);
        
        if(pos->GetDateInvested() >m_QStartDate && pos->GetDateInvested() < m_EndingDate){
            additionalCapital += pos->GetPaid();
        }

        for(auto move : pos->GetReturnOfCapitalMap()){
            if(move.first > m_QStartDate && move.first < m_EndingDate){
                returnedCapital += move.second;
            }
        }
        for(auto ni : pos->GetNetIncome()){
            //gained distributions in period
            if(ni.distribution.first < m_EndingDate && ni.distribution.first > m_QStartDate){
                totalExecutedDistributionsInPeriod += ni.distribution.second; 
            }
            //gained since start
            if(ni.distribution.first <m_EndingDate){
                totalIncomeUptoEndDate+=ni.distribution.second;
            }
        }

    }
    returnedPrincipal = paidcapital - committedCapital;//returned capital all time up to end date
    changeInValuation = endingValuation - startingValuation;//change in period valuation
    totalGainInPeriod = changeInValuation + totalExecutedDistributionsInPeriod;//net income + valuation change
    totalGainInValuation = endingValuation - beginningValuation;//valuation change from dateInvested -> ending date
    totalGainUptoEndDate = totalGainInValuation + totalIncomeUptoEndDate;//net income from date invested -> ending date + total gain in valuation
    //call all funcs
    SetCommittedCapital(committedCapital);
    SetReturnedPrincipal(returnedPrincipal);
    SetQStartValuation(startingValuation);
    SetEndValuation(endingValuation);
    SetValuationChange(changeInValuation);
    SetAdditionalCapital(additionalCapital);
    SetReturnedCapital(returnedCapital);
    SetExecutedDistributions(totalExecutedDistributionsInPeriod);
    SetTotalDistributions(totalIncomeUptoEndDate);
    SetTotalValuation(totalGainInValuation);
    SetTotalGains(totalGainUptoEndDate);
}

void Statement::SetCommittedCapital(double value){
    m_CommittedCapital = value;
}
void Statement::SetReturnedPrincipal(double value){
    m_ReturnedPrincipal = value;
}
void Statement::SetQStartValuation(double value){
    m_QStartValuation = value;
}
void Statement::SetEndValuation(double value){
    m_EndValuation = value;
}
void Statement::SetAdditionalCapital(double value){
    m_AdditionalCapitalThisPeriod = value;
}
void Statement::SetReturnedCapital(double value){
    m_ReturnedCapitalThisPeriod = value;
}
void Statement::SetExecutedDistributions(double value){
    m_executedDistributionsThisPeriod = value;
}
void Statement::SetTotalDistributions(double value){
    m_TotalNetDistributions = value;
}
void Statement::SetTotalValuation(double value){
    m_InceptionToDateValuation = value;
}
void Statement::SetTotalGains(double value){
    m_TotalGain = value;
}
void Statement::SetValuationChange(double value){
    m_ValuationChange = value;
}

void Statement::SetAssetDetails(){
    std::set<std::shared_ptr<Asset>> allAssets;
    for(auto pos : m_InvestorPtr->GetPositions()){
        allAssets.insert(pos->GetAssetPointer());
    }
    for(auto &asset : allAssets){
        Details assetDetails;
        double committed = 0.0;
        double returnedCapital = 0.0;
        double startingValuation = 0.0;
        double endingValuation = 0.0;
        double totalDistributions = 0.0;
        double totalReturn = 0.0;
        double irr = 0.0;
        double allocationPercent = 0.0;
        for(auto &pos : m_InvestorPtr->GetPositions()){
            if(pos->GetAssetPointer() == asset){
                committed+=pos->GetPaid();
                for(auto&roc : pos->GetReturnOfCapitalMap()){
                    if(roc.first< m_EndingDate){
                        returnedCapital+=roc.second;
                    }
                }
                if(pos->GetDateInvested() > m_QStartDate){
                    wxDateTime dateInvested = pos->GetDateInvested();
                    startingValuation += asset->GetValuationOnDate(dateInvested) * pos->CalculateOwnershipAtDate(pos->GetDateInvested());
                }else{
                    startingValuation += asset->GetValuationOnDate(m_QStartDate) * pos->CalculateOwnershipAtDate(m_QStartDate);    
                }
                endingValuation = asset->GetValuationOnDate(m_EndingDate) * pos->CalculateOwnershipAtDate(m_EndingDate);
                for(auto &ni : pos->GetNetIncome()){
                    if(ni.distribution.first < m_EndingDate){
                        totalDistributions +=ni.distribution.second;    
                    }
                }
                totalReturn +=endingValuation + totalDistributions;
                
            }
        }
    }

}