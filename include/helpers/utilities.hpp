#ifndef UTILITIES_HPP
#define UTILITIES_HPP
#include <wx/datetime.h>
#include <wx/window.h>
#include <wx/font.h>
#include "models/supporting/cashflow.hpp"
#include "models/supporting/distribution.hpp"
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
    bool AreSameQuarter(const wxDateTime &date1, const wxDateTime &date2);
    template <typename T>
    std::string formatDollarAmount(T value);
    template <typename T>
    std::string FormatPercentage(T value);
    void SetFontForWindowAndChildren(wxWindow*window, const wxFont&font);
    void SetBackgroundColorForWindowAndChildren(wxWindow *window, const wxColour&color, const wxColour &foregroundColor);
}

#endif
