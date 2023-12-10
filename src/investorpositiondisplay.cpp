#include "investorpositiondisplay.hpp"
#include "investor.hpp"
#include "position.hpp"


wxVariant InvestorPositionDisplay::GetValue(int col)const{
    switch (col)
    {
    case 0: return wxVariant(investorPtr->clientName);break;
    case 1: return wxVariant(investorPtr->type);break;
    case 2: return wxVariant(positionPtr->dateInvested);break;
    case 3: return wxVariant(positionPtr->subscribed);break;
    case 4: return wxVariant(positionPtr->paid);break;
    case 5: return wxVariant(positionPtr->deployed);break;
    case 6: return wxVariant(positionPtr->reserve);break;
    case 7: return wxVariant(positionPtr->returnOfCapital);break;
    case 8: return wxVariant(positionPtr->percentOwnership);break;
    default: return wxVariant();
        break;
    }
}

void InvestorPositionDisplay::SetValue(int col, const wxVariant &v){
    switch(col){
        case 0: investorPtr->clientName = v.GetString();break;
        case 1: investorPtr->type = v.GetString();break;
        case 2: positionPtr->dateInvested = v.GetDateTime();break;
        case 3: positionPtr->subscribed = v.GetDouble();break;
        case 4: positionPtr->paid = v.GetDouble();break;
        case 5: positionPtr->deployed = v.GetDouble();break;
        case 6: positionPtr->reserve = v.GetDouble();break;
        case 7: positionPtr->returnOfCapital = v.GetDouble();break;
        case 8: positionPtr->percentOwnership = v.GetDouble();break;
    }
}

std::vector<wxString> InvestorPositionDisplay::columnNames = {"Investor", "Type","Date Invested","Subscribed", "Paid", "Deployed","Reserve","Return Capital","Ownership %"};
std::vector<int> InvestorPositionDisplay::columnWidths = {120,100,150,100,100,100,100,125,150};