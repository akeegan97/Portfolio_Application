#ifndef FEEDISPLAY_HPP
#define FEEDISPLAY_HPP
#include <wx/variant.h>
#include "fee.hpp"

class FeeDisplay{
    public:
        std::shared_ptr<Fee> feePtr;
        static std::vector<wxString> columnNames;
        static std::vector<int> columnwidths;

        FeeDisplay(std::shared_ptr<Fee> feePtr):
            feePtr(std::move(feePtr)){
        }

        wxVariant GetValue(int col)const;
        void SetValue(int col, const wxVariant &v);
};

#endif