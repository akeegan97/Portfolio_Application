#ifndef INVESTOREDIT_HPP
#define INVESTOREDIT_HPP
#include "investor.hpp"
using json = nlohmann::json;

class InvestorEdit{
    public:

        wxDateTime editDate;
        wxString type;
        wxString notes;

        static std::vector<wxString> columnNames;
        static std::vector<int> columnWidths;


        InvestorEdit()=default;
        InvestorEdit(const wxDateTime &editDate, const wxString &type, const wxString &notes):
                    editDate(editDate),type(type),notes(notes){};
        
        wxVariant GetValue(int col)const;
        void SetValue(int col, const wxVariant &v);
};
void to_json(json&j, const InvestorEdit &ie);
void from_json(const json&j, InvestorEdit &ie);
#endif