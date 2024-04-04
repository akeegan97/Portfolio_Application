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