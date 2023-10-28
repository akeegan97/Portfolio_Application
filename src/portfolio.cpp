#include "portfolio.hpp"
#include "asset.hpp"


void to_json(json &j, const Portfolio &por) {
    std::vector<json> assetsJson;
    for (const auto& assetPtr : por.assetPtrs) {
        json assetJson;
        to_json(assetJson, *assetPtr);
        assetsJson.push_back(assetJson);
    }
    j["Assets"] = assetsJson;
}


void from_json(const json &j, Portfolio &por) {
    for(const auto &assetJson : j["Assets"]){
        auto asset = std::make_shared<Asset>();
        from_json(assetJson, *asset, por);
        por.assetPtrs.push_back(asset);
    }
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