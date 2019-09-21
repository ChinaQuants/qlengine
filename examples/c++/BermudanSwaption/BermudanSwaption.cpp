/* Reproduce the BBG deal */

#include <ql/qldefines.hpp>
#include <ql/instruments/swaption.hpp>
#include <ql/pricingengines/swaption/fdhullwhiteswaptionengine.hpp>
#include <ql/pricingengines/swaption/treeswaptionengine.hpp>
#include <ql/math/array.hpp>
#include <ql/math/interpolations/forwardflatinterpolation.hpp>
#include <ql/math/interpolations/linearinterpolation.hpp>
#include <ql/math/interpolations/cubicinterpolation.hpp>
/* which libor curve? */
#include <ql/indexes/ibor/USDLibor.hpp>
#include <ql/indexes/ibor/fedfunds.hpp>

#include <ql/cashflows/coupon.hpp>
#include <ql/quotes/simplequote.hpp>
#include <ql/termstructures/yield/zerocurve.hpp>
#include <ql/termstructures/yield/forwardcurve.hpp>
#include <ql/termstructures/yield/discountcurve.hpp>
#include <ql/time/calendars/target.hpp>
#include <ql/time/daycounters/thirty360.hpp>

#include <ql/experimental/shortrate/generalizedhullwhite.hpp>

#include <iostream>
#include <iomanip>

using namespace QuantLib;

#if defined(QL_ENABLE_SESSONS)
namespace QuantLib {
    Integer sessionId() { return 0; }
}
#endif

#define NOTIONAL 10000000

ext::shared_ptr<YieldTermStructure> makeTermStructures() {
    // discount curve directly
    Date discountingDates[] = { Date(15, July, 2019 ),Date( 15, September, 2019 ),
                     Date( 15, December, 2019 ), Date( 15, March, 2020 ),
                     Date( 15, June, 2020 ), Date( 15, September, 2020 ),
                     Date( 15, December, 2020 ), Date( 15, March, 2021 ),
                     Date( 15, July, 2021 ),
                     Date( 15, July, 2022 ), Date( 15, July, 2023 ),
                     Date( 15, July, 2024 ), Date( 15, July, 2025 ),
                     Date( 15, July, 2026 ), Date( 15, July, 2027 ),
                     Date( 15, July, 2028 ), Date( 15, July, 2029 ),
                     Date( 15, July, 2030 ), Date( 15, July, 2031 ),
                     Date( 15, July, 2034 ), Date( 15, July, 2039 ),
                     Date( 15, July, 2044 ), Date( 15, July, 2049 ),
                     Date( 15, July, 2059 ), Date( 15, July, 2069 ) };
    Rate discountingRates[] = { 1.0,
        0.994100, 0.990833, 0.985851, 0.981365, 0.977135, 0.973075,
        0.969010, 0.963438, 0.947077, 0.930235, 0.912599, 0.894150,
        0.875090, 0.855718, 0.836047, 0.816152, 0.796257, 0.776939,
        0.721127, 0.637235, 0.565823, 0.504040, 0.406721, 0.333739 };

    std::vector<Date> dcd(std::begin(discountingDates), std::end(discountingDates));
    std::vector<Rate> dcr(std::begin(discountingRates), std::end(discountingRates));
    ext::shared_ptr<InterpolatedDiscountCurve<Linear> > dcc =
            ext::make_shared<InterpolatedDiscountCurve<Linear> >( dcd, dcr, Thirty360());
    // show discount factor
    Size size = sizeof(discountingRates) / sizeof(discountingRates[0]);
    for (Size i=0; i<size; i++)
        std::cout << discountingDates[i] << ", " << dcc->zeroRate(discountingDates[i], Thirty360(), Simple) << std::endl;

    return dcc;
}

ext::shared_ptr<GeneralizedHullWhite> buildGhw(
            const Handle<YieldTermStructure> &yt, Real speed) {
    Date dates[] = { Date(16, July,    2019),
                     Date(16, August,  2019),
                     Date(15, October, 2019),
                     Date(15, January, 2020),
                     Date(16, April,   2020),
                     Date(16, July,    2020),
                     Date(16, July,    2021),
                     Date(18, July,    2022),
                     Date(17, July,    2023),
                     Date(16, July,    2024) };
    Real vols[] = { 0.0075, 0.0073, 0.0073, 
                    0.0072, 0.0073, 0.0072,
                    0.0070, 0.0072, 0.0070,
                    0.0075 };
    std::vector<Date> vd = std::vector<Date>(std::begin(dates),
                                             std::end(dates));
    std::vector<Real> vv = std::vector<Real>(std::begin(vols),
                                             std::end(vols));
    std::vector<Real> vr = std::vector<Real>(vv.size(), speed);
    return ext::make_shared<GeneralizedHullWhite>(yt, vd, vd, vr, vv, ForwardFlat(), ForwardFlat());
};

