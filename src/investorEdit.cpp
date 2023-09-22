#include "investorEdit.hpp"

wxVariant InvestorEdit::GetValue(int col)const{
    switch(col){
        case 0: return wxVariant(editDate);
        case 1: return wxVariant(type);
        case 2: return wxVariant(notes);
        default: return wxVariant();  
    }
}

void InvestorEdit::SetValue(int col, const wxVariant &v){
    switch(col){
        case 1: editDate = v.GetDateTime();break;
        case 2: type = v.GetString();break;
        case 3: notes = v.GetString();break;
    }
}

std::vector<wxString> InvestorEdit::columnNames = {"Edited Date", "Type", "Notes"};
std::vector<int> InvestorEdit::columnWidths = {100,80,200};

void to_json(json&j, const InvestorEdit &ie){
    j=json{
        {"Edited Date",ie.editDate.FormatISODate()},
        {"Type",ie.type.ToStdString()},
        {"Notes",ie.notes.ToStdString()}
    };
}

void from_json(const json &j, InvestorEdit &ie){
    wxString dateStr = wxString::FromUTF8(j["Edited Date"].get<std::string>().c_str());
    wxDateTime dateParsed;
    dateParsed.ParseDate(dateStr);
    ie.editDate = dateParsed;
    ie.type = j["Type"].get<std::string>().c_str();
    ie.notes = j["Notes"].get<std::string>().c_str();
}
