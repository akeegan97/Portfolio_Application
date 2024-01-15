#ifndef FEEDISPLAY_HPP
#define FEEDISPLAY_HPP
#include <wx/variant.h>
#include "managementFee.hpp"

class FeeDisplay{
    public:
        std::shared_ptr<ManagementFee> managementFeePtr;
        static std::vector<wxString> columnNames;
        static std::vector<int> columnwidths;

        FeeDisplay(std::shared_ptr<ManagementFee> managementFeePtr):
            managementFeePtr(std::move(managementFeePtr)){
        }

        wxVariant GetValue(int col)const;
        void SetValue(int col, const wxVariant &v);
};

#endif