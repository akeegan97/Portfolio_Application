#ifndef PROMOTEFEE_HPP
#define PROMOTEFEE_HPP
#include "wx/datetime.h"
#include "utility"
#include "json.hpp"
using json = nlohmann::json;

class PromoteFee{
    public:
        std::pair<wxDateTime, double> promotefee;
        PromoteFee()=default;
};

void to_json(json&j,const PromoteFee &promoteFee);
void from_json(const json&j,PromoteFee &promoteFee);

#endif
