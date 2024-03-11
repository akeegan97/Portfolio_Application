#include "models/supporting/valuation.hpp"
#include "helpers/utilities.hpp"

wxVariant Valuation::GetValue(int col)const{
    switch(col){
        case 0:return wxVariant(valuationDate.FormatISODate());
        case 1:return wxVariant(utilities::formatDollarAmount(valuation));
        default: return wxVariant();
    }
}


std::vector<wxString> Valuation::columnNames = {"Valuation Date","Valuation"};
std::vector<int> Valuation::columnWidths = {200,200};

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
