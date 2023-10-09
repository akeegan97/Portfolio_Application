#include "investorpositiondisplay.hpp"


wxVariant InvestorPositionDisplay::GetValue(int col)const{
    switch (col)
    {
    case 0: return wxVariant(investorRef.clientName);break;
    case 1: return wxVariant(investorRef.type);break;
    case 2: return wxVariant(positionRef.dateInvested);break;
    case 3: return wxVariant(positionRef.committedAmountUp);break;
    case 4: return wxVariant(positionRef.committedAmountDown);break;
    case 5: return wxVariant(positionRef.calledCapitalUp);break;
    case 6: return wxVariant(positionRef.calledCapitalDown);break;
    case 7: return wxVariant(positionRef.investedAmountUp);break;
    case 8: return wxVariant(positionRef.investedAmountDown);break;
    case 9: return wxVariant(positionRef.waitingDeployUp);break;
    case 10: return wxVariant(positionRef.waitingDeployDown);break;
    default: return wxVariant();
        break;
    }
}

void InvestorPositionDisplay::SetValue(int col, const wxVariant &v){
    switch(col){
        case 0: investorRef.clientName = v.GetString();break;
        case 1: investorRef.type = v.GetString();break;
        case 2: positionRef.dateInvested = v.GetDateTime();break;
        case 3: positionRef.committedAmountUp = v.GetDouble();break;
        case 4: positionRef.committedAmountDown = v.GetDouble();break;
        case 5: positionRef.calledCapitalUp = v.GetDouble();break;
        case 6: positionRef.calledCapitalDown = v.GetDouble();break;
        case 7: positionRef.investedAmountUp = v.GetDouble();break;
        case 8: positionRef.investedAmountDown = v.GetDouble();break;
        case 9: positionRef.waitingDeployUp = v.GetDouble();break;
        case 10: positionRef.waitingDeployDown = v.GetDouble();break;
    }
}

std::vector<wxString> InvestorPositionDisplay::columnNames = {"Client","Type","Date Invested","Committed Up",
"Committed Down","Called Up","Called Down","Invested Up","Invested Down","Waiting Up","Waiting Down"};
std::vector<int> InvestorPositionDisplay::columnwidths = {100,50,50,75,75,75,75,75,75,75,75};