#ifndef INVESTORPOSITIONDISPLAY_HPP
#define INVESTORPOSITIONDISPLAY_HPP

#include "investor.hpp"
#include "position.hpp"
#include <wx/variant.h>

class InvestorPositionDisplay{
    private:
        Investor& investorRef;
        Position& positionRef;
    public:
        static std::vector<wxString> columnNames;
        static std::vector<int> columnwidths;

        InvestorPositionDisplay(Investor&investorRef, Position&positionRef):
        investorRef(investorRef),positionRef(positionRef){}

        wxVariant GetValue(int col)const;
        void SetValue(int col, const wxVariant &v);
};

#endif
