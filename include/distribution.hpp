#ifndef DISTRIBUTION_HPP
#define DISTRIBUTION_HPP
#include "wx/datetime.h"
#include "json.hpp"
#include <utility>
using json = nlohmann::json;

class Distribution{
    public:
        std::pair<wxDateTime, double> distribution;
        Distribution()=default;
};

void to_json(json &j, const Distribution &distribution);
void from_json(const json &j, Distribution &distribution);

#endif