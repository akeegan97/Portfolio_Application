#ifndef UTILITIES_HPP
#define UTILITIES_HPP
#include <wx/datetime.h>
#include "baseclasses/cashflow.hpp"
#include "baseclasses/investor2.hpp"
#include "baseclasses/distribution.hpp"
#include <math.h>
#include <sstream>
#include <iomanip>
#include <algorithm>


namespace utilities{

    wxDateTime GetQuarterEndDate(wxDateTime &currentDate);
    bool IsWithinQuarter(const wxDateTime &date, const wxDateTime &quarterEndDate);
    wxDateTime GetQuarterStartDate(wxDateTime &date);
    wxDateTime GetNextQuarterEndDate(wxDateTime &currentEndDate);
    wxDateTime GetNextQuarterStartDate(wxDateTime &date);
    std::pair<wxDateTime, wxDateTime> GetCurrentQuarterDates(const wxDateTime &currentDate);
    double CalculateDaysBetween(const wxDateTime &start, const wxDateTime &end);
    void SortDistributions(std::vector<Distribution> &distributions);
    template <typename T>
    std::string formatDollarAmount(T value);
}

#endif
