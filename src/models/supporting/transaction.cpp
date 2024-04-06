#include "models/supporting/transaction.hpp"

wxDateTime Transaction::GetDate()const{
    return m_transactionDate;
}
double Transaction::GetAmount()const{
    return m_amount;
}
std::string Transaction::GetType()const{
    return m_type;
}
std::string Transaction::GetNote()const{
    return m_note;
}

wxVariant Transaction::GetValue(int col)const{
    switch(col){
        case 0: return wxVariant(m_transactionDate.FormatISODate());break;
        case 1: return wxVariant(utilities::formatDollarAmount(m_amount));break;
        case 2: return wxVariant(m_type);break;
        case 3: return wxVariant(m_investorName);break;
        case 4: return wxVariant(m_note);break;
        default : return wxVariant();break;
    }
}

std::vector<wxString> Transaction::columnNames ={"Date","Amount","Type","Name","Note"}; 
std::vector<int> Transaction::columnWidths = {150,100,150,150,300};