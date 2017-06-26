/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
Copyright (C) 2004, 2005 StatPro Italia srl
Copyright (C) 2007, 2012 Ferdinando Ametrano
Copyright (C) 2007, 2009 Piter Dias
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
#include "bonds.hpp"
#include "utilities.hpp"
#include <qlext/instruments/bonds/ctbfixedbond.hpp>
#include <qlext/instruments/bonds/ctbzerobond.hpp>
#include <qlext/time/daycounters/actualactualnl.hpp>
#include <ql/time/calendars/target.hpp>

using namespace QuantLib;
using namespace boost::unit_test_framework;

using boost::shared_ptr;

#define ASSERT_CLOSE(name, settlement, calculated, expected, tolerance)  \
if (std::fabs(calculated-expected) > tolerance) { \
    BOOST_FAIL("Failed to reproduce " << name << " at " << settlement \
    << "\n    calculated: " << std::setprecision(8) << calculated \
    << "\n    expected:   " << std::setprecision(8) << expected); \
}

namespace {

    struct CommonVars {
        // common data
        Calendar calendar;
        Date today;
        Real faceAmount;

        // cleanup
        SavedSettings backup;

        // setup
        CommonVars() {
            calendar = TARGET();
            today = calendar.adjust(Date::todaysDate());
            Settings::instance().evaluationDate() = today;
            faceAmount = 1000000.0;
        }
    };

}

void BondTest::testCachedCTBZero() {
    BOOST_TEST_MESSAGE("Testing Chinese CTB zero-coupon bond prices against cached values...");
    /*
    Static Info
    Code:         139903
    Issue Date:   2013/5/13
    Maturity:     2014/2/10
    Frequency:    Annual
    Coupon:       0.00%
    Face  :       100
    IssuePrice:   97.9030

    Quote
    Quote Date:   2013/12/2
    Settle Date:  2013/12/3
    Dirty Price:  99.166
    Clean Price:  97.5988
    YTM:          4.4500%
    Acccrued:     1.567
    */
    CommonVars vars;
    Date issueDate(13, May, 2013);
    Date maturity(10, Feb, 2014);
    Calendar calendar = NullCalendar();
    Natural settlementDays = 1;
    Date today(2, Dec, 2013);
    Settings::instance().evaluationDate() = today;
    Real faceAmount = 100.0;
    Real issuePrice = 97.9030;
    Real redemption = 100.0;
	ActualActualNoLeap accrualDayCounter;
    BusinessDayConvention paymentConvention = Unadjusted;
    CTBZeroBond bond(settlementDays,
        calendar,
        faceAmount,
        issuePrice,
        issueDate,
        accrualDayCounter,
        maturity,
        paymentConvention,
        redemption);
        const Real cleanPrice = 97.5988;
        Compounding compounding = Simple;
        Frequency frequency = Annual;
        Real accrual = bond.accruedAmount();
        Real yield = bond.yield(cleanPrice,
            accrualDayCounter,
            compounding,
            frequency);
            const Real benchmarkAccrual = 1.567;
            const Real benchmarkYield = 0.04450;
            const Real tolerance = 1e-4;
            if (std::fabs(yield-benchmarkYield) > tolerance) {
                BOOST_FAIL("failed to reproduce cached yield:\n"
                << QL_FIXED
                << "    calculated: " << yield << "\n"
                << "    expected:   " << benchmarkYield << "\n"
                << "    error:      " << yield-benchmarkYield);
            }
            if (std::fabs(accrual - benchmarkAccrual) > tolerance) {
                BOOST_FAIL("failed to reproduce cached accrued amount:\n"
                << QL_FIXED
                << "    calculated: " << accrual << "\n"
                << "    expected:   " << benchmarkAccrual << "\n"
                << "    error:      " << accrual - benchmarkAccrual);
            }
        }


        void BondTest::testCachedCTBFixed() {
            BOOST_TEST_MESSAGE("Testing Chinese CTB fixed-coupon bond prices against cached values...");
            /*
            Static Info
            Code:         120014
            Issue Date:   2012/8/16
            Maturity:     2017/8/16
            Frequency:    Annual
            Coupon:       2.95%
            Face  :       100
            Quote
            Quote Date:   2013/12/2
            Settle Date:  2013/12/3
            Dirty Price:  96.314
            Clean Price:  95.4326
            YTM:          4.3074%
            Acccrued:     0.881
            */

            CommonVars var;

            Date issueDate(16, Aug, 2012);
            Date startDate(16, Aug, 2012);
            Date maturity(16, Aug, 2017);
            Period tenor = 1 * Years;
            Natural settlementDays = 1;
            ActualActualNoLeap accrualDayCounter;
            Rate coupon = 0.0295;
            std::vector<Rate> coupons(1, coupon);
            Real faceAmount = 100.0;
            Date today(2, Dec, 2013);
            Settings::instance().evaluationDate() = today;
            CTBFixedBond bond(issueDate,
                settlementDays,
                faceAmount,
                startDate,
                maturity,
                tenor,
                coupons,
                accrualDayCounter);
                const Real cleanPrice = 95.4326;
                Compounding compounding = Compounded;
                Frequency frequency = Annual;
                Real accrual = bond.accruedAmount();
                Real yield = bond.yield(cleanPrice,
                    accrualDayCounter,
                    compounding,
                    frequency);
                    const Real benchmarkAccrual = 0.881;
                    const Real benchmarkYield = 0.043074;
                    const Real tolerance = 1e-4;
                    if (std::fabs(yield-benchmarkYield) > tolerance) {
                        BOOST_FAIL("failed to reproduce cached yield:\n"
                        << QL_FIXED
                        << "    calculated: " << yield << "\n"
                        << "    expected:   " << benchmarkYield << "\n"
                        << "    error:      " << yield-benchmarkYield);
                    }
                    if (std::fabs(accrual-benchmarkAccrual) > tolerance) {
                        BOOST_FAIL("failed to reproduce cached accrued amount:\n"
                        << QL_FIXED
                        << "    calculated: " << accrual << "\n"
                        << "    expected:   " << benchmarkAccrual << "\n"
                        << "    error:      " << accrual-benchmarkAccrual);
                    }
                }

                test_suite* BondTest::suite() {
                    test_suite* suite = BOOST_TEST_SUITE("Bond tests");
                    suite->add(QUANTLIB_TEST_CASE(&BondTest::testCachedCTBZero));
                    suite->add(QUANTLIB_TEST_CASE(&BondTest::testCachedCTBFixed));
                    return suite;
                }
