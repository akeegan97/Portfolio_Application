#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include "wx/app.h"
#include "ui/mainframe/mainframe.hpp"
#include "ui/assetpopout/assetpopout.hpp"
#include "ui/investorpopout/investorpopout.hpp"
#include "ui/mainframe/dialogs/addassetdialog.hpp"
#include "ui/assetpopout/dialogs/addinvestordialog.hpp"

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size, Portfolio &port)
      : wxFrame(NULL, wxID_ANY, title, pos, size),
         portfolio(port),
         allAssetVListControl(nullptr),
         totalInvestedText(nullptr),
         totalInvestorCountText(nullptr),
         totalValuationText(nullptr),
         chartPanelHolderPanel(nullptr),
         quoteOfTheDate(nullptr){
            wxFont font = wxFont(12, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
            wxColour foregroundcolor = wxColor(0,0,0);
            wxColour color = wxColor(255,255,255);
            setupLayout();
            ReadPickQuote("../storage/RugenBergQuotes.txt");
            #ifdef __WXMSW__
            utilities::SetBackgroundColorForWindowAndChildren(this, color, foregroundcolor);
            utilities::SetFontForWindowAndChildren(this, font);
            #endif
            UpdatePortfolioDisplayValues();
            Bind(ASSET_POPOUT_CLOSED, &MainFrame::OnAssetPopoutClose, this);
            Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);
         };

void MainFrame::setupLayout(){
   for(auto&asset:portfolio.assetPtrs){
      asset->SetCurrentValue();
      asset->SetPositionValues();
      asset->TriggerUpdateDerivedValues();
   }
   if(!portfolio.allInvestorPtrs.empty()){
         portfolio.PopulateValuationMaps();    
   }
   //main sizer for the page
   auto mainSizer = new wxBoxSizer(wxHORIZONTAL);
   //left sizer for the left side of the page needs to be vertical 
   wxBoxSizer* lSideSizer = new wxBoxSizer(wxVERTICAL);
   auto buttonSizer = new wxBoxSizer(wxHORIZONTAL);

   allAssetVListControl = new VListControl<std::shared_ptr<Asset>>(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

   if(!portfolio.assetPtrs.empty()){
      allAssetVListControl->setItems(portfolio.assetPtrs);
   }

   addAssetButton = new wxButton(this, wxID_ANY, "Add Asset");
   addInvestorButton = new wxButton(this, wxID_ANY, "Add Investor");
   addInvestorButton->Bind(wxEVT_BUTTON, &MainFrame::OnAddInvestor, this);
   addAssetButton->Bind(wxEVT_BUTTON, &MainFrame::OnAddAsset, this);

   lSideSizer->Add(allAssetVListControl, 3, wxEXPAND | wxALL, 10);
   buttonSizer->Add(addAssetButton,1,10);
   buttonSizer->Add(addInvestorButton, 1,10);
   lSideSizer->Add(buttonSizer, 2, wxEXPAND,10);
   allAssetVListControl->Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, &MainFrame::OnAssetVLCClick, this);
   allAssetVListControl->Bind(wxEVT_LIST_ITEM_ACTIVATED, &MainFrame::OnAssetVLCClick, this);
   allInvestorVListControl = new VListControl<std::shared_ptr<Investor>>(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

   if(!portfolio.allInvestorPtrs.empty()){
      allInvestorVListControl->setItems(portfolio.allInvestorPtrs);
   }
   allInvestorVListControl->Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, &MainFrame::OnInvestorVLCClick, this);

   lSideSizer->Add(allInvestorVListControl, 3, wxEXPAND | wxALL, 10);
   mainSizer->Add(lSideSizer, 5, wxEXPAND | wxALL, 10);
   wxBoxSizer* rSideSizer = new wxBoxSizer(wxVERTICAL);

   quoteOftheDatePanel = new wxPanel(this);
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
   botRSidePanel->SetBackgroundColour(wxColor(255,255,255));

   totalInvestedText = new wxStaticText(botRSidePanel, wxID_ANY,"Total Amount Committed: $0.00");
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
#ifdef __WXMAC__
   wxFont font = wxFont(14,wxDEFAULT, wxNORMAL,wxFONTWEIGHT_BOLD, false);
   wxColor bgColor = wxColor(255,255,255);
   wxColor fgColor = wxColor(0, 0, 0);

   allAssetVListControl->SetBackgroundColour(bgColor);
   allAssetVListControl->SetForegroundColour(fgColor);
   allAssetVListControl->SetFont(font);
   allInvestorVListControl->SetBackgroundColour(bgColor);
   allInvestorVListControl->SetForegroundColour(fgColor);
   allInvestorVListControl->SetFont(font);

   addAssetButton->SetBackgroundColour(bgColor);
   addAssetButton->SetForegroundColour(fgColor);
   addAssetButton->SetFont(font);
   addInvestorButton->SetBackgroundColour(bgColor);
   addInvestorButton->SetForegroundColour(fgColor);
   addInvestorButton->SetFont(font);
   botRSidePanel->SetFont(font);
   totalInvestedText->SetFont(font);
   totalInvestedText->SetForegroundColour(fgColor);
   totalInvestorCountText->SetFont(font);
   totalInvestorCountText->SetForegroundColour(fgColor);
   totalInvestedText->SetFont(font);
   totalInvestedText->SetForegroundColour(fgColor);
   totalValuationText->SetFont(font);
   totalValuationText->SetForegroundColour(fgColor);
   botRSidePanel->SetForegroundColour(fgColor);

   this->SetBackgroundColour(bgColor);
   
#endif

   this->Bind(wxEVT_SIZE, &MainFrame::OnFrameResizeForQuote, this);
   this->SetSizer(mainSizer);
   this->Layout();
   this->Refresh();
   this->Update();
}

