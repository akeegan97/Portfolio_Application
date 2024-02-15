#include "baseclasses/investorpositiondisplay.hpp"
#include "baseclasses/investor2.hpp"
#include "baseclasses/position2.hpp"


wxVariant InvestorPositionDisplay::GetValue(int col)const{
    switch (col)
    {
    case 0: return wxVariant(positionPtr->GetInvestorPtr()->GetName());break;
    case 1: return wxVariant(positionPtr->GetInvestorPtr()->GetType());break;
    case 2: return wxVariant(positionPtr->GetDateInvested());break;
    case 3: return wxVariant(positionPtr->GetCommitted());break;
    case 4: return wxVariant(positionPtr->GetPaid());break;
    case 5: return wxVariant(positionPtr->GetDeployed());break;
    case 6: return wxVariant(positionPtr->GetReserve());break;
    case 7: return wxVariant(positionPtr->GetReturnOfCapital());break;
    case 8: return wxVariant(positionPtr->GetOwnership());break;
    default: return wxVariant();
        break;
    }
}



std::vector<wxString> InvestorPositionDisplay::columnNames = {"Investor", "Type","Date Invested","Subscribed", "Paid", "Deployed","Reserve","Return Capital","Ownership %"};
std::vector<int> InvestorPositionDisplay::columnWidths = {120,100,150,100,100,100,100,125,150};