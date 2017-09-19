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

#ifndef qlext_ratehelpers_hpp
#define qlext_ratehelpers_hpp

#include <ql/termstructures/yield/ratehelpers.hpp>
#include <qlext/cashflows/subperiodscoupon.hpp>

namespace QuantLib {

class Shibor;
class ShiborSwap;

class ShiborSwapRateHelper : public RelativeDateRateHelper {

public:
    ShiborSwapRateHelper(const Handle<Quote>& rate, const Period& swapTenor, Frequency fixedFreq,
                         const boost::shared_ptr<Shibor>& shiborIndex, const Period& fwdStart = 0 * Days,
                         const Handle<YieldTermStructure>& discountingCurve = Handle<YieldTermStructure>());

    Real impliedQuote() const;
    void setTermStructure(YieldTermStructure*);

    boost::shared_ptr<ShiborSwap> swap() const;
    const Period& forwardStart() const;

    void accept(AcyclicVisitor&);

private:
    void initializeDates();
    Natural settlementDays_;
    RelinkableHandle<YieldTermStructure> termStructureHandle_;
    Period tenor_;
    Frequency fixedFrequency_;
    Period fwdStart_;
    boost::shared_ptr<Shibor> shiborIndex_;
    boost::shared_ptr<ShiborSwap> swap_;
    Handle<YieldTermStructure> discountHandle_;
    RelinkableHandle<YieldTermStructure> discountRelinkableHandle_;
};

inline boost::shared_ptr<ShiborSwap> ShiborSwapRateHelper::swap() const { return swap_; }

inline const Period& ShiborSwapRateHelper::forwardStart() const { return fwdStart_; }

class SubPeriodsSwap;

class SubPeriodsSwapRateHelper : public RelativeDateRateHelper {
public:
    SubPeriodsSwapRateHelper(const Handle<Quote>& rate,
        const Period& swapTenor,
        Frequency fixedFreq,
        const Calendar& fixedCalendar,
        const DayCounter& fixedDayCount,
        BusinessDayConvention fixedConvention,
        const Period& floatPayTenor,
        const boost::shared_ptr<IborIndex>& iborIndex,
        const DayCounter& floatingDayCount,
        DateGeneration::Rule rule = DateGeneration::Backward,
        Ext::SubPeriodsCoupon::Type type = Ext::SubPeriodsCoupon::Compounding,
        const Period& fwdStart = 0 * Days,
        const Handle<YieldTermStructure>& discountingCurve
        = Handle<YieldTermStructure>());

    Real impliedQuote() const;
    void setTermStructure(YieldTermStructure*);

    boost::shared_ptr<SubPeriodsSwap> swap() const;
    const Period& forwardStart() const;

    void accept(AcyclicVisitor&);

private:
    void initializeDates();
    Natural settlementDays_;
    RelinkableHandle<YieldTermStructure> termStructureHandle_;
    Period tenor_;
    Frequency fixedFrequency_;
    Calendar fixedCalendar_;
    DayCounter fixedDayCount_;
    BusinessDayConvention fixedConvention_;
    Period floatPayTenor_;
    DayCounter floatingDayCount_;
    DateGeneration::Rule rule_;
    Ext::SubPeriodsCoupon::Type type_;
    Period fwdStart_;
    boost::shared_ptr<IborIndex> iborIndex_;
    boost::shared_ptr<SubPeriodsSwap> swap_;
    Handle<YieldTermStructure> discountHandle_;
    RelinkableHandle<YieldTermStructure> discountRelinkableHandle_;
};

inline boost::shared_ptr<SubPeriodsSwap> SubPeriodsSwapRateHelper::swap() const { return swap_; }

inline const Period& SubPeriodsSwapRateHelper::forwardStart() const { return fwdStart_; }

} // namespace QuantLib

#endif
