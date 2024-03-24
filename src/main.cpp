#include <iostream>
#include <wx/wx.h>
#include <wx/stdpaths.h>
#include "models/entities/portfolio.hpp"
#include "ui/mainframe/mainframe.hpp"

class RugenBerg : public wxApp
{
public:
    bool OnInit() override;
    int OnExit() override;
    Portfolio portfolio;
};

bool RugenBerg::OnInit()
{
    std::string jsonFilePath;

#ifdef __WXMAC__
    // macOS-specific path construction
    wxString resourcePath = wxStandardPaths::Get().GetResourcesDir();
    jsonFilePath = resourcePath + "/data/test.json";
#elif defined(__WXMSW__)
    jsonFilePath = "../resources/data/testoutput.json";
#endif

    portfolio.LoadFromFile(jsonFilePath);
    std::cout << " TOTAL INVESTORS IN PORTFOLIO>INVESTORPTRS: " << portfolio.allInvestorPtrs.size() << std::endl;

    MainFrame *frame = new MainFrame("rugenberg", wxDefaultPosition, wxSize(1300, 800), portfolio);
    frame->Show(true);
    return true;
}

int RugenBerg::OnExit()
{
    std::string jsonFilePath;

#ifdef __WXMAC__
    // macOS-specific path construction
    wxString resourcePath = wxStandardPaths::Get().GetResourcesDir();
    jsonFilePath = resourcePath + "/data/test.json";
#endif
#ifdef __WXMSW__
    // Windows-specific path (adjust as needed)
    jsonFilePath = "../resources/data/testoutput.json";
#endif
    portfolio.SavePortfolioToFile(portfolio, jsonFilePath);
    return wxApp::OnExit();
}

wxIMPLEMENT_APP(RugenBerg);