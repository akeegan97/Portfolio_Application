#include "models/supporting/transaction.hpp"
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/file.h>
wxDateTime Transaction::GetDate()const{
    return m_transactionDate;
}
void Transaction::SetDate(wxDateTime &date){
    m_transactionDate = date;
}
double Transaction::GetAmount()const{
    return m_amount;
}
void Transaction::SetAmount(double amount){
    m_amount = amount;
}
std::string Transaction::GetType()const{
    return m_type;
}
void Transaction::SetType(std::string &type){
    m_type=type;
}
std::string Transaction::GetNote()const{
    return m_note;
}
void Transaction::SetNote(std::string &note){
    m_note = note;
}

std::string Transaction::GetName()const{
    return m_investorName;
}

void Transaction::SetName(std::string &name){
    m_investorName = name;
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


void to_json(json &j, const Transaction &transaction){
    j=json{
        {"Transaction Date",transaction.GetDate().FormatISODate()},
        {"Transaction Type",transaction.GetType()},
        {"Transaction Amount",transaction.GetAmount()},
        {"Transaction Name",transaction.GetName()},
        {"Transaction Note",transaction.GetNote()}
    };
}

void from_json(const json&j, Transaction &transaction){
    wxString dateStr = wxString::FromUTF8(j["Transaction Date"].get<std::string>().c_str());
    wxDateTime dateParsed;
    dateParsed.ParseDate(dateStr);
    transaction.SetDate(dateParsed);
    std::string type = j["Transaction Type"].get<std::string>().c_str();
    transaction.SetType(type);
    std::string name = j["Transaction Name"].get<std::string>().c_str();
    transaction.SetName(name);
    double amount = j["Transaction Amount"].get<double>();
    transaction.SetAmount(amount);
    std::string note = j["Transaction Note"].get<std::string>().c_str();
    transaction.SetNote(note);
}

wxString Transaction::ToCSV()const{
    return wxString::Format("%s,%.2f,%s,%s,%s\n",m_transactionDate.FormatISODate(),m_amount,m_type,m_investorName,m_note);
}
