#ifndef FEE_HPP
#define FEE_HPP
#include "wx/datetime.h"
#include "map"
#include "json.hpp"
using json = nlohmann::json;

class Fee{
    public:
        Fee()=default;

        std::map<wxDateTime, double> managementFeesAsset;
        std::map<wxDateTime, double> promoteFees;
};

//for serialization/deserialization
void to_json(const Fee &fee, json&j);
void from_json(const json&j, Fee &fee);


#endif