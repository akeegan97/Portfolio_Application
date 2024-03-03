#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include "ui/mainframe/mainframe.hpp"
#include "ui/assetpopout/assetpopout.hpp"
#include "ui/investorpopout/investorpopout.hpp"
#include "ui/mainframe/dialogs/addassetdialog.hpp"


void MainFrame::setupLayout(){
   for(auto&asset:portfolio.assetPtrs){
      asset->SetCurrentValue();
      asset->SetPositionValues();
      std::cout<<"Asset Total Commit: "<<asset->GetTotalCommitted();
      for(auto&position:asset->GetPositions()){
         position->TriggerUpdateOfManagementFeeVector();
      }
      asset->TriggerUpdateOfDistributionsForPositions();
   }
   if(!portfolio.allInvestorPtrs.empty()){
         portfolio.PopulateValuationMaps();    
   }
   //main sizer for the page
   auto mainSizer = new wxBoxSizer(wxHORIZONTAL);
   //left sizer for the left side of the page needs to be vertical 
   wxBoxSizer* lSideSizer = new wxBoxSizer(wxVERTICAL);

   allAssetVListControl = new VListControl<std::shared_ptr<Asset>>(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
   allAssetVListControl->SetBackgroundColour(wxColor(0,0,0));

   if(!portfolio.assetPtrs.empty()){
      allAssetVListControl->setItems(portfolio.assetPtrs);
   }
   addAssetButton = new wxButton(this, wxID_ANY, "Add Asset");
   addAssetButton->Bind(wxEVT_BUTTON, &MainFrame::OnAddAsset, this);

   lSideSizer->Add(allAssetVListControl, 4, wxEXPAND | wxALL, 10);
   lSideSizer->Add(addAssetButton, 1, wxEXPAND, 10);
   allAssetVListControl->Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, &MainFrame::OnAssetVLCClick, this);
   allAssetVListControl->Bind(wxEVT_LIST_ITEM_ACTIVATED, &MainFrame::OnAssetVLCClick, this);
   allInvestorVListControl = new VListControl<std::shared_ptr<Investor>>(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
   allInvestorVListControl->SetBackgroundColour(wxColor(0,0,0));

   if(!portfolio.assetPtrs.empty()){
      allInvestorVListControl->setItems(portfolio.allInvestorPtrs);
   }
   allInvestorVListControl->Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, &MainFrame::OnInvestorVLCClick, this);

   lSideSizer->Add(allInvestorVListControl, 4, wxEXPAND | wxALL, 10);
   mainSizer->Add(lSideSizer, 5, wxEXPAND | wxALL, 10);
   wxBoxSizer* rSideSizer = new wxBoxSizer(wxVERTICAL);

   quoteOftheDatePanel = new wxPanel(this);
   quoteOftheDatePanel->SetBackgroundColour(wxColor(0,0,0));
   quoteOfTheDate = new wxStaticText(quoteOftheDatePanel, wxID_ANY, "");
   int widthOfPAnel = quoteOftheDatePanel->GetMaxWidth();

   wxFont quoteFont = wxFont(22, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
   quoteOfTheDate->Wrap(widthOfPAnel);

   quoteOfTheDate->SetFont(quoteFont);
   wxBoxSizer* TopRSiderSizer = new wxBoxSizer(wxVERTICAL);
   TopRSiderSizer->Add(quoteOfTheDate,1, wxEXPAND | wxALL, 15);
   
   rSideSizer->Add(quoteOftheDatePanel, 1, wxEXPAND | wxALL, 10);
   // Create the holder panel
   chartPanelHolderPanel = new wxPanel(this, wxID_ANY);
   chartPanelHolderPanel->SetBackgroundColour(wxColor(0, 0, 0));

   // Create a sizer for the holder panel
   wxBoxSizer* holderSizer = new wxBoxSizer(wxVERTICAL);
   chartPanelHolderPanel->SetSizer(holderSizer);

   // Create the chart panel
   wxChartPanel* chartPanel = new wxChartPanel(chartPanelHolderPanel, wxID_ANY);
   chartPanel->SetBackgroundColour(wxColor(0, 0, 0));

   // If there is an existing chart, delete it
   if (chartPanel->GetChart() != nullptr) {
      delete chartPanel->GetChart();
   }

   // Populate and set the new chart
   Chart* valuationChart = PopulateDrawChart(portfolio);
   if (valuationChart != nullptr) {
      chartPanel->SetChart(valuationChart);
      holderSizer->Add(chartPanel, 1, wxEXPAND); // Add chartPanel to the holder sizer with proportion 1 and expand flag
   }

   // Add chartPanelHolderPanel to the right side sizer
   rSideSizer->Add(chartPanelHolderPanel, 7, wxEXPAND | wxALL, 10);

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
   this->Bind(wxEVT_SIZE, &MainFrame::OnFrameResizeForQuote, this);
   this->SetSizer(mainSizer);
   this->Layout();
   this->Refresh();
   this->Update();
}

void MainFrame::UpdatePortfolioDisplayValues(){
   double totalInvested = portfolio.TotalInvestedCapital();
   std::string formattedTotalInvested = utilities::formatDollarAmount(totalInvested);
   double totalInvestors = portfolio.TotalInvestors();
   double totalValuation_value = portfolio.TotalValuation();
   std::string formattedTotalvaluation = utilities::formatDollarAmount(totalValuation_value);

   totalInvestedText->SetLabel("Total Invested Capital: "+formattedTotalInvested);
   totalInvestedText->SetForegroundColour(wxColor(51, 245, 12));
   totalInvestorCountText->SetLabel(wxString::Format("Total Investors in Fund: %.2f", totalInvestors));
   totalInvestorCountText->SetForegroundColour(wxColor(51, 245, 12));
   totalValuationText->SetLabel("Total Valuation of Fund: "+formattedTotalvaluation);
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

      originalQuoteText = lines[randomIndex];

      quoteOfTheDate->SetLabel(originalQuoteText);
      quoteOfTheDate->SetForegroundColour(wxColor(51,245,12));
      int width = quoteOftheDatePanel->GetClientSize().GetWidth();
      quoteOfTheDate->Wrap(width);

   }else{
      quoteOfTheDate->SetLabel(wxString::Format("Testing"));
      quoteOfTheDate->SetForegroundColour(wxColor(51,245,12));
   }

}

