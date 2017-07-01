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

#include "calibratedcurve.hpp"
#include "utilities.hpp"
#include <ql/indexes/ibor/shibor.hpp>
#include <ql/pricingengines/swap/discountingswapengine.hpp>
#include <ql/termstructures/yield/piecewiseyieldcurve.hpp>
#include <ql/time/calendars/china.hpp>
#include <ql/utilities/dataformatters.hpp>
#include <qlext/instruments/shiborswap.hpp>
#include <qlext/termstructures/yield/ratehelpers.hpp>

using namespace QuantLib;
using namespace boost::unit_test_framework;

namespace {

struct CommonVars {
    // common data
    Calendar calendar;
    Date today;
    Date spotDate;
    Real faceAmount;

    std::vector<boost::shared_ptr<RateHelper> > instruments;
    std::vector<boost::shared_ptr<SimpleQuote> > rates;
    std::vector<Period> tenors;
    boost::shared_ptr<YieldTermStructure> termStructure;

    // cleanup
    SavedSettings backup;

    // setup
    CommonVars() {
        calendar = China(China::IB);
        today = calendar.adjust(Date::todaysDate());

        Settings::instance().evaluationDate() = today;

        Rate rs[] = {0.02, 0.022, 0.024, 0.026, 0.028};
        Integer years[] = {1, 2, 3, 4, 5};

        rates = std::vector<boost::shared_ptr<SimpleQuote> >(LENGTH(rs));
        tenors = std::vector<Period>(LENGTH(rs));

        for (Size i = 0; i < rates.size(); ++i) {
            rates[i] = boost::shared_ptr<SimpleQuote>(new SimpleQuote(rs[i]));
            tenors[i] = years[i] * Years;
        }

        instruments = std::vector<boost::shared_ptr<RateHelper> >(LENGTH(rs));
        boost::shared_ptr<Shibor> shiborIndex(new Shibor(3 * Months));

        spotDate = today + shiborIndex->fixingDays() * Days;
        spotDate = calendar.adjust(spotDate);

        for (Size i = 0; i < rates.size(); ++i) {
            Handle<Quote> r(rates[i]);
            instruments[i] =
                boost::shared_ptr<ShiborSwapRateHelper>(new ShiborSwapRateHelper(r, tenors[i], Quarterly, shiborIndex));
        }
    }
};

} // namespace

void CalibratedCurveTest::testCalibratedShiborSwapCurve() {

    BOOST_TEST_MESSAGE("Testing shibor-swap yield curve construction...");

    CommonVars vars;

    Real tolerance = 1.e-9;

    vars.termStructure =
        boost::shared_ptr<YieldTermStructure>(new PiecewiseYieldCurve<Discount, Linear, IterativeBootstrap>(
            vars.today, vars.instruments, Actual365Fixed(), Linear()));

    RelinkableHandle<YieldTermStructure> curveHandle;
    curveHandle.linkTo(vars.termStructure);

    boost::shared_ptr<Shibor> shiborIndex(new Shibor(3 * Months, curveHandle));

    for (Size i = 0; i < vars.instruments.size(); ++i) {
        ShiborSwap swap(ShiborSwap::Payer, 1., vars.spotDate, vars.tenors[i], 3 * Months, 0., shiborIndex);

        Handle<YieldTermStructure> disc = shiborIndex->forwardingTermStructure();
        bool includeSettlementDateFlows = false;
        boost::shared_ptr<PricingEngine> engine(new DiscountingSwapEngine(disc, includeSettlementDateFlows));
        swap.setPricingEngine(engine);

        Rate expectedRate = vars.rates[i]->value();
        Rate estimatedRate = swap.fairRate();
        Spread error = std::fabs(expectedRate - estimatedRate);

        if (error > tolerance) {
            BOOST_ERROR(vars.tenors[i] << " year(s) swap:\n"
                                       << std::setprecision(8) << "\n estimated rate: " << io::rate(estimatedRate)
                                       << "\n expected rate:  " << io::rate(expectedRate) << "\n error:          "
                                       << io::rate(error) << "\n tolerance:      " << io::rate(tolerance));
        }
    }
}

test_suite* CalibratedCurveTest::suite() {
    test_suite* suite = BOOST_TEST_SUITE("Calibrated curve tests");
    suite->add(QUANTLIB_TEST_CASE(&CalibratedCurveTest::testCalibratedShiborSwapCurve));
    return suite;
}
