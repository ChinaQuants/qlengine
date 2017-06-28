/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2003 RiskMap srl

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

#include "swaps.hpp"
#include "utilities.hpp"
#include <ql/indexes/ibor/shibor.hpp>
#include <ql/instruments/vanillaswap.hpp>
#include <ql/pricingengines/swap/discountingswapengine.hpp>
#include <ql/termstructures/yield/flatforward.hpp>
#include <ql/time/calendars/china.hpp>
#include <ql/utilities/dataformatters.hpp>
#include <qlext/instruments/shiborswap.hpp>

using namespace QuantLib;
using namespace boost::unit_test_framework;

namespace {
struct CommonVars {

    VanillaSwap::Type type;
    Date settlement;
    Natural settlementDays;
    Real nominal;
    Calendar calendar;
    RelinkableHandle<YieldTermStructure> termStructure;

    boost::shared_ptr<ShiborSwap> makeShiborSwap(Integer length, Rate fixedRate, Spread floatingSpread) {
        VanillaSwap::Type swapType = type;
        Date startDate = settlement;
        Period swapTenor(length, Years);
        Period fixedTenor(3, Months);
        Period floatTenor(3, Months);
        boost::shared_ptr<Shibor> shiborIndex(new Shibor(floatTenor, termStructure));

        boost::shared_ptr<ShiborSwap> swap(
            new ShiborSwap(swapType, nominal, startDate, swapTenor, fixedTenor, fixedRate, shiborIndex));
        swap->setPricingEngine(boost::shared_ptr<PricingEngine>(new DiscountingSwapEngine(termStructure)));
        return swap;
    }

    CommonVars() {
        type = VanillaSwap::Payer;
        calendar = China(China::IB);
        nominal = 100.;
        settlementDays = 2;
        Date today = calendar.adjust(Settings::instance().evaluationDate());
        settlement = calendar.advance(today, settlementDays, Days);
        termStructure.linkTo(flatRate(settlement, 0.05, Actual365Fixed()));
    }
};
} // namespace

void SwapTest::testShiborSwap() {

    BOOST_TEST_MESSAGE("Testing shibor-swap calculation of fair fixed rate...");

    CommonVars vars;

    Integer lengths[] = {1, 2, 5, 10, 20};
    Spread spreads[] = {0.04, 0.05, 0.06, 0.07};

    for (Size i = 0; i < LENGTH(lengths); i++) {
        for (Size j = 0; j < LENGTH(spreads); j++) {
            boost::shared_ptr<ShiborSwap> swap = vars.makeShiborSwap(lengths[i], 0., spreads[j]);
            swap = vars.makeShiborSwap(lengths[i], swap->fairRate(), spreads[j]);
            if (std::fabs(swap->NPV()) > 1.0e-10) {
                BOOST_ERROR("recalculating with implied rate:\n"
                            << std::setprecision(2) << "    length: " << lengths[i] << " years\n"
                            << "    floating spread: " << io::rate(spreads[j]) << "\n"
                            << "    swap value: " << swap->NPV());
            }
        }
    }
}

test_suite* SwapTest::suite() {
    test_suite* suite = BOOST_TEST_SUITE("Swap tests");
    suite->add(QUANTLIB_TEST_CASE(&SwapTest::testShiborSwap));
    return suite;
}
