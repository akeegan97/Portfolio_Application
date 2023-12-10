#include "valuation.hpp"

wxVariant Valuation::GetValue(int col)const{
    switch(col){
        case 0:return wxVariant(valuationDate);
        case 1:return wxVariant(valuation);
        default: return wxVariant();
    }
}

void Valuation::SetValue(int col, const wxVariant &v){
    switch(col){
        case 0: valuationDate = v.GetDateTime();break;
        case 1: valuation = v.GetDouble();break;
    }
}

std::vector<wxString> Valuation::columnNames = {"Valuation Date","Valuation"};
std::vector<int> Valuation::columnWidths = {100,75};

void to_json(json&j, const Valuation &val){
    j=json{
        {"Valuation Date",val.valuationDate.FormatISODate()},
        {"Valuation Amount",val.valuation}
    };
}

void from_json(const json&j, Valuation &val){
    wxString dateStr = wxString::FromUTF8(j["Valuation Date"].get<std::string>().c_str());
    wxDateTime dateParsed;
    dateParsed.ParseDate(dateStr);
    val.valuationDate = dateParsed;
    val.valuation = j["Valuation Amount"].get<double>();
}

template class std::vector<Valuation>;
