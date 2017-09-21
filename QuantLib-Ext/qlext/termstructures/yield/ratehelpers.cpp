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
#include <ql/cashflows/iborcoupon.hpp>
#include <ql/indexes/ibor/shibor.hpp>
#include <ql/pricingengines/swap/discountingswapengine.hpp>
#include <ql/time/calendars/china.hpp>
#include <ql/utilities/null_deleter.hpp>
#include <qlext/instruments/shiborswap.hpp>
#include <qlext/instruments/subperiodsswap.hpp>
#include <qlext/termstructures/yield/ratehelpers.hpp>

namespace QuantLib {

    ShiborSwapRateHelper::ShiborSwapRateHelper(const Handle<Quote>& rate, const Period& swapTenor, Frequency fixedFreq,
                                               const boost::shared_ptr<Shibor>& shiborIndex, const Period& fwdStart,
                                               const Handle<YieldTermStructure>& discountingCurve)
        : RelativeDateRateHelper(rate), tenor_(swapTenor), fixedFrequency_(fixedFreq), fwdStart_(fwdStart),
          discountHandle_(discountingCurve) {

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

        swap_ = boost::make_shared<ShiborSwap>(VanillaSwap::Payer, 1., startDate, tenor_, Period(fixedFrequency_), 0.,
                                               shiborIndex_);

        bool includeSettlementDateFlows = false;
        boost::shared_ptr<PricingEngine> engine(
            new DiscountingSwapEngine(discountRelinkableHandle_, includeSettlementDateFlows));

        swap_->setPricingEngine(engine);

        earliestDate_ = swap_->startDate();
        maturityDate_ = swap_->maturityDate();

        boost::shared_ptr<IborCoupon> lastCoupon = boost::dynamic_pointer_cast<IborCoupon>(swap_->floatingLeg().back());
        latestRelevantDate_ = std::max(maturityDate_, lastCoupon->fixingEndDate());
        latestDate_ = maturityDate_;
    }

    void ShiborSwapRateHelper::accept(AcyclicVisitor& v) {
        Visitor<ShiborSwapRateHelper>* v1 = dynamic_cast<Visitor<ShiborSwapRateHelper>*>(&v);
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

    void ShiborSwapRateHelper::setTermStructure(YieldTermStructure* t) {
        // do not set the relinkable handle as an observer -
        // force recalculation when needed---the index is not lazy
        bool observer = false;

        boost::shared_ptr<YieldTermStructure> temp(t, null_deleter());
        termStructureHandle_.linkTo(temp, observer);

        if (discountHandle_.empty())
            discountRelinkableHandle_.linkTo(temp, observer);
        else
            discountRelinkableHandle_.linkTo(*discountHandle_, observer);

        RelativeDateRateHelper::setTermStructure(t);
    }

    SubPeriodsSwapRateHelper::SubPeriodsSwapRateHelper(
        const Handle<Quote>& rate, const Period& swapTenor, Frequency fixedFreq, const Calendar& fixedCalendar,
        const DayCounter& fixedDayCount, BusinessDayConvention fixedConvention, const Period& floatPayTenor,
        const boost::shared_ptr<IborIndex>& iborIndex, const DayCounter& floatingDayCount, DateGeneration::Rule rule,
        Ext::SubPeriodsCoupon::Type type, const Period& fwdStart, const Handle<YieldTermStructure>& discountingCurve)
        : RelativeDateRateHelper(rate), tenor_(swapTenor), fixedFrequency_(fixedFreq), fixedCalendar_(fixedCalendar),
          fixedDayCount_(fixedDayCount), fixedConvention_(fixedConvention), floatPayTenor_(floatPayTenor),
          floatingDayCount_(floatingDayCount), rule_(rule), type_(type), fwdStart_(fwdStart),
          discountHandle_(discountingCurve) {

        settlementDays_ = iborIndex->fixingDays();
        iborIndex_ = boost::dynamic_pointer_cast<IborIndex>(iborIndex->clone(termStructureHandle_));
        iborIndex_->unregisterWith(termStructureHandle_);

        registerWith(iborIndex_);
        registerWith(discountHandle_);

        initializeDates();
    }

    void SubPeriodsSwapRateHelper::initializeDates() {

        Date refDate = Settings::instance().evaluationDate();
        Calendar floatCalendar = iborIndex_->fixingCalendar();
        refDate = floatCalendar.adjust(refDate);

        Date spotDate = floatCalendar.advance(refDate, settlementDays_ * Days);
        Date startDate = spotDate + fwdStart_;

        if (fwdStart_.length() < 0)
            startDate = floatCalendar.adjust(startDate, Preceding);
        else
            startDate = floatCalendar.adjust(startDate, Following);

        swap_ = boost::make_shared<SubPeriodsSwap>(startDate, 1., tenor_, true, Period(fixedFrequency_), 0.,
                                                   fixedCalendar_, fixedDayCount_, fixedConvention_, floatPayTenor_,
                                                   iborIndex_, floatingDayCount_, rule_, type_);

        bool includeSettlementDateFlows = false;
        boost::shared_ptr<PricingEngine> engine(
            new DiscountingSwapEngine(discountRelinkableHandle_, includeSettlementDateFlows));

        swap_->setPricingEngine(engine);

        earliestDate_ = swap_->startDate();
        maturityDate_ = swap_->maturityDate();
        Leg floatLeg = swap_->floatLeg();

        boost::shared_ptr<Ext::SubPeriodsCoupon> lastCoupon =
            boost::dynamic_pointer_cast<Ext::SubPeriodsCoupon>(floatLeg[floatLeg.size() - 1]);
        latestRelevantDate_ = std::max(latestDate_, lastCoupon->latestRelevantDate());
        latestDate_ = maturityDate_;
    }

    void SubPeriodsSwapRateHelper::accept(AcyclicVisitor& v) {
        Visitor<SubPeriodsSwapRateHelper>* v1 = dynamic_cast<Visitor<SubPeriodsSwapRateHelper>*>(&v);
        if (v1 != 0)
            v1->visit(*this);
        else
            RateHelper::accept(v);
    }

    Real SubPeriodsSwapRateHelper::impliedQuote() const {
        QL_REQUIRE(termStructure_ != 0, "term structure not set");
        // we didn't register as observers - force calculation
        swap_->recalculate();
        // weak implementation... to be improved
        static const Spread basisPoint = 1.0e-4;
        Real floatingLegNPV = swap_->floatLegNPV();
        Real totNPV = -floatingLegNPV;
        Real result = totNPV / (swap_->fixedLegBPS() / basisPoint);
        return result;
    }

    void SubPeriodsSwapRateHelper::setTermStructure(YieldTermStructure* t) {
        // do not set the relinkable handle as an observer -
        // force recalculation when needed---the index is not lazy
        bool observer = false;

        boost::shared_ptr<YieldTermStructure> temp(t, null_deleter());
        termStructureHandle_.linkTo(temp, observer);

        if (discountHandle_.empty())
            discountRelinkableHandle_.linkTo(temp, observer);
        else
            discountRelinkableHandle_.linkTo(*discountHandle_, observer);

        RelativeDateRateHelper::setTermStructure(t);
    }

} // namespace QuantLib
