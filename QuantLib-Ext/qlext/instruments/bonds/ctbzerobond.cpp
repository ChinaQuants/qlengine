#include <qlext/instruments/bonds/ctbzerobond.hpp>

namespace QuantLib {

    CTBZeroBond::CTBZeroBond(Natural settlementDays,
                             const Calendar& calendar,
                             Real faceAmount,
                             Real issueValue,
                             const Date& issueDate,
                             const DayCounter& accrualDayCounter,
                             const Date& maturityDate,
                             BusinessDayConvention paymentConvention,
                             Real redemption)
                             : Bond(settlementDays, calendar, issueDate) {
            this->dayCounter_ = accrualDayCounter;
            this->maturityDate_ = maturityDate;
            this->issueValue_ = issueValue;
            Date redemptionDate = this->calendar_.adjust(maturityDate, paymentConvention);
            this->setSingleRedemption(faceAmount, redemption, redemptionDate);
    }

    Real CTBZeroBond::accruedAmount(Date settleDate) const {
        if (settleDate == Date())
            settleDate = this->settlementDate();
        Real yf = static_cast<Real>(this->dayCounter_.dayCount(this->issueDate_, settleDate)) 
                  / static_cast<Real>(this->dayCounter_.dayCount(this->issueDate_, this->maturityDate_));
        Real totalAmount = (this->redemption()->amount() - this->issueValue_);
        return 100.0 / this->notional(settleDate) * totalAmount * yf;
    }
}