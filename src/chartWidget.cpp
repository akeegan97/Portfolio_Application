#include "chartWidget.hpp"
#include <wx/settings.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

ChartControl::ChartControl(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size):
    wxPanel(parent, id, pos, size, wxFULL_REPAINT_ON_RESIZE){
        this->SetBackgroundStyle(wxBG_STYLE_PAINT);
        this->Bind(wxEVT_PAINT, &ChartControl::OnPaint, this);
}


void ChartControl::OnPaint(wxPaintEvent &evt){
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();
    wxGraphicsContext *gc = wxGraphicsContext::Create(dc);

    if(gc && values.size() > 0){
        wxFont titleFont = wxFont(wxNORMAL_FONT->GetPointSize()*2.0,wxFONTFAMILY_DEFAULT,
        wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

        gc->SetFont(titleFont,wxColor(51, 245, 12));

        double titleHeight, titleWidth;

        gc->GetTextExtent(this->title, &titleWidth, &titleHeight);

        const double titleTopBottomMinimumMargin = this->FromDIP(10);

        wxRect2DDouble fullArea{0,0,static_cast<double>(GetSize().GetWidth()), static_cast<double>(GetSize().GetHeight())};

        const double marginX = fullArea.GetSize().GetWidth() / 8.0;
        const double marginTop = std::max(fullArea.GetSize().GetHeight()/8.0, titleTopBottomMinimumMargin*2.0 + titleHeight);
        const double marginBottom = fullArea.GetSize().GetHeight() / 8.0;
        const double labelsToChartAreaMargin = this->FromDIP(10);

        wxRect2DDouble chartArea = fullArea;
        chartArea.Inset(marginX, marginTop, marginX, marginBottom);

        gc->DrawText(this->title, (fullArea.GetSize().GetWidth() - titleWidth)/2.0, (marginTop - titleHeight)/2.0);

        wxAffineMatrix2D normalizedToChartArea{};
        normalizedToChartArea.Translate(chartArea.GetLeft(), chartArea.GetTop());
        normalizedToChartArea.Scale(chartArea.m_width, chartArea.m_height);

        double lowValue = *std::min_element(values.begin(),values.end());
        double highValue = *std::max_element(values.begin(), values.end());

        const auto &[segmentCount, rangeLow, rangeHigh] = calculateChartSegmentCountAndRange(lowValue, highValue);

        double yValueSpan = rangeHigh - rangeLow;

        lowValue = rangeLow;
        highValue = rangeHigh;

        double yLinesCount = segmentCount + 1;

        wxAffineMatrix2D noramlizedToValue{};
        noramlizedToValue.Translate(0, highValue);
        noramlizedToValue.Scale(1,-1);
        noramlizedToValue.Scale(static_cast<double>(values.size()-1), yValueSpan);

        gc->SetPen(wxPen(wxColor(128,128,128)));
        gc->SetFont(*wxNORMAL_FONT,wxColor(51, 245, 12));

        for(int i = 0; i<yLinesCount;i++){
            double normalizedLineY = static_cast<double>(i) / (yLinesCount - 1);
        }


    }
}
