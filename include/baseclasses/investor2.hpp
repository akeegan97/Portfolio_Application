#ifndef INVESTOR2_HPP
#define INVESTOR2_HPP
#include <memory>
#include <wx/string.h>
#include <wx/variant.h>
#include <wx/datetime.h>
#include <wx/wxcrt.h>
#include <json.hpp>
#include "baseclasses/portfolio.hpp"
#include <baseclasses/asset2.hpp>
#include "baseclasses/position2.hpp"
#include "baseclasses/investorpositiondisplay.hpp"
class Position2;
class Portfolio;

class Investor2{
    private:
        wxString m_clientName;
        wxString m_type;
        double m_managementFeePercentage;
        double m_promoteFeePercentage;
        std::vector<std::shared_ptr<Position2>> m_positions;
        //
    public:
        Investor2()=default;
        Investor2(const wxString &clientName, const wxString &type, const double &mgmtFeePercent, const double &promoteFeePercent):
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
        const std::vector<std::shared_ptr<Position2>> GetPositions()const;
    //public setters
        void SetInvestorName(wxString &clientName);
        void SetInvestorType(wxString &clientType);
        void SetInvestorMgmtFee(double &mgmtFee);
        void SetInvestorPromoteFee(double &promoteFee);
        void AddPosition(std::shared_ptr<Position2> position);

};
void to_json(json &j, const Investor2 &inv);
void from_json(const json &j, Investor2 &inv, Portfolio &port);
#endif 