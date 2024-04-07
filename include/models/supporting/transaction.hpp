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
        std::string m_note;
    public:
        Transaction(wxDateTime &transactionDate, std::string &investorName, double &amount,std::string &type,std::string &note):
        m_transactionDate(transactionDate),m_investorName(investorName),m_amount(amount),m_type(type),m_note(note){
        };
        Transaction()=default;
        wxDateTime GetDate()const;
        void SetDate(wxDateTime &date);
        double GetAmount()const;
        void SetAmount(double amount);
        std::string GetType()const;
        void SetType(std::string &type);
        std::string GetNote()const;
        void SetNote(std::string &note);
        wxVariant GetValue(int col)const;
        std::string GetName()const;
        void SetName(std::string &name);
        static std::vector<wxString> columnNames;
        static std::vector<int> columnWidths;
        wxString ToCSV()const;
};

void to_json(json &j, const Transaction &transaction);
void from_json(const json&j, Transaction &transaction);

#endif