void MainFrame::OnAssetVLCClick(wxListEvent&e){
   long listIndex = e.GetIndex();
   auto& selectedAsset = allAssetVListControl->GetItemAtListIndex(listIndex);
   std::string selectedAssetName = selectedAsset->GetAssetName().ToStdString();
   auto* assetPopout = new AssetPopout(this, selectedAssetName, wxDefaultPosition, wxSize(FromDIP(1200),FromDIP(800)),
   portfolio, selectedAsset);
   assetPopout->SetBackgroundColour(wxColor(0,0,0));
   assetPopout->Show(true);
}

void MainFrame::OnInvestorVLCClick(wxListEvent &e){
   long listIndex = e.GetIndex();
   auto& selectedInvestor = allInvestorVListControl->GetItemAtListIndex(listIndex);
   std::string selectedInvestorName  = selectedInvestor->GetName();
   auto* investorPopout  = new InvestorPopout(this, selectedInvestorName, wxDefaultPosition,wxSize(FromDIP(1200),FromDIP(800)),
   portfolio, selectedInvestor);
   investorPopout->SetBackgroundColour(wxColor(0,0,0));
   investorPopout->Show(true);
}

void MainFrame::OnAssetPopoutClose(wxCommandEvent &e){
   portfolio.PopulateValuationMaps();
   UpdateChart();
   UpdateAssetListControl();
   UpdateInvestorListControl();
   UpdatePortfolioDisplayValues();

   this->Refresh();
}

