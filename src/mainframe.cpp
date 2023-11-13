#include "mainframe.hpp"


void MainFrame::setupLayout(){
   //main sizer for the page
   auto mainSizer = new wxBoxSizer(wxHORIZONTAL);
   //left sizer for the left side of the page needs to be vertical 
   wxBoxSizer* lSideSizer = new wxBoxSizer(wxVERTICAL);
   //Adding in All Asset Events
   if(!portfolio.assetEventPtrs.empty()){
      VListControl<std::shared_ptr<AssetEvent>>* allAssetEventVListControl = new VListControl<std::shared_ptr<AssetEvent>>(this, wxID_ANY, FromDIP(wxDefaultPosition), FromDIP(wxDefaultSize));
      allAssetEventVListControl->SetBackgroundColour(wxColor(0,0,0));
      allAssetEventVListControl->setItems(portfolio.assetEventPtrs);
      lSideSizer->Add(allAssetEventVListControl,4,wxEXPAND | wxALL, 10);
   }
   //   Check to make sure VLC is only created if assetPtrs is not empty if it is skip initializing this VLC and will initialize it on a future
   if(!portfolio.assetPtrs.empty()){
      VListControl<std::shared_ptr<Asset>>* allAssetVListControl = new VListControl<std::shared_ptr<Asset>>(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
      allAssetVListControl->SetBackgroundColour(wxColor(0,0,0));
      //using utility functions from Asset class to updated member vars
      for(auto&assetPtr:portfolio.assetPtrs){
         assetPtr->UpdateDerivedValues();
      }

      allAssetVListControl->setItems(portfolio.assetPtrs);
      lSideSizer->Add(allAssetVListControl, 6, wxEXPAND | wxALL, 10);
   }
   //add left side elements to sizer and left sizer to the mainSizer
   mainSizer->Add(lSideSizer, 5, wxEXPAND | wxALL, 10);
   //creating the right side sizer
   wxBoxSizer* rSideSizer = new wxBoxSizer(wxVERTICAL);

   //Where the Quote of the day panel will go
   wxPanel* topRSidePanel = new wxPanel(this);
   topRSidePanel->SetBackgroundColour(wxColor(0,0,0));
   quoteOfTheDate = new wxStaticText(topRSidePanel, wxID_ANY, "");
   wxBoxSizer* TopRSiderSizer = new wxBoxSizer(wxVERTICAL);
   TopRSiderSizer->Add(quoteOfTheDate,1, wxEXPAND | wxALL, 15);
   
   rSideSizer->Add(topRSidePanel, 1, wxEXPAND | wxALL, 10);

   size_t count = portfolio.valuationVectorPlotting.size();
   double* data = new double[count];
   time_t* times = new time_t[count];

   for(size_t i = 0; i<count;++i){
      data[i] = portfolio.valuationVectorPlotting[i].second;
      times[i] = portfolio.valuationVectorPlotting[i].first.GetTicks();
   }

   TimeSeriesDataset* valuationTimeSeries = new TimeSeriesDataset(data, times, count);
   XYLineRenderer* customcoloredLine = new XYLineRenderer();
   wxPen* myPen = new wxPen(wxColor(51,245,12));
   customcoloredLine->SetSeriePen(0, myPen);
   valuationTimeSeries->SetRenderer(customcoloredLine);

   XYPlot *xyPlot = new XYPlot();
   xyPlot->AddDataset(valuationTimeSeries);

   wxPen* borderPen = new wxPen(wxColor(51,245,12));
   wxBrush* fillBrush = new wxBrush(wxColor(0,0,0));

   FillAreaDraw* fillarea = new FillAreaDraw(*borderPen, *fillBrush);

   xyPlot->SetDrawGrid(true, true);

   wxPen gridlinePen(wxColor(51,245,12),1,wxPENSTYLE_SOLID);

   NumberAxis *leftAxis = new NumberAxis(AXIS_LEFT);
   leftAxis->SetTitle("Valuations");
   wxColor myColor =  wxColor(51,245,12);
   leftAxis->SetTitleColour(myColor);
   leftAxis->SetLabelTextColour(myColor);
   leftAxis->SetMajorGridlinePen(gridlinePen);
   leftAxis->SetMinorGridlinePen(gridlinePen);
   leftAxis->SetLabelPen(*myPen);
   
   DateAxis *bottomAxis = new DateAxis(AXIS_BOTTOM);
   bottomAxis->SetTitle("Dates");
   bottomAxis->SetTitleColour(myColor);
   bottomAxis->SetLabelTextColour(myColor);

   bottomAxis->SetVerticalLabelText(true);
   bottomAxis->SetDateFormat(wxT("%d-%m"));
   bottomAxis->SetMajorGridlinePen(gridlinePen);
   bottomAxis->SetLabelPen(*myPen);

   xyPlot->AddAxis(leftAxis);
   xyPlot->AddAxis(bottomAxis);

   xyPlot->LinkDataHorizontalAxis(0,0);
   xyPlot->LinkDataVerticalAxis(0,0);

   xyPlot->SetBackground(fillarea);
   Chart* myChart = new Chart(xyPlot, "Valuations");
   wxString titleText = "Valuation Chart";
   wxFont titleFont = *wxNORMAL_FONT; 
   titleFont.SetPointSize(22);

   TextElement* chartTitle = new TextElement(titleText, wxALIGN_CENTER_HORIZONTAL, titleFont);
   chartTitle->SetColour(wxColour(51, 245, 12)); 

   Header* myHeader = new Header(*chartTitle);
   myChart->SetHeader(myHeader);

   wxPen* chartPen = new wxPen(wxColor(51,245,12));
   wxBrush* chartBrush = new wxBrush(wxColor(0,0,0));

   FillAreaDraw* chartFillArea = new FillAreaDraw(*borderPen, *fillBrush);
   myChart->SetBackground(chartFillArea);
   chartPanel = new wxChartPanel(this, wxID_ANY);
   chartPanel->SetChart(myChart);
   //add valuation chart to the right side panel here
   rSideSizer->Add(chartPanel, 7, wxEXPAND | wxALL, 10);

   wxPanel* botRSidePanel = new wxPanel(this);
   botRSidePanel->SetBackgroundColour(wxColor(0,0,0));

   totalInvestedText = new wxStaticText(botRSidePanel, wxID_ANY,"Total Amount Invested: $0.00");
   totalInvestorCountText = new wxStaticText(botRSidePanel, wxID_ANY, "Total Investors in fund: 0");
   totalValuationText = new wxStaticText(botRSidePanel, wxID_ANY, "Total Valuation: $0.00");

   wxBoxSizer* botRSiderSizer = new wxBoxSizer(wxVERTICAL);
   botRSiderSizer->Add(totalInvestedText, 1, wxEXPAND|wxALL, 5);
   botRSiderSizer->Add(totalInvestorCountText, 1, wxEXPAND| wxALL, 5);
   botRSiderSizer->Add(totalValuationText, 1, wxEXPAND | wxALL, 5);

   botRSidePanel->SetSizer(botRSiderSizer);
   //add the bottom right side panel which houses the static text lines
   rSideSizer->Add(botRSidePanel, 2, wxEXPAND | wxALL, 10);
   //add right side sizer to the main sizer
   mainSizer->Add(rSideSizer, 5, wxEXPAND | wxALL,10);
   mainSizer->Layout();
   //set mainframe sizer to be the main sizer here
   this->SetSizer(mainSizer);
   this->Layout();
}


void MainFrame::UpdatePortfolioDisplayValues(){
   double totalInvested = portfolio.TotalInvestedCapital();
   double totalInvestors = portfolio.TotalInvestors();
   double totalValuation_value = portfolio.TotalValuation();

   totalInvestedText->SetLabel(wxString::Format("Total Invested Capital : $%.2f",totalInvested));
   totalInvestedText->SetForegroundColour(wxColor(51, 245, 12));
   totalInvestorCountText->SetLabel(wxString::Format("Total Investors in Fund: %f",totalInvestors));
   totalInvestorCountText->SetForegroundColour(wxColor(51, 245, 12));
   totalValuationText->SetLabel(wxString::Format("Total Valuation of Fund : $%.2f",totalValuation_value));
   totalValuationText->SetForegroundColour(wxColor(51, 245, 12));
}

void MainFrame::ReadPickQuote(const std::string&filePath){
   std::vector<std::string> lines;
   std::string line;
   std::ifstream file(filePath);
   while(std::getline(file, line)){
      lines.push_back(line);
   }
   if(!lines.empty()){
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<> distrib(0,lines.size()-1);
      int randomIndex = distrib(gen);

      std::string pickedQuote = lines[randomIndex];

      quoteOfTheDate->SetLabel(pickedQuote);
      quoteOfTheDate->SetForegroundColour(wxColor(51,245,12));
   }else{
      quoteOfTheDate->SetLabel(wxString::Format("NO QUOTES FOUND"));
      quoteOfTheDate->SetForegroundColour(wxColor(51,245,12));
   }

}


