#ifndef MANAGEMENTFEE_HPP
#define MANAGEMENTFEE_HPP
#include "wx/datetime.h"
#include <utility>
#include "json.hpp"
using json = nlohmann::json;

class ManagementFee{
    public:
        ManagementFee()=default;
        std::pair<wxDateTime, double> managementFeesAsset;
};

void to_json(const ManagementFee &fee, json&j);
void from_json(const json&j, ManagementFee &fee);


#endif