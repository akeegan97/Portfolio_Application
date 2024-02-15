#ifndef CASHFLOW_HPP
#define CASHFLOW_HPP
#include <wx/datetime.h>
class CashFlow{
    public:
        CashFlow()=default;
        wxDateTime date;
        double amount;
};

#endif