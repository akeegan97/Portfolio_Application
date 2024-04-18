#ifndef STATEMENT_HPP
#define STATEMENT_HPP
#include <wx/datetime.h>
#include "models/entities/asset.hpp"
#include "models/entities/investor.hpp"
#include "models/entities/position.hpp"
struct Details;

class Statement{
    public:
        Statement()=default;
        Statement(wxDateTime &startDate, wxDateTime &endDate);
        //setters
        void SetInternalValues();
        void SetCommittedCapital(double value);
        void SetReturnedPrincipal(double value);
        void SetQStartValuation(double value);
        void SetEndValuation(double value);
        void SetValuationChange(double value);
        void SetAdditionalCapital(double value);
        void SetReturnedCapital(double value);
        void SetExecutedDistributions(double value);
        void SetTotalDistributions(double value);
        void SetTotalValuation(double value);
        void SetTotalGains(double value);
        void SetIrr(double value);
        void SetAssetDetails();

    private:
        std::shared_ptr<Investor> m_InvestorPtr;
        wxDateTime m_QStartDate;
        wxDateTime m_EndingDate;
        double m_CommittedCapital;//
        double m_ReturnedPrincipal;//
        double m_QStartValuation;//
        double m_EndValuation;//
        double m_ValuationChange;// 
        double m_AdditionalCapitalThisPeriod;//
        double m_ReturnedCapitalThisPeriod;//
        double m_executedDistributionsThisPeriod;//
        double m_TotalNetDistributions;//
        double m_InceptionToDateValuation;//
        double m_TotalGain;//
        double m_CombinedIRR;
        std::vector<Details> m_ITDDetails;
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
};
#endif