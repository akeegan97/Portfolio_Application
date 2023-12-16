#ifndef PROMOTE_HPP
#define PROMOTE_HPP
#include "wx/datetime.h"
#include "json.hpp"
#include <utility>
using json = nlohmann::json;

class Promote{
    public:
        std::pair<wxDateTime, double> promote;
};

void to_json(const Promote &promote, json &j);
void from_json(const json &j, Promote &promote);

#endif