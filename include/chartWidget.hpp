#ifndef CHARTWIDGET_HPP
#define CHARTWIDGET_HPP
#include <wx/wx.h>

class ChartControl : public wxPanel{

    public:
        ChartControl(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size);

        std::vector<double> values;
        std::string title;

    private:
        void OnPaint(wxPaintEvent &evt);
        std::tuple<int, double, double> calculateChartSegmentCountAndRange(double origLow, double origHigh);
};




#endif