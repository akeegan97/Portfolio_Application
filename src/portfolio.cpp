#include "portfolio.hpp"


void to_json(json &j, const Portfolio &por) {
    j = {
        {"Assets", por.assets},
    };
}

void from_json(const json &j, Portfolio &por) {
    por.assets = j["Assets"].get<std::vector<Asset>>();
}

void Portfolio::SavePortfolioToFile(const Portfolio &portfolio, const std::string &filePath){
    json j;
    to_json(j,portfolio);
    std::ofstream file(filePath,std::ios::trunc);
    file << j.dump(4);
}

Portfolio Portfolio::LoadFromFile(const std::string &filePath){
    std::ifstream file(filePath);
    json j;
    file >> j;
    Portfolio portfolio;
    from_json(j, portfolio);
    return portfolio;
}