#include <wx/datetime.h>
#include <wx/string.h>


struct PositionDetails{
    wxDateTime dateInvested;
    double subscribed;
    double paid;
    double reserve;
    double deployed;
    double returnOfCapital;
}; 
struct InvestorDetails{
    wxString clientName;
    wxString type;
    double managementFeePercentage;
    double promoteFeePercentage;
};