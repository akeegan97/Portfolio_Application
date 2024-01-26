#ifndef PROMOTEFEE_HPP
#define PROMOTEFEE_HPP
#include "wx/datetime.h"
#include "wx/variant.h"
#include "utility"
#include "json.hpp"
using json = nlohmann::json;

class PromoteFee{
    public:
        std::pair<wxDateTime, double> promotefee;
        PromoteFee()=default;
        static std::vector<wxString> columnNames;
        static std::vector<int> columnWidths;
        wxVariant GetValue(int col)const;
        void SetValue(int col, const wxVariant &v);
};

void to_json(json&j,const PromoteFee &promoteFee);
void from_json(const json&j,PromoteFee &promoteFee);

#endif
