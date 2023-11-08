#ifndef CHARTCONTROL_HPP
#define CHARTCONTROL_HPP
#include <wx/wx.h>
#include <wx/scrolwin.h>  // Include the wxScrolledWindow header

class ChartControl : public wxScrolledWindow { // Inherit from wxScrolledWindow

    public:
        ChartControl(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size);

        std::vector<double> values;
        std::string title;

    private:
        void OnPaint(wxPaintEvent &evt);
        std::tuple<int, double, double> calculateChartSegmentCountAndRange(double origLow, double origHigh);
};

#endif // CHARTCONTROL_HPP
