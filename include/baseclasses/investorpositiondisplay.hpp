#ifndef INVESTORPOSITIONDISPLAY_HPP
#define INVESTORPOSITIONDISPLAY_HPP
#include <wx/variant.h>
#include "json.hpp"
#include <memory>


class Investor;
class Position;
using json = nlohmann::json;

class InvestorPositionDisplay{
    public:
        std::shared_ptr<Investor> investorPtr;
        std::shared_ptr<Position> positionPtr;
        static std::vector<wxString> columnNames;
        static std::vector<int> columnWidths;

        InvestorPositionDisplay(std::shared_ptr<Investor> investorPtr, std::shared_ptr<Position>positionPtr):
        investorPtr(std::move(investorPtr)),positionPtr(std::move(positionPtr)){}

        wxVariant GetValue(int col)const;
        void SetValue(int col, const wxVariant &v);
};
#endif
