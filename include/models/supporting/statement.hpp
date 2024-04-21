#ifndef STATEMENT_HPP
#define STATEMENT_HPP
#include <wx/datetime.h>
#include "models/entities/asset.hpp"
#include "models/entities/investor.hpp"
#include "models/entities/position.hpp"
struct Details;
class Investor;

class Statement{
    public:
        Statement()=default;
        Statement(wxDateTime &startDate, wxDateTime &endDate,std::shared_ptr<Investor> &investorPtr);
        //setters
        void SetBeginningBalance(double value);
        void SetAdditionalCapital(double value);
        void SetReturnedCapital(double value);
        void SetChangeInValuationThisPeriod(double value);
        void SetEndingBalance(double value);

        void SetPaid(double value);
        void SetReturnedPrincipal(double value);

        void SetNetIncomeThisPeriod(double value);
        void SetReturnThisPeriod(double value);
        void SetReturnPercentThisPeriod(double value);

        void SetTotalNetIncome(double value);
        void SetTotalChangeInValuation(double value);
        void SetTotalGain(double value);
        void SetCombinedIrr(double value);
        
        void SetInternalValues();
        void SetAssetDetails();
        //Getters
        double GetBeginningBalance();
        double GetAdditionalCapital();
        double GetReturnedCapital();
        double GetChangeInValuationThisPeriod();
        double GetEndingBalance();

        double GetPaidCapital();
        double GetReturnedPrincipal();

        double GetNetIncomeThisPeriod();
        double GetReturnAmountThisPeriod();
        double GetReturnPercentThisPeriod();

        double GetTotalNetIncomeToEndDate();
        double GetChangeInValuationToEndDate();
        double GetTotalGain();
        double GetCombinedIrr();
        std::vector<Details> GetDetails();
        
    private:
        std::shared_ptr<Investor> m_InvestorPtr;
        wxDateTime m_QStartDate;
        wxDateTime m_EndingDate;

        double m_BeginningBalance;//
        double m_AdditionalCapitalThisPeriod;//
        double m_ReturnedCapitalThisPeriod;//
        double m_ChangeValuationThisPeriod;// 
        double m_EndingBalance;//

        double m_PaidCapital;
        double m_ReturnedPrincipal;//

        double m_NetIncomeThisPeriod;//
        double m_ReturnThisPeriodAmount;//net income + change valuation this period
        double m_ReturnThisPeriodPercent;//returnThisPeriod / (paid capital - returned Principal)

        double m_TotalNetDistributions;//
        double m_ChangeInValuationToEndDate;//ending balance - starting valuation
        double m_TotalGain;//total net income + change In valuation to end date
        double m_CombinedIRR;

        std::vector<Details> m_ITDDetails;
        double CalculateNPV(std::vector<CashFlow> &cashFlows, double rate);
        double CalculateIrr(std::vector<CashFlow> &cashFlow);
};
struct Details{
    std::shared_ptr<Asset> m_AssetPtr;
    double m_CommittedAmount;
    double m_ReturnedCapital;
    double m_ChangeInValuation;
    double m_NetDistributionsExecuted;
    double m_TotalReturn;
    double m_IRR;
    double m_AllocationPercent;
    wxVariant GetValue(int col)const;
    static std::vector<wxString> columnNames;
    static std::vector<int> columnWidths;
};
#endif