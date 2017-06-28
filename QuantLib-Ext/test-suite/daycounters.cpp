/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2003 RiskMap srl
 Copyright (C) 2006 Piter Dias
 Copyright (C) 2012 Simon Shakeshaft

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

#include "daycounters.hpp"
#include "utilities.hpp"
#include <qlext/time/daycounters/actualactualnl.hpp>
#include <iomanip>

using namespace QuantLib;
using namespace boost::unit_test_framework;


void DayCounterTest::testActualActualNoLeap() {
	BOOST_TEST_MESSAGE("Testing actual/actual (no leap) date day counters...");

	DayCounter dayCounter = ActualActualNoLeap();

	std::vector<Date> testStartDates;
	std::vector<Date> testEndDates;
	std::vector<Date> testRefStartDates;
	std::vector<Date> testRefEndDates;

	testStartDates.push_back(Date(1, Jan, 2012));
	testStartDates.push_back(Date(1, Mar, 2012));

	testEndDates.push_back(Date(20, Apr, 2012));
	testEndDates.push_back(Date(30, Apr, 2012));

	testRefStartDates.push_back(Date(1, Jun, 2011));
	testRefStartDates.push_back(Date(1, Jan, 2012));

	testRefEndDates.push_back(Date(1, Jun, 2012));
	testRefEndDates.push_back(Date(1, July, 2012));

	Time expected[] = {0.29863013698630,
		               0.16574585635359};

	for(Size i=0;i<testStartDates.size();++i) {
		Time calculated = dayCounter.yearFraction(testStartDates[i], testEndDates[i], testRefStartDates[i], testRefEndDates[i]);
		if (std::fabs(calculated-expected[i]) > 1.0e-12) {
			BOOST_ERROR("from " << testStartDates[i]
			<< " to " << testEndDates[i] << ":\n"
			<< " in reference period: " << testRefStartDates[i] << " to " << testRefEndDates[i] << ":\n"
				<< "    calculated: " << calculated << "\n"
				<< "    expected:   " << expected[i]);
		}
	}
}


test_suite* DayCounterTest::suite() {
    test_suite* suite = BOOST_TEST_SUITE("Day counter tests");
	suite->add(QUANTLIB_TEST_CASE(&DayCounterTest::testActualActualNoLeap));
    return suite;
}



