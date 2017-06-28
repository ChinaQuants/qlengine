#include <qlext/instruments/bonds/ctbzerobond.hpp>

namespace QuantLib {

CTBZeroBond::CTBZeroBond(Natural settlementDays, const Calendar& calendar, Real faceAmount, Real issueValue,
                         const Date& issueDate, const DayCounter& accrualDayCounter, const Date& maturityDate,
                         BusinessDayConvention paymentConvention, Real redemption)
    : Bond(settlementDays, calendar, issueDate) {
    dayCounter_ = accrualDayCounter;
    maturityDate_ = maturityDate;
    issueValue_ = issueValue;
    Date redemptionDate = calendar_.adjust(maturityDate, paymentConvention);
    setSingleRedemption(faceAmount, redemption, redemptionDate);
}

Real CTBZeroBond::accruedAmount(Date settleDate) const {
    if (settleDate == Date())
        settleDate = settlementDate();
    Real yf = static_cast<Real>(dayCounter_.dayCount(issueDate_, settleDate)) /
              static_cast<Real>(dayCounter_.dayCount(issueDate_, maturityDate_));
    Real totalAmount = (redemption()->amount() - issueValue_);
    return 100.0 / notional(settleDate) * totalAmount * yf;
}
} // namespace QuantLib
