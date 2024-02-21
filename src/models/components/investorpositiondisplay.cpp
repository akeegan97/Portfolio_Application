#include "models/components/investorpositiondisplay.hpp"
#include "models/entities/investor.hpp"
#include "models/entities/position.hpp"


wxVariant InvestorPositionDisplay::GetValue(int col)const{
    switch (col)
    {
    case 0: return wxVariant(positionPtr->GetInvestorPtr()->GetName());break;
    case 1: return wxVariant(positionPtr->GetInvestorPtr()->GetType());break;
    case 2: return wxVariant(positionPtr->GetDateInvested());break;
    case 3: return wxVariant(positionPtr->GetPaid());break;
    case 4: return wxVariant(positionPtr->GetReturnOfCapital());break;
    case 5: return wxVariant(positionPtr->GetCommitted());break;
    case 6: return wxVariant(positionPtr->GetDeployed());break;
    case 7: return wxVariant(positionPtr->GetReserve());break;
    case 8: return wxVariant(positionPtr->GetOwnership());break;
    case 9: return wxVariant(positionPtr->GetPositionValue());break;
    default: return wxVariant();
        break;
    }
}



std::vector<wxString> InvestorPositionDisplay::columnNames = {"Investor", "Type","Date Invested","Paid", "Returned Capital", "Committed","Deployed","Reserve","Ownership %","Current Value"};
std::vector<int> InvestorPositionDisplay::columnWidths = {120,100,150,100,100,100,100,125,150,100};