#include "models/components/allocation.hpp"
#include "helpers/utilities.hpp"

Allocation::Allocation(std::shared_ptr<Investor>investorPtr, const wxDateTime &qEndDate):
    m_InvestorPtr(investorPtr),m_qEndDate(qEndDate){
        
    SetInternals();
};

void Allocation::SetInternals(){
    wxDateTime qStartDate = utilities::GetQuarterStartDate(m_qEndDate);
    double totalUnpaidMgmtFees = 0.0;
    double totalNetIncome = 0.0;
    for(const auto&pos : m_InvestorPtr->GetPositions()){
        // for(const auto &mgmtFee : pos->GetManagementFees()){
        //     if(mgmtFee.paid == false){
        //             if(mgmtFee.quarter> qStartDate && mgmtFee.quarter <=m_qEndDate){
        //             totalUnpaidMgmtFees+=mgmtFee.amount;
        //         }  
        //     }
        // } 
        for(const auto& netIncome : pos->GetNetIncome()){
            if(netIncome.distribution.first > qStartDate && netIncome.distribution.first <= m_qEndDate){
                totalNetIncome+=netIncome.distribution.second;
            }
        }
    }
    m_MgmtFees = totalUnpaidMgmtFees * m_InvestorPtr->GetPromoteFeePercentage();
    m_NetIncome = totalNetIncome;
    m_AmountToDistribute = m_NetIncome - m_MgmtFees;
}

std::vector<wxString> Allocation::columnNames = {"Investor","Net Income","To Distribute"};
std::vector<int> Allocation::columnWidths = {175,150, 175};

wxVariant Allocation::GetValue(int col)const{
    switch(col){
        case 0 : return wxVariant(m_InvestorPtr->GetName());break;
        case 1 : return wxVariant(utilities::formatDollarAmount(m_NetIncome));break;
        case 2 : return wxVariant(utilities::formatDollarAmount(m_AmountToDistribute));break;
        default: return wxVariant();break;
    }
}

