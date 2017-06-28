/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
Copyright (C) 2017 Cheng Li

This file is part of QuantLib, a free-software/open-source library
for financial quantitative analysts and developers - http://quantlib.org/

QuantLib is free software: you can redistribute it and/or modify it
under the terms of the QuantLib license.  You should have received a
copy of the license along with this program; if not, please email
<quantlib-dev@lists.sf.net>. The license is also available online at
<http://quantlib.org/license.shtml>.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#include <qlext/time/daycounters/actualactualnl.hpp>

namespace QuantLib {

Time ActualActualNoLeap::Impl::yearFraction(const Date& d1, const Date& d2, const Date& d3, const Date& d4) const {
    if (d1 == d2)
        return 0.0;

    if (d1 > d2)
        return -yearFraction(d2, d1, d3, d4);

    // when the reference period is not specified, try taking
    // it equal to (d1,d2)
    Date refPeriodStart = (d3 != Date() ? d3 : d1);
    Date refPeriodEnd = (d4 != Date() ? d4 : d2);

    QL_REQUIRE(refPeriodEnd > refPeriodStart && refPeriodEnd > d1, "invalid reference period: "
                                                                       << "date 1: " << d1 << ", date 2: " << d2
                                                                       << ", reference period start: " << refPeriodStart
                                                                       << ", reference period end: " << refPeriodEnd);

    // estimate roughly the length in months of a period
    Integer months = Integer(0.5 + 12 * Real(refPeriodEnd - refPeriodStart) / 365);

    // for short periods...
    if (months == 0) {
        // ...take the reference period as 1 year from d1
        refPeriodStart = d1;
        refPeriodEnd = d1 + 1 * Years;
        months = 12;
    }

    Time period = Real(months) / 12.0;

    if (d2 <= refPeriodEnd) {
        // here refPeriodEnd is a future (notional?) payment date
        if (d1 >= refPeriodStart) {
            // here refPeriodStart is the last (maybe notional)
            // payment date.
            // refPeriodStart <= d1 <= d2 <= refPeriodEnd
            // [maybe the equality should be enforced, since
            // refPeriodStart < d1 <= d2 < refPeriodEnd
            // could give wrong results] ???
            return period * Real(dayCount(d1, d2)) / dayCount(refPeriodStart, refPeriodEnd);
        } else {
            // here refPeriodStart is the next (maybe notional)
            // payment date and refPeriodEnd is the second next
            // (maybe notional) payment date.
            // d1 < refPeriodStart < refPeriodEnd
            // AND d2 <= refPeriodEnd
            // this case is long first coupon

            // the last notional payment date
            Date previousRef = refPeriodStart - months * Months;
            if (d2 > refPeriodStart)
                return yearFraction(d1, refPeriodStart, previousRef, refPeriodStart) +
                       yearFraction(refPeriodStart, d2, refPeriodStart, refPeriodEnd);
            else
                return yearFraction(d1, d2, previousRef, refPeriodStart);
        }
    } else {
        // here refPeriodEnd is the last (notional?) payment date
        // d1 < refPeriodEnd < d2 AND refPeriodStart < refPeriodEnd
        QL_REQUIRE(refPeriodStart <= d1, "invalid dates: "
                                         "d1 < refPeriodStart < refPeriodEnd < d2");
        // now it is: refPeriodStart <= d1 < refPeriodEnd < d2

        // the part from d1 to refPeriodEnd
        Time sum = yearFraction(d1, refPeriodEnd, refPeriodStart, refPeriodEnd);

        // the part from refPeriodEnd to d2
        // count how many regular periods are in [refPeriodEnd, d2],
        // then add the remaining time
        Integer i = 0;
        Date newRefStart, newRefEnd;
        for (;;) {
            newRefStart = refPeriodEnd + (months * i) * Months;
            newRefEnd = refPeriodEnd + (months * (i + 1)) * Months;
            if (d2 < newRefEnd) {
                break;
            } else {
                sum += period;
                i++;
            }
        }
        sum += yearFraction(newRefStart, d2, newRefStart, newRefEnd);
        return sum;
    }
}

Date::serial_type ActualActualNoLeap::Impl::dayCount(const Date& d1, const Date& d2) const {
    static const Integer MonthOffset[] = {
        0,   31,  59,  90,  120, 151, // Jan - Jun
        181, 212, 243, 273, 304, 334  // Jun - Dec
    };
    Date::serial_type s1, s2;

    s1 = d1.dayOfMonth() + MonthOffset[d1.month() - 1] + (d1.year() * 365);
    s2 = d2.dayOfMonth() + MonthOffset[d2.month() - 1] + (d2.year() * 365);

    if (d1.month() == Feb && d1.dayOfMonth() == 29) {
        --s1;
    }

    if (d2.month() == Feb && d2.dayOfMonth() == 29) {
        --s2;
    }

    return s2 - s1;
}
} // namespace QuantLib