Chart* MainFrame::PopulateDrawChart(Portfolio &portfolio){
   size_t count = portfolio.valuationVectorPlotting.size();
   double greatestValue=0;
   for(const auto value : portfolio.valuationVectorPlotting){
      greatestValue += value.second;
   }
   if(count == 0 || greatestValue == 0){
      return nullptr;
   }
   double * data = new double[count];
   time_t* times = new time_t[count];

   for(size_t i = 0; i<count;i++){
      data[i] = portfolio.valuationVectorPlotting[i].second;
      times[i] = portfolio.valuationVectorPlotting[i].first.GetTicks();
   }

   TimeSeriesDataset* valuationTimeSeries = new TimeSeriesDataset(data, times, count);

   XYLineRenderer* customColoredLine = new XYLineRenderer();
   wxPen* myPen = new wxPen(wxColor(52, 219, 235),2);
   customColoredLine->SetSeriePen(0,myPen);
   valuationTimeSeries->SetRenderer(customColoredLine);

   XYPlot *xyPlot = new XYPlot();
   xyPlot->AddDataset(valuationTimeSeries);

   wxPen* borderPen = new wxPen(wxColor(51,245,12));
   wxBrush* fillBrush = new wxBrush(wxColor(0,0,0));

   FillAreaDraw* fillArea = new FillAreaDraw(*borderPen, *fillBrush);

   xyPlot->SetDrawGrid(true, true);

   wxPen gridLinePen(wxColor(51,245,12),1, wxPENSTYLE_SOLID);

   NumberAxis *leftAxis = new NumberAxis(AXIS_LEFT);
   leftAxis->SetTitle("Valuations");
   wxColor myColor = wxColor(51,245,12);
   leftAxis->SetTitleColour(myColor);
   leftAxis->SetLabelTextColour(myColor);
   leftAxis->SetMajorGridlinePen(gridLinePen);
   leftAxis->SetMinorGridlinePen(gridLinePen);
   leftAxis->SetLabelPen(gridLinePen);


   DateAxis *bottomAxis = new DateAxis(AXIS_BOTTOM);
   bottomAxis->SetTitle("Valuation Dates");
   bottomAxis->SetTitleColour(myColor);
   bottomAxis->SetLabelTextColour(myColor);
   bottomAxis->SetVerticalLabelText(true);
   bottomAxis->SetDateFormat(wxT("%b-%Y"));
   bottomAxis->SetMajorGridlinePen(gridLinePen);
   bottomAxis->SetLabelPen(*myPen);

   xyPlot->AddAxis(leftAxis);
   xyPlot->AddAxis(bottomAxis);

   xyPlot->LinkDataHorizontalAxis(0,0);
   xyPlot->LinkDataVerticalAxis(0,0);

   xyPlot->SetBackground(fillArea);
   Chart* myChart = new Chart(xyPlot, "Valuations");
   wxString titleText = "Valuation Chart";
   wxFont titleFont = *wxNORMAL_FONT;
   titleFont.SetPointSize(22);

   TextElement* chartTitle = new TextElement(titleText, wxALIGN_CENTER_HORIZONTAL, titleFont);
   chartTitle->SetColour(myColor);

   Header* myHeader = new Header(*chartTitle);
   myChart->SetHeader(myHeader);

   wxPen* chartPen = new wxPen(myColor);
   wxBrush* chartBrush = new wxBrush(wxColor(0,0,0));

   FillAreaDraw* chartFillArea = new FillAreaDraw(*borderPen, *fillBrush);
   myChart->SetBackground(chartFillArea);

   return myChart;

}  

void MainFrame::UpdateChart() {
   chartPanelHolderPanel->DestroyChildren(); // Destroy previous chart panel

   wxBoxSizer* holderSizer = new wxBoxSizer(wxVERTICAL);
   chartPanelHolderPanel->SetSizer(holderSizer);

   Chart *newChart = PopulateDrawChart(portfolio);
   wxChartPanel* newChartPanel = new wxChartPanel(chartPanelHolderPanel, wxID_ANY);
   newChartPanel->SetBackgroundColour(wxColor(0,0,0));
   newChartPanel->SetChart(newChart);

   holderSizer->Add(newChartPanel, 1, wxEXPAND);

   chartPanelHolderPanel->Layout(); 
   this->Layout(); 
}

void MainFrame::UpdateAssetListControl(){
   allAssetVListControl->setItems(portfolio.assetPtrs);
   allAssetVListControl->Update();
   this->Layout();
}

void MainFrame::UpdateInvestorListControl(){
   allInvestorVListControl->setItems(portfolio.allInvestorPtrs);
   allInvestorVListControl->Update();
   this->Layout();
}

void MainFrame::OnFrameResizeForQuote(wxSizeEvent &e){
   e.Skip();

   quoteOfTheDate->SetLabel(originalQuoteText); 
   int newWidth = quoteOftheDatePanel->GetClientSize().GetWidth();

   quoteOfTheDate->Wrap(newWidth);
   
   quoteOftheDatePanel->Layout();
   quoteOfTheDate->Refresh();
   quoteOfTheDate->Update();
}

void MainFrame::OnAddAsset(wxCommandEvent &e){
   AddAssetDialog dialog(this);
   int retValue = dialog.ShowModal();
   if(retValue == wxID_OK){
      wxDateTime newAssetExitDate = dialog.GetExitDate();
      wxString newAssetName = dialog.GetAssetName();
      wxString newAssetSponser = dialog.GetAssetSponser();
      Asset newAsset(newAssetName, newAssetSponser, newAssetExitDate);
      std::shared_ptr<Asset> newAssetPtr = std::make_shared<Asset>(newAsset);
      portfolio.AddAsset(newAssetPtr);
      allAssetVListControl->setItems(portfolio.assetPtrs);
      this->Refresh();
   }
}