#ifndef ALLOCATION_HPP
#define ALLOCATION_HPP
#include "models/entities/investor.hpp"
#include "models/entities/position.hpp"
class Investor;
class Position;

class Allocation{
    public:
        Allocation(std::shared_ptr<Investor> investorPtr,const wxDateTime &qEndDate);
        void SetInternals();
        wxVariant GetValue(int col)const;
        static std::vector<int> columnWidths;
        static std::vector<wxString> columnNames;
    private:
        std::shared_ptr<Investor> m_InvestorPtr;
        wxDateTime m_qEndDate;
        double m_NetIncome;
        double m_MgmtFees;
        double m_AmountToDistribute;
};


#endif