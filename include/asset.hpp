#ifndef ASSET_HPP
#define ASSET_HPP
#include <wx/datetime.h>
#include "portfolio.hpp"
#include "valuation.hpp"
#include "investor.hpp"
#include "investorpositiondisplay.hpp"
#include "asset_event.hpp"
#include <json.hpp>
using json = nlohmann::json;
class Portfolio;
/**
    *@class Asset
    *@brief Represents an individual asset with associated valuations, investors, and events.
    *
    * This class encapsulates information about a financial asset, including its 
    * name, exit date, valuations over time, and associated investors. It also 
    * provides utility functions to calculate derived values like total invested 
    * capital and the number of investors.
*/
class Asset{
    public:
        wxString assetName;///< The name of the Asset.
        wxDateTime assetExitDate;///< The planned Exit Date.
        double totalInvestedCapital;///<Total Capital Invested in the Asset.
        double countOfInvestors;///<The total number of investors for the Asset.
        double currentValue;///<The Current Value of the asset based off of the latest valuation.
        //Vectors containing related information for the Asset.
        std::vector<Valuation> valuations;///<Historical valuations of the Asset.
        std::vector<std::shared_ptr<InvestorPositionDisplay>> investorsPositionsDisplays;///<Display information for use in our VLC template class shows investor + their position as one row
        std::vector<Investor> investors;///<Investors in the asset.
        std::vector<std::shared_ptr<AssetEvent>> events;///<Events associated with the Asset

        static std::vector<wxString> columnNames;///<Column Names for displaying class in VLC
        static std::vector<int> columnWidths;///<Column widths for displaying class in VLC

        Asset()=default;///<Default Constuctor
        Asset(const wxString &assetName, const wxDateTime &assetExitDate):
            assetName(assetName),assetExitDate(assetExitDate){};///<Constructor with Asset Name and Exit Date Initialization.
        bool operator<(const Asset& other) const {
            return assetName < other.assetName; 
        }///<Comparison operator to compare Asset objects by assetName value.


        wxVariant GetValue(int col)const;///<
        void SetValue(int col, const wxVariant &v);

        //Utility functions to calculate the totalInvestedCapital, countOfInvestors and currentValue

        double CalculateNumberOfInvestors()const;///<Calculates the number of investors in the investors vector.
        double CalculateInvestedCapital()const;///<Calculates the total number of invested capital.
        double CalculateReturnedCapital();
        double CalculateSubscribedCapital();
        double CalculateReserveCapital();
        double CalculatePaidCapital();
        double GetLastValuation()const;///<Gets latest valuation of the Asset.
        void UpdateDerivedValues();///<Updated derived values and sets member variables
};  
//serialization to and from json
void to_json(json &j, const Asset &as);///<Converts Asset to JSON format. 
void from_json(const json&j, Asset &as,Portfolio &porf);///<Converts JSON data to Asset object.

#endif
