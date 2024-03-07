#ifndef INVESTOR_HPP
#define INVESTOR_HPP
#include <memory>
#include <wx/string.h>
#include <wx/variant.h>
#include <wx/datetime.h>
#include <wx/wxcrt.h>
#include <json.hpp>

#include "models/entities/portfolio.hpp"
#include "models/entities/asset.hpp"
#include "models/entities/position.hpp"
#include "models/components/investorpositiondisplay.hpp"
#include "models/components/investorassetdisplay.hpp"
class Position;
class Portfolio;
class InvestorAssetDisplay;

class Investor{
    private:
        wxString m_clientName;
        wxString m_type;
        double m_managementFeePercentage;
        double m_promoteFeePercentage;
        std::vector<std::shared_ptr<Position>> m_positions;
        std::vector<std::shared_ptr<InvestorAssetDisplay>> m_assetDisplays;
        //
    public:
        Investor()=default;
        Investor(const wxString &clientName, const wxString &type, const double &mgmtFeePercent, const double &promoteFeePercent):
        m_clientName(clientName),m_type(type),m_managementFeePercentage(mgmtFeePercent),m_promoteFeePercentage(promoteFeePercent){};
    //Methods to Interact with VLC
        wxVariant GetValue(int col)const;
        void SetValue(int col, const wxVariant  &v);
        static std::vector<wxString> columnNames;
        static std::vector<int> columnWidths;
    //Public Getters
        double GetPromoteFeePercentage()const;
        double GetManagementFeePercentage()const;
        std::string GetName()const;
        const std::vector<std::shared_ptr<Position>> GetPositions()const;
        std::string GetType()const;
        std::vector<std::shared_ptr<InvestorAssetDisplay>> GetAssetDisplaysNonConst();
    //public setters
        void SetInvestorName(wxString &clientName);
        void SetInvestorType(wxString &clientType);
        void SetInvestorMgmtFee(double &mgmtFee);
        void SetInvestorPromoteFee(double &promoteFee);
        void AddPosition(std::shared_ptr<Position> position);
        void AddPositionDisplay(std::shared_ptr<InvestorAssetDisplay> positionDisplay);
        void ClearPositionDisplays();


};
void to_json(json &j, const Investor &inv);
void from_json(const json &j, Investor &inv, Portfolio &port);
#endif 