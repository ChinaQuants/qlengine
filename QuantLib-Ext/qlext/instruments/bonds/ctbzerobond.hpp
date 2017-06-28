#ifndef qlext_zero_coupon_bond_china_hpp
#define qlext_zero_coupon_bond_china_hpp

#include <ql/instruments/bond.hpp>
#include <ql/time/daycounter.hpp>

namespace QuantLib {

class CTBZeroBond : public Bond {
public:
    CTBZeroBond(Natural settlementDays, const Calendar& calendar, Real faceAmount, Real issuePrice,
                const Date& issueDate, const DayCounter& accrualDayCounter, const Date& maturityDate,
                BusinessDayConvention paymentConvention = Following, Real redemption = 100.0);

    Real accruedAmount(Date settleDate = Date()) const;
    const DayCounter& dayCounter() const { return this->dayCounter_; }

private:
    Real issueValue_;
    DayCounter dayCounter_;
};

} // namespace QuantLib

#endif
