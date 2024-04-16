#include "ui/mainframe/dialogs/gettransactionpopout.hpp"
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/file.h>

GetTransactionPopout::GetTransactionPopout(wxWindow* parentWindow,const wxString &title,const wxPoint &pos, const wxSize &size, Portfolio &portfolio):
    wxFrame(NULL,wxID_ANY,title,pos,size),m_portfolio(portfolio){
        SetupLayout();
        wxFont font = wxFont(12, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
        wxColour color = wxColor(255,255,255);
        wxColour foregroundcolor = wxColor(0,0,0);
        #ifdef __WXMSW__
        utilities::SetBackgroundColorForWindowAndChildren(this, color, foregroundcolor);
        utilities::SetFontForWindowAndChildren(this, font);
        #endif
    };

void GetTransactionPopout::SetupLayout(){
    auto mainSizer = new wxBoxSizer(wxVERTICAL);
    auto topSizer = new wxBoxSizer(wxHORIZONTAL);
    auto bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    auto topRSizer = new wxBoxSizer(wxVERTICAL);
    auto topLSidetopSizer = new wxBoxSizer(wxHORIZONTAL);
    auto topLSidebottomSizer = new wxBoxSizer(wxHORIZONTAL);
    auto topLSizer = new wxBoxSizer(wxVERTICAL);

    wxDateTime today = wxDateTime::Today();
    wxDateTime setDate = wxDateTime::Today().Add(wxDateSpan(0,0,0,-90));
    startDate = new wxDatePickerCtrl(this, wxID_ANY,wxDefaultDateTime,wxDefaultPosition,wxSize(125,25));
    endDate = new wxDatePickerCtrl(this, wxID_ANY,wxDefaultDateTime,wxDefaultPosition,wxSize(125,25));
    startDate->SetValue(setDate);
    endDate->SetValue(today);
    wxStaticText *startDateText = new wxStaticText(this, wxID_ANY,"Start Date");
    wxStaticText *endDateText = new wxStaticText(this, wxID_ANY,"End Date");
    topLSidetopSizer->Add(startDateText,1,wxALL,5);
    topLSidetopSizer->Add(startDate, 1, wxALL,5);
    topLSidebottomSizer->Add(endDateText,1,wxALL,5);
    topLSidebottomSizer->Add(endDate,1,wxALL,5);
    topLSizer->Add(topLSidetopSizer,1,wxEXPAND| wxALL,5);
    topLSizer->Add(topLSidebottomSizer,1,wxEXPAND| wxALL,5);
    topSizer->Add(topLSizer,1,wxALL|wxEXPAND,5);

    writeToCsv = new wxButton(this, wxID_ANY,"Export To CSV ../Documents/ Directory");
    writeToCsv->Bind(wxEVT_BUTTON,&GetTransactionPopout::ExecuteFileWrite,this);

    topRSizer->Add(writeToCsv,1,wxALL|wxEXPAND,5);

    refreshVLCButton = new wxButton(this, wxID_ANY,"Refresh Transactions");
    refreshVLCButton->Bind(wxEVT_BUTTON,&GetTransactionPopout::RefreshVLC,this);
    topRSizer->Add(refreshVLCButton,1,wxALL|wxEXPAND,5);

    topSizer->Add(topRSizer,1,wxALL|wxEXPAND,5);

    SetTransactions();

    if(!m_allTransactions.empty()){
        SetDisplayTransactions(setDate,today);
    }

    allAssetsTransactionsVLC = new VListControl<Transaction>(this, wxID_ANY,wxDefaultPosition,wxDefaultSize);
    allAssetsTransactionsVLC->setItems(m_displayTransactions);
    bottomSizer->Add(allAssetsTransactionsVLC,1,wxALL|wxEXPAND,10);

    mainSizer->Add(topSizer,1,wxALL|wxEXPAND,5);
    mainSizer->Add(bottomSizer,2,wxALL|wxEXPAND,5);

    this->SetSizer(mainSizer);
    #ifdef __WXMAC__
        wxFont font = wxFont(14,wxDEFAULT, wxNORMAL,wxFONTWEIGHT_BOLD, false);
        wxColor bgColor = wxColor(255,255,255);
        wxColor fgColor = wxColor(0, 0, 0);

        allAssetsTransactionsVLC->SetBackgroundColour(bgColor);
        allAssetsTransactionsVLC->SetForegroundColour(fgColor);
        allAssetsTransactionsVLC->SetFont(font);

        startDate->SetBackgroundColour(bgColor);
        startDate->SetForegroundColour(fgColor);
        startDate->SetFont(font);

        endDate->SetBackgroundColour(bgColor);
        endDate->SetForegroundColour(fgColor);
        endDate->SetFont(font);

        startDateText->SetForegroundColour(fgColor);
        startDateText->SetFont(font);

        endDateText->SetForegroundColour(fgColor);
        endDateText->SetFont(font);

        writeToCsv->SetBackgroundColour(bgColor);
        writeToCsv->SetForegoundColour(fgColor);
        writeToCsv->SetFont(font);

        refreshVLCButton->SetBackgroundColour(bgColor);
        refreshVLCButton->SetForegroundColour(fgColor);
        refreshVLCButton->SetFont(font);

        this->SetBackgroundColour(bgColor);

    #endif
    this->Layout();
}

void GetTransactionPopout::SetDisplayTransactions(wxDateTime &startDate, wxDateTime &endDate){
    m_displayTransactions.clear();
    for(auto transaction : m_allTransactions){
        if(transaction.GetDate() > startDate && transaction.GetDate() < endDate){
            m_displayTransactions.push_back(transaction);
        }
    }
    std::sort(m_displayTransactions.begin(),m_displayTransactions.end(),
        [](const Transaction &a, const Transaction &b){
            return a.GetDate() < b.GetDate();
        });
}

void GetTransactionPopout::SetTransactions(){
    for(auto &asset : m_portfolio.assetPtrs){
        std::vector<Transaction> tempTransactions;
        tempTransactions = asset->GetTransactions();
        if(!tempTransactions.empty()){
            std::move(std::begin(tempTransactions),std::end(tempTransactions),std::back_inserter(m_allTransactions));
        }
    }
}

wxDateTime GetTransactionPopout::GetStartDate(){
    return startDate->GetValue();
}
wxDateTime GetTransactionPopout::GetEndDate(){
    return endDate->GetValue();
}

void GetTransactionPopout::RefreshVLC(wxCommandEvent &e){
    wxDateTime startDate = GetStartDate();
    wxDateTime endDate = GetEndDate();
    SetDisplayTransactions(startDate,endDate);
    allAssetsTransactionsVLC->setItems(m_displayTransactions);
    this->Refresh();
    this->Update();
}

void GetTransactionPopout::ExecuteFileWrite(wxCommandEvent &e){
    wxDateTime start = GetStartDate();
    wxDateTime end = GetEndDate();

    wxString fileName = "Transactions From "+start.FormatISODate()+" To "+end.FormatISODate()+".csv";
    wxFileName filePath(wxStandardPaths::Get().GetDocumentsDir(),fileName);
    wxFile file(filePath.GetFullPath(),wxFile::write);
    if(file.IsOpened()){
        file.Write("Date,Amount,Type,Name,Note\n");
        for(const auto&transaction : m_displayTransactions){
            file.Write(transaction.ToCSV());
        }
        file.Close();
    }
}