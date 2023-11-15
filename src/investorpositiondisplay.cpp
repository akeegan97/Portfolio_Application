#include "investorpositiondisplay.hpp"
#include "investor.hpp"
#include "position.hpp"


wxVariant InvestorPositionDisplay::GetValue(int col)const{
    switch (col)
    {
    case 0: return wxVariant(investorPtr->clientName);break;
    case 1: return wxVariant(investorPtr->type);break;
    case 2: return wxVariant(positionPtr->dateInvested);break;
    case 3: return wxVariant(positionPtr->committedAmountUp);break;
    case 4: return wxVariant(positionPtr->committedAmountDown);break;
    case 5: return wxVariant(positionPtr->calledCapitalUp);break;
    case 6: return wxVariant(positionPtr->calledCapitalDown);break;
    case 7: return wxVariant(positionPtr->investedAmountUp);break;
    case 8: return wxVariant(positionPtr->investedAmountDown);break;
    case 9: return wxVariant(positionPtr->waitingDeployUp);break;
    case 10: return wxVariant(positionPtr->waitingDeployDown);break;
    default: return wxVariant();
        break;
    }
}

void InvestorPositionDisplay::SetValue(int col, const wxVariant &v){
    switch(col){
        case 0: investorPtr->clientName = v.GetString();break;
        case 1: investorPtr->type = v.GetString();break;
        case 2: positionPtr->dateInvested = v.GetDateTime();break;
        case 3: positionPtr->committedAmountUp = v.GetDouble();break;
        case 4: positionPtr->committedAmountDown = v.GetDouble();break;
        case 5: positionPtr->calledCapitalUp = v.GetDouble();break;
        case 6: positionPtr->calledCapitalDown = v.GetDouble();break;
        case 7: positionPtr->investedAmountUp = v.GetDouble();break;
        case 8: positionPtr->investedAmountDown = v.GetDouble();break;
        case 9: positionPtr->waitingDeployUp = v.GetDouble();break;
        case 10: positionPtr->waitingDeployDown = v.GetDouble();break;
    }
}

std::vector<wxString> InvestorPositionDisplay::columnNames = {"Client","Type","Date Invested","Committed Up",
"Committed Down","Called Up","Called Down","Invested Up","Invested Down","Waiting Up","Waiting Down"};
std::vector<int> InvestorPositionDisplay::columnWidths = {100,100,100, 100,120,100,100,100,100,100,100};