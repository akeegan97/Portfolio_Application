#ifndef DISTRIBUTION_HPP
#define DISTRIBUTION_HPP
#include "wx/datetime.h"
#include "wx/variant.h"
#include "json.hpp"
#include <utility>
using json = nlohmann::json;

class Distribution{
    public:
        std::pair<wxDateTime, double> distribution;
        Distribution()=default;

        static std::vector<wxString> columnNames;
        static std::vector<int> columnWidths;
        wxVariant GetValue(int col)const;
        void SetValue(int col, const wxVariant &v);
};

void to_json(json &j, const Distribution &distribution);
void from_json(const json &j, Distribution &distribution);

#endif