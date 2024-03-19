#ifndef MANAGEMENTFEE_HPP
#define MANAGEMENTFEE_HPP
#include "wx/datetime.h"
#include "wx/variant.h"
#include <utility>
#include "json.hpp"
using json = nlohmann::json;

class ManagementFee{
    public:
        ManagementFee()=default;
        wxDateTime quarter;
        double amount;
        bool paid;
        static std::vector<wxString> columnNames;
        static std::vector<int> columnWidths;
        wxVariant GetValue(int col)const;

};

void to_json(json&j,const ManagementFee &fee);
void from_json(const json&j, ManagementFee &fee);


#endif