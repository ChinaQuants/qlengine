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
#include <qlext/instruments/subperiodsswap.hpp>
#include <qlext/termstructures/yield/ratehelpers.hpp>

using namespace QuantLib;
using namespace boost::unit_test_framework;

namespace {

struct CommonVars {
    // common data
    Calendar calendar;
    Date today;
    Date spotDate;
    DayCounter dc;

    std::vector<boost::shared_ptr<RateHelper> > shiborInstruments;
    std::vector<boost::shared_ptr<RateHelper> > subPeriodsInstruments;
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

        shiborInstruments = std::vector<boost::shared_ptr<RateHelper> >(LENGTH(rs));
        boost::shared_ptr<Shibor> shiborIndex(new Shibor(3 * Months));

        spotDate = today + shiborIndex->fixingDays() * Days;
        spotDate = calendar.adjust(spotDate);

        dc = Actual365Fixed();

        for (Size i = 0; i < rates.size(); ++i) {
            Handle<Quote> r(rates[i]);
            shiborInstruments[i] =
                boost::shared_ptr<ShiborSwapRateHelper>(new ShiborSwapRateHelper(r, tenors[i], Quarterly, shiborIndex));
        }

        boost::shared_ptr<IborIndex> index(new Shibor(7 * Days));
        subPeriodsInstruments = std::vector<boost::shared_ptr<RateHelper> >(LENGTH(rs));

        for (Size i = 0; i < rates.size(); ++i) {
            Handle<Quote> r(rates[i]);
            subPeriodsInstruments[i] =
                boost::shared_ptr<SubPeriodsSwapRateHelper>(new SubPeriodsSwapRateHelper(r, tenors[i], Quarterly, calendar, dc, ModifiedFollowing,
                    3 * Months, index, dc));
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
            vars.today, vars.shiborInstruments, Actual365Fixed(), Linear()));

    RelinkableHandle<YieldTermStructure> curveHandle;
    curveHandle.linkTo(vars.termStructure);

    boost::shared_ptr<Shibor> shiborIndex(new Shibor(3 * Months, curveHandle));

    for (Size i = 0; i < vars.shiborInstruments.size(); ++i) {
        boost::shared_ptr<ShiborSwap> swap(new ShiborSwap(ShiborSwap::Payer, 1., vars.spotDate, vars.tenors[i], 3 * Months, 0., shiborIndex));

        Handle<YieldTermStructure> disc = shiborIndex->forwardingTermStructure();
        bool includeSettlementDateFlows = false;
        boost::shared_ptr<PricingEngine> engine(new DiscountingSwapEngine(disc, includeSettlementDateFlows));
        swap->setPricingEngine(engine);

        Rate expectedRate = vars.rates[i]->value();
        Rate estimatedRate = swap->fairRate();
        Spread error = std::fabs(expectedRate - estimatedRate);

        if (error > tolerance) {
            BOOST_ERROR(vars.tenors[i] << " year(s) swap:\n"
                                       << std::setprecision(8) << "\n estimated rate: " << io::rate(estimatedRate)
                                       << "\n expected rate:  " << io::rate(expectedRate) << "\n error:          "
                                       << io::rate(error) << "\n tolerance:      " << io::rate(tolerance));
        }
    }
}

void CalibratedCurveTest::testCalibratedSubPeriodsSwapCurve() {
    BOOST_TEST_MESSAGE("Testing sub periods swap yield curve construction...");

    CommonVars vars;
    Real tolerance = 1.e-9;

    vars.termStructure =
        boost::shared_ptr<YieldTermStructure>(new PiecewiseYieldCurve<Discount, Linear, IterativeBootstrap>(
            vars.today, vars.subPeriodsInstruments, Actual365Fixed(), Linear()));

    RelinkableHandle<YieldTermStructure> curveHandle;
    curveHandle.linkTo(vars.termStructure);

    boost::shared_ptr<IborIndex> index = boost::shared_ptr<IborIndex>(new Shibor(7 * Days, curveHandle));

    for (Size i = 0; i < vars.subPeriodsInstruments.size(); ++i) {
        boost::shared_ptr<SubPeriodsSwap> swap(new SubPeriodsSwap(vars.spotDate, 1., vars.tenors[i], true, 3 * Months, 0., vars.calendar, vars.dc, ModifiedFollowing,
            3 * Months, index, vars.dc));

        Handle<YieldTermStructure> disc = index->forwardingTermStructure();
        bool includeSettlementDateFlows = false;
        boost::shared_ptr<PricingEngine> engine(new DiscountingSwapEngine(disc, includeSettlementDateFlows));
        swap->setPricingEngine(engine);

        Rate expectedRate = vars.rates[i]->value();
        Rate estimatedRate = swap->fairRate();
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
    suite->add(QUANTLIB_TEST_CASE(&CalibratedCurveTest::testCalibratedSubPeriodsSwapCurve));
    return suite;
}