void MainFrame::UpdatePortfolioDisplayValues(){
   double totalCommmittedCapital = 0;
   for(auto asset:portfolio.assetPtrs){
      totalCommmittedCapital+=asset->GetTotalCommitted();
   }
   std::string formattedTotalInvested = utilities::formatDollarAmount(totalCommmittedCapital);
   double totalInvestors = portfolio.TotalInvestors();
   double totalValuation_value = portfolio.TotalValuation();
   std::string formattedTotalvaluation = utilities::formatDollarAmount(totalValuation_value);

   totalInvestedText->SetLabel("Total Amount Committed: "+formattedTotalInvested);
   totalInvestorCountText->SetLabel(wxString::Format("Total Investors in Fund: %.2f", totalInvestors));
   totalValuationText->SetLabel("Total Valuation of Fund: "+formattedTotalvaluation);
   #ifdef __WXMAC__
   
   #endif
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
   assetPopout->Show(true);
}

void MainFrame::OnInvestorVLCClick(wxListEvent &e){
   long listIndex = e.GetIndex();
   auto& selectedInvestor = allInvestorVListControl->GetItemAtListIndex(listIndex);
   std::string selectedInvestorName  = selectedInvestor->GetName();
   auto* investorPopout  = new InvestorPopout(this, selectedInvestorName, wxDefaultPosition,wxSize(FromDIP(1200),FromDIP(800)),
   portfolio, selectedInvestor);
   investorPopout->Show(true);
}

void MainFrame::OnAssetPopoutClose(wxCommandEvent &e){
   portfolio.PopulateValuationMaps();
   UpdateChart();
   for(auto asset:portfolio.assetPtrs){
      asset->SetCurrentValue();
      asset->SetPositionValues();      
      asset->TriggerUpdateDerivedValues();

   }
   UpdateAssetListControl();
   UpdateInvestorListControl();
   UpdatePortfolioDisplayValues();

   this->Refresh();
}

