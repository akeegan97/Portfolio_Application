#ifndef INVESTORPOSITIONDISPLAY_HPP
#define INVESTORPOSITIONDISPLAY_HPP
#include <wx/variant.h>
#include "json.hpp"
#include <memory>


class Investor2;
class Position2;
using json = nlohmann::json;

class InvestorPositionDisplay{
    public:
        std::shared_ptr<Position2> positionPtr;
        static std::vector<wxString> columnNames;
        static std::vector<int> columnWidths;

        InvestorPositionDisplay(std::shared_ptr<Position2>positionPtr):
        positionPtr(positionPtr){
        }

        wxVariant GetValue(int col)const;
        void SetValue(int col, const wxVariant &v);
};
#endif
