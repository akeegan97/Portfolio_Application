#include "mainframe.hpp"
#include "chartWidget.hpp"


void MainFrame::setupLayout(){
   
   auto mainSizer = new wxBoxSizer(wxVERTICAL);
   
   wxPanel* a1Panel = new wxPanel(this);
   a1Panel->SetBackgroundColour(wxColour(200, 200, 200));  
   mainSizer->Add(a1Panel, 1, wxEXPAND | wxALL, 20);

 
   wxBoxSizer* middleSizer = new wxBoxSizer(wxHORIZONTAL);
   mainSizer->Add(middleSizer, 8, wxEXPAND | wxALL, 10);

   
   wxBoxSizer* lSideSizer = new wxBoxSizer(wxVERTICAL);

   middleSizer->Add(lSideSizer, 5, wxEXPAND | wxALL, 10);
   // wxPanel* topLSidePanel = new wxPanel(this);
   // topLSidePanel->SetBackgroundColour(wxColor(35, 207, 61));
   // lSideSizer->Add(topLSidePanel, 3, wxEXPAND | wxALL, 10);

   //Adding in All Asset Events
   if(!portfolio.assetEventPtrs.empty()){
      VListControl<std::shared_ptr<AssetEvent>>* allAssetEventVListControl = new VListControl<std::shared_ptr<AssetEvent>>(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
      allAssetEventVListControl->SetBackgroundColour(wxColor(0,0,0));
      allAssetEventVListControl->setItems(portfolio.assetEventPtrs);
      lSideSizer->Add(allAssetEventVListControl,3,wxEXPAND|wxALL, 10);
   }
   //Check to make sure VLC is only created if assetPtrs is not empty if it is skip initializing this VLC and will initialize it on a future
   if(!portfolio.assetPtrs.empty()){
      VListControl<std::shared_ptr<Asset>>* allAssetVListControl = new VListControl<std::shared_ptr<Asset>>(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
      allAssetVListControl->SetBackgroundColour(wxColor(0,0,0));
      //using utility functions from Asset class to updated member vars
      for(auto&assetPtr:portfolio.assetPtrs){
         assetPtr->UpdateDerivedValues();
      }

      allAssetVListControl->setItems(portfolio.assetPtrs);
      lSideSizer->Add(allAssetVListControl, 7, wxEXPAND | wxALL, 10);
   }

  
   wxBoxSizer* rSideSizer = new wxBoxSizer(wxVERTICAL);
   middleSizer->Add(rSideSizer, 5, wxEXPAND | wxALL, 10);
  
   wxPanel* topRSidePanel = new wxPanel(this);
   topRSidePanel->SetBackgroundColour(wxColor(178, 35, 207));
   rSideSizer->Add(topRSidePanel, 1, wxEXPAND | wxALL, 10);
   
   ChartControl* chartControl = new ChartControl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
   chartControl->SetBackgroundColour(wxColor(0,0,0));
   rSideSizer->Add(chartControl, 7, wxEXPAND | wxALL, 10);

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

   rSideSizer->Add(botRSidePanel, 2, wxEXPAND | wxALL, 10);


   
   wxPanel* a3Panel = new wxPanel(this);
   a3Panel->SetBackgroundColour(wxColour(200, 200, 200));  
   auto bottomSizer = new wxBoxSizer(wxVERTICAL);
   bottomSizer->Add(a3Panel, 1, wxEXPAND | wxALL, 20);

   mainSizer->Add(bottomSizer, 1, wxEXPAND | wxALL, 10);  

   this->SetSizer(mainSizer);
   
}
//Desired Layout of the mainpage of application
// +---------------------------------------+
// |                  A1                   | <- 10%
// +---------------------------------------+
// |    LSide         |       RSide        |
// |                  |                    |
// |                  |       TSection     |
// |                  |         20%        |
// |     TSection     +--------------------+
// |       30%        |       MSection     |
// |                  |         50%        |
// |                  +--------------------+
// |     BSection     |       BSection     |
// |       70%        |         30%        |
// +---------------------------------------+
// |                  A3                   | <- 10%
// +---------------------------------------+

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