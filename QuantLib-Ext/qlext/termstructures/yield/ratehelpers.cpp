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

#include <boost/make_shared.hpp>
#include <qlext/termstructures/yield/ratehelpers.hpp>
#include <ql/indexes/ibor/shibor.hpp>
#include <ql/time/calendars/china.hpp>
#include <ql/pricingengines/swap/discountingswapengine.hpp>
#include <qlext/instruments/shiborswap.hpp>

namespace QuantLib {

    ShiborSwapRateHelper::ShiborSwapRateHelper(const Handle<Quote>& rate,
                             const Period& swapTenor,
                             Frequency fixedFreq,
                             const boost::shared_ptr<Shibor>& shiborIndex,
                             const Period& fwdStart,
                             const Handle<YieldTermStructure>& discountingCurve)
                             : RelativeDateRateHelper(rate), tenor_(swapTenor),  fixedFrequency_(fixedFreq),
                               fwdStart_(fwdStart), discountHandle_(discountingCurve) {

        settlementDays_ = shiborIndex->fixingDays();
        shiborIndex_ = boost::dynamic_pointer_cast<Shibor>(shiborIndex->clone(termStructureHandle_));
        shiborIndex_->unregisterWith(termStructureHandle_);

        registerWith(shiborIndex_);
        registerWith(discountHandle_);

        initializeDates();
        
    }

    void ShiborSwapRateHelper::initializeDates() {
        
        Date refDate = Settings::instance().evaluationDate();
        China floatCalendar(China::IB);
        refDate = floatCalendar.adjust(refDate);

        Date spotDate = floatCalendar.advance(refDate, settlementDays_ * Days);
        Date startDate = spotDate + fwdStart_;

        if (fwdStart_.length() < 0)
            startDate = floatCalendar.adjust(startDate, Preceding);
        else
            startDate = floatCalendar.adjust(startDate, Following);

        swap_ = boost::make_shared<ShiborSwap>(VanillaSwap::Payer, 1., startDate, tenor_, Period(fixedFrequency_), 0., shiborIndex_);

		bool includeSettlementDateFlows = false;
		boost::shared_ptr<PricingEngine> engine(new DiscountingSwapEngine(discountHandle_, includeSettlementDateFlows));

		swap_->setPricingEngine(engine);
    }

	void ShiborSwapRateHelper::accept(AcyclicVisitor& v) {
		Visitor<ShiborSwapRateHelper>* v1 =
			dynamic_cast<Visitor<ShiborSwapRateHelper>*>(&v);
		if (v1 != 0)
			v1->visit(*this);
		else
			RateHelper::accept(v);
	}

	Real ShiborSwapRateHelper::impliedQuote() const {
		QL_REQUIRE(termStructure_ != 0, "term structure not set");
		// we didn't register as observers - force calculation
		swap_->recalculate();
		// weak implementation... to be improved
		static const Spread basisPoint = 1.0e-4;
		Real floatingLegNPV = swap_->floatingLegNPV();
		Real totNPV = -floatingLegNPV;
		Real result = totNPV / (swap_->fixedLegBPS() / basisPoint);
		return result;
	}
}