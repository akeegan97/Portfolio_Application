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