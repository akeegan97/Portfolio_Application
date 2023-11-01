#include "mainframe.hpp"


void MainFrame::setupLayout(){

   auto mainSizer = new wxBoxSizer(wxVERTICAL);
   
   wxPanel* a1Panel = new wxPanel(this);
   a1Panel->SetBackgroundColour(wxColour(200, 200, 200));  
   mainSizer->Add(a1Panel, 1, wxEXPAND | wxALL, 20);

 
   wxBoxSizer* middleSizer = new wxBoxSizer(wxHORIZONTAL);
   mainSizer->Add(middleSizer, 8, wxEXPAND | wxALL, 10);

   
   wxBoxSizer* lSideSizer = new wxBoxSizer(wxVERTICAL);

   middleSizer->Add(lSideSizer, 5, wxEXPAND | wxALL, 10);
   wxPanel* topLSidePanel = new wxPanel(this);
   topLSidePanel->SetBackgroundColour(wxColor(35, 207, 61));
   lSideSizer->Add(topLSidePanel, 3, wxEXPAND | wxALL, 10);
  
   VListControl<std::shared_ptr<Asset>>* allAssetVListControl = new VListControl<std::shared_ptr<Asset>>(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
   allAssetVListControl->SetBackgroundColour(wxColor(207, 35, 141));
   allAssetVListControl->setItems(portfolio.assetPtrs);
   lSideSizer->Add(allAssetVListControl, 7, wxEXPAND | wxALL, 10);

  
   wxBoxSizer* rSideSizer = new wxBoxSizer(wxVERTICAL);
   middleSizer->Add(rSideSizer, 5, wxEXPAND | wxALL, 10);
  
   wxPanel* topRSidePanel = new wxPanel(this);
   topRSidePanel->SetBackgroundColour(wxColor(178, 35, 207));
   rSideSizer->Add(topRSidePanel, 2, wxEXPAND | wxALL, 10);
   
   wxPanel* midRSidePanel = new wxPanel(this);
   midRSidePanel->SetBackgroundColour(wxColor(242, 153, 212));
   rSideSizer->Add(midRSidePanel, 5, wxEXPAND | wxALL, 10);
   
   wxPanel* botRSidePanel = new wxPanel(this);
   botRSidePanel->SetBackgroundColour(wxColor(82, 227, 159));
   rSideSizer->Add(botRSidePanel, 3, wxEXPAND | wxALL, 10);

   
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
