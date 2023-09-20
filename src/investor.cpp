#include "investor.hpp"
//constructor for reading in bulk list of Investors for ease of use
Investor Investor::fromCsv(const wxString line){
    wxStringTokenizer token(line, ',');
    Investor inv;
    inv.clientName = token.GetNextToken();
    inv.type = token.GetNextToken();
    wxString dateStr = token.GetNextToken();
    wxDateTime dateParsed;
    dateParsed.ParseDate(dateStr);
    inv.dateInvested = dateParsed;
    inv.committedAmountDown = wxAtof(token.GetNextToken());
    inv.committedAmountDown = wxAtof(token.GetNextToken());
    inv.calledCapitalUp = wxAtof(token.GetNextToken());
    inv.calledCapitalDown = wxAtof(token.GetNextToken());
    inv.investedAmountUp = wxAtof(token.GetNextToken());
    inv.investedAmountDown = wxAtof(token.GetNextToken());
    inv.waitingDeployUp = wxAtof(token.GetNextToken());
    inv.waitingDeployDown = wxAtof(token.GetNextToken());
    return inv;
}//TODO: guards on csv file to make sure it conforms to the correct strucutre expected

wxVariant Investor::GetValue(int col)const{
    switch(col){
        case 0: return wxVariant(clientName);
        case 1: return wxVariant(type);
        case 2: return wxVariant(dateInvested);
        case 3: return wxVariant(committedAmountUp);
        case 4: return wxVariant(committedAmountDown);
        case 5: return wxVariant(calledCapitalUp);
        case 6: return wxVariant(calledCapitalDown);
        case 7: return wxVariant(investedAmountUp);
        case 8: return wxVariant(investedAmountDown);
        case 9: return wxVariant(waitingDeployUp);
        case 10: return wxVariant(waitingDeployDown);
    }
}

void Investor::SetValue(int col, const wxVariant &v){
    switch(col){
        case 0: clientName = v.GetString();break;
        case 1: type = v.GetString();break;
        case 2: dateInvested = v.GetDateTime();break;
        case 3: committedAmountUp = v.GetDouble();break;
        case 4: committedAmountDown = v.GetDouble();break;
        case 5: calledCapitalUp = v.GetDouble();break;
        case 6: calledCapitalDown = v.GetDouble();break;
        case 7: investedAmountUp = v.GetDouble();break;
        case 8: investedAmountDown = v.GetDouble();break;
        case 9: waitingDeployUp = v.GetDouble();break;
        case 10: waitingDeployDown = v.GetDouble();break;
    }
}

void to_json(json&j, const Investor &inv){
    j=json{
        {"Client Name", inv.clientName.ToStdString()},
        {"Type", inv.type.ToStdString()},
        {"Date",inv.dateInvested.FormatISODate()},
        {"Committed Up", inv.committedAmountUp},
        {"Committed Down", inv.committedAmountDown},
        {"Called Up",inv.calledCapitalUp},
        {"Called Down", inv.calledCapitalDown},
        {"Invested Up", inv.investedAmountUp},
        {"Invested Down", inv.investedAmountDown},
        {"Waiting Up", inv.waitingDeployUp},
        {"Waiting Down", inv.waitingDeployDown}
    };
}

void from_json(const json&j, Investor &inv){
    inv.clientName = j["Client Name"].get<std::string>();
    inv.type = j["Type"].get<std::string>();
    wxString dateStr = wxString::FromUTF8(j["Date"].get<std::string>().c_str());
    wxDateTime dateParsed;
    dateParsed.ParseDate(dateStr);
    inv.dateInvested = dateParsed;
    inv.committedAmountUp = j["Committed Up"].get<double>();
    inv.committedAmountDown = j["Committed Down"].get<double>();
    inv.calledCapitalUp = j["Called Up"].get<double>();
    inv.calledCapitalDown = j["Called Down"].get<double>();
    inv.investedAmountUp = j["Invested Up"].get<double>();
    inv.investedAmountDown = j["Invested Down"].get<double>();
    inv.waitingDeployUp = j["Waiting Up"].get<double>();
    inv.waitingDeployDown = j["Waiting Down"].get<double>();
}

std::vector<wxString> Investor::columnNames = {"Client","Type","Date","Committed Up","Committed Down","Called Capital Up","Called Capital Down",
"Invested Amount Up", "Invested Amount Down", "Waiting Deploy Up","Waiting Deploy Down"};

std::vector<int> Investor::columnWidths = {80, 50, 120, 120, 120, 120, 120, 120, 120, 120, 120};
