#include "mainframe.hpp"


void MainFrame::setupLayout(){

   auto mainSizer = new wxBoxSizer(wxVERTICAL);
   wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
   wxPanel* a1Panel = new wxPanel(this);
   topSizer->Add(a1Panel, wxEXPAND | wxALL, 10);
   mainSizer->Add(topSizer, 1, wxEXPAND | wxALL, 10); // 10 units border

   // Middle section (80%)
   wxBoxSizer* middleSizer = new wxBoxSizer(wxHORIZONTAL);
   mainSizer->Add(middleSizer, 8, wxEXPAND | wxALL, 10);

   // LSide (60% of middle section)
   wxBoxSizer* lSideSizer = new wxBoxSizer(wxVERTICAL);
   middleSizer->Add(lSideSizer, 5, wxEXPAND | wxALL, 10);
   // LSide - Top (30%)
   lSideSizer->Add(new wxPanel(this), 3, wxEXPAND | wxALL, 10);
   // LSide - Bottom (70%)
   lSideSizer->Add(new wxPanel(this), 7, wxEXPAND | wxALL, 10);

   // RSide (40% of middle section)
   wxBoxSizer* rSideSizer = new wxBoxSizer(wxVERTICAL);
   middleSizer->Add(rSideSizer, 5, wxEXPAND | wxALL, 10);
   // RSide - Top (20%)
   rSideSizer->Add(new wxPanel(this), 2, wxEXPAND | wxALL, 10);
   // RSide - Middle (50%)
   rSideSizer->Add(new wxPanel(this), 5, wxEXPAND | wxALL, 10);
   // RSide - Bottom (30%)
   rSideSizer->Add(new wxPanel(this), 3, wxEXPAND | wxALL, 10);

   // Bottom horizontal section (10%)
   wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL);
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
