#include <ql/time/schedule.hpp>
#include <qlext/instruments/bonds/ctbfixedbond.hpp>

namespace QuantLib {

CTBFixedBond::CTBFixedBond(const Date& issueDate, Natural settlementDays, Real faceAmount, const Date& startDate,
                           const Date& maturity, const Period& tenor, const std::vector<Real>& coupons,
                           const DayCounter& accrualDayCounter, bool endOfMonth, const Calendar& calendar,
                           const Calendar& paymentCalendar, BusinessDayConvention convention, DateGeneration::Rule rule,
                           BusinessDayConvention paymentConvention, Real redemption, const Date& firstDate,
                           const Date& nextToLastDate)
    : FixedRateBond(settlementDays, faceAmount,
                    Schedule(startDate, maturity, tenor, calendar, convention, convention, rule, endOfMonth, firstDate,
                             nextToLastDate),
                    coupons, accrualDayCounter, paymentConvention, redemption, issueDate, paymentCalendar) {}

} // namespace QuantLib
