#ifndef qlext_ctb_fixed_bond_hpp
#define qlext_ctb_fixed_bond_hpp

#include <ql/instruments/bonds/fixedratebond.hpp>
#include <ql/time/dategenerationrule.hpp>
#include <ql/time/daycounters/actualactual.hpp>
#include <ql/time/calendars/nullcalendar.hpp>
#include <ql/time/calendars/china.hpp>


namespace QuantLib {

    class DayCounter;
    class Calendar;

    class CTBFixedBond
    :public FixedRateBond {
    public:
        CTBFixedBond(const Date& issueDate,
                     Natural settlementDays,
                     Real faceAmount,
                     const Date& startDate,
                     const Date& maturity,
                     const Period& tenor,
                     const std::vector<Real>& coupons,
                     const DayCounter& accrualDayCounter = ActualActual(ActualActual::ISMA),
                     bool endOfMonth = false,
                     const Calendar& calendar = NullCalendar(),
                     const Calendar& paymentCalendar = China(China::IB),
                     BusinessDayConvention convention = Unadjusted,
                     DateGeneration::Rule rule = DateGeneration::Backward,
                     BusinessDayConvention paymentConvention = Unadjusted,
                     Real redemption = 100.0,
                     const Date& firstDate = Date(),
                     const Date& nextToLastDate = Date());
    };
}

#endif