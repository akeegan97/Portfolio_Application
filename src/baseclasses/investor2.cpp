#include "baseclasses/investor2.hpp"

double Investor2::GetPromoteFeePercentage()const{
    return m_managementFeePercentage;
}
double Investor2::GetManagementFeePercentage()const{
    return m_promoteFeePercentage;
}
std::string Investor2::GetName()const{
    return m_clientName.ToStdString();
}
void Investor2::SetInvestorMgmtFee(double &mgmtFee){    
    m_managementFeePercentage = mgmtFee;
}
void Investor2::SetInvestorPromoteFee(double &promoteFee){
    m_promoteFeePercentage = promoteFee;
}
void Investor2::SetInvestorName(wxString &clientName){
    m_clientName = clientName;
}
void Investor2::SetInvestorType(wxString &clientType){
    m_type = clientType;
}
void Investor2::AddPosition(std::shared_ptr<Position2> position){
    m_positions.push_back(position);
}
const std::vector<std::shared_ptr<Position2>> Investor2::GetPositions()const{
    return m_positions;
}
wxVariant Investor2::GetValue(int col)const{
    switch(col){
        case 0: return wxVariant(m_clientName);break;
        case 1: return wxVariant(m_type);break;
        case 2: return wxVariant(m_managementFeePercentage);break;
        case 3: return wxVariant(m_promoteFeePercentage);break;
        default: return wxVariant();break;
    }
}

void Investor2::SetValue(int col, const wxVariant &v){
    switch(col){
        case 0: m_clientName = v.GetString();break;
        case 1: m_type = v.GetString();break;
        case 3: m_managementFeePercentage = v.GetDouble();break;
        case 4: m_promoteFeePercentage = v.GetDouble();break;
    }
}

std::vector<wxString> Investor2::columnNames = {"Name", "Type","Management Fee", "Promote Fee"};
std::vector<int> Investor2::columnWidths = {100,100,150,150};


void from_json(const json&j, Investor2 &investor, Portfolio &port){
    wxString investorName = wxString::FromUTF8(j["Client Name"].get<std::string>());
    investor.SetInvestorName(investorName);
    wxString investorType = wxString::FromUTF8(j["Type"].get<std::string>());
    investor.SetInvestorType(investorType);
    double mgmtfeePercent = j["Management Fee Percentage"].get<double>();
    double promoteFeePercent = j["Promote Fee Percentage"].get<double>();
    investor.SetInvestorMgmtFee(mgmtfeePercent);
    investor.SetInvestorPromoteFee(promoteFeePercent);
}
