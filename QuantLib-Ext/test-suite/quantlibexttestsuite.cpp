/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2004, 2005, 2006, 2007 Ferdinando Ametrano
 Copyright (C) 2004, 2005, 2006, 2007, 2008 StatPro Italia srl

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

#include <ql/types.hpp>
#include <ql/settings.hpp>
#include <ql/utilities/dataparsers.hpp>
#include <ql/version.hpp>

#ifdef QL_ENABLE_PARALLEL_UNIT_TEST_RUNNER
#include "paralleltestrunner.hpp"
#else
#include <boost/test/unit_test.hpp>
#endif

#include <boost/timer.hpp>

/* Use BOOST_MSVC instead of _MSC_VER since some other vendors (Metrowerks,
   for example) also #define _MSC_VER
*/
#ifdef BOOST_MSVC
#include <qlext/auto_link.hpp>
#ifndef QL_ENABLE_PARALLEL_UNIT_TEST_RUNNER
#  define BOOST_LIB_NAME boost_unit_test_framework
#  include <boost/config/auto_link.hpp>
#  undef BOOST_LIB_NAME
#endif

/* uncomment the following lines to unmask floating-point exceptions.
   See http://www.wilmott.com/messageview.cfm?catid=10&threadid=9481
*/
//#  include <float.h>
//   namespace { unsigned int u = _controlfp(_EM_INEXACT, _MCW_EM); }

#endif
#include "utilities.hpp"
#include "speedlevel.hpp"

#include "bonds.hpp"
#include "calibratedcurve.hpp"
#include "swaps.hpp"
#include "daycounters.hpp"

#include <iostream>
#include <iomanip>

using namespace boost::unit_test_framework;

namespace {

    boost::timer t;

    void startTimer() { t.restart(); }
    void stopTimer() {
        double seconds = t.elapsed();
        int hours = int(seconds/3600);
        seconds -= hours * 3600;
        int minutes = int(seconds/60);
        seconds -= minutes * 60;
        std::cout << " \nTests completed in ";
        if (hours > 0)
            std::cout << hours << " h ";
        if (hours > 0 || minutes > 0)
            std::cout << minutes << " m ";
        std::cout << std::fixed << std::setprecision(0)
                  << seconds << " s\n" << std::endl;
    }

    void configure(QuantLib::Date evaluationDate) {
        /* if needed, a subset of the lines below can be
           uncommented and/or changed to run the test suite with a
           different configuration. In the future, we'll need a
           mechanism that doesn't force us to recompile (possibly a
           couple of command-line flags for the test suite?)
        */

        // QuantLib::Settings::instance().includeReferenceDateCashFlows() = true;
        // QuantLib::Settings::instance().includeTodaysCashFlows() = boost::none;

        QuantLib::Settings::instance().evaluationDate() = evaluationDate;
    }

}

#if defined(QL_ENABLE_SESSIONS)
namespace QuantLib {

    Integer sessionId() { return 0; }

}
#endif

QuantLib::Date evaluation_date(int argc, char** argv) {
    /*! Dead simple parser:
        - passing --date=YYYY-MM-DD causes the test suite to run on
          that date;
        - passing --date=today causes it to run on today's date;
        - passing nothing causes it to run on a known date for which
          there should be no date-dependent errors as far as we know.

        Dates that should eventually be checked include:
        - 2015-08-29 causes three tests to fail;
        - 2016-02-29 causes two tests to fail.
    */

    QuantLib::Date knownGoodDefault =
        QuantLib::Date(16, QuantLib::September, 2015);

    for (int i=1; i<argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--date=today")
            return QuantLib::Date::todaysDate();
        else if (arg.substr(0, 7) == "--date=")
            return QuantLib::DateParser::parseISO(arg.substr(7));
    }
    return knownGoodDefault;
}


SpeedLevel speed_level(int argc, char** argv) {
    /*! Again, dead simple parser:
        - passing --slow causes all tests to be run;
        - passing --fast causes most tests to be run, except the slowest;
        - passing --faster causes only the faster tests to be run;
        - passing nothing is the same as --slow
    */

    for (int i=1; i<argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--slow")
            return Slow;
        else if (arg == "--fast")
            return Fast;
        else if (arg == "--faster")
            return Faster;
    }
    return Slow;
}


test_suite* init_unit_test_suite(int, char* []) {

    int argc = boost::unit_test::framework::master_test_suite().argc;
    char **argv = boost::unit_test::framework::master_test_suite().argv;
    configure(evaluation_date(argc, argv));
    SpeedLevel speed = speed_level(argc, argv);

    const QuantLib::Settings& settings = QuantLib::Settings::instance();
    std::ostringstream header;
    header <<
        " Testing "
        #ifdef BOOST_MSVC
        QLEXT_LIB_NAME
        #else
        "QuantLib " QL_VERSION
        #endif
        "\n  QL_NEGATIVE_RATES "
        #ifdef QL_NEGATIVE_RATES
        "       defined"
        #else
        "     undefined"
        #endif
        "\n  QL_EXTRA_SAFETY_CHECKS "
        #ifdef QL_EXTRA_SAFETY_CHECKS
        "  defined"
        #else
        "undefined"
        #endif
        "\n  QL_USE_INDEXED_COUPON "
        #ifdef QL_USE_INDEXED_COUPON
        "   defined"
        #else
        " undefined"
        #endif
        "\n"
           << "evaluation date is " << settings.evaluationDate() << ",\n"
           << (settings.includeReferenceDateEvents()
               ? "reference date events are included,\n"
               : "reference date events are excluded,\n")
           << (settings.includeTodaysCashFlows() == boost::none ?
               "" : (*settings.includeTodaysCashFlows() ?
                     "today's cashflows are included,\n"
                     : "today's cashflows are excluded,\n"))
           << (settings.enforcesTodaysHistoricFixings()
               ? "today's historic fixings are enforced."
               : "today's historic fixings are not enforced.")
           << "\nRunning "
           << (speed == Faster ? "faster" :
               (speed == Fast ?   "fast" : "all"))
           << " tests.";

    std::string rule = std::string(41, '=');

    BOOST_TEST_MESSAGE(rule);
    BOOST_TEST_MESSAGE(header.str());
    BOOST_TEST_MESSAGE(rule);
    test_suite* test = BOOST_TEST_SUITE("QuantLibExt test suite");

    test->add(QUANTLIB_TEST_CASE(startTimer));

    test->add(BondTest::suite());
	test->add(CalibratedCurveTest::suite());
    test->add(DayCounterTest::suite());
	test->add(SwapTest::suite());
    
    test->add(QUANTLIB_TEST_CASE(stopTimer));

    return test;
}
