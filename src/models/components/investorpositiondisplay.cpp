#include "models/components/investorpositiondisplay.hpp"
#include "models/entities/investor.hpp"
#include "models/entities/position.hpp"
#include "helpers/utilities.hpp"


wxVariant InvestorPositionDisplay::GetValue(int col)const{
    switch (col)
    {
    case 0: return wxVariant(positionPtr->GetInvestorPtr()->GetName());break;
    case 1: return wxVariant(positionPtr->GetInvestorPtr()->GetType());break;
    case 2: return wxVariant(positionPtr->GetDateInvested().FormatISODate());break;
    case 3: return wxVariant(utilities::formatDollarAmount(positionPtr->GetPaid()));break;
    case 4: return wxVariant(utilities::formatDollarAmount(positionPtr->GetReturnOfCapital()));break;
    case 5: return wxVariant(utilities::formatDollarAmount(positionPtr->GetCommitted()));break;
    case 6: return wxVariant(utilities::formatDollarAmount(positionPtr->GetDeployed()));break;
    case 7: return wxVariant(utilities::formatDollarAmount(positionPtr->GetReserve()));break;
    case 8: return wxVariant(utilities::FormatPercentage(positionPtr->GetOwnership()));break;
    case 9: return wxVariant(utilities::formatDollarAmount(positionPtr->GetPositionValue()));break;
    default: return wxVariant();
        break;
    }
}



std::vector<wxString> InvestorPositionDisplay::columnNames = {"Investor", "Type","Date Invested","Paid", "Returned Capital", "Committed","Deployed","Reserve","Ownership %","Current Value"};
std::vector<int> InvestorPositionDisplay::columnWidths = {120,100,120,110,100,125,120,120,120,135};