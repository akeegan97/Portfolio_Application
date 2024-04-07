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
        bool paid;
        Distribution()=default;

        static std::vector<wxString> columnNames;
        static std::vector<int> columnWidths;
        wxVariant GetValue(int col)const;
};

void to_json(json &j, const Distribution &distribution);
void from_json(const json &j, Distribution &distribution);
void to_json(json &j, const std::pair<Distribution,bool> &qdistribution);
void from_json(const json &j, std::pair<Distribution,bool> &qdistribution);
#endif