#ifndef INVESTOR_HPP
#define INVESTOR_HPP

#include <wx/string.h>
#include <wx/variant.h>
#include <wx/datetime.h>
#include <wx/tokenzr.h>
#include <wx/wxcrt.h>
#include <json.hpp>
using json = nlohmann::json;


class Investor{
    public:
        wxString clientName;
        wxString type;
        wxDateTime dateInvested;
        double committedAmountUp;
        double committedAmountDown;
        double calledCapitalUp;
        double calledCapitalDown;
        double investedAmountUp;
        double investedAmountDown;
        double waitingDeployUp;
        double waitingDeployDown;

        static std::vector<wxString> columnNames;
        static std::vector<int> columnWidths;

        Investor()=default;
        Investor(const wxString &clientName, const wxString &type, const wxDateTime &dateInvested, const double &committedAmountUp, const double &committedAmountDown, const double &calledCapitalUp,
                const double &calledCapitalDown, const double &investedAmountUp, const double &investedAmountDown, const double &waitingDeployUp, const double &waitingDeployDown):
                clientName(clientName),type(type),dateInvested(dateInvested),committedAmountUp(committedAmountUp),committedAmountDown(committedAmountDown),calledCapitalUp(calledCapitalUp),
                calledCapitalDown(calledCapitalDown),investedAmountUp(investedAmountUp),investedAmountDown(investedAmountDown),waitingDeployUp(waitingDeployUp),waitingDeployDown(waitingDeployDown){};
        
        static Investor fromCsv(const wxString line);
        wxVariant GetValue(int col)const;
        void SetValue(int col, const wxVariant &v);
};

void to_json(json &j, const Investor &inv);
void from_json(const json &j, Investor &inv);
#endif