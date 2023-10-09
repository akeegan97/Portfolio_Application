#include "portfolio.hpp"


void to_json(json &j, const Portfolio &por) {
    j = {
        {"Assets", por.assets},
        {"Investors", por.investors}
    };
}

void from_json(const json &j, Portfolio &por) {
    por.assets = j["Assets"].get<std::vector<Asset>>();
    por.investors = j["Investors"].get<std::vector<Investor>>();
}