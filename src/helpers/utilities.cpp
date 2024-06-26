#include "helpers/utilities.hpp"
#include "helpers/vlistcontrol.hpp"

class CustomControlBase;


namespace utilities{
    wxDateTime GetQuarterEndDate(const wxDateTime &currentDate){
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

    wxDateTime GetQuarterStartDate(const wxDateTime &date){
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

    wxDateTime GetNextQuarterEndDate(const wxDateTime &currentEndDate){
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

    wxDateTime GetNextQuarterStartDate(const wxDateTime &date){
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

    bool AreSameQuarter(const wxDateTime &date1, const wxDateTime &date2){
        int year1 = date1.GetYear();
        int year2 = date2.GetYear();

        int month1 = date1.GetMonth()+1;
        int month2 = date2.GetMonth()+1;

        int q1 = (month1-1)/3 + 1;
        int q2 = (month2-1)/3 + 1;

        return year1 == year2 && q1==q2;
    }

    template <typename T>
    std::string formatDollarAmount(T value) {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << std::abs(value);
        std::string formatted = stream.str();

        size_t dotPos = formatted.find('.');
        std::string intPart = formatted.substr(0, dotPos);
        std::string decimalPart = formatted.substr(dotPos + 1);

        std::string formattedWithCommas;
        int count = 0;
        for (auto it = intPart.rbegin(); it != intPart.rend(); ++it) {
            if (count == 3) {
                formattedWithCommas.push_back(',');
                count = 0;
            }
            count++;
            formattedWithCommas.push_back(*it);
        }

        std::reverse(formattedWithCommas.begin(), formattedWithCommas.end());
        
        std::string prefix = value < 0 ? "-$" : "$";
        return prefix + formattedWithCommas + '.' + decimalPart;
    }

    template <typename T>
    std::string FormatPercentage(T value){
        std::stringstream stream;
        value = value *100;
        stream <<std::fixed<<std::setprecision(5)<<std::abs(value);
        std::string formatted = stream.str();
        if(value < 0){
            return "-"+formatted + "%";
        }else{
            return formatted + "%";    
        }
    }

    void SetFontForWindowAndChildren(wxWindow*window, const wxFont&font){
        CustomControlBase *customCtrl  = dynamic_cast<CustomControlBase*>(window);
        if(customCtrl && customCtrl->IsVListCtrl()){
            window->SetFont(font);
        }else{
            window->SetFont(font);
        }
        const wxWindowList &children = window->GetChildren();
        for(wxWindowList::const_iterator it = children.begin(); it!=children.end();++it){
            wxWindow* child = *it;
            SetFontForWindowAndChildren(child, font);
        }
    }
    void SetBackgroundColorForWindowAndChildren(wxWindow* window, const wxColour& color,const wxColour &foregroundColor) {
        CustomControlBase *customCtrl = dynamic_cast<CustomControlBase*>(window);
        if(customCtrl && customCtrl->IsVListCtrl()){
            wxColor vlcBackGroundColor =  wxColor(255,255,255);
            window->SetBackgroundColour(vlcBackGroundColor);
            window->SetForegroundColour(wxColor(0,0,0));
        }else{
            window->SetBackgroundColour(color); 
            window->SetForegroundColour(foregroundColor);
            window->Refresh();
        }
        window->Refresh();
        const wxWindowList& children = window->GetChildren();
        for (wxWindowList::const_iterator it = children.begin(); it != children.end(); ++it) {
            wxWindow* child = *it;
            SetBackgroundColorForWindowAndChildren(child, color,foregroundColor); 
        }
    }
    bool HasCurrency(const wxString &value){
        return value.find('$') != wxString::npos;
    }
    bool HasPercent(const wxString &value){
        return value.find('%') != wxString::npos;
    }
    double CurrencyStringToDouble(const wxString &value){
        std::string stlString = value.ToStdString();
        stlString.erase(std::remove(stlString.begin(),stlString.end(),'$'),stlString.end());
        stlString.erase(std::remove(stlString.begin(),stlString.end(),','),stlString.end());
        return std::stod(stlString);
    }
    double PercentageStringToDouble(const wxString &value){
        std::string stlString = value.ToStdString();
        stlString.erase(std::remove(stlString.begin(),stlString.end(),'%'),stlString.end());
        return std::stod(stlString);
    }
    bool IsQEndDate(const wxDateTime& date) {
        int year = date.GetYear();
        return (date == wxDateTime(31, wxDateTime::Mar, year) ||
                date == wxDateTime(30, wxDateTime::Jun, year) ||
                date == wxDateTime(30, wxDateTime::Sep, year) ||
                date == wxDateTime(31, wxDateTime::Dec, year));
    }
}
template std::string utilities::formatDollarAmount<double>(double);
template std::string utilities::FormatPercentage<double>(double);