Chart* MainFrame::PopulateDrawChart(Portfolio &portfolio){
   wxPen *seriePen = new wxPen(wxColor(0,0,0));
   wxPen *borderPen = new wxPen(wxColor(0,0,0));
   wxColor *textColor = new wxColor(5, 45, 247);
   wxBrush* fillBrush = new wxBrush(wxColor(255, 255, 255));
   wxPen gridLinePen(*textColor,1, wxPENSTYLE_SOLID);
   wxCoord(5);

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
   customColoredLine->SetSeriePen(2,seriePen);
   valuationTimeSeries->SetRenderer(customColoredLine);

   XYPlot *xyPlot = new XYPlot();
   xyPlot->AddDataset(valuationTimeSeries);

   FillAreaDraw* fillArea = new FillAreaDraw(gridLinePen, *fillBrush);

   xyPlot->SetDrawGrid(true, true);

   wxFont axisFont = *wxNORMAL_FONT;
   axisFont.SetPointSize(18);
   wxFont labelFont = *wxNORMAL_FONT;
   labelFont.SetPointSize(14);


   NumberAxis *leftAxis = new NumberAxis(AXIS_LEFT);
   leftAxis->SetTitle("Valuations");
   leftAxis->SetTitleFont(axisFont);
   leftAxis->SetTitleColour(*textColor);
   leftAxis->SetLabelTextColour(*textColor);
   leftAxis->SetMajorGridlinePen(gridLinePen);
   leftAxis->SetMinorGridlinePen(gridLinePen);
   leftAxis->SetLabelPen(gridLinePen);
   leftAxis->SetLabelTextFont(labelFont);


   DateAxis *bottomAxis = new DateAxis(AXIS_BOTTOM);
   bottomAxis->SetTitle("Valuation Dates");
   bottomAxis->SetTitleFont(axisFont);
   bottomAxis->SetTitleColour(*textColor);
   bottomAxis->SetLabelTextColour(*textColor);
   bottomAxis->SetVerticalLabelText(true);
   bottomAxis->SetDateFormat(wxT("%b-%Y"));
   bottomAxis->SetMajorGridlinePen(gridLinePen);
   bottomAxis->SetLabelPen(gridLinePen);
   bottomAxis->SetLabelTextFont(labelFont);
   bottomAxis->SetMargins(wxCoord(15),wxCoord(35));

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
   chartTitle->SetColour(*textColor);

   Header* myHeader = new Header(*chartTitle);
   myChart->SetHeader(myHeader);

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
   portfolio.EnsureFundPositionExists();
   AddAssetDialog dialog(this,portfolio);
   int retValue = dialog.ShowModal();
   if(retValue == wxID_OK){
      wxDateTime newAssetExitDate = dialog.GetExitDate();
      wxString newAssetName = dialog.GetAssetName();
      wxString newAssetSponser = dialog.GetAssetSponser();
      if(newAssetName == ""){
         return;
      }
      Asset newAsset(newAssetName, newAssetSponser, newAssetExitDate);
      
      wxDateTime dateInvested = dialog.GetEffectiveDate();
      double paidAmount = dialog.GetPaidAmount();
      double deployedAmount = dialog.GetDeployedAmount();
      double reserveAmount = dialog.GetReserveAmount();
      std::shared_ptr<Position> initializedPosition = std::make_shared<Position>();
      std::shared_ptr<Asset> newAssetPtr = std::make_shared<Asset>(newAsset);
      initializedPosition->SetAssetPtr(newAssetPtr);
      initializedPosition->SetPaid(paidAmount);
      initializedPosition->SetDateInvested(dateInvested);
      std::string investorName = dialog.GetInvestorChoiceName();
      auto associatedInvestor = portfolio.GetInvestorByName(investorName);
      associatedInvestor->AddPosition(initializedPosition);
      initializedPosition->SetInvestorPtr(associatedInvestor);
      auto pair = std::make_pair(dateInvested, deployedAmount);
      initializedPosition->AddMovementDeploy(pair);
      newAssetPtr->DeserializeSetAssetCommittedCapital(paidAmount);
      newAssetPtr->AddMovement(pair);
      newAssetPtr->AddPosition(initializedPosition);
      newAssetPtr->SetDeployedCapital(deployedAmount);
      newAssetPtr->SetReserveCapital(reserveAmount);
      newAssetPtr->SetCurrentValue();
      newAssetPtr->SetPositionValues();
      portfolio.AddAsset(newAssetPtr);
      allAssetVListControl->setItems(portfolio.assetPtrs);
      allInvestorVListControl->setItems(portfolio.allInvestorPtrs);
      portfolio.PopulateValuationMaps();
      UpdateChart();
      UpdatePortfolioDisplayValues();
      if(initializedPosition->GetInvestorPtr()->GetName() != "Fund"){
         initializedPosition->TriggerUpdateOfManagementFeeVector();
      }
      double amountTransaction;
      std::string positionName;
      wxDateTime transactionDate;
      std::string transactionType;
      amountTransaction = paidAmount;
      positionName = initializedPosition->GetInvestorPtr()->GetName();
      transactionDate = dateInvested;
      transactionType = "New Position";
      std::string note = dialog.GetNote();
      Transaction newTransaction(transactionDate,positionName,paidAmount,transactionType,note);
      newAssetPtr->AddNewTransaction(newTransaction);
      double amountToDeploy =deployedAmount;
      std::string name = newAssetName.ToStdString();
      std::string type2 = "To Deploy";
      wxDateTime date = transactionDate;
      Transaction newTransaction2(date,name,amountToDeploy,type2,note);
      newAssetPtr->AddNewTransaction(newTransaction2);
      this->Refresh();
   }
}

void MainFrame::OnAddInvestor(wxCommandEvent &e){
   AddInvestorDialog dialog(this);
   int retValue = dialog.ShowModal();
   if(retValue == wxID_OK){
      wxString name = dialog.GetInvestorName();
      wxString type = dialog.GetInvestorType();
      double promotefee = dialog.GetInvestorPromoteFee();
      double managementFee = dialog.GetInvestorMgmtFee();
      if(name == ""||type ==""){
         return;
      }

      Investor newInvestor(name,type,managementFee, promotefee);
      std::shared_ptr<Investor> newInvestorPtr = std::make_shared<Investor>(newInvestor);

      portfolio.AddInvestor(newInvestorPtr);

      allInvestorVListControl->setItems(portfolio.GetInvestors());
      this->Refresh();
   }
}

void MainFrame::OnClose(wxCloseEvent& event) {
    std::cout << "Closing MainFrame..." << std::endl;
    wxExit();
}