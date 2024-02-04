#include "helpers/utilities.hpp"


namespace utilities{
    wxDateTime GetQuarterEndDate(wxDateTime &currentDate){
        int year = currentDate.GetYear();

        wxDateTime quarterEnd;
        if (currentDate >= wxDateTime(1, wxDateTime::Jan, year) && currentDate < wxDateTime(1, wxDateTime::Apr, year)) {
            // Q1
            quarterEnd = wxDateTime(31, wxDateTime::Mar, year);
        } else if (currentDate >= wxDateTime(1, wxDateTime::Apr, year) && currentDate < wxDateTime(1, wxDateTime::Jul, year)) {
            // Q2
            quarterEnd = wxDateTime(30, wxDateTime::Jun, year);
        } else if (currentDate >= wxDateTime(1, wxDateTime::Jul, year) && currentDate < wxDateTime(1, wxDateTime::Oct, year)) {
            // Q3
            quarterEnd = wxDateTime(30, wxDateTime::Sep, year);
        } else {
            // Q4
            quarterEnd = wxDateTime(31, wxDateTime::Dec, year);
        }
        return quarterEnd;
    }

    bool IsWithinQuarter(const wxDateTime &date, const wxDateTime &quarterEndDate){
        wxDateTime qStart, qEnd;
        int year = quarterEndDate.GetYear();
        if (quarterEndDate >= wxDateTime(1, wxDateTime::Jan, year) && quarterEndDate < wxDateTime(1, wxDateTime::Apr, year)) {
            // Q1
            qStart = wxDateTime(1, wxDateTime::Jan, year);
            qEnd = wxDateTime(31, wxDateTime::Mar, year);
        } else if (quarterEndDate >= wxDateTime(1, wxDateTime::Apr, year) && quarterEndDate < wxDateTime(1, wxDateTime::Jul, year)) {
            // Q2
            qStart = wxDateTime(1, wxDateTime::Apr, year);
            qEnd = wxDateTime(30, wxDateTime::Jun, year);
        } else if (quarterEndDate >= wxDateTime(1, wxDateTime::Jul, year) && quarterEndDate < wxDateTime(1, wxDateTime::Oct, year)) {
            // Q3
            qStart = wxDateTime(1, wxDateTime::Jul, year);
            qEnd = wxDateTime(30, wxDateTime::Sep, year);
        } else {
            // Q4
            qStart = wxDateTime(1, wxDateTime::Oct, year);
            qEnd = wxDateTime(31, wxDateTime::Dec, year);
        }

        return date.IsBetween(qStart, qEnd);
    }

    wxDateTime GetQuarterStartDate(wxDateTime &date){
        int year = date.GetYear();

        wxDateTime quarterStartDate;
        if (date >= wxDateTime(1, wxDateTime::Jan, year) && date < wxDateTime(1, wxDateTime::Apr, year)) {
            // Q1
            quarterStartDate = wxDateTime(1, wxDateTime::Jan, year);
        } else if (date >= wxDateTime(1, wxDateTime::Apr, year) && date < wxDateTime(1, wxDateTime::Jul, year)) {
            // Q2
            quarterStartDate = wxDateTime(1, wxDateTime::Apr, year);
        } else if (date >= wxDateTime(1, wxDateTime::Jul, year) && date < wxDateTime(1, wxDateTime::Oct, year)) {
            // Q3
            quarterStartDate = wxDateTime(1, wxDateTime::Jul, year);
        } else {
            // Q4
            quarterStartDate = wxDateTime(1, wxDateTime::Oct, year);
        }
        return quarterStartDate;
    }

    wxDateTime GetNextQuarterEndDate(wxDateTime &currentEndDate){
        wxDateTime nextEndingQuarter;
        int year = currentEndDate.GetYear();
        if(currentEndDate.GetMonth()<= wxDateTime::Mar){
            nextEndingQuarter = wxDateTime(30, wxDateTime::Jun, year);
        }else if(currentEndDate.GetMonth()<=wxDateTime::Jun){
            nextEndingQuarter = wxDateTime(30, wxDateTime::Sep, year);
        }else if(currentEndDate.GetMonth()<=wxDateTime::Sep){
            nextEndingQuarter = wxDateTime(31,wxDateTime::Dec, year);
        }else if(currentEndDate.GetMonth()<=wxDateTime::Dec){
            nextEndingQuarter = wxDateTime(31, wxDateTime::Mar, year+1);
        }
        return nextEndingQuarter;
    }
    
    std::pair<wxDateTime, wxDateTime> GetCurrentQuarterDates(const wxDateTime &currentDate){
        int year = currentDate.GetYear();
        wxDateTime quarterStart, quarterEnd;

        if (currentDate >= wxDateTime(1, wxDateTime::Jan, year) && currentDate < wxDateTime(1, wxDateTime::Apr, year)) {
            // Q1
            quarterStart = wxDateTime(1, wxDateTime::Jan, year);
            quarterEnd = wxDateTime(31, wxDateTime::Mar, year);
        } else if (currentDate >= wxDateTime(1, wxDateTime::Apr, year) && currentDate < wxDateTime(1, wxDateTime::Jul, year)) {
            // Q2
            quarterStart = wxDateTime(1, wxDateTime::Apr, year);
            quarterEnd = wxDateTime(30, wxDateTime::Jun, year);
        } else if (currentDate >= wxDateTime(1, wxDateTime::Jul, year) && currentDate < wxDateTime(1, wxDateTime::Oct, year)) {
            // Q3
            quarterStart = wxDateTime(1, wxDateTime::Jul, year);
            quarterEnd = wxDateTime(30, wxDateTime::Sep, year);
        } else {
            // Q4
            quarterStart = wxDateTime(1, wxDateTime::Oct, year);
            quarterEnd = wxDateTime(31, wxDateTime::Dec, year);
        }
        return {quarterStart, quarterEnd};
    }

    double CalculateDaysBetween(const wxDateTime &start, const wxDateTime &end){
        if(end.IsEarlierThan(start)){
            return 0;
        }else{
            wxTimeSpan span = end - start;
            return span.GetDays();
        }
    }

    wxDateTime GetNextQuarterStartDate(wxDateTime &date){
        wxDateTime nextQStartDate;
        int year =date.GetYear();
        if(date.GetMonth()<= wxDateTime::Mar){
            nextQStartDate = wxDateTime(1, wxDateTime::Apr, year);
        }else if(date.GetMonth()<=wxDateTime::Jun){
            nextQStartDate = wxDateTime(1, wxDateTime::Jul, year);
        }else if(date.GetMonth()<=wxDateTime::Sep){
            nextQStartDate = wxDateTime(1,wxDateTime::Oct, year);
        }else if(date.GetMonth()<=wxDateTime::Dec){
            nextQStartDate = wxDateTime(1, wxDateTime::Jan, year+1);
        }
        return nextQStartDate;
    }

}