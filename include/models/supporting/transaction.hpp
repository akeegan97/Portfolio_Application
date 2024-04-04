#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP
#include <wx/datetime.h>
#include <json.hpp>
#include "models/entities/position.hpp"


class Position;
class Transaction{
    private:
        wxDateTime m_transactionDate;
        std::string m_investorName;
        double m_amount;
        std::string m_type;
        std::shared_ptr<Position> m_position;//unknown if this is needed could be helpful accumulating transactions per position
    public:
        Transaction(wxDateTime &transactionDate, std::string &investorName, double &amount, std::shared_ptr<Position> position,std::string &type):
        m_transactionDate(transactionDate),m_investorName(investorName),m_amount(amount),m_position(position),m_type(type){
        };
        Transaction()=default;
        wxDateTime GetDate()const;
        double GetAmount()const;
        std::string GetType()const;
};

#endif