int main(int, char* []) {
    Date todaysDate(15, July, 2019);
    Calendar calendar = TARGET();
    Date settlementDate(16, July, 2019);
    Settings::instance().evaluationDate() = settlementDate;
    Settings::instance().includeReferenceDateEvents() = false;
    Settings::instance().includeTodaysCashFlows() = false;

    // discounting curve
    Handle<YieldTermStructure> bbgDiscountingCurve(
            makeTermStructures());

    // define the deal
    // deal property
    VanillaSwap::Type type = VanillaSwap::Receiver;

    // fixed convention
    Frequency fixedLegFrequency = Semiannual;
    BusinessDayConvention fixedLegConvention = ModifiedFollowing;
    // float convetion
    Frequency floatLegFrequency = Quarterly;
    BusinessDayConvention floatLegConvention = ModifiedFollowing;

    Date startDate = calendar.advance(todaysDate, 1, Years,
                            floatLegConvention);
    Date maturity = calendar.advance(startDate, 5, Years,
                            floatLegConvention);

    // fixed leg property
    DayCounter fixedLegDayCounter = Thirty360(Thirty360::BondBasis);
    Rate fixedRate = 0.02;
    Schedule fixedSchedule(startDate, maturity, Period(fixedLegFrequency),
                           calendar, fixedLegConvention, fixedLegConvention,
                           DateGeneration::Backward, true);

    // float leg property
    // TODO how to specify the US0003M?
    // what Euribor3M did
    ext::shared_ptr<IborIndex> bbgIndex3M(new USDLibor(Period(floatLegFrequency), bbgDiscountingCurve));
    Schedule floatSchedule(startDate, maturity, Period(floatLegFrequency),
                           calendar, floatLegConvention, floatLegConvention,
                           DateGeneration::Backward, true);

    // TODO discount factor consistent?
    // No...

    ext::shared_ptr<VanillaSwap> swap(new VanillaSwap(
        type, NOTIONAL,
        fixedSchedule, fixedRate, fixedLegDayCounter,
        floatSchedule, bbgIndex3M, 0.0, /* spread */
        bbgIndex3M->dayCounter()));

    // Bermudan swaption
    // construct coupon dates
    std::vector<Date> bbgBermudanDates;
    Schedule exerciseSchedule(todaysDate, maturity, Period(floatLegFrequency),
        calendar, floatLegConvention, floatLegConvention,
        DateGeneration::Backward, true);
    for (Size i=0; i<exerciseSchedule.size(); i++) {
        bbgBermudanDates.push_back(exerciseSchedule[i]);
    }

    ext::shared_ptr<Exercise> bbgBermudanExercise(
                         new BermudanExercise(bbgBermudanDates));
    Swaption bbgBermudanSwaption(swap, bbgBermudanExercise, Settlement::Cash, Settlement::Method::CollateralizedCashPrice);

    // set Hull-white one factor model
    ext::shared_ptr<HullWhite> bbgHW(new HullWhite(bbgDiscountingCurve,
                0.03, 0.0073));
    bbgBermudanSwaption.setPricingEngine(ext::shared_ptr<PricingEngine>(
            new FdHullWhiteSwaptionEngine(bbgHW, 500, 500, 2)));

    std::cout << std::endl;
    std::cout << "Pricing with Hull-white 1 factor: "
              << "a = " << bbgHW->params()[0] << ", "
              << "sigma = " << bbgHW->params()[1] << std::endl
              << "Price at " << bbgBermudanSwaption.NPV() << std::endl;

    // pricing with generalized hull white for piece-wise term structure fit
    ext::shared_ptr<GeneralizedHullWhite> bbgPiecewiseHW(buildGhw(
                    bbgDiscountingCurve, 0.03));
    bbgBermudanSwaption.setPricingEngine(ext::shared_ptr<PricingEngine>(
            new TreeSwaptionEngine(bbgPiecewiseHW, 500, bbgDiscountingCurve)));
    std::cout << "Piecewise Hull-White price at " << bbgBermudanSwaption.NPV() << std::endl;
